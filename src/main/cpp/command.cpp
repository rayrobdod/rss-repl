#include "command.h"
#include <unordered_set>
#include <unordered_map>
#include <iomanip>	// std::setw

using std::wstring;
using std::vector;

struct CommandString {
	std::unordered_map<wstring, wstring> flags;
	std::vector<wstring> positional;
};

typedef ProcessCommandReturnValue(*CommandFunction)(FeedElement* const currentFolder, CommandString command, std::wostream& out);

// Determine which arguments are flags and which are positional arguments
CommandString extractFlags(const vector<wstring> command) {
	std::unordered_map<wstring, wstring> flags;
	std::vector<wstring> positional;
	
	for (auto i = command.cbegin(); i != command.cend(); ++i) {
		if (i->size() > 0 && L'/' == i->at(0)) {
			size_t splitIndex = i->find(':');
			if (splitIndex != std::string::npos) {
				// +/- 1 is to exclude the ':' from the parsed result
				flags.insert({ i->substr(1, splitIndex - 1), i->substr(splitIndex + 1) });
			} else {
				flags.insert({ i->substr(1), i->substr(1) });
			}
		} else {
			wstring i2 = *i;
			positional.push_back( i2 );
		}
	}
	
	if (positional.empty()) {
		wstring a = L"help";
		positional.push_back(a);
	}
	
	return CommandString{ flags = flags, positional = positional };
}

const std::unordered_map<wstring, std::tuple<wstring, CommandFunction>> replCommands{
	std::make_pair(L"exit", std::make_tuple(L"Exit the program", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		return std::make_tuple(true, currentFolder);
	})),
	std::make_pair(L"dir", std::make_tuple(L"Show the contents of the current directory", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");
		const bool filterNew = (command.flags.count(L"n") > 0 ? true : false);

		FeedElement* targetFolder = currentFolder->followPath(path);
		targetFolder->printContents(filterNew, out);
		out << std::endl;
		delete targetFolder;
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"print", std::make_tuple(L"Print details about the specified item", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

		FeedElement* const targetFolder = currentFolder->followPath(path);
		targetFolder->printDetails(out);
		out << std::endl;
		delete targetFolder;
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"open_attachment", std::make_tuple(L"Open the attachment in an extermal program", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

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
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"open", std::make_tuple(L"Print the contents of an item", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

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
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"markAsRead", std::make_tuple(L"Mark an item as read", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

		FeedElement* targetFolder = currentFolder->followPath(path);
		HRESULT result = targetFolder->markAsRead();
		if (SUCCEEDED(result)) {
			out << "Mark as read succeeded" << std::endl;
		} else {
			out << "Mark as read failed" << std::endl;
		}
		delete targetFolder;
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"attachImageFromDescription", std::make_tuple(L"If the item's html includes a <img>, make that image an attachment", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

		FeedElement* targetFolder = currentFolder->followPath(path);
		HRESULT result = targetFolder->attachImageFromDescription();
		if (result == S_FALSE) {
			out << "No image found to attach" << std::endl;
		} else if (result == E_NOTIMPL) {
			out << "No attachments in this type of thing" << std::endl;
		} else if (SUCCEEDED(result)) {
			out << "Attachment succeeded" << std::endl;
		} else {
			out << "Attachment failed" << std::endl;
		}
		delete targetFolder;
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"download_attachment", std::make_tuple(L"Asynchronously download the item's attachment", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

		FeedElement* targetFolder = currentFolder->followPath(path);
		HRESULT result = targetFolder->downloadAttachmentAsync();
		if (result == S_FALSE) {
			out << "S_FALSE" << std::endl;
		} else if (result == E_NOTIMPL) {
			out << "E_NOTIMPL" << std::endl;
		} else if (SUCCEEDED(result)) {
			out << "started async download" << std::endl;
		} else {
			out << "failed" << std::endl;
		}
		delete targetFolder;
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"echo", std::make_tuple(L"Echo arguments to command line", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		out << "Flags:" << std::endl;
		for (auto i = command.flags.cbegin(); i != command.flags.cend(); ++i) {
			out << "\t" << i->first << " -> " << i->second << std::endl;
		}
		out << "Positional:" << std::endl;
		for (auto i = command.positional.cbegin(); i != command.positional.cend(); ++i) {
			out << "\t" << *i << std::endl;
		}
		return std::make_tuple(false, currentFolder);
	})),
	std::make_pair(L"cd", std::make_tuple(L"Change directory", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		const wstring path = (command.positional.size() > 1 ? command.positional[1] : L".");

		FeedElement* newFolder = currentFolder->followPath(path);
		if (newFolder->isError()) {
			newFolder->printDetails(out);
			out << std::endl;
			delete newFolder;
			return std::make_tuple(false, currentFolder);
		} else {
			delete currentFolder;
			return std::make_tuple(false, newFolder);
		}
	})),
	std::make_pair(L"help", std::make_tuple(L"Print a list of commands", [](FeedElement* const currentFolder, CommandString command, std::wostream& out) {
		for (auto i = replCommands.cbegin(); i != replCommands.cend(); ++i) {
			auto name = i->first;
			auto description = std::get<0>(i->second);

			out <<
				"  " <<
				std::setw(10) << std::setiosflags(std::ios_base::left) << name <<
				std::setw(0) << " " << description <<
				std::endl;
		}
		return std::make_tuple(false, currentFolder);
	}))
};

ProcessCommandReturnValue processCommand(FeedElement* const currentFolder, const std::vector<std::wstring> commandString, std::wostream& out) {

	if (commandString.size() == 0 || commandString[0].compare(L"") == 0) {
		// do nothing
		return std::make_tuple(false, currentFolder);
	} else {
		auto commandString2 = extractFlags(commandString);
		auto commandData = replCommands.find(commandString2.positional[0]);
		
		if (commandData != replCommands.end() ) {
			auto commandValue = commandData->second;
			auto commandFunction = std::get<1>(commandValue);

			return commandFunction(currentFolder, commandString2, out);
			
		} else {
			out << "Unknown Command" << std::endl;
			return std::make_tuple(true, currentFolder);
		}
	}
}

// An iterator for extracting the keys from `replCommands` using `replCommands`'s native iterator
class key_iterator : public std::iterator<std::input_iterator_tag, std::wstring> {
private:
	std::unordered_map<wstring, std::tuple<wstring, CommandFunction>>::const_iterator current;
public:
	key_iterator(std::unordered_map<wstring, std::tuple<wstring, CommandFunction>>::const_iterator init) : current(init) {}
	bool operator==(const key_iterator& other) const {return this->current == other.current;}
	bool operator!=(const key_iterator& other) const {return this->current != other.current;}
	std::wstring operator*() const {return current->first;}
	key_iterator& operator++() {current++; return *this;}
	key_iterator operator++(int) {key_iterator tmp(*this); this->operator++(); return tmp;}
};

std::vector<wstring> commandNames() {
	return std::vector<wstring>(
		key_iterator(replCommands.cbegin()),
		key_iterator(replCommands.cend())
	);
}
