
#include <utility>
#include <sstream>
#include "commands.h"

using std::pair;
using std::wstring;
const wchar_t* UP_ONE_LEVEL = L"..";
const size_t MAX_STRING_SIZE = 1024;

FeedElement::FeedElement() {}

FeedFolder::FeedFolder(IFeedFolder* backing) : backing(backing) {}

FeedFeed::FeedFeed(IFeed* backing) : backing(backing) {}

FeedItem::FeedItem(IFeedItem* backing) : backing(backing) {}

ErrorFeedElement::ErrorFeedElement(const wstring message) : message(message) {}

/**
 * @return _1 the first element of the path
 *   _2 the rest of the path
 */
pair<wstring, wstring> splitPathString(const wstring path) {
	wstring::const_iterator i;
	for (i = path.cbegin(); i < path.cend();  i++) {
		if (*i == '/' || *i == '\\') {
			break;
		}
	}
	
	const wstring a = wstring(path.cbegin(), i);
	const wstring b = (i == path.cend() ? L"" : wstring(i + 1, path.cend()));
	
	pair<wstring, wstring> retVal(a,b);
	return retVal;
}

/*
 0x80070003 // ERROR_PATH_NOT_FOUND
 0x80070005 // ERROR_ACCESS_DENIED
*/
FeedElement* FeedFolder::cd(const wstring path) const {
	const pair<wstring, wstring> parts = splitPathString(path);
	FeedElement* iterationStep;
	
	HRESULT error;
	if (parts.first.compare(UP_ONE_LEVEL) == 0) {
		IFeedFolder* result;
		error = backing->get_Parent((IDispatch**)&result);
		if (error) {
			iterationStep = new ErrorFeedElement(L"Already top level; no parent to cd to");
		} else {
			iterationStep = new FeedFolder(result);
		}
	} else if (parts.first.compare(L".") == 0) {
		iterationStep = new FeedFolder(this->backing);
	} else {
		VARIANT_BOOL feedExists;
		VARIANT_BOOL folderExists;
		BSTR name = SysAllocString(parts.first.c_str());
		
		error = backing->ExistsFeed(name, &feedExists);
		error = backing->ExistsSubfolder(name, &folderExists);
		
		if (feedExists == VARIANT_TRUE) {
			IFeed* result;
			error = backing->GetFeed(name, (IDispatch**)&result);
			iterationStep = new FeedFeed(result);
		} else if (folderExists == VARIANT_TRUE) {
			IFeedFolder* result;
			error = backing->GetSubfolder(name, (IDispatch**)&result);
			iterationStep = new FeedFolder(result);
		} else {
			iterationStep = new ErrorFeedElement(L"No such element");
		}
		SysFreeString(name);
	}
	
	
	if (parts.second.compare(L"") == 0) {
		return iterationStep;
	} else {
		FeedElement* retVal = iterationStep->cd(parts.second);
		delete iterationStep;
		return retVal;
	}
}

FeedElement* FeedFeed::cd(const wstring path) const {
	const pair<wstring, wstring> parts = splitPathString(path);
	FeedElement* iterationStep;
	
	HRESULT error;
	if (parts.first.compare(UP_ONE_LEVEL) == 0) {
		IFeedFolder* result;
		error = backing->get_Parent((IDispatch**)&result);
		if (error) {
			iterationStep = new ErrorFeedElement(L"Already top level; no parent to cd to");
		} else {
			iterationStep = new FeedFolder(result);
		}
	} else if (parts.first.compare(L".") == 0) {
		iterationStep = new FeedFeed(this->backing);
	} else {
		try {
			LONG id = stoi(parts.first);
			IFeedItem* result;
			error = backing->GetItem(id, (IDispatch**)&result);
			if (error) {
				iterationStep = new ErrorFeedElement(L"Unknown feed item number");
			} else {
				iterationStep = new FeedItem(result);
			}
		} catch (const std::invalid_argument& e) {
			iterationStep = new ErrorFeedElement(L"FeedItem names are integers");
		}
	}
	
	
	if (parts.second.compare(L"") == 0) {
		return iterationStep;
	} else {
		FeedElement* retVal = iterationStep->cd(parts.second);
		delete iterationStep;
		return retVal;
	}
}

FeedElement* FeedItem::cd(const wstring path) const {
	const pair<wstring, wstring> parts = splitPathString(path);
	FeedElement* iterationStep;
	
	HRESULT error;
	if (parts.first.compare(UP_ONE_LEVEL) == 0) {
		IFeed* result;
		error = backing->get_Parent((IDispatch**)&result);
		if (error) {
			iterationStep = new ErrorFeedElement(L"Already top level; no parent to cd to");
		} else {
			iterationStep = new FeedFeed(result);
		}
	} else if (parts.first.compare(L".") == 0) {
		iterationStep = new FeedItem(this->backing);
	} else {
		iterationStep = new ErrorFeedElement(L"No subelements of a feed item");
	}
	
	
	if (parts.second.compare(L"") == 0) {
		return iterationStep;
	} else {
		FeedElement* retVal = iterationStep->cd(parts.second);
		delete iterationStep;
		return retVal;
	}
}

FeedElement* ErrorFeedElement::cd(const wstring path) const {
	return new ErrorFeedElement(this->message);
}

wstring FeedFolder::getContentsString(const bool filterUnread) const {
	IFeedsEnum* currentFeeds;
	BSTR name;
	std::wostringstream retVal;
	char inbetween[MAX_STRING_SIZE];
	
	
	// folders
	backing->get_Subfolders((IDispatch**)&currentFeeds);
	
	LONG feedCount;
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeedFolder* currentFeed;
		currentFeeds->Item(i, (IDispatch**)&currentFeed);
		
		currentFeed->get_Name(&name);
		snprintf(inbetween, MAX_STRING_SIZE, "%ls/", (wchar_t *)name);
		retVal << inbetween << std::endl;
		SysFreeString(name);
		currentFeed->Release();
	}
	currentFeeds->Release();
	
	
	// not folders; feeds
	backing->get_Feeds((IDispatch**)&currentFeeds);
	
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeed* currentFeed;
		LONG unreadCount;
		currentFeeds->Item(i, (IDispatch**)&currentFeed);
		
		currentFeed->get_Name(&name);
		currentFeed->get_UnreadItemCount(&unreadCount);
		snprintf(inbetween, MAX_STRING_SIZE, "%ls (%ld)", (wchar_t *)name, unreadCount);
		retVal << inbetween << std::endl;
		SysFreeString(name);
		currentFeed->Release();
	}
	currentFeeds->Release();
	
	return retVal.str();
}

// print the contents of a feed
wstring FeedFeed::getContentsString(const bool filterUnread) const {
	IFeedsEnum* items;
	BSTR name;
	LONG localId;
	DATE pubDate;
	VARIANT_BOOL isRead;
	std::wostringstream retVal;
	char inbetween[MAX_STRING_SIZE];
	
	// The feed name
	backing->get_Name(&name);
	snprintf(inbetween, MAX_STRING_SIZE, " Feed: %ls", (wchar_t*) name);
	retVal << inbetween << std::endl;
	SysFreeString(name);
	
	// the feed items
	backing->get_Items((IDispatch**)&items);
	
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
			
			snprintf(inbetween, MAX_STRING_SIZE, "%4ld %5s %ls", localId, isReadMessage, (wchar_t *)name);
			retVal << inbetween << std::endl;
			
			SysFreeString(name);
		}
		
		curItem->Release();
	}
	items->Release();
	
	return retVal.str();
}

wstring FeedItem::getContentsString(const bool filterUnread) const {
	return L"No contents inside an item";
}

wstring ErrorFeedElement::getContentsString(const bool filterUnread) const {
	return this->message;
}

wstring FeedFolder::getDetailsString() const {
	return L"No details about a folder";
}

wstring FeedFeed::getDetailsString() const {
	return L"No details about a feed";
}

wstring FeedItem::getDetailsString() const {
	BSTR str;
	DATE pubDate;
	IDispatch* dispatch;
	std::wostringstream retVal;
	char inbetween[MAX_STRING_SIZE];
	
	backing->get_Title(&str);
	if (str) {
		snprintf(inbetween, MAX_STRING_SIZE, "\n%ls", str);
		retVal << inbetween << std::endl;
		SysFreeString(str);
	}
	
	backing->get_Author(&str);
	if (str) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Author:  %ls", str);
		retVal << inbetween << std::endl;
		SysFreeString(str);
	}
	
 //	backing->get_Enclosure(&dispatch);
 //	if (dispatch != NULL) {
 //		snprintf(inbetween, MAX_STRING_SIZE, "    Has Enclosure\n");
 //		retVal << inbetween << std::endl;
 //		dispatch->Release();
 //	}
	
	backing->get_PubDate(&pubDate);
	if (pubDate) {
		// snprintf(inbetween, MAX_STRING_SIZE, "    PubDate: %ls\n", (BSTR) pubDate);
	}
	
	backing->get_Modified(&pubDate);
	if (pubDate) {
		// snprintf(inbetween, MAX_STRING_SIZE, "    PubDate: %ls\n", (BSTR) pubDate);
	}
	
	snprintf(inbetween, MAX_STRING_SIZE, "\n");
	
	backing->get_Description(&str);
	if (str) {
		snprintf(inbetween, MAX_STRING_SIZE, "%ls", str);
		retVal << inbetween << std::endl << std::endl;
		SysFreeString(str);
	}
	
	return retVal.str();
}

wstring ErrorFeedElement::getDetailsString() const {
	return this->message;
}

wstring FeedFolder::getPath() const {
	BSTR  currFolderPath;
	backing->get_Path(&currFolderPath);
	wstring retVal((wchar_t*)currFolderPath);
	SysFreeString(currFolderPath);
	return retVal;
}

wstring FeedFeed::getPath() const {
	BSTR  currFolderPath;
	backing->get_Path(&currFolderPath);
	wstring retVal((wchar_t*)currFolderPath);
	SysFreeString(currFolderPath);
	return retVal;
}

wstring FeedItem::getPath() const {
	IFeed* parent;
	backing->get_Parent((IDispatch**)&parent);
	
	BSTR  parentPath;
	parent->get_Path(&parentPath);
	
	LONG localid;
	backing->get_LocalId(&localid);
	
	wchar_t retVal[MAX_STRING_SIZE];
	swprintf(retVal, MAX_STRING_SIZE, L"%ls\\%d", (wchar_t *)parentPath, localid);
	
	parent->Release();
	SysFreeString(parentPath);
	return retVal;
}

wstring ErrorFeedElement::getPath() const {
	return L"ERROR";
}

bool FeedFolder::isError() const { return false; }
bool FeedFeed::isError() const { return false; }
bool FeedItem::isError() const { return false; }
bool ErrorFeedElement::isError() const { return true; }
