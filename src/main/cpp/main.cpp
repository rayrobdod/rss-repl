
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <windows.h>
#include <msfeeds.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "FeedElement.h"
#include "SplitStringIterator.h"

using std::wstring;
using std::vector;

#define END_LOOP	L"exit"
#define SHOW_CONTENTS	L"dir"
#define OPEN_INTERNAL	L"print"
#define OPEN_EXTERNAL	L"open"
#define OPEN_EXTERNAL_ATTACHMENT	L"open_attachment"
#define CHANGE_DIRECTORY	L"cd"
#define MAKE_DIRECTORY	L"md"
#define FEED_INFO	L"info"
#define MARK_READ	L"markAsRead"


int main(int argc, char** argv) {
	FeedElement* currentFolder;
	wchar_t  input[STR_BUFFER_SIZE];
	wstring command = L"";
	
	SetConsoleTitle(TEXT("RSS REPL"));
	CoInitializeEx(NULL, 2);
	
	currentFolder = getRootFolder();
	
	while (command.compare(END_LOOP) != 0) {
		wprintf(L"%%Feeds%%\\%ls> ", currentFolder->getPath().c_str());
		
		fflush(stdout);
		fgetws(input, STR_BUFFER_SIZE, stdin);
		
		std::vector<std::wstring> param(SplitStringIterator(input, (wstring) L" \n\t", (wstring) L"\""), ::SplitStringIterator::end());
		command = param[0];
		
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
