
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

using std::wstring;
using std::vector;

typedef std::tuple<bool, FeedElement*>
	ProcessCommandReturnValue;


/**
 * @param currentFolder the folder to act upon. This method will then own the pointer.
 * @param command the entered command
 * @param out the output stream to print output to
 * @return [0] true iff the program should exit
 * @return [1] the new folder to act upon
 */
ProcessCommandReturnValue processCommand(FeedElement* currentFolder, const std::vector<std::wstring> command, std::wostream& out) {
	
	if (command[0].compare(L"") == 0) {
		// do nothing
		
	} else if (command[0].compare(END_LOOP) == 0) {
		// do nothing
		
	} else if (command[0].compare(SHOW_CONTENTS) == 0) {
		const wstring path = (command.size() > 1 ? command[1] : L".");
		const bool filterNew = (command.size() > 2 ? (command[2].compare(L"-n") == 0) : false);
		
		FeedElement* targetFolder = currentFolder->followPath(path);
		
		out << targetFolder->getContentsString(filterNew) << std::endl;
		delete targetFolder;
		
	} else if (command[0].compare(OPEN_INTERNAL) == 0) {
		const wstring path = (command.size() > 1 ? command[1] : L".");
		
		FeedElement* targetFolder = currentFolder->followPath(path);
		out << targetFolder->getDetailsString() << std::endl;
		delete targetFolder;
		
	} else if (command[0].compare(OPEN_EXTERNAL_ATTACHMENT) == 0) {
		const wstring path = (command.size() > 1 ? command[1] : L".");
		
		FeedElement* targetFolder = currentFolder->followPath(path);
		if (targetFolder->isError()) {
			out << "No such element" << std::endl;
		} else {
			std::pair<HRESULT, std::wstring> res = targetFolder->getAttachmentFile();
			if (res.first == S_FALSE) {
				out << "Element does not contain attachment" << std::endl;
			} else if (res.first == E_NOTIMPL) {
				out << "Not a thing that can contain an attachment" << std::endl;
			} else if (SUCCEEDED(res.first)) {
				ShellExecute(GetConsoleWindow(), L"open", res.second.c_str(), NULL, NULL, SW_SHOWNORMAL);
				out << "Attachment opened." << std::endl;
			} else {
				out << "Failed to open attachment" << std::endl;
			}
		}
		delete targetFolder;
		
	} else if (command[0].compare(OPEN_EXTERNAL) == 0) {
		const wstring path = (command.size() > 1 ? command[1] : L".");
		
		FeedElement* targetFolder = currentFolder->followPath(path);
		if (targetFolder->isError()) {
			out << "No such element" << std::endl;
		} else {
			std::pair<HRESULT, std::wstring> res = targetFolder->getUrl();
			if (res.first == S_FALSE) {
				out << "Element does not contain attachment" << std::endl;
			} else if (res.first == E_NOTIMPL) {
				out << "Not a thing that can contain an attachment" << std::endl;
			} else if (SUCCEEDED(res.first)) {
				ShellExecute(GetConsoleWindow(), L"open", res.second.c_str(), NULL, NULL, SW_SHOWNORMAL);
				out << "Attachment opened." << std::endl;
			} else {
				out << "Failed to open attachment" << std::endl;
			}
		}
		delete targetFolder;
		
	} else if (command[0].compare(MARK_READ) == 0) {
		const wstring path = (command.size() > 1 ? command[1] : L".");
		
		FeedElement* targetFolder = currentFolder->followPath(path);
		HRESULT result = targetFolder->markAsRead();
		if (SUCCEEDED(result)) {
			out << "Mark as read succeeded" << std::endl;
		} else {
			out << "Mark as read failed" << std::endl;
		}
		delete targetFolder;
		
	} else if (command[0].compare(L"echo") == 0) {
		for (size_t i = 0; i < command.size(); i++) {
			out << "\t" << command[i] << std::endl;
		}
		
	} else if (command[0].compare(CHANGE_DIRECTORY) == 0) {
		const wstring path = (command.size() > 1 ? command[1] : L".");
		
		FeedElement* newFolder = currentFolder->followPath(path);
		if (newFolder->isError()) {
			out << newFolder->getDetailsString() << std::endl;
			delete newFolder;
		} else {
			delete currentFolder;
			currentFolder = newFolder;
		}
	} else {
		out << "Unknown Command" << std::endl;
	}
	
	return std::make_tuple(command[0].compare(END_LOOP) == 0, currentFolder);
}