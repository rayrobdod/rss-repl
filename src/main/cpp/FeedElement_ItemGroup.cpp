
#include "FeedElement.h"
#include "loadResources.h"
#include "..\\resource\\string_table_keys.h"
#include <regex>

using std::pair;
using std::wstring;
using std::literals::string_literals::operator""s;


FeedItemGroup::FeedItemGroup(CComPtr<IFeed> backing, std::vector<LONG> indexes) : backing(backing), indexes(indexes) {}


std::shared_ptr<FeedElement> FeedItemGroup::getParent() const {
	return std::make_shared<FeedFeed>(backing);
}

std::shared_ptr<FeedElement> FeedItemGroup::getChild(const wstring name) const {
	const std::wregex item(L"\\d{1,6}"s);
	const std::wregex range(L"(\\d{1,6})\\-(\\d{1,6})"s);
	std::wcmatch results;
	HRESULT error;

	if (std::regex_match(name.c_str(), results, item)) {
		LONG id = stoi(results[0]);
		if (std::count(indexes.cbegin(), indexes.cend(), id)) {
			CComPtr<IFeedItem> result;
			error = backing->GetItem(id, (IDispatch**)&result);
			if (SUCCEEDED(error)) {
				return std::make_shared<FeedItem>(result);
			} else {
				return std::make_shared<ErrorFeedElement>(L"Unknown feed item number");
			}
		} else {
			return std::make_shared<ErrorFeedElement>(L"Child element not found");
		}

	} else if (std::regex_match(name.c_str(), results, range)) {
		LONG low = stoi(results[1]);
		LONG high = stoi(results[2]);
		std::vector<LONG> range;
		for (LONG i = low; i <= high; ++i) {
			if (std::count(indexes.cbegin(), indexes.cend(), i)) {
				range.push_back(i);
			}
		}
		return std::make_shared<FeedItemGroup>(backing, range);

	} else {
		return std::make_shared<ErrorFeedElement>(L"Child element not found");
	}
}

std::shared_ptr<FeedElement> FeedItemGroup::clone() const {
	return std::make_shared<FeedItemGroup>(this->backing, this->indexes);
}

void FeedItemGroup::printContents(const bool filterUnread, std::wostream& out) const {
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

	for (auto iter = indexes.cbegin(); iter != indexes.cend(); ++iter) {
		CComPtr<IFeedItem> curItem;
		error = backing->GetItem(*iter, (IDispatch**)&curItem);
		if (SUCCEEDED(error)) {
			curItem->get_IsRead(&isRead);

			if (!filterUnread || (isRead == VARIANT_FALSE)) {
				out << get_dir_line(curItem) << std::endl;
			}
		} else {
			out << "???? COULD NOT READ ITEM" << std::endl;
		}
	}
}

std::vector<wstring> FeedItemGroup::getContents() const {
	std::vector<wstring> retval;
	wchar_t inbetween[STR_BUFFER_SIZE];

	for (auto iter = indexes.cbegin(); iter != indexes.cend(); ++iter) {
		LONG id = *iter;

		swprintf(inbetween, STR_BUFFER_SIZE, L"%ld", id);
		retval.push_back(inbetween);
	}

	return retval;
}

void FeedItemGroup::printDetails(std::wostream& out) const {
	std::vector<wstring> childrenNames = this->getContents();
	for (auto i = childrenNames.cbegin(); i < childrenNames.cend(); ++i) {
		std::shared_ptr<FeedElement> child = this->getChild(*i);
		child->printDetails(out);
		out << std::endl << std::endl << "--------------------" << std::endl << std::endl;
	}
}

wstring FeedItemGroup::getPath() const {
	wstring branch = this->getParent()->getPath();
	wstring leaf = L"\\(group)";
	return branch + leaf;
}

bool FeedItemGroup::isError() const { return false; }

std::pair<HRESULT, std::wstring> FeedItemGroup::getAttachmentFile() const {
	return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L"");
}

std::pair<HRESULT, std::wstring> FeedItemGroup::getUrl() const {
	return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L"");
}

HRESULT FeedItemGroup::delet() { return E_NOTIMPL; }

HRESULT FeedItemGroup::markAsRead() {
	std::vector<wstring> childrenNames = this->getContents();
	HRESULT finalResult = S_OK;

	for (auto i = childrenNames.cbegin(); i < childrenNames.cend(); ++i) {
		std::shared_ptr<FeedElement> child = this->getChild(*i);
		HRESULT thisResult = child->markAsRead();

		if (SUCCEEDED(finalResult) && FAILED(thisResult)) {
			finalResult = thisResult;
		}
	}

	return finalResult;
}

HRESULT FeedItemGroup::attachImageFromDescription() {
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

HRESULT FeedItemGroup::downloadAttachmentAsync() {
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
