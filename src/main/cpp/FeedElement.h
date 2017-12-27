#ifndef FEED_ELEMENT_H
#define FEED_ELEMENT_H

#include <msfeeds.h>
#include <string>
#include <utility> /** std::pair */
#include <vector>
#include <memory>
#include <iostream>
#include <atlcomcli.h>

const size_t STR_BUFFER_SIZE = 1024;
const std::wstring INDENT = L"    ";


/**
 * Returns a string that representing the status
 */
std::wstring downloadStatus2String(const FEEDS_DOWNLOAD_STATUS status);

/**
 * Returns a string that representing the status
 */
std::wstring downloadError2String(const FEEDS_DOWNLOAD_ERROR status);


class FeedElement {
 public:
	FeedElement();
	virtual ~FeedElement();
	
	/** 
	 * Returns the FeedElement obtained by starting at `base` and following `path`.
	 */
	std::shared_ptr<FeedElement> followPath(const std::wstring path) const;
	
	/** 
	 * Returns the object's path in the program's file system
	 */
	virtual std::wstring getPath() const = 0;
	
	/** 
	 * Returns a string describing the object
	 */
	virtual void printDetails(std::wostream& out) const = 0;
	
	/** 
	 * Returns a string describing the object's contents
	 */
	virtual void printContents(const bool filterUnread, std::wostream& out) const = 0;
	
	/** 
	 * Returns a list of the object's contents
	 */
	virtual std::vector<std::wstring> getContents() const = 0;

	/**
	 * Returns a path (either a local file or a url) that represents the attachment location
	 */
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const = 0;
	
	/** 
	 * Returns a url representing the item
	 */
	virtual std::pair<HRESULT, std::wstring> getUrl() const = 0;
	
	/** 
	 * True if this is to be treated as the error object
	 */
	virtual bool isError() const = 0;
	
	/**
	 * Attempts to mark the item as read
	 * @return S_OK if succeeded; E_NOTIMPL if the item cannot be marked as read; maybe others
	 */
	virtual HRESULT markAsRead() = 0;
	
	/**
	 * Find an image in this's description and make that image this's attachment.
	 * Will do nothing if this already has an attachment.
	 */
	virtual HRESULT attachImageFromDescription() = 0;
	
	/**
	 * Start an asynchronous download of this's attachments
	 */
	virtual HRESULT downloadAttachmentAsync() = 0;
 protected:
	
	/** 
	 * Returns the parent object
	 */
	virtual std::shared_ptr<FeedElement> getParent() const = 0;
	
	/** 
	 * Returns the child object with the specified name
	 */
	virtual std::shared_ptr<FeedElement> getChild(const std::wstring name) const = 0;
	
	// Yes, I'm using smart pointers everywhere. That does not mean that
	// `FeedElement::followPath` has access to that smart pointer.
	virtual std::shared_ptr<FeedElement> clone() const = 0;
};

class FeedFolder : public FeedElement {
 public:
	FeedFolder(CComPtr<IFeedFolder>);
	virtual std::wstring getPath() const;
	virtual void printDetails(std::wostream& out) const;
	virtual void printContents(const bool filterUnread, std::wostream& out) const;
	virtual std::vector<std::wstring> getContents() const;
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
	
	virtual HRESULT markAsRead();
	virtual HRESULT attachImageFromDescription();
	virtual HRESULT downloadAttachmentAsync();
 protected:
	virtual std::shared_ptr<FeedElement> getParent() const;
	virtual std::shared_ptr<FeedElement> getChild(const std::wstring name) const;
	virtual std::shared_ptr<FeedElement> clone() const;
 private:
	CComPtr<IFeedFolder> const backing;
};

class FeedFeed : public FeedElement {
 public:
	FeedFeed(CComPtr<IFeed>);
	virtual std::wstring getPath() const;
	virtual void printDetails(std::wostream& out) const;
	virtual void printContents(const bool filterUnread, std::wostream& out) const;
	virtual std::vector<std::wstring> getContents() const;
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
	
	virtual HRESULT markAsRead();
	virtual HRESULT attachImageFromDescription();
	virtual HRESULT downloadAttachmentAsync();
 protected:
	virtual std::shared_ptr<FeedElement> getParent() const;
	virtual std::shared_ptr<FeedElement> getChild(const std::wstring name) const;
	virtual std::shared_ptr<FeedElement> clone() const;
 private:
	CComPtr<IFeed> backing;
};

class FeedItem : public FeedElement {
 public:
	FeedItem(CComPtr<IFeedItem>);
	virtual std::wstring getPath() const;
	virtual void printDetails(std::wostream& out) const;
	virtual void printContents(const bool filterUnread, std::wostream& out) const;
	virtual std::vector<std::wstring> getContents() const;
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
	
	virtual HRESULT markAsRead();
	virtual HRESULT attachImageFromDescription();
	virtual HRESULT downloadAttachmentAsync();
 protected:
	virtual std::shared_ptr<FeedElement> getParent() const;
	virtual std::shared_ptr<FeedElement> getChild(const std::wstring name) const;
	virtual std::shared_ptr<FeedElement> clone() const;
 private:
	CComPtr<IFeedItem> backing;
};

class FeedItemGroup : public FeedElement {
 public:
	FeedItemGroup(CComPtr<IFeed>, std::vector<LONG>);
	virtual std::wstring getPath() const;
	virtual void printDetails(std::wostream& out) const;
	virtual void printContents(const bool filterUnread, std::wostream& out) const;
	virtual std::vector<std::wstring> getContents() const;
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
	
	virtual HRESULT markAsRead();
	virtual HRESULT attachImageFromDescription();
	virtual HRESULT downloadAttachmentAsync();
 protected:
	virtual std::shared_ptr<FeedElement> getParent() const;
	virtual std::shared_ptr<FeedElement> getChild(const std::wstring name) const;
	virtual std::shared_ptr<FeedElement> clone() const;
 private:
	CComPtr<IFeed> const backing;
	std::vector<LONG> const indexes;
};

/**
 * Effectively a null object. Used when `FeedElement::cd` would otherwise have nothing useful to return.
 */
class ErrorFeedElement : public FeedElement {
 public:
	ErrorFeedElement(const std::wstring message);
	virtual std::wstring getPath() const;
	virtual void printDetails(std::wostream& out) const;
	virtual void printContents(const bool filterUnread, std::wostream& out) const;
	virtual std::vector<std::wstring> getContents() const;
	virtual std::pair<HRESULT, std::wstring> getAttachmentFile() const;
	virtual std::pair<HRESULT, std::wstring> getUrl() const;
	virtual bool isError() const;
	
	virtual HRESULT markAsRead();
	virtual HRESULT attachImageFromDescription();
	virtual HRESULT downloadAttachmentAsync();
 protected:
	virtual std::shared_ptr<FeedElement> getParent() const;
	virtual std::shared_ptr<FeedElement> getChild(const std::wstring name) const;
	virtual std::shared_ptr<FeedElement> clone() const;
 private:
	const std::wstring message;
};

/**
 * Returns the global root folder in the feeds hierarchy
 */
std::shared_ptr<FeedElement> getRootFolder();


#endif        //  #ifndef FEED_ELEMENT_H
