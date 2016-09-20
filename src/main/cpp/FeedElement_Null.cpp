
#include "FeedElement.h"

using std::pair;
using std::wstring;


ErrorFeedElement::ErrorFeedElement(const wstring message) : message(message) {}

FeedElement* ErrorFeedElement::getParent() const {
	return new ErrorFeedElement(this->message);
}

FeedElement* ErrorFeedElement::getChild(const wstring name) const {
	return new ErrorFeedElement(this->message);
}

FeedElement* ErrorFeedElement::clone() const {
	return new ErrorFeedElement(this->message);
}

void ErrorFeedElement::printContents(const bool filterUnread, std::wostream& out) const {
	out << this->message;
}

std::vector<wstring> ErrorFeedElement::getContents() const {
	return std::vector<wstring>();
}

void ErrorFeedElement::printDetails(std::wostream& out) const {
	out << this->message;
}

wstring ErrorFeedElement::getPath() const {
	return L"ERROR";
}

bool ErrorFeedElement::isError() const { return true; }

std::pair<HRESULT, std::wstring> ErrorFeedElement::getAttachmentFile() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

std::pair<HRESULT, std::wstring> ErrorFeedElement::getUrl() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

HRESULT ErrorFeedElement::markAsRead() { return E_NOTIMPL; }

HRESULT ErrorFeedElement::attachImageFromDescription() { return E_NOTIMPL; }
