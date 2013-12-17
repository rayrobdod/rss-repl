
#include <windows.h>
#include <msfeeds.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <cwchar>
#include <iostream>
#include "commands.h"

#define END_LOOP         L"exit"
#define PRINT_DIRECTORY  L"dir"
#define PRINT_FEED       L"printfeed"
#define CHANGE_DIRECTORY L"cd"
#define MAKE_DIRECTORY   L"md"
#define FEED_INFO        L"info"
#define BUFFER_SIZE      (1028)
#define MAX_PARAM_COUNT  (10)


/**
 * 
 * @note uses and modifies the input array
 * @param input the line
 * @param output indexies indicating the start of strings
 * @return the length of the output array
 */
int tokenify(wchar_t* input, wchar_t** output)
{
	const int inputLen = wcslen(input);
	int i;
	int outputLen = 1;
	bool quoteMode = false;
	
	output[0] = input;
	
	for (i = 0; i < inputLen; i++) {
		if (outputLen >= MAX_PARAM_COUNT) break; 
		
		if (input[i] == L' ' && !quoteMode) {
			input[i] = L'\0';
			output[outputLen] = input + i + 1;
			outputLen++;
			
		} else if (input[i] == L'"') {
			quoteMode = !quoteMode;
		} else {
			// do nothing
		}
	}
	
	return outputLen;
}



int main(int argc, char** argv)
{
	wchar_t* input   = (wchar_t*) malloc(BUFFER_SIZE * sizeof(wchar_t));
	wchar_t** output = (wchar_t**) malloc(MAX_PARAM_COUNT * sizeof(wchar_t*));
	
	wcscpy(input, L"Hello World");
	
	int count = tokenify(input, output);
	
	printf("%d %ls %ls", count, output[0], output[1]);
	
	return 0;
}

int main2(int argc, char** argv)
{
	IFeedsManager* manager;
	IFeedFolder* currFolder;
	BSTR  currFolderPath;
	BSTR  name;
	wchar_t* input = (wchar_t*) malloc(BUFFER_SIZE * 2/* sizeof(wchar_t) */);
	wchar_t* command = L"";
	wchar_t** paramv = (wchar_t**) malloc(32 * sizeof(wchar_t*));
	
	
	CoInitializeEx(NULL, 2);
	CoCreateInstance(
			CLSID_FeedsManager, NULL, CLSCTX_ALL, IID_IFeedsManager ,
			(void**)&manager);
	
	manager->get_RootFolder((IDispatch**)&currFolder);
	
	
	while (command == NULL || wcscmp(command, END_LOOP) != 0) {
		currFolder->get_Path(&currFolderPath);
		printf("%%Feeds%%\\%ls> ", (char *)currFolderPath);
		
		fflush(stdout);
		fgetws(input, BUFFER_SIZE, stdin);
		
		paramv[0] = command = wcstok(input, L" \n\r");
		int paramc = 0;
		while(NULL != paramv[paramc]) {
			paramc++;
			paramv[paramc] = wcstok(NULL, L" \n\r");
		}
		
		
		if (command == NULL) {
			// do nothing
			
		} else if (wcscmp(command, END_LOOP) == 0) {
			// do nothing
			
		} else if (wcsstr(command, PRINT_DIRECTORY) == command) {
			printFolder(currFolder);
			
		} else if (wcsstr(command, PRINT_FEED) == command) {
			IFeed* currentFeed;
			BSTR path_b = SysAllocString(paramv[1]);
			currFolder->GetFeed(path_b, (IDispatch**)&currentFeed);
			SysFreeString(path_b);
			
			
			printFeed(currentFeed, true);
			
		} else if (wcsstr(command, L"markAsRead") == command) {
			IFeed* currentFeed;
			BSTR path_b = SysAllocString(paramv[1]);
			currFolder->GetFeed(path_b, (IDispatch**)&currentFeed);
			SysFreeString(path_b);
			
			long int index = wcstol(paramv[2], NULL, 10);
			
			IFeedItem* currentItem;
			currentFeed->GetItem((LONG) index, (IDispatch**)&currentItem);
			
			currentItem->put_IsRead(VARIANT_TRUE);
			
		} else if (wcsstr(command, L"echo") == command) {
			for (int i = 0; i < paramc; i++) {
				printf("\t%ls\n", paramv[i]);
			}
			
		} else if (wcsstr(command, CHANGE_DIRECTORY) == command) {
			if (paramc >= 1) {
				currFolder = changeDirectory(currFolder, paramv[1]);
			} else {
				printf("Needs a paramter; the directory to change to\n");
			}
			
		} else {
			printf("Unknown Command\n");
		}
	}
	
	return 0;
}


