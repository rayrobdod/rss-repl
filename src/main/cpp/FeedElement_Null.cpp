
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

wstring ErrorFeedElement::getContentsString(const bool filterUnread) const {
	return this->message;
}

wstring ErrorFeedElement::getDetailsString() const {
	return this->message;
}

wstring ErrorFeedElement::getPath() const {
	return L"ERROR";
}

bool ErrorFeedElement::isError() const { return true; }

HRESULT ErrorFeedElement::markAsRead() { return E_NOTIMPL; }

std::pair<HRESULT, std::wstring> ErrorFeedElement::getAttachmentFile() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }

std::pair<HRESULT, std::wstring> ErrorFeedElement::getUrl() const { return std::pair<HRESULT, std::wstring>(E_NOTIMPL, L""); }