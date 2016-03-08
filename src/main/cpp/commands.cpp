
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

/** Returns a string that representing the status */
wstring downloadStatus2String(const FEEDS_DOWNLOAD_STATUS status) {
	switch (status) {
		case FDS_NONE: return L"Not Attempted";
		case FDS_PENDING: return L"Pending";
		case FDS_DOWNLOADING: return L"Downloading";
		case FDS_DOWNLOADED: return L"Success";
		case FDS_DOWNLOAD_FAILED: return L"Failed";
		default: return L"Unknown";
	}
}

/** Returns a string that representing the status */
wstring downloadError2String(const FEEDS_DOWNLOAD_ERROR status) {
	switch (status) {
		case FDE_NONE: return L"Successful";
		case FDE_DOWNLOAD_FAILED: return L"Download failed";
		case FDE_INVALID_FEED_FORMAT: return L"Feed is in an unsupported format";
		case FDE_NORMALIZATION_FAILED: return L"Feed has xml errors";
		case FDE_PERSISTENCE_FAILED: return L"Could not save file to disk";
		case FDE_DOWNLOAD_BLOCKED: return L"The security manager blocked the download";
		case FDE_CANCELED: return L"The download was interrupted";
		case FDE_UNSUPPORTED_AUTH: return L"UNSUPPORTED_AUTH";
		case FDE_BACKGROUND_DOWNLOAD_DISABLED: return L"The background download service was disabled";
		case FDE_NOT_EXIST: return L"The feed no longer exists";
		case FDE_UNSUPPORTED_MSXML: return L"UNSUPPORTED_MSXML";
		case FDE_UNSUPPORTED_DTD: return L"Feed contains a DTD";
		case FDE_DOWNLOAD_SIZE_LIMIT_EXCEEDED: "The XML source of the feed exceeds the maximum allowed size.";
		case FDE_ACCESS_DENIED: return L"ACCESS_DENIED";
		case FDE_AUTH_FAILED: return L"AUTH_FAILED";
		case FDE_INVALID_AUTH: return L"INVALID_AUTH";
		default: return L"Unknown";
	}
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
	BSTR str;
	LONG number;
	FEEDS_DOWNLOAD_STATUS dlstatus;
	FEEDS_DOWNLOAD_ERROR dlerror;
	DATE pubDate;
	std::wostringstream retVal;
	char inbetween[MAX_STRING_SIZE];
	HRESULT error;

	error = backing->get_Title(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "%ls", str);
		retVal << std::endl << inbetween << std::endl << std::endl;
		SysFreeString(str);
	}

	error = backing->get_UnreadItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Unread Item Count:  %d", number);
		retVal << inbetween << std::endl;
	}

	error = backing->get_ItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Item Count:  %d", number);
		retVal << inbetween << std::endl;
	}

	error = backing->get_MaxItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Max Item Count:  %d", number);
		retVal << inbetween << std::endl;
	}

	error = backing->get_DownloadStatus(&dlstatus);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Download Status: %ls", downloadStatus2String(dlstatus).c_str());
		retVal << inbetween << std::endl;
	}

	error = backing->get_LastDownloadError(&dlerror);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Download Error: %ls", downloadError2String(dlerror).c_str());
		retVal << inbetween << std::endl;
	}

	error = backing->get_Image(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Image: %ls", str);
		retVal << inbetween << std::endl;
	}

	error = backing->get_Link(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Link: %ls", str);
		retVal << inbetween << std::endl;
	}

	error = backing->get_DownloadUrl(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Download Url: %ls", str);
		retVal << inbetween << std::endl;
		SysFreeString(str);
	}

	retVal << std::endl;

	error = backing->get_Description(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "%ls", str);
		retVal << inbetween << std::endl << std::endl;
		SysFreeString(str);
	}

	return retVal.str();
}

wstring FeedItem::getDetailsString() const {
	BSTR str;
	DATE pubDate;
	IFeedEnclosure* enclosure;
	std::wostringstream retVal;
	char inbetween[MAX_STRING_SIZE];
	HRESULT error;
	
	error = backing->get_Title(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "%ls", str);
		retVal << std::endl << inbetween << std::endl << std::endl;
		SysFreeString(str);
	}
	
	error = backing->get_Author(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		snprintf(inbetween, MAX_STRING_SIZE, "    Author:  %ls", str);
		retVal << inbetween << std::endl;
		SysFreeString(str);
	}
	
	error = backing->get_Enclosure((IDispatch**) &enclosure);
	if (SUCCEEDED(error) && error != S_FALSE) {
		FEEDS_DOWNLOAD_STATUS dlstatus;
		FEEDS_DOWNLOAD_ERROR dlerror;

		retVal << "    Enclosure:" << std::endl;

		error = enclosure->get_Type(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Type: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_Url(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Url:  %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_DownloadStatus(&dlstatus);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Download Status: %ls", downloadStatus2String(dlstatus).c_str());
			retVal << inbetween << std::endl;
		}

		error = enclosure->get_LocalPath(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Local Path: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_LastDownloadError(&dlerror);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Download Error: %ls", downloadError2String(dlerror).c_str());
			retVal << inbetween << std::endl;
		}

		error = enclosure->get_DownloadMimeType(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Download Type: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_DownloadUrl(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			snprintf(inbetween, MAX_STRING_SIZE, "        Download Url: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		}


 		enclosure->Release();
 	}
	
	error = backing->get_PubDate(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		// snprintf(inbetween, MAX_STRING_SIZE, "    PubDate: %ls\n", (BSTR) pubDate);
	}
	
	error = backing->get_Modified(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		// snprintf(inbetween, MAX_STRING_SIZE, "    PubDate: %ls\n", (BSTR) pubDate);
	}

	retVal << std::endl;
	
	error = backing->get_Description(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
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
