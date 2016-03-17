
#include <sstream>
#include "FeedElement.h"

using std::pair;
using std::wstring;

FeedFolder::FeedFolder(IFeedFolder* backing) : backing(backing) {}

/*
 0x80070003 // ERROR_PATH_NOT_FOUND
 0x80070005 // ERROR_ACCESS_DENIED
*/
FeedElement* FeedFolder::getParent() const {
	HRESULT error;
	IFeedFolder* result;
	error = backing->get_Parent((IDispatch**)&result);
	if (error) {
		return new ErrorFeedElement(L"Already top level; no parent to cd to");
	} else {
		return new FeedFolder(result);
	}
}

FeedElement* FeedFolder::getChild(const wstring name2) const {
	FeedElement* iterationStep;
	HRESULT error;
	VARIANT_BOOL feedExists;
	VARIANT_BOOL folderExists;
	BSTR name = SysAllocString(name2.c_str());
	
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
	return iterationStep;
}

FeedElement* FeedFolder::clone() const {
	return new FeedFolder(this->backing);
}

wstring FeedFolder::getContentsString(const bool filterUnread) const {
	IFeedsEnum* currentFeeds;
	BSTR name;
	HRESULT error;
	std::wostringstream retVal;
	wchar_t inbetween[MAX_STRING_SIZE];
	
	
	// folders
	backing->get_Subfolders((IDispatch**)&currentFeeds);
	
	LONG feedCount;
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeedFolder* currentFeed;
		error = currentFeeds->Item(i, (IDispatch**)&currentFeed);
		
		if (SUCCEEDED(error)) {
			error = currentFeed->get_Name(&name);
			if (SUCCEEDED(error)) {
				swprintf(inbetween, MAX_STRING_SIZE, L"%ls/", (wchar_t *)name);
				retVal << inbetween << std::endl;
				SysFreeString(name);
			} else {
				retVal << "ERROR" << std::endl;
			}
			currentFeed->Release();
		} else {
			retVal << "ERROR" << std::endl;
		}
	}
	currentFeeds->Release();
	
	
	// not folders; feeds
	backing->get_Feeds((IDispatch**)&currentFeeds);
	
	currentFeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		IFeed* currentFeed;
		LONG unreadCount;
		error = currentFeeds->Item(i, (IDispatch**)&currentFeed);

		if (SUCCEEDED(error)) {
			error = currentFeed->get_Name(&name);
			if (SUCCEEDED(error)) {
				error = currentFeed->get_UnreadItemCount(&unreadCount);
				if (SUCCEEDED(error)) {
					swprintf(inbetween, MAX_STRING_SIZE, L"%ls (%ld)", (wchar_t *)name, unreadCount);
					retVal << inbetween << std::endl;
				} else {
					swprintf(inbetween, MAX_STRING_SIZE, L"%ls (???)", (wchar_t *)name);
					retVal << inbetween << std::endl;
				}
			} else {
				retVal << "ERROR" << std::endl;
			}
			SysFreeString(name);
			currentFeed->Release();
		} else {
			retVal << "ERROR" << std::endl;
		}
	}
	currentFeeds->Release();
	
	return retVal.str();
}

wstring FeedFolder::getDetailsString() const {
	return L"No details about a folder";
}

wstring FeedFolder::getPath() const {
	BSTR  currFolderPath;
	backing->get_Path(&currFolderPath);
	wstring retVal((wchar_t*)currFolderPath);
	SysFreeString(currFolderPath);
	return retVal;
}

bool FeedFolder::isError() const { return false; }

HRESULT FeedFolder::markAsRead() { return E_NOTIMPL; }

std::pair<HRESULT, std::wstring> FeedFolder::getAttachmentFile() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

std::pair<HRESULT, std::wstring> FeedFolder::getUrl() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }
