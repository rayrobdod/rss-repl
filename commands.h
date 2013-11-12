#ifndef COMMANDS_H
#define COMMANDS_H

#include <msfeeds.h>
#include <cwchar>

union AnyFeedType {
	IFeedFolder* folder;
	IFeed*       feed;
	IFeedItem*   item;
};

struct AnyFeedThing { 
	enum {IFEEDFOLDER, IFEED, IFEEDITEM} type;
	AnyFeedType item;
};



// dir Feed
void printFeed(IFeed* base);

// dir Folder
void printFolder(IFeedFolder* folder);

// cd
IFeedFolder* changeDirectory(IFeedFolder* base, const wchar_t* const path);


#endif        //  #ifndef COMMANDS_H
