#ifndef COMMAND_H
#define COMMAND_H

#include <tuple>
#include <iostream>
#include "FeedElement.h"

#define END_LOOP	L"exit"
#define SHOW_CONTENTS	L"dir"
#define OPEN_INTERNAL	L"print"
#define OPEN_EXTERNAL	L"open"
#define OPEN_EXTERNAL_ATTACHMENT	L"open_attachment"
#define CHANGE_DIRECTORY	L"cd"
#define MAKE_DIRECTORY	L"md"
#define FEED_INFO	L"info"
#define MARK_READ	L"markAsRead"

typedef std::tuple<bool, FeedElement*>
	ProcessCommandReturnValue;

/**
 * @param currentFolder the folder to act upon. This method will then own the pointer.
 * @param command the entered command
 * @param out the output stream to print output to
 * @return [0] true iff the program should exit
 * @return [1] the new folder to act upon
 */
ProcessCommandReturnValue processCommand(
	FeedElement* const currentFolder,
	const std::vector<std::wstring> command,
	std::wostream& out
);

#endif        //  #ifndef COMMAND_H
