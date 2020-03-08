
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
#include "loadResources.h"
#include "..\\resource\\string_table_keys.h"

using std::wstring;
using std::vector;

#define _CRT_STDIO_ISO_WIDE_SPECIFIERS


class WindowsConsoleOutputStreamBuf : public std::wstreambuf {
 public:
	WindowsConsoleOutputStreamBuf(HANDLE console) : console(console) {};
	virtual ~WindowsConsoleOutputStreamBuf() {};
	virtual int_type overflow(int_type ch) {
		if (ch == traits_type::eof()) {
			return traits_type::eof();
		} else {
			BOOL errorResult;
			DWORD charsWritten;
			if (0xD800 <= ch && ch < 0xE000) {
				if (first_half_of_surrogate_pair == 0) {
					first_half_of_surrogate_pair = ch;
					errorResult = 1;
				} else {
					wchar_t buf[2] = {first_half_of_surrogate_pair, ch};
					errorResult = WriteConsole(console, buf, 2, &charsWritten, NULL);
					first_half_of_surrogate_pair = 0;
				}
			} else {
				errorResult = WriteConsole(console, &ch, 1, &charsWritten, NULL);
			}
#ifdef DEBUG
			if (0 == errorResult) {
				// complain loudly and explicitly
				LPVOID lpMsgBuf;
				DWORD dw = GetLastError();

				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					dw,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf,
					0, NULL );

				wchar_t lpDisplayBuf[256] = {0};
				swprintf(lpDisplayBuf, 256,
					L"%ls failed with error %d: %s",
					L"WriteConsole", dw, lpMsgBuf);
				MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
			}
#endif
			return ch;
		}
	}

 private:
	WindowsConsoleOutputStreamBuf(const WindowsConsoleOutputStreamBuf&);
	WindowsConsoleOutputStreamBuf& operator=(const WindowsConsoleOutputStreamBuf&);
 private:
	const HANDLE console;
	int_type first_half_of_surrogate_pair;
};


// set in main; read in main and lineNoiseCompletionHook
std::shared_ptr<FeedElement> currentFolder;


// NOTE: prefix contains at most one word, so there is no way to tell whether the line already has a command or not
void lineNoiseCompletionHook(char const* prefix, linenoiseCompletions* lc) {
	const int wstringBufferLen = MultiByteToWideChar(CP_UTF8, 0, prefix, strlen(prefix), NULL, 0);
	const LPWSTR wstr = new wchar_t[wstringBufferLen];
	const int wstrLen = MultiByteToWideChar(CP_UTF8, 0, prefix, strlen(prefix), wstr, wstringBufferLen);
	
	const wstring prefix2(wstr, wstrLen);
	// directories
	vector<wstring> values = currentFolder->getContents();
	for (auto i = values.cbegin(); i != values.cend(); i++) {
		if (prefix2.compare(i->substr(0, prefix2.length())) == 0) {
			int utf8StringBufferLen = WideCharToMultiByte(CP_UTF8, 0, i->c_str(), i->length(), NULL, 0, NULL, NULL);
			LPSTR utf8String = new char[utf8StringBufferLen];
			int utf8StringLen = WideCharToMultiByte(CP_UTF8, 0, i->c_str(), i->length(), utf8String, utf8StringBufferLen, NULL, NULL);
			std::string command2(utf8String, utf8StringLen);
			
			if (command2.rfind(L' ') != wstring::npos
					|| command2.rfind(L'\n') != wstring::npos
					|| command2.rfind(L'\t') != wstring::npos) {
				command2.insert(0, "\"");
				command2.append("\"");
			}
			linenoiseAddCompletion(lc, command2.c_str());
			
			delete[] utf8String;
		}
	}

	// complete command
	auto commandNames2 = commandNames();
	for (auto i = commandNames2.cbegin(); i != commandNames2.cend(); ++i) {
		wstring commandName(*i);
		
		if (prefix2.compare(commandName.substr(0, prefix2.length())) == 0) {
			std::string command2(commandName.cbegin(), commandName.cend());
			linenoiseAddCompletion(lc, command2.c_str());
		}
	}
	delete[] wstr;
}



int main(int argc, char** argv) {
	bool exit = false;
	
	SetConsoleTitle(LoadStringRrdLpcT(IDS_TITLE));
	const UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP(CP_UTF8);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	
	linenoiseInstallWindowChangeHandler();
	linenoiseSetCompletionCallback(lineNoiseCompletionHook);
	
	WindowsConsoleOutputStreamBuf myOutStreamBuf(GetStdHandle(STD_OUTPUT_HANDLE));
	std::wostream myOutStream(&myOutStreamBuf);
	
	currentFolder = getRootFolder();
	
	while (! exit) {
		wstring input;
		{
			int utf8StringBufferLen = WideCharToMultiByte(CP_UTF8, 0, currentFolder->getPath().c_str(), currentFolder->getPath().length(), NULL, 0, NULL, NULL);
			LPSTR utf8String = new char[utf8StringBufferLen];
			int utf8StringLen = WideCharToMultiByte(CP_UTF8, 0, currentFolder->getPath().c_str(), currentFolder->getPath().length(), utf8String, utf8StringBufferLen, NULL, NULL);
			std::string prompt(LoadStringRrdStlA(IDS_PROMPT));
			prompt.append("\\");
			prompt.append(utf8String, utf8StringLen);
			prompt.append("> ");
			
			char* const input_n = linenoise(prompt.c_str());
			
			int input_w_buflen = MultiByteToWideChar(CP_UTF8, 0, input_n, strlen(input_n), NULL, 0);
			LPWSTR input_w = new wchar_t[input_w_buflen];
			int input_wlen = MultiByteToWideChar(CP_UTF8, 0, input_n, strlen(input_n), input_w, input_w_buflen);
			input = wstring(input_w, input_wlen);
			
			linenoiseHistoryAdd(input_n);
			free(input_n);
			delete[] utf8String;
			delete[] input_w;
		}
		
		std::vector<std::wstring> param(SplitStringIterator(input, (wstring) L" \n\t", (wstring) L"\""), ::SplitStringIterator::end());
		
		auto result = processCommand(currentFolder, param, myOutStream);
		std::tie (exit, currentFolder) = result;
	}
	
	linenoiseHistoryFree();
	currentFolder.reset();
	SetConsoleOutputCP(oldcp);
	CoUninitialize();
	
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtDumpMemoryLeaks();
	return 0;
}
