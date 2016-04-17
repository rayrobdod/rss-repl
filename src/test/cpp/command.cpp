#include "CppUnitTest.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/command.cpp"
#include "../../main/cpp/FeedElement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::wostringstream;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {
	template<> inline std::wstring ToString<FeedElement>(FeedElement* t) {
		char buffer [50];
		snprintf( buffer, 50, "FeedElement %x", t );
		RETURN_WIDE_STRING( buffer );
	}
} } }

namespace Tests
{
	// FeedElement interface declares several things const; this cannot be a member of StubFeedElement
	std::vector<std::wstring> calls;
	
	class StubFeedElement : public FeedElement {
	 public:
		StubFeedElement() {}
		StubFeedElement(const StubFeedElement& other) {}
		std::wstring getPath() const {calls.push_back(L"getPath"); return L"";}
		void printDetails(std::wostream& out) const {calls.push_back(L"getDetailsString"); }
		void printContents(const bool filterUnread, std::wostream& out) const { calls.push_back(L"getContentsString"); }
		std::vector<std::wstring> getContents() const {calls.push_back(L"getContents"); return std::vector<wstring>();}
		HRESULT markAsRead() {calls.push_back(L"markAsRead"); return S_OK;}
		std::pair<HRESULT, std::wstring> getAttachmentFile() const {calls.push_back(L"getAttachmentFile"); return std::make_pair(S_FALSE, L"");}
		std::pair<HRESULT, std::wstring> getUrl() const {calls.push_back(L"getUrl"); return std::make_pair(S_FALSE, L"");}
		bool isError() const {calls.push_back(L"isError"); return false;}
	 protected:
		FeedElement* getParent() const {calls.push_back(L"getParent"); return new StubFeedElement(*this);}
		FeedElement* getChild(const std::wstring name) const {calls.push_back(L"getChild"); return new StubFeedElement(*this);}
		FeedElement* clone() const {calls.push_back(L"clone"); return new StubFeedElement(*this);}
	};

	TEST_CLASS(processCommand)
	{
	public:

		TEST_METHOD(whenEmptyVector_thenElemNotTouched) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(0, calls.size(), L"elem recieved method calls");
			
			Assert::AreEqual(L"", out.str().c_str());
			Assert::IsFalse(std::get<0>(res), L"return[0] was true");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}
		
		TEST_METHOD(whenEmptyCommand_thenElemNotTouched) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(L"");
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(0, calls.size(), L"elem recieved method calls");
			
			Assert::AreEqual(L"", out.str().c_str());
			Assert::IsFalse(std::get<0>(res), L"return[0] was true");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}

		TEST_METHOD(whenExitCommand_thenElemNotTouched) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(END_LOOP);
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(0, calls.size(), L"elem recieved method calls");
			
			Assert::AreEqual(L"", out.str().c_str());
			Assert::IsTrue(std::get<0>(res), L"return[0] was false");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}

		TEST_METHOD(whenShowContents) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(SHOW_CONTENTS);
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(2, calls.size(), L"elem recieved method calls");
			Assert::AreEqual(L"clone", calls[0].c_str());
			Assert::AreEqual(L"getContentsString", calls[1].c_str());
			
			Assert::IsFalse(std::get<0>(res), L"return[0] was false");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}

		TEST_METHOD(whenShowContentsWithSubdir) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(SHOW_CONTENTS);
			command.push_back(L"12345");
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(3, calls.size());
			Assert::AreEqual(L"clone", calls[0].c_str());
			Assert::AreEqual(L"getChild", calls[1].c_str());
			Assert::AreEqual(L"getContentsString", calls[2].c_str());
			
			Assert::IsFalse(std::get<0>(res), L"return[0] was false");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}

		TEST_METHOD(whenOpenInternal) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(OPEN_INTERNAL);
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(2, calls.size());
			Assert::AreEqual(L"clone", calls[0].c_str());
			Assert::AreEqual(L"getDetailsString", calls[1].c_str());
			
			Assert::IsFalse(std::get<0>(res), L"return[0] was false");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}

		TEST_METHOD(whenMarkAsRead) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(MARK_READ);
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(2, calls.size());
			Assert::AreEqual(L"clone", calls[0].c_str());
			Assert::AreEqual(L"markAsRead", calls[1].c_str());
			
			Assert::IsFalse(std::get<0>(res), L"return[0] was false");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}

		TEST_METHOD(whenUnkown) {
			calls.clear();
			wostringstream out;
			StubFeedElement elem;
			vector<wstring> command;
			command.push_back(L"ARGLEBARGLE");
			auto res = ::processCommand(&elem, command, out);
			Assert::AreEqual<size_t>(0, calls.size());
			
			Assert::AreNotEqual(wstring::npos, out.str().find(L"Unknown Command"));
			Assert::IsFalse(std::get<0>(res), L"return[0] was false");
			Assert::IsTrue(&elem == std::get<1>(res), L"return[1] was not input feedelem");
		}
	};
}
