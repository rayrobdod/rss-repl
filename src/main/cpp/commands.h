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
};

class FeedFolder : public FeedElement {
 public:
	FeedFolder(IFeedFolder*);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
 private:
	IFeedFolder* backing;
};

class FeedFeed : public FeedElement {
 public:
	FeedFeed(IFeed*);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
 private:
	IFeed* backing;
};

class FeedItem : public FeedElement {
 public:
	FeedItem(IFeedItem*);
	virtual FeedElement* cd(const wstring path) const;
	virtual wstring getPath() const;
	virtual wstring getDetailsString() const;
	virtual wstring getContentsString(const bool filterUnread) const;
 private:
	IFeedItem* backing;
};


#endif        //  #ifndef COMMANDS_H
