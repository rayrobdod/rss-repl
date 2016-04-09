
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
#include "FeedElement.h"
#include "SplitStringIterator.h"
#include "linenoise.h"

using std::wstring;
using std::vector;

#define _CRT_STDIO_ISO_WIDE_SPECIFIERS

#define END_LOOP	L"exit"
#define SHOW_CONTENTS	L"dir"
#define OPEN_INTERNAL	L"print"
#define OPEN_EXTERNAL	L"open"
#define OPEN_EXTERNAL_ATTACHMENT	L"open_attachment"
#define CHANGE_DIRECTORY	L"cd"
#define MAKE_DIRECTORY	L"md"
#define FEED_INFO	L"info"
#define MARK_READ	L"markAsRead"


static const wchar_t* const commands[] = {
	END_LOOP, SHOW_CONTENTS, OPEN_INTERNAL, OPEN_EXTERNAL, OPEN_EXTERNAL_ATTACHMENT, CHANGE_DIRECTORY, MAKE_DIRECTORY, FEED_INFO, MARK_READ
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
	for (int i = 0; i < 9; ++i) {
		wstring command(commands[i]);
		
		if (prefix2.compare(command.substr(0, prefixLen)) == 0) {
			std::string command2(command.cbegin(), command.cend());
			linenoiseAddCompletion(lc, command2.c_str());
		}
	}
}


int main(int argc, char** argv) {
	wstring command = L"";
	
	SetConsoleTitle(TEXT("RSS REPL"));
	CoInitializeEx(NULL, 2);
	
	linenoiseInstallWindowChangeHandler();
	linenoiseSetCompletionCallback(lineNoiseCompletionHook);
	
	currentFolder = getRootFolder();
	
	while (command.compare(END_LOOP) != 0) {
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
		command = (param.size() > 0 ? param[0] : L"");
		
		if (command.compare(L"") == 0 ||
				command.compare(END_LOOP) == 0 ) {
			// do nothing
			
		} else if (command.compare(SHOW_CONTENTS) == 0) {
			const wstring path = (param.size() > 1 ? param[1] : L".");
			const bool filterNew = (param.size() > 2 ? (param[2].compare(L"-n") == 0) : false);
			
			FeedElement* targetFolder = currentFolder->followPath(path);
			wprintf(L"%ls\n", targetFolder->getContentsString(filterNew).c_str());
			delete targetFolder;
			
		} else if (command.compare(OPEN_INTERNAL) == 0) {
			const wstring path = (param.size() > 1 ? param[1] : L".");

			FeedElement* targetFolder = currentFolder->followPath(path);
			wprintf(L"%ls\n", targetFolder->getDetailsString().c_str());
			delete targetFolder;
			
		} else if (command.compare(OPEN_EXTERNAL_ATTACHMENT) == 0) {
			const wstring path = (param.size() > 1 ? param[1] : L".");

			FeedElement* targetFolder = currentFolder->followPath(path);
			if (targetFolder->isError()) {
				printf("No such element\n");
			} else {
				std::pair<HRESULT, std::wstring> res = targetFolder->getAttachmentFile();
				if (res.first == S_FALSE) {
					printf("Element does not contain attachment\n");
				} else if (res.first == E_NOTIMPL) {
					printf("Not a thing that can contain an attachment\n");
				} else if (SUCCEEDED(res.first)) {
					ShellExecute(GetConsoleWindow(), L"open", res.second.c_str(), NULL, NULL, SW_SHOWNORMAL);
					printf("Attachment opened.\n");
				} else {
					printf("Failed to open attachment\n");
				}
			}
			delete targetFolder;

		} else if (command.compare(OPEN_EXTERNAL) == 0) {
			const wstring path = (param.size() > 1 ? param[1] : L".");

			FeedElement* targetFolder = currentFolder->followPath(path);
			if (targetFolder->isError()) {
				printf("No such element\n");
			} else {
				std::pair<HRESULT, std::wstring> res = targetFolder->getUrl();
				if (res.first == S_FALSE) {
					printf("Element does not contain attachment\n");
				} else if (res.first == E_NOTIMPL) {
					printf("Not a thing that can contain an attachment\n");
				} else if (SUCCEEDED(res.first)) {
					ShellExecute(GetConsoleWindow(), L"open", res.second.c_str(), NULL, NULL, SW_SHOWNORMAL);
					printf("Attachment opened.\n");
				} else {
					printf("Failed to open attachment\n");
				}
			}
			delete targetFolder;

		} else if (command.compare(MARK_READ) == 0) {
			const wstring path = (param.size() > 1 ? param[1] : L".");

			FeedElement* targetFolder = currentFolder->followPath(path);
			HRESULT result = targetFolder->markAsRead();
			if (SUCCEEDED(result)) {
				printf("Mark as read succeeded\n");
			} else {
				printf("Mark as read failed\n");
			}
			delete targetFolder;

		} else if (command.compare(L"echo") == 0) {
			for (size_t i = 0; i < param.size(); i++) {
				std::wcout << "\t" << param[i] << "\n";
			}
			
		} else if (command.compare(CHANGE_DIRECTORY) == 0) {
			const wstring path = (param.size() > 1 ? param[1] : L".");
			
			FeedElement* newFolder = currentFolder->followPath(path);
			if (newFolder->isError()) {
				wprintf(L"%ls\n", newFolder->getDetailsString().c_str());
				delete newFolder;
			} else {
				delete currentFolder;
				currentFolder = newFolder;
			}
		} else {
			printf("Unknown Command\n");
		}
	}
	
	delete currentFolder;
	
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtDumpMemoryLeaks();
	return 0;
}
