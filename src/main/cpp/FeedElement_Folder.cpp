
#include "FeedElement.h"
#include "loadResources.h"
#include "..\\resource\\string_table_keys.h"

using std::pair;
using std::wstring;

FeedFolder::FeedFolder(CComPtr<IFeedFolder> backing) : backing(backing) {}

/*
 0x80070003 // ERROR_PATH_NOT_FOUND
 0x80070005 // ERROR_ACCESS_DENIED
*/
std::shared_ptr<FeedElement> FeedFolder::getParent() const {
	HRESULT error;
	CComPtr<IFeedFolder> result;
	error = backing->get_Parent((IDispatch**)&result);
	if (error) {
		return std::make_shared<ErrorFeedElement>(LoadStringRrdStlW(IDS_ERR_TOP_LEVEL));
	} else {
		return std::make_shared<FeedFolder>(result);
	}
}

std::shared_ptr<FeedElement> FeedFolder::getChild(const wstring name2) const {
	std::shared_ptr<FeedElement> iterationStep;
	HRESULT error;
	VARIANT_BOOL feedExists;
	VARIANT_BOOL folderExists;
	BSTR name = SysAllocString(name2.c_str());
	
	error = backing->ExistsFeed(name, &feedExists);
	error = backing->ExistsSubfolder(name, &folderExists);
	
	if (feedExists == VARIANT_TRUE) {
		CComPtr<IFeed> result;
		error = backing->GetFeed(name, (IDispatch**)&result);
		iterationStep = std::make_shared<FeedFeed>(result);
	} else if (folderExists == VARIANT_TRUE) {
		CComPtr<IFeedFolder> result;
		error = backing->GetSubfolder(name, (IDispatch**)&result);
		iterationStep = std::make_shared<FeedFolder>(result);
	} else {
		iterationStep = std::make_shared<ErrorFeedElement>(LoadStringRrdStlW(IDS_ERR_NO_SUCH_CHILD));
	}
	SysFreeString(name);
	return iterationStep;
}

std::shared_ptr<FeedElement> FeedFolder::clone() const {
	return std::make_shared<FeedFolder>(this->backing);
}

void FeedFolder::printContents(const bool filterUnread, std::wostream& out) const {
	CComPtr<IFeedsEnum> subfolders;
	CComPtr<IFeedsEnum> subfeeds;
	BSTR name;
	HRESULT error;
	wchar_t inbetween[STR_BUFFER_SIZE];
	
	
	// folders
	backing->get_Subfolders((IDispatch**)&subfolders);
	
	LONG feedCount;
	subfolders->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		CComPtr<IFeedFolder> currentFeed;
		error = subfolders->Item(i, (IDispatch**)&currentFeed);
		
		if (SUCCEEDED(error)) {
			error = currentFeed->get_Name(&name);
			if (SUCCEEDED(error)) {
				swprintf(inbetween, STR_BUFFER_SIZE, L"%ls/", (wchar_t *)name);
				out << inbetween << std::endl;
				SysFreeString(name);
			} else {
				out << LoadStringRrdStlW(ERROR) << std::endl;
			}
		} else {
			out << LoadStringRrdStlW(ERROR) << std::endl;
		}
	}
	
	
	// not folders; feeds
	backing->get_Feeds((IDispatch**)&subfeeds);
	
	subfeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		CComPtr<IFeed> currentFeed;
		LONG unreadCount;
		error = subfeeds->Item(i, (IDispatch**)&currentFeed);

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
				out << LoadStringRrdStlW(ERROR) << std::endl;
			}
			SysFreeString(name);
		} else {
			out << LoadStringRrdStlW(ERROR) << std::endl;
		}
	}
}

std::vector<wstring> FeedFolder::getContents() const {
	CComPtr<IFeedsEnum> subfolders;
	CComPtr<IFeedsEnum> subfeeds;
	BSTR name;
	HRESULT error;
	std::vector<std::wstring> retVal;
	
	
	// folders
	backing->get_Subfolders((IDispatch**)&subfolders);
	
	LONG feedCount;
	subfolders->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		CComPtr<IFeedFolder> currentFeed;
		error = subfolders->Item(i, (IDispatch**)&currentFeed);
		
		if (SUCCEEDED(error)) {
			error = currentFeed->get_Name(&name);
			if (SUCCEEDED(error)) {
				retVal.push_back(name);
			} else {
			}
			SysFreeString(name);
		} else {
		}
	}
	
	
	// not folders; feeds
	backing->get_Feeds((IDispatch**)&subfeeds);
	
	subfeeds->get_Count(&feedCount);
	for (int i = 0; i < feedCount; i++) {
		CComPtr<IFeed> currentFeed;
		LONG unreadCount;
		error = subfeeds->Item(i, (IDispatch**)&currentFeed);

		if (SUCCEEDED(error)) {
			error = currentFeed->get_Name(&name);
			if (SUCCEEDED(error)) {
				retVal.push_back(name);
			} else {
			}
			SysFreeString(name);
		} else {
		}
	}
	
	return retVal;
}

void FeedFolder::printDetails(std::wostream& out) const {
	out << LoadStringRrdStlW(IDS_ERR_FOLDER_DETAILS_UNKNOWN) << std::endl;
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
