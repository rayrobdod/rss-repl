// the normal `command.cpp` includes a statically allocated map which 
// isn't really a problem, I think, but the `_CRT` memory leak checker
// catches it and includes that map in the memory leak report. And
// I don't care about that.

#include "command.h"
#include <unordered_set>
#include <unordered_map>
#include <iomanip>	// std::setw

using std::wstring;
using std::vector;

ProcessCommandReturnValue processCommand(std::shared_ptr<FeedElement> currentFolder, const std::vector<std::wstring> commandString, std::wostream& out) {

	if (commandString.size() == 0 || commandString[0].compare(L"") == 0) {
		// do nothing
		return std::make_tuple(false, currentFolder);
	} else {
		if (commandString[0].compare(L"exit") == 0) {
			return std::make_tuple(true, currentFolder);
		} else {
			out << "Unknown Command" << std::endl;
			return std::make_tuple(false, currentFolder);
		}
	}
}


std::vector<wstring> commandNames() {
	return std::vector<wstring>({L"exit"});
}
