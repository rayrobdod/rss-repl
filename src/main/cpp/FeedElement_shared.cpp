
#include "FeedElement.h"
#include "SplitStringIterator.h"

using std::wstring;
using std::pair;
using std::literals::string_literals::operator""s;


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
		case FDE_DOWNLOAD_SIZE_LIMIT_EXCEEDED: return L"The file was larger than the maximum download size";
		case FDE_ACCESS_DENIED: return L"ACCESS_DENIED";
		case FDE_AUTH_FAILED: return L"AUTH_FAILED";
		case FDE_INVALID_AUTH: return L"INVALID_AUTH";
		default: return L"Unknown";
	}
}

std::shared_ptr<FeedElement> getRootFolder() {
	CComPtr<IFeedsManager> manager;
	CComPtr<IFeedFolder> backing;
	
	manager.CoCreateInstance(CLSID_FeedsManager);
	manager->get_RootFolder((IDispatch**)&backing);
	
	return std::make_shared<FeedFolder>(backing);
}


FeedElement::FeedElement() {}
FeedElement::~FeedElement() {}

std::shared_ptr<FeedElement> FeedElement::followPath(const wstring path) const {
	const wstring CD_PARENT = L"..";
	const wstring CD_SELF = L".";
	const wstring SEPARATORS = L"/\\";

	std::shared_ptr<FeedElement> current;
	if (find(SEPARATORS.cbegin(), SEPARATORS.cend(), path[0]) != SEPARATORS.cend()) {
		current = getRootFolder();
	} else {
		current = this->clone();
	}

	for (SplitStringIterator i(path, SEPARATORS); i != SplitStringIterator::end(); ++i) {
		if (*i == CD_SELF) {
			// do nothing
		} else if (*i == CD_PARENT) {
			current = current->getParent();
		} else {
			current = current->getChild(*i);
		}
	}
	return current;
}

std::wstring get_dir_line(CComPtr<IFeedItem> item) {
	BSTR name;
	LONG localId;
	VARIANT_BOOL isRead;
	wchar_t retval[STR_BUFFER_SIZE];

	HRESULT e1 = item->get_IsRead(&isRead);
	HRESULT e2 = item->get_Title(&name);
	HRESULT e3 = item->get_LocalId(&localId);
	if (SUCCEEDED(e1) && SUCCEEDED(e2) && SUCCEEDED(e3)) {
		const wchar_t* const isReadMessage = (isRead ? L"" : L"<NEW>");

		wstring name_normalized; {
			SplitStringIterator name_normalizer((wstring)(wchar_t*)name, L" \n\t"s);
			for (auto i = name_normalizer; i != SplitStringIterator::end(); ++i) {
				name_normalized += *i;
				name_normalized += L" ";
			}
		}

		swprintf(retval, STR_BUFFER_SIZE, L"%5ld %5ls %ls", localId, isReadMessage, name_normalized.c_str());
	} else {
		swprintf(retval, STR_BUFFER_SIZE, L"%5ls %5ls %ls", L"????", L"", L"COULD NOT READ ITEM");
	}

	if (SUCCEEDED(e2)) {SysFreeString(name);}
	return retval;
}
