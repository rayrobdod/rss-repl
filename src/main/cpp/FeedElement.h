#ifndef FEED_ELEMENT_H
#define FEED_ELEMENT_H

#include <msfeeds.h>
#include <cwchar>
#include <string>
#include <utility> /** std::pair */

const size_t MAX_PARAM_COUNT = 10;
const size_t BUFFER_SIZE = 1024;
const size_t MAX_STRING_SIZE = 1024;


/** Returns a string that representing the status */
std::wstring downloadStatus2String(const FEEDS_DOWNLOAD_STATUS status);

/** Returns a string that representing the status */
std::wstring downloadError2String(const FEEDS_DOWNLOAD_ERROR status);


class FeedElement {
 public:
	FeedElement();
	/** Returns the FeedElement obtained by starting at `base` and following `path`. */
	FeedElement* followPath(const std::wstring path) const;
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
 protected:
	/** Returns the parent object */
	virtual FeedElement* getParent() const = 0;
	/** Returns the child object with the specified name */
	virtual FeedElement* getChild(const std::wstring name) const = 0;
	virtual FeedElement* clone() const = 0;
};

class FeedFolder : public FeedElement {
 public:
	/** @ref This takes possession of the IFeedFolder */
	FeedFolder(IFeedFolder*);
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
 protected:
	virtual FeedElement* getParent() const;
	virtual FeedElement* getChild(const std::wstring name) const;
	virtual FeedElement* clone() const;
 private:
	IFeedFolder* const backing;
};

class FeedFeed : public FeedElement {
 public:
	 /** @ref This takes possession of the IFeedFolder */
	FeedFeed(IFeed*);
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
 protected:
	virtual FeedElement* getParent() const;
	virtual FeedElement* getChild(const std::wstring name) const;
	virtual FeedElement* clone() const;
 private:
	IFeed* const backing;
};

class FeedItem : public FeedElement {
 public:
	/** @ref This takes possession of the IFeedFolder */
	FeedItem(IFeedItem*);
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
 protected:
	virtual FeedElement* getParent() const;
	virtual FeedElement* getChild(const std::wstring name) const;
	virtual FeedElement* clone() const;
 private:
	IFeedItem* const backing;
};

/**
 * Effectively a null object. Used when `FeedElement::cd` would otherwise have nothing useful to return.
 */
class ErrorFeedElement : public FeedElement {
 public:
	ErrorFeedElement(const std::wstring message);
	virtual std::wstring getPath() const;
	virtual std::wstring getDetailsString() const;
	virtual std::wstring getContentsString(const bool filterUnread) const;
	virtual HRESULT markAsRead();
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
 protected:
	virtual FeedElement* getParent() const;
	virtual FeedElement* getChild(const std::wstring name) const;
	virtual FeedElement* clone() const;
 private:
	const std::wstring message;
};


/** Returns the global root folder in the feeds hiearchy */
FeedFolder* getRootFolder();



#endif        //  #ifndef FEED_ELEMENT_H