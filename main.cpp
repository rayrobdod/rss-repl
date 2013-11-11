
#include <windows.h>
#include <msfeeds.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <cwchar>
#include <iostream>

#define END_LOOP         L"exit"
#define PRINT_DIRECTORY  L"dir"
#define CHANGE_DIRECTORY L"cd"
#define BUFFER_SIZE      1028


// dir
void printFolder(IFeedFolder* folder);

// cd
IFeedFolder* changeDirectory(IFeedFolder* base, const wchar_t* const path);

int main(int argc, char** argv)
{
	IFeedsManager* manager;
	IFeedFolder* currFolder;
	BSTR  currFolderPath;
	BSTR  name;
	wchar_t* input = (wchar_t*) malloc(BUFFER_SIZE * 2/* sizeof(wchar_t) */);
	wchar_t* command = L"";
	wchar_t* param1;
	
	
	CoInitializeEx(NULL, 2);
	CoCreateInstance(
			CLSID_FeedsManager, NULL, CLSCTX_ALL, IID_IFeedsManager ,
			(void**)&manager);
	
	manager->get_RootFolder((IDispatch**)&currFolder);
	
	
	while (wcscmp(command, END_LOOP) != 0) {
		currFolder->get_Path(&currFolderPath);
		printf("%ls>", (char *)currFolderPath);
		
		fflush(stdout);
		fgetws(input, BUFFER_SIZE, stdin);
		
		command = wcstok(input, L" \n\r");
		param1  = wcstok(NULL,  L" \n\r");
		
		
		if (wcscmp(command, END_LOOP) == 0) {
			// do nothing
			
		} else if (wcsstr(command, PRINT_DIRECTORY) == input) {
			printFolder(currFolder);
			
		} else if (wcsstr(command, CHANGE_DIRECTORY) == input) {
			if (param1 != NULL) {
				currFolder = changeDirectory(currFolder, param1);
			}
			
		}
		
	}
}



// cd
IFeedFolder* changeDirectory(IFeedFolder* base, const wchar_t* const path) {
	IFeedFolder* result;
	const wchar_t* UP_ONE_LEVEL = L"..";
	
	if (wcscmp(path, UP_ONE_LEVEL)) {
		base->get_Parent((IDispatch**)&result);
	} else {
		BSTR path_b = SysAllocString(path);
		base->GetSubfolder(path_b, (IDispatch**)&result);
		SysFreeString(path_b);
	}
	
	return result;
}


// dir
void printFolder(IFeedFolder* folder) {
	IFeedsEnum* currentFeeds;
	BSTR name;
	
	
	
	// folders
	folder->get_Subfolders((IDispatch**)&currentFeeds);
	
	LONG feedCount;
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeedFolder* currentFeed;
		currentFeeds->Item(i, (IDispatch**)&currentFeed);
		
		currentFeed->get_Name(&name);
		printf("%ls/\n", (char *)name);
	}
	
	
	// not folders; feeds
	folder->get_Feeds((IDispatch**)&currentFeeds);
	
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeed* currentFeed;
		LONG unreadCount;
		currentFeeds->Item(i, (IDispatch**)&currentFeed);
		
		currentFeed->get_Name(&name);
		currentFeed->get_UnreadItemCount(&unreadCount);
		printf("%ls (%ld)\n", (char *)name, unreadCount);
	}
}
