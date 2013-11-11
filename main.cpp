
#include <windows.h>
#include <msfeeds.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>


// dir
void printFolder(IFeedFolder* folder);

// cd
IFeedFolder* changeDirectory(IFeedFolder* base, BSTR path);

int main(int argc, char** argv)
{
	IFeedsManager* manager;
	IFeedFolder* currFolder;
	BSTR  name;
	wchar_t* input = (wchar_t*) malloc(1028 /* * sizeof(wchar_t) */);
	
	
	CoInitializeEx(NULL, 2);
	CoCreateInstance(
			CLSID_FeedsManager, NULL, CLSCTX_ALL, IID_IFeedsManager ,
			(void**)&manager);
	
	manager->get_RootFolder((IDispatch**)&currFolder);
	
	
	while (input
	
	
	
	currFolder->get_Path(&name);
	printf("%ls>dir\n", (char *)name);
	
	printFolder(currFolder);
	
	
	
	
	printf("%ls>cd %s\n", (char *)name, "Comics");
	currFolder->GetSubfolder(SysAllocString(L"Comics"), (IDispatch**)&currFolder);
	currFolder->get_Path(&name);
	printf("%ls>dir\n", (char *)name);
	
	printFolder(currFolder);
	
	
	printf(">");
	fflush(stdout);
	scanf("%ls", input);
	printf("%ls\n", input);

}



// cd
IFeedFolder* changeDirectory(IFeedFolder* base, BSTR path) {
	IFeedFolder* result;
	BSTR name;
	const BSTR UP_ONE_LEVEL = SysAllocString(L"..");
	
	if (path == UP_ONE_LEVEL) {
		base->get_Parent((IDispatch**)&result);
	} else {
		base->GetSubfolder(path, (IDispatch**)&result);
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

/* BSTR cstrToBstr(const char* orig) {
	
	size_t newsize = strlen(orig) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);
	
	return BSTR(wcstring);
} */
