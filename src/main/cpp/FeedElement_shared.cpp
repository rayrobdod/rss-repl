
#include "FeedElement.h"


using std::wstring;
using std::pair;



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

FeedElement::FeedElement() {}


