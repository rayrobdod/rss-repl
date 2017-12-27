#include "CppUnitTest.h"
#include "SimpleFeedElems.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/FeedElement_Folder.cpp"
#include "../../main/cpp/FeedElement_Item.cpp"
#include "../../main/cpp/FeedElement_ItemGroup.cpp"
#include "../../main/cpp/FeedElement_shared.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	FeedItemBstr(Title);
	FeedItemBstr(Link);

	
	
	TEST_CLASS(FeedItemTest)
	{
	public:

		TEST_METHOD(FeedItem_DetailsContainsTitle) {
			TitleFeedItem backing(L"The Title");
			std::wostringstream stream;
			::FeedItem dut(& backing);

			dut.printDetails(stream);
			Assert::AreNotEqual(wstring::npos, stream.str().find(L"The Title"));
		}

		TEST_METHOD(FeedItem_DetailsNotContainsUrlWhenSfalse) {
			LinkFeedItem backing(L"Theurl", S_FALSE);
			std::wostringstream stream;
			::FeedItem dut(& backing);

			dut.printDetails(stream);
			Assert::AreEqual(wstring::npos, stream.str().find(L"Theurl"));
		}

		TEST_METHOD(FeedItem_DetailsContainsUrlWhenSok) {
			LinkFeedItem backing(L"Theurl", S_OK);
			std::wostringstream stream;
			::FeedItem dut(& backing);

			dut.printDetails(stream);
			Assert::AreNotEqual(wstring::npos, stream.str().find(L"Theurl"));
		}

		TEST_METHOD(FeedItem_IsErrorIsFalse) {
			NotimplFeedItem backing;
			::FeedItem dut(& backing);
			Assert::IsFalse(dut.isError());
		}

		TEST_METHOD(FeedItem_ContentsIsEmpty) {
			std::vector<std::wstring> empty;
			NotimplFeedItem backing;
			::FeedItem dut(& backing);
			Assert::IsTrue(empty == dut.getContents());
		}

		TEST_METHOD(FeedItem_ContentsStringIsEmpty) {
			NotimplFeedItem backing;
			::FeedItem dut(& backing);
			std::wostringstream stream;

			dut.printContents(false, stream);
			Assert::AreEqual(wstring::npos, stream.str().find(L"\n"));
		}

		TEST_METHOD(FeedItem_ChildIsError) {
			NotimplFeedItem backing;
			::FeedItem dut(& backing);
			auto child = dut.followPath(L"asdasd");
			Assert::IsTrue(child->isError());
		}

		TEST_METHOD(FeedItem_GetUrlWhenError) {
			LinkFeedItem backing(L"Theurl", E_NOTIMPL);
			::FeedItem dut(& backing);
			std::pair<HRESULT, std::wstring> exp(E_NOTIMPL, L"");
			Assert::IsTrue(exp == dut.getUrl());
		}

		TEST_METHOD(FeedItem_GetUrlWhenOk) {
			LinkFeedItem backing(L"Theurl", S_OK);
			::FeedItem dut(& backing);
			std::pair<HRESULT, std::wstring> exp(S_OK, L"Theurl");
			Assert::IsTrue(exp == dut.getUrl());
		}
	};
}
