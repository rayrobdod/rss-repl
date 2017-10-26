
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

void FeedFolder::printContents(const bool filterUnread, std::wostream& out) const {
	IFeedsEnum* currentFeeds;
	BSTR name;
	HRESULT error;
	wchar_t inbetween[STR_BUFFER_SIZE];
	
	
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
				swprintf(inbetween, STR_BUFFER_SIZE, L"%ls/", (wchar_t *)name);
				out << inbetween << std::endl;
				SysFreeString(name);
			} else {
				out << "ERROR" << std::endl;
			}
			currentFeed->Release();
		} else {
			out << "ERROR" << std::endl;
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
					swprintf(inbetween, STR_BUFFER_SIZE, L"%ls (%ld)", (wchar_t *)name, unreadCount);
					out << inbetween << std::endl;
				} else {
					swprintf(inbetween, STR_BUFFER_SIZE, L"%ls (???)", (wchar_t *)name);
					out << inbetween << std::endl;
				}
			} else {
				out << "ERROR" << std::endl;
			}
			SysFreeString(name);
			currentFeed->Release();
		} else {
			out << "ERROR" << std::endl;
		}
	}
	currentFeeds->Release();
}

std::vector<wstring> FeedFolder::getContents() const {
	IFeedsEnum* currentFeeds;
	BSTR name;
	HRESULT error;
	std::vector<std::wstring> retVal;
	
	
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
				retVal.push_back(name);
			} else {
			}
			SysFreeString(name);
			currentFeed->Release();
		} else {
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
				retVal.push_back(name);
			} else {
			}
			SysFreeString(name);
			currentFeed->Release();
		} else {
		}
	}
	currentFeeds->Release();
	
	
	return retVal;
}

void FeedFolder::printDetails(std::wostream& out) const {
	out << L"No details about a folder" << std::endl;
}

wstring FeedFolder::getPath() const {
	BSTR  currFolderPath;
	backing->get_Path(&currFolderPath);
	wstring retVal((wchar_t*)currFolderPath);
	SysFreeString(currFolderPath);
	return retVal;
}

bool FeedFolder::isError() const { return false; }

std::pair<HRESULT, std::wstring> FeedFolder::getAttachmentFile() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

std::pair<HRESULT, std::wstring> FeedFolder::getUrl() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

HRESULT FeedFolder::markAsRead() { return E_NOTIMPL; }

HRESULT FeedFolder::attachImageFromDescription() { return E_NOTIMPL; }

HRESULT FeedFolder::downloadAttachmentAsync() { return E_NOTIMPL; }
