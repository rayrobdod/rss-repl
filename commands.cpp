
#include <iostream>
#include "commands.h"

// cd
// TODO: wcstok to allow "cd path\to\dir" to work
// TODO: cd into a feed(?)
IFeedFolder* changeDirectory(IFeedFolder* base, const wchar_t* const path) {
	IFeedFolder* result;
	HRESULT error;
	const wchar_t* UP_ONE_LEVEL = L"..";
	
	if (wcscmp(path, UP_ONE_LEVEL) == 0) {	
		error = base->get_Parent((IDispatch**)&result);
	} else {
		BSTR path_b = SysAllocString(path);
		error = base->GetSubfolder(path_b, (IDispatch**)&result);
		SysFreeString(path_b);
	}
	
	if (error == 0x80070003) { // ERROR_PATH_NOT_FOUND
		printf("No such directory\n");
		return base;
	} else if (error == 0x80070005) { // ERROR_ACCESS_DENIED; seems wider than 
		printf("Access denied\n");
		return base;
	} else if (error) {
		printf("Can't do that: %x\n", error);
		return base;
	} else {
		return result;
	}
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

// print the contents of a feed
void printFeed(IFeed* feed) {
	IFeedsEnum* items;
	BSTR name;
	LONG localId;
	DATE pubDate;
	VARIANT_BOOL isRead;
	
	feed->get_Name(&name);
	printf(" Feed: %ls\n", (char*) name);
	
	
	// items
	feed->get_Items((IDispatch**)&items);
	
	LONG feedCount;
	items->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeedItem* curItem;
		items->Item(i, (IDispatch**)&curItem);
		
		curItem->get_Title(&name);
		curItem->get_PubDate(&pubDate);
		curItem->get_LocalId(&localId);
		curItem->get_IsRead(&isRead);
		char* isReadMessage = (isRead ? "" : "<NEW>");
		
		printf("%4ld %5s %ls\n", localId, isReadMessage, (char *)name);
	}
	
}
