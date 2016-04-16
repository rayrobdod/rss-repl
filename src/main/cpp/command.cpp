
#include "command.h"


using std::wstring;
using std::vector;

ProcessCommandReturnValue processCommand(FeedElement* const currentFolder, const std::vector<std::wstring> command, std::wostream& out) {
	
	if (command.size() == 0 || command[0].compare(L"") == 0) {
		// do nothing
		
	} else if (command[0].compare(END_LOOP) == 0) {
		return std::make_tuple(true, currentFolder);
		
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
			return std::make_tuple(false, newFolder);
		}
	} else {
		out << "Unknown Command" << std::endl;
	}

	return std::make_tuple(false, currentFolder);
}