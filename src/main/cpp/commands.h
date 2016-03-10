#ifndef COMMANDS_H
#define COMMANDS_H

#include <msfeeds.h>
#include <cwchar>
#include <string>

class FeedElement {
 public:
	FeedElement();
	/** Returns the object obtained by following the specified path */
	virtual FeedElement* cd(const std::wstring path) const = 0;
	/** Returns the object's path */
	virtual std::wstring getPath() const = 0;
	/** Returns a string describing the object's contents */
	virtual std::wstring getDetailsString() const = 0;
	/** Returns a string describing the object */
	virtual std::wstring getContentsString(const bool filterUnread) const = 0;
	/**
	 * Attempts to mark the item as read
	 * @return S_OK if succeeded; E_NOTIMPL if the item cannot be marked as read; maybe others
	 */
	virtual HRESULT markAsRead() = 0;
	/** Returns a path (either a local file or a url) that represents the attachment location */
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const = 0;
	/** Returns a url representing the item */
	virtual std::pair<HRESULT, std::wstring> getUrl() const = 0;
	/** True if this is to be treated as the error object */
	virtual bool isError() const = 0;
};

class FeedFolder : public FeedElement {
 public:
	/** @ref This takes possession of the IFeedFolder */
	FeedFolder(IFeedFolder*);
	virtual FeedElement* cd(const std::wstring path) const;
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
 private:
	IFeedFolder* const backing;
};

class FeedFeed : public FeedElement {
 public:
	 /** @ref This takes possession of the IFeedFolder */
	FeedFeed(IFeed*);
	virtual FeedElement* cd(const std::wstring path) const;
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
 private:
	IFeed* const backing;
};

class FeedItem : public FeedElement {
public:
	/** @ref This takes possession of the IFeedFolder */
	FeedItem(IFeedItem*);
	virtual FeedElement* cd(const std::wstring path) const;
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
private:
	IFeedItem* const backing;
};

/**
 * Effectively a null object. Used when `FeedElement::cd` would otherwise have nothing useful to return.
 */
class ErrorFeedElement : public FeedElement {
public:
	ErrorFeedElement(const std::wstring message);
	virtual FeedElement* cd(const std::wstring path) const;
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
private:
	const std::wstring message;
};


#endif        //  #ifndef COMMANDS_H
