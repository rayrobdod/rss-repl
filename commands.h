#ifndef COMMANDS_H
#define COMMANDS_H

#include <msfeeds.h>
#include <cwchar>





// dir
void printFolder(IFeedFolder* folder);

// cd
IFeedFolder* changeDirectory(IFeedFolder* base, const wchar_t* const path);

#endif        //  #ifndef COMMANDS_H

