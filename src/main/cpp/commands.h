#ifndef COMMANDS_H
#define COMMANDS_H

#include <msfeeds.h>
#include <cwchar>
#include <string>

using std::wstring;

class FeedElement {
 public:
	FeedElement();
	virtual FeedElement* cd(const wstring path) const = 0;
	virtual wstring getPath() const = 0;
	virtual wstring getDetailsString() const = 0;
	virtual wstring getContentsString(const bool filterUnread) const = 0;
	virtual bool isError() const = 0;
};

class FeedFolder : public FeedElement {
 public:
	FeedFolder(IFeedFolder*);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
	virtual bool isError() const;
 private:
	IFeedFolder* const backing;
};

class FeedFeed : public FeedElement {
 public:
	FeedFeed(IFeed*);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
	virtual bool isError() const;
 private:
	IFeed* const backing;
};

class FeedItem : public FeedElement {
public:
	FeedItem(IFeedItem*);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
	virtual bool isError() const;
private:
	IFeedItem* const backing;
};

/**
 * Effectively a null object. Used when `FeedElement::cd` would otherwise have nothing useful to return.
 */
class ErrorFeedElement : public FeedElement {
public:
	ErrorFeedElement(const wstring message);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
	virtual bool isError() const;
private:
	const wstring message;
};


#endif        //  #ifndef COMMANDS_H
