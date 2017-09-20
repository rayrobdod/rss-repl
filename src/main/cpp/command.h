#ifndef COMMAND_H
#define COMMAND_H

#include <tuple>
#include <iostream>
#include "FeedElement.h"

typedef std::tuple<bool, FeedElement*>
	ProcessCommandReturnValue;

/**
 * Perform an operation based on the commandstring
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

/**
 * Returns the names of the avaliable commands
 */
std::vector<std::wstring> commandNames();

#endif        //  #ifndef COMMAND_H
