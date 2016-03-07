
#include <windows.h>
#include <msfeeds.h>
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <cwchar>
#include <iostream>
#include "commands.h"

#define END_LOOP	L"exit"
#define SHOW_CONTENTS	L"dir"
#define SHOW_ITEM	L"print"
#define CHANGE_DIRECTORY	L"cd"
#define MAKE_DIRECTORY	L"md"
#define FEED_INFO	L"info"
#define BUFFER_SIZE	(1028)
#define MAX_PARAM_COUNT	(10)


/**
 * 
 * @note uses and modifies the input array
 * @param input the line
 * @param output indexies indicating the start of strings
 * @return the length of the output array
 */
int tokenify(wchar_t* const input, wchar_t** const output) {
	const int inputLen = wcslen(input);
	int i;
	int outputLen = 0;
	bool quoteMode = false;
	bool outsideToken = true;
	for (i = 0; i < MAX_PARAM_COUNT; i++) {
		output[i] = L"\0";
	}
	
	for (i = 0; i < inputLen; i++) {
		if (outputLen >= MAX_PARAM_COUNT) break; 
		
		if ((input[i] == ' ' || input[i] == '\n') && !quoteMode && !outsideToken) {
			input[i] = L'\0';
			outsideToken = true;
			
		} else if (input[i] == L'"') {
			quoteMode = !quoteMode;
			input[i] = L'\0';
		} else {
			if (outsideToken) {
				outsideToken = false;
				output[outputLen] = input + i;
				outputLen++;
			}
		}
	}
	
	return outputLen;
}


int main(int argc, char** argv) {
	IFeedsManager* manager;
	FeedElement* currentFolder;
	BSTR  currFolderPath;
	BSTR  name;
	
	wchar_t* const  input  = (wchar_t*) malloc(BUFFER_SIZE * sizeof(wchar_t));
	wchar_t** const paramv = (wchar_t**) malloc(MAX_PARAM_COUNT * sizeof(wchar_t*));
	const wchar_t* command = L"";
	
	
	SetConsoleTitle(TEXT("RSS REPL"));
	
	CoInitializeEx(NULL, 2);
	CoCreateInstance(
			CLSID_FeedsManager, NULL, CLSCTX_ALL, IID_IFeedsManager ,
			(void**)&manager);
	
	{
		IFeedFolder* startingFolder;
		manager->get_RootFolder((IDispatch**)&startingFolder);
		currentFolder = new FeedFolder(startingFolder);
	}
	
	
	while (command == NULL || wcscmp(command, END_LOOP) != 0) {
		printf("%%Feeds%%\\%ls> ", currentFolder->getPath().c_str());
		
		fflush(stdout);
		fgetws(input, BUFFER_SIZE, stdin);
		
		const int paramc = tokenify(input, paramv);
		command = paramv[0];
		
		if (command == NULL || wcscmp(command, L"") == 0 ||
				wcscmp(command, END_LOOP) == 0 ) {
			// do nothing
			
		} else if (wcscmp(command, SHOW_CONTENTS) == 0) {
			wchar_t* path;
			if (paramc == 1) {
				path = L".";
			} else {
				path = paramv[1];
			}
			const bool filterNew = (wcscmp(paramv[2], L"-n") == 0);
			
			FeedElement* targetFolder = currentFolder->cd(path);
			printf("%ls\n", targetFolder->getContentsString(filterNew).c_str());
			delete targetFolder;
			
		} else if (wcscmp(command, SHOW_ITEM) == 0) {
			wchar_t* path;
			if (paramc == 1) {
				path = L".";
			} else {
				path = paramv[1];
			}
			const bool filterNew = (wcscmp(paramv[2], L"-n") == 0);
			
			FeedElement* targetFolder = currentFolder->cd(path);
			printf("%ls\n", targetFolder->getDetailsString().c_str());
			delete targetFolder;
			
/*		} else if (wcscmp(command, L"markAsRead") == 0) {
			IFeed* currentFeed;
			BSTR path_b = SysAllocString(paramv[1]);
			currFolder->GetFeed(path_b, (IDispatch**)&currentFeed);
			SysFreeString(path_b);
			
			long int index = wcstol(paramv[2], NULL, 10);
			
			IFeedItem* currentItem;
			currentFeed->GetItem((LONG) index, (IDispatch**)&currentItem);
			
			currentItem->put_IsRead(VARIANT_TRUE);
			
*/		} else if (wcscmp(command, L"echo") == 0) {
			for (int i = 0; i < paramc; i++) {
				printf("\t%ls\n", paramv[i]);
			}
			
		} else if (wcscmp(command, CHANGE_DIRECTORY) == 0) {
			FeedElement* prevFolder = currentFolder;
			wchar_t* path;
			if (paramc == 1) {
				path = L".";
			} else {
				path = paramv[1];
			}
			
			currentFolder = currentFolder->cd(path);
			delete prevFolder;
			
		} else {
			printf("Unknown Command\n");
		}
	}
	
	return 0;
}
