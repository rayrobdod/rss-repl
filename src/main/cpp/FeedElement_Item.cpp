
#include "FeedElement.h"

using std::pair;
using std::wstring;


FeedItem::FeedItem(IFeedItem* backing) : backing(backing) {}


FeedElement* FeedItem::getParent() const {
	HRESULT error;
	IFeed* result;
	error = backing->get_Parent((IDispatch**)&result);
	if (error) {
		return new ErrorFeedElement(L"Already top level; no parent to cd to");
	} else {
		return new FeedFeed(result);
	}
}

FeedElement* FeedItem::getChild(const wstring name) const {
	return new ErrorFeedElement(L"No subelements of a feed item");
}

FeedElement* FeedItem::clone() const {
	return new FeedItem(this->backing);
}

void FeedItem::printContents(const bool filterUnread, std::wostream& out) const {
	out << L"No contents inside an item";
}

std::vector<wstring> FeedItem::getContents() const {
	return std::vector<wstring>();
}

void FeedItem::printDetails(std::wostream& out) const {
	BSTR str;
	DATE pubDate;
	VARIANT_BOOL isRead;
	IFeedEnclosure* enclosure;
	HRESULT error;
	
	error = backing->get_Title(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << std::endl << str << std::endl << std::endl;
		SysFreeString(str);
	}
	
	error = backing->get_Author(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Author: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_Link(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Url: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_Comments(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Comments: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_IsRead(&isRead);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Read: " << (isRead ? "TRUE" : "FALSE") << std::endl;
	}
	
	error = backing->get_Enclosure((IDispatch**) &enclosure);
	if (SUCCEEDED(error) && error != S_FALSE) {
		FEEDS_DOWNLOAD_STATUS dlstatus;
		FEEDS_DOWNLOAD_ERROR dlerror;

		out << INDENT << "Enclosure:" << std::endl;

		error = enclosure->get_Type(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Type: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_Url(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Url: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_DownloadStatus(&dlstatus);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Status: " << downloadStatus2String(dlstatus) << std::endl;
		}

		error = enclosure->get_LocalPath(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Local Path: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_LastDownloadError(&dlerror);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Error: " << downloadError2String(dlerror) << std::endl;
		}

		error = enclosure->get_DownloadMimeType(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Type: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_DownloadUrl(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Url: " << str << std::endl;
			SysFreeString(str);
		}


 		enclosure->Release();
 	}
	
	error = backing->get_PubDate(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			out << INDENT << "Published: " << str << std::endl;
			SysFreeString(str);
		} else {
			out << INDENT << "Published: " << "ERROR" << std::endl;
		}
	}
	
	error = backing->get_Modified(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			out << INDENT << "Modified: " << str << std::endl;
			SysFreeString(str);
		} else {
			out << INDENT << "Modified: " << "ERROR" << std::endl;
		}
	}

	out << std::endl;
	
	error = backing->get_Description(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << str << std::endl << std::endl;
		SysFreeString(str);
	}
}

wstring FeedItem::getPath() const {
	IFeed* parent;
	backing->get_Parent((IDispatch**)&parent);
	
	BSTR  parentPath;
	parent->get_Path(&parentPath);
	
	LONG localid;
	backing->get_LocalId(&localid);
	
	wchar_t retVal[STR_BUFFER_SIZE];
	swprintf(retVal, STR_BUFFER_SIZE, L"%ls\\%d", (wchar_t *)parentPath, localid);
	
	parent->Release();
	SysFreeString(parentPath);
	return retVal;
}

bool FeedItem::isError() const { return false; }

HRESULT FeedItem::markAsRead() {
	return backing->put_IsRead(VARIANT_TRUE);
}

std::pair<HRESULT, std::wstring> FeedItem::getAttachmentFile() const {
	FEEDS_DOWNLOAD_STATUS status;
	IFeedEnclosure* enclosure;
	HRESULT result;
	
	result = backing->get_Enclosure((IDispatch**)&enclosure);
	if (SUCCEEDED(result) && result != S_FALSE) {
		result = enclosure->get_DownloadStatus(&status);
		if (SUCCEEDED(result) && result != S_FALSE) {
			BSTR bpath;

			if (FDS_DOWNLOADED == status) {
				result = enclosure->get_LocalPath(&bpath);
			} else {
				result = enclosure->get_Url(&bpath);
			}

			if (SUCCEEDED(result) && result != S_FALSE) {
				wchar_t wpath[STR_BUFFER_SIZE];
				swprintf(wpath, STR_BUFFER_SIZE, L"%s", bpath);
				SysFreeString(bpath);
				return std::pair<HRESULT, std::wstring>(S_OK, wpath);
			} else {
				return std::pair<HRESULT, std::wstring>(result, L"");
			}

		} else {
			return std::pair<HRESULT, std::wstring>(result, L"");
		}
	} else {
		return std::pair<HRESULT, std::wstring>(result, L"");
	}
}

std::pair<HRESULT, std::wstring> FeedItem::getUrl() const {
	HRESULT result;
	BSTR bpath;

	result = backing->get_Link(&bpath);
	if (SUCCEEDED(result) && result != S_FALSE) {
		wchar_t wpath[STR_BUFFER_SIZE];
		swprintf(wpath, STR_BUFFER_SIZE, L"%s", bpath);
		SysFreeString(bpath);
		return std::pair<HRESULT, std::wstring>(S_OK, wpath);
	}
	else {
		return std::pair<HRESULT, std::wstring>(result, L"");
	}
}
