
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


int main(int argc, char** argv)
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
			
			
			printFeed(currentFeed);
			
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
}


