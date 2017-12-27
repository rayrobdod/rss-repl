
#include "FeedElement.h"
#include <regex>

using std::pair;
using std::wstring;
using std::literals::string_literals::operator""s;


FeedFeed::FeedFeed(CComPtr<IFeed> backing) : backing(backing) {}


std::shared_ptr<FeedElement> FeedFeed::getParent() const {
	HRESULT error;
	CComPtr<IFeedFolder> result;
	error = backing->get_Parent((IDispatch**)&result);
	if (error) {
		return std::make_shared<ErrorFeedElement>(L"Already top level; no parent to cd to");
	} else {
		return std::make_shared<FeedFolder>(result);
	}
}

std::shared_ptr<FeedElement> FeedFeed::getChild(const wstring name) const {
	const std::wregex item(L"\\d{1,6}"s);
	const std::wregex range(L"(\\d{1,6})\\-(\\d{1,6})"s);
	std::wcmatch results;
	HRESULT error;

	if (std::regex_match(name.c_str(), results, item)) {
		LONG id = stoi(results[0]);
		CComPtr<IFeedItem> result;
		error = backing->GetItem(id, (IDispatch**)&result);
		if (SUCCEEDED(error)) {
			return std::make_shared<FeedItem>(result);
		} else {
			return std::make_shared<ErrorFeedElement>(L"Unknown feed item number");
		}

	} else if (std::regex_match(name.c_str(), results, range)) {
		LONG low = stoi(results[1]);
		LONG high = stoi(results[2]);
		std::vector<LONG> range;
		for (LONG i = low; i <= high; ++i) {
			CComPtr<IFeedItem> result;
			error = backing->GetItem(i, (IDispatch**)&result);
			if (SUCCEEDED(error)) {
				range.push_back(i);
			} else {
				// don't include item if item does not exist
			}
		}
		return std::make_shared<FeedItemGroup>(backing, range);

	} else {
		return std::make_shared<ErrorFeedElement>(L"Child element not found");
	}
}

std::shared_ptr<FeedElement> FeedFeed::clone() const {
	return std::make_shared<FeedFeed>(this->backing);
}

void FeedFeed::printContents(const bool filterUnread, std::wostream& out) const {
	CComPtr<IFeedsEnum> items;
	BSTR name;
	HRESULT error;
	VARIANT_BOOL isRead;
	
	// The feed name
	error = backing->get_Name(&name);
	if (SUCCEEDED(error)) {
		out << " Feed: " << name << std::endl << std::endl;
		SysFreeString(name);
	} else {
		out << " Feed: " << "???" << std::endl << std::endl;
	}
	
	// the feed items
	error = backing->get_Items((IDispatch**)&items);
	if (SUCCEEDED(error)) {

		LONG feedCount;
		error = items->get_Count(&feedCount);
		if (SUCCEEDED(error)) {
			for (int i = 0; i < feedCount; i++) {
				CComPtr<IFeedItem> curItem;
				items->Item(i, (IDispatch**)&curItem);
				curItem->get_IsRead(&isRead);

				if (!filterUnread || (isRead == VARIANT_FALSE)) {
					out << get_dir_line(curItem) << std::endl;
				}
			}
		} else {
			out << "ERROR: could not read item count" << std::endl;
		}
	} else {
		out << "ERROR: could not read feed items" << std::endl;
	}
}

std::vector<wstring> FeedFeed::getContents() const {
	CComPtr<IFeedsEnum> items;
	LONG localId;
	HRESULT error;
	std::vector<wstring> retVal;
	wchar_t inbetween[STR_BUFFER_SIZE];

	// the feed items
	error = backing->get_Items((IDispatch**)&items);
	if (SUCCEEDED(error)) {

		LONG feedCount;
		error = items->get_Count(&feedCount);
		if (SUCCEEDED(error)) {
			for (int i = 0; i < feedCount; i++) {
				CComPtr<IFeedItem> curItem;
				error = items->Item(i, (IDispatch**)&curItem);
				if (SUCCEEDED(error)) {
					error = curItem->get_LocalId(&localId);
					if (SUCCEEDED(error)) {
						swprintf(inbetween, STR_BUFFER_SIZE, L"%ld", localId);
						retVal.push_back(inbetween);
					}
				}
			}
		}
	}
	else {
	}

	return retVal;
}

void FeedFeed::printDetails(std::wostream& out) const {
	BSTR str;
	LONG number;
	FEEDS_DOWNLOAD_STATUS dlstatus;
	FEEDS_DOWNLOAD_ERROR dlerror;
	DATE pubDate;
	HRESULT error;

	error = backing->get_Title(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << std::endl << str << std::endl << std::endl;
		SysFreeString(str);
	}

	error = backing->get_UnreadItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Unread Item Count: " << number << std::endl;
	}

	error = backing->get_ItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Item Count: " << number << std::endl;
	}

	error = backing->get_MaxItemCount(&number);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Max Item Count: " << number << std::endl;
	}

	error = backing->get_DownloadStatus(&dlstatus);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Download Status: " << downloadStatus2String(dlstatus) << std::endl;
	}

	error = backing->get_LastDownloadError(&dlerror);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Download Error: " << downloadError2String(dlerror) << std::endl;
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

	error = backing->get_LastBuildDate(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			out << INDENT << "Built: " << str << std::endl;
			SysFreeString(str);
		} else {
			out << INDENT << "Built: " << "ERROR" << std::endl;
		}
	}

	error = backing->get_LastDownloadTime(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			out << INDENT << "Downloaded: " << str << std::endl;
			SysFreeString(str);
		} else {
			out << INDENT << "Downloaded: " << "ERROR" << std::endl;
		}
	}

	error = backing->get_Image(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Image: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_Link(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Link: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_DownloadUrl(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Download Url: " << str << std::endl;
		SysFreeString(str);
	}

	out << std::endl;

	error = backing->get_Description(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << str << std::endl;
		SysFreeString(str);
	}
}

wstring FeedFeed::getPath() const {
	BSTR  currFolderPath;
	backing->get_Path(&currFolderPath);
	wstring retVal((wchar_t*)currFolderPath);
	SysFreeString(currFolderPath);
	return retVal;
}

bool FeedFeed::isError() const { return false; }

std::pair<HRESULT, std::wstring> FeedFeed::getAttachmentFile() const {
	return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L"");
}

std::pair<HRESULT, std::wstring> FeedFeed::getUrl() const {
	HRESULT result;
	BSTR bpath;

	result = backing->get_Url(&bpath);
	if (SUCCEEDED(result) && result != S_FALSE) {
		wchar_t wpath[STR_BUFFER_SIZE];
		swprintf(wpath, STR_BUFFER_SIZE, L"%s", bpath);
		SysFreeString(bpath);
		return std::pair<HRESULT, std::wstring>(S_OK, wpath);
	} else {
		return std::pair<HRESULT, std::wstring>(result, L"");
	}
}

HRESULT FeedFeed::markAsRead() { return E_NOTIMPL; }

HRESULT FeedFeed::attachImageFromDescription() {
	std::vector<wstring> childrenNames = this->getContents();
	HRESULT finalResult = S_OK;

	for (auto i = childrenNames.cbegin(); i < childrenNames.cend(); ++i) {
		std::shared_ptr<FeedElement> child = this->getChild(*i);
		HRESULT thisResult = child->attachImageFromDescription();

		if (SUCCEEDED(finalResult) && FAILED(thisResult)) {
			finalResult = thisResult;
		}
	}

	return finalResult;
}

HRESULT FeedFeed::downloadAttachmentAsync() {
	std::vector<wstring> childrenNames = this->getContents();
	HRESULT finalResult = S_OK;

	for (auto i = childrenNames.cbegin(); i < childrenNames.cend(); ++i) {
		std::shared_ptr<FeedElement> child = this->getChild(*i);
		HRESULT thisResult = child->downloadAttachmentAsync();

		if (SUCCEEDED(finalResult) && FAILED(thisResult)) {
			finalResult = thisResult;
		}
	}

	return finalResult;
}
