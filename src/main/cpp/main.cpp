
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <windows.h>
#include <msfeeds.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "FeedElement.h"
#include "SplitStringIterator.h"
#include "linenoise.h"
#include "command.h"

using std::wstring;
using std::vector;

#define _CRT_STDIO_ISO_WIDE_SPECIFIERS


static const wchar_t* const commands[] = {
	END_LOOP, SHOW_CONTENTS, OPEN_INTERNAL, OPEN_EXTERNAL,
	OPEN_EXTERNAL_ATTACHMENT, CHANGE_DIRECTORY, MAKE_DIRECTORY, FEED_INFO,
	MARK_READ, ATTACH_IMAGE
};


class FilteringOutputStreambuf : public std::wstreambuf {
 public:
	FilteringOutputStreambuf(std::wstreambuf* backing) : backing(backing) {}
	virtual ~FilteringOutputStreambuf() {};
	virtual int_type overflow(int_type ch) {
		int result(EOF);
		if (ch == EOF) {
			return sync();
		} else if (backing != NULL) {
			const int_type newChar = (ch > 0x7E ? '?' : ch);
			return backing->sputc(newChar);			
		} else {
			return traits_type::eof();
		}
	}
	virtual int_type underflow() { return traits_type::eof(); }
	virtual int sync() { return backing->pubsync(); }
	virtual std::wstreambuf* setbuf(wchar_t* p, int len) { return backing->pubsetbuf(p, len); }

 private:
	std::wstreambuf* backing;
};


// set in main; read in main and lineNoiseCompletionHook
FeedElement* currentFolder;


// NOTE: prefix contains at most one word, so there is no way to tell whether the line already has a command or not
void lineNoiseCompletionHook(char const* prefix, linenoiseCompletions* lc) {
	const size_t prefixLen(strlen(prefix));
	const wstring prefix2(prefix, prefix + prefixLen);
	SplitStringIterator prefixSplit(prefix2, (wstring)L" \n\t", (wstring)L"\"");

	// directories
	vector<wstring> values = currentFolder->getContents();
	for (auto i = values.cbegin(); i != values.cend(); i++) {
		if ((*prefixSplit).compare(i->substr(0, prefixLen)) == 0) {
			std::string command2(i->cbegin(), i->cend());
			// could bother to check whether the dir has spaces in it, but effort.
			command2.insert(0, "\"");
			command2.append("\"");
			linenoiseAddCompletion(lc, command2.c_str());
		}
	}

	// complete command
	for (int i = 0; i < 10; ++i) {
		wstring command(commands[i]);
		
		if (prefix2.compare(command.substr(0, prefixLen)) == 0) {
			std::string command2(command.cbegin(), command.cend());
			linenoiseAddCompletion(lc, command2.c_str());
		}
	}
}



int main(int argc, char** argv) {
	bool exit = false;
	
	SetConsoleTitle(TEXT("RSS REPL"));
	const UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP(CP_UTF8);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	
	linenoiseInstallWindowChangeHandler();
	linenoiseSetCompletionCallback(lineNoiseCompletionHook);
	
	currentFolder = getRootFolder();
	
	while (! exit) {
		wstring input;
		{
			char prompt[STR_BUFFER_SIZE];
			snprintf(prompt, STR_BUFFER_SIZE, "%%Feeds%%\\%ls> ", currentFolder->getPath().c_str());
			char* const input_n = linenoise(prompt);
			std::string input_ns(input_n);
			input = wstring(input_ns.cbegin(), input_ns.cend());
			
			linenoiseHistoryAdd(input_n);
			free(input_n);
		}
		
		std::vector<std::wstring> param(SplitStringIterator(input, (wstring) L" \n\t", (wstring) L"\""), ::SplitStringIterator::end());
		
		FilteringOutputStreambuf buf(std::wcout.rdbuf());
		auto result = processCommand(currentFolder, param, std::wostream(&buf));
		std::tie (exit, currentFolder) = result;
	}
	
	linenoiseHistoryFree();
	delete currentFolder;
	SetConsoleOutputCP(oldcp);
	
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtDumpMemoryLeaks();
	return 0;
}
