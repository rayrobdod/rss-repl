
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
	} else if (wcscmp(path, L".") == 0) {
		result = base;
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
		printf("%ls/\n", (wchar_t *)name);
		SysFreeString(name);
		currentFeed->Release();
	}
	currentFeeds->Release();
	
	
	// not folders; feeds
	folder->get_Feeds((IDispatch**)&currentFeeds);
	
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeed* currentFeed;
		LONG unreadCount;
		currentFeeds->Item(i, (IDispatch**)&currentFeed);
		
		currentFeed->get_Name(&name);
		currentFeed->get_UnreadItemCount(&unreadCount);
		printf("%ls (%ld)\n", (wchar_t *)name, unreadCount);
		SysFreeString(name);
		currentFeed->Release();
	}
	currentFeeds->Release();
}

// print the contents of a feed
void printFeed(IFeed* feed, bool filterUnread) {
	IFeedsEnum* items;
	BSTR name;
	LONG localId;
	DATE pubDate;
	VARIANT_BOOL isRead;
	
	feed->get_Name(&name);
	printf(" Feed: %ls\n", (wchar_t*) name);
	
	
	// items
	feed->get_Items((IDispatch**)&items);
	
	LONG feedCount;
	items->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeedItem* curItem;
		items->Item(i, (IDispatch**)&curItem);
		curItem->get_IsRead(&isRead);
		
		if (!filterUnread || (isRead == VARIANT_FALSE)) {
			
			curItem->get_Title(&name);
			curItem->get_PubDate(&pubDate);
			curItem->get_LocalId(&localId);
			char* isReadMessage = (isRead ? "" : "<NEW>");
			
			printf("%4ld %5s %ls\n", localId, isReadMessage, (wchar_t *)name);
			
			SysFreeString(name);
		}
		
		curItem->Release();
	}
	items->Release();
}

void printItem(IFeedItem* item) {
	BSTR str;
	DATE pubDate;
	IDispatch* dispatch;
	
	item->get_Title(&str);
	if (str) {
		printf("\n%ls\n", str);
		SysFreeString(str);
	}
	
	item->get_Author(&str);
	if (str) {
		printf("    Author:  %ls\n", str);
		SysFreeString(str);
	}
	
//	item->get_Enclosure(&dispatch);
//	if (dispatch != NULL) {
//		printf("    Has Enclosure\n");
//		dispatch->Release();
//	}
	
	item->get_PubDate(&pubDate);
	if (pubDate) {
		// printf("    PubDate: %ls\n", (BSTR) pubDate);
	}
	
	item->get_Modified(&pubDate);
	if (pubDate) {
		// printf("    PubDate: %ls\n", (BSTR) pubDate);
	}
	
	printf("\n");
	
	item->get_Description(&str);
	if (str) {
		printf("%ls\n\n", str);
		SysFreeString(str);
	}
	
	
}