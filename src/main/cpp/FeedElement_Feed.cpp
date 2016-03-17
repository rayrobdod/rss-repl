
#include <sstream>
#include "FeedElement.h"

using std::pair;
using std::wstring;


FeedFeed::FeedFeed(IFeed* backing) : backing(backing) {}


FeedElement* FeedFeed::getParent() const {
	HRESULT error;
	IFeedFolder* result;
	error = backing->get_Parent((IDispatch**)&result);
	if (error) {
		return new ErrorFeedElement(L"Already top level; no parent to cd to");
	} else {
		return new FeedFolder(result);
	}
}

FeedElement* FeedFeed::getChild(const wstring name) const {
	HRESULT error;

	try {
		LONG id = stoi(name);
		IFeedItem* result;
		error = backing->GetItem(id, (IDispatch**)&result);
		if (error) {
			return new ErrorFeedElement(L"Unknown feed item number");
		} else {
			return new FeedItem(result);
		}
	} catch (const std::invalid_argument& e) {
		return new ErrorFeedElement(L"FeedItem names are integers");
	}
}

FeedElement* FeedFeed::clone() const {
	return new FeedFeed(this->backing);
}

wstring FeedFeed::getContentsString(const bool filterUnread) const {
	IFeedsEnum* items;
	BSTR name;
	LONG localId;
	DATE pubDate;
	HRESULT error;
	VARIANT_BOOL isRead;
	std::wostringstream retVal;
	wchar_t inbetween[MAX_STRING_SIZE];
	
	// The feed name
	error = backing->get_Name(&name);
	if (SUCCEEDED(error)) {
		swprintf(inbetween, MAX_STRING_SIZE, L" Feed: %ls", (wchar_t*)name);
		retVal << inbetween << std::endl;
		SysFreeString(name);
	} else {
		retVal << " Feed: ???" << std::endl << std::endl;
	}
	
	// the feed items
	error = backing->get_Items((IDispatch**)&items);
	if (SUCCEEDED(error)) {

		LONG feedCount;
		error = items->get_Count(&feedCount);
		if (SUCCEEDED(error)) {
			for (int i = 0; i < feedCount; i++) {
				IFeedItem* curItem;
				items->Item(i, (IDispatch**)&curItem);
				curItem->get_IsRead(&isRead);

				if (!filterUnread || (isRead == VARIANT_FALSE)) {
					HRESULT error2;
					HRESULT error3;

					error = curItem->get_Title(&name);
					error2 = curItem->get_PubDate(&pubDate);
					error3 = curItem->get_LocalId(&localId);
					if (SUCCEEDED(error3) && SUCCEEDED(error)) {
						wchar_t* isReadMessage = (isRead ? L"" : L"<NEW>");

						swprintf(inbetween, MAX_STRING_SIZE, L"%4ld %5ls %ls", localId, isReadMessage, (wchar_t *)name);
						retVal << inbetween << std::endl;

						SysFreeString(name);
					} else {
						retVal << "???? COULD NOT READ ITEM" << std::endl;
					}
				}

				curItem->Release();
			}
		} else {
			retVal << "ERROR: could not read item count" << std::endl;
		}
		items->Release();
	} else {
		retVal << "ERROR: could not read feed items" << std::endl;
	}
	
	return retVal.str();
}

wstring FeedFeed::getDetailsString() const {
	BSTR str;
	LONG number;
	FEEDS_DOWNLOAD_STATUS dlstatus;
	FEEDS_DOWNLOAD_ERROR dlerror;
	DATE pubDate;
	std::wostringstream retVal;
	wchar_t inbetween[MAX_STRING_SIZE];
	HRESULT error;

	error = backing->get_Title(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"%ls", str);
		retVal << std::endl << inbetween << std::endl << std::endl;
		SysFreeString(str);
	}

	error = backing->get_UnreadItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Unread Item Count:  %d", number);
		retVal << inbetween << std::endl;
	}

	error = backing->get_ItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Item Count:  %d", number);
		retVal << inbetween << std::endl;
	}

	error = backing->get_MaxItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Max Item Count:  %d", number);
		retVal << inbetween << std::endl;
	}

	error = backing->get_DownloadStatus(&dlstatus);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Download Status: %ls", downloadStatus2String(dlstatus).c_str());
		retVal << inbetween << std::endl;
	}

	error = backing->get_LastDownloadError(&dlerror);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Download Error: %ls", downloadError2String(dlerror).c_str());
		retVal << inbetween << std::endl;
	}

	error = backing->get_PubDate(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			swprintf(inbetween, MAX_STRING_SIZE, L"    Published: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		} else {
			swprintf(inbetween, MAX_STRING_SIZE, L"    Published: ERROR");
			retVal << inbetween << std::endl;
		}
	}

	error = backing->get_LastBuildDate(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			swprintf(inbetween, MAX_STRING_SIZE, L"    Built: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		} else {
			swprintf(inbetween, MAX_STRING_SIZE, L"    Built: ERROR");
			retVal << inbetween << std::endl;
		}
	}

	error = backing->get_LastDownloadTime(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			swprintf(inbetween, MAX_STRING_SIZE, L"    Downloaded: %ls", str);
			retVal << inbetween << std::endl;
			SysFreeString(str);
		} else {
			swprintf(inbetween, MAX_STRING_SIZE, L"    Downloaded: ERROR");
			retVal << inbetween << std::endl;
		}
	}

	error = backing->get_Image(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Image: %ls", str);
		retVal << inbetween << std::endl;
	}

	error = backing->get_Link(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Link: %ls", str);
		retVal << inbetween << std::endl;
	}

	error = backing->get_DownloadUrl(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"    Download Url: %ls", str);
		retVal << inbetween << std::endl;
		SysFreeString(str);
	}

	retVal << std::endl;

	error = backing->get_Description(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		swprintf(inbetween, MAX_STRING_SIZE, L"%ls", str);
		retVal << inbetween << std::endl << std::endl;
		SysFreeString(str);
	}

	return retVal.str();
}

wstring FeedFeed::getPath() const {
	BSTR  currFolderPath;
	backing->get_Path(&currFolderPath);
	wstring retVal((wchar_t*)currFolderPath);
	SysFreeString(currFolderPath);
	return retVal;
}

bool FeedFeed::isError() const { return false; }

HRESULT FeedFeed::markAsRead() { return E_NOTIMPL; }

std::pair<HRESULT, std::wstring> FeedFeed::getAttachmentFile() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

std::pair<HRESULT, std::wstring> FeedFeed::getUrl() const {
	HRESULT result;
	BSTR bpath;

	result = backing->get_Url(&bpath);
	if (SUCCEEDED(result) && result != S_FALSE) {
		wchar_t wpath[MAX_STRING_SIZE];
		swprintf(wpath, MAX_STRING_SIZE, L"%s", bpath);
		SysFreeString(bpath);
		return std::pair<HRESULT, std::wstring>(S_OK, wpath);
	} else {
		return std::pair<HRESULT, std::wstring>(result, L"");
	}
}
