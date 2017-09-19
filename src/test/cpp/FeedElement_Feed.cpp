#include "CppUnitTest.h"
#include "SimpleFeedElems.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/FeedElement_Feed.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::wstring;

namespace Tests
{
	FeedItemLong(LocalId);
	FeedItemBstr(Title);
	FeedFeedBstr(Title);
	FeedFeedBstr(Url);
	FeedFeedBstr(DownloadUrl);
	FeedFeedBstr(Image);


	TEST_CLASS(FeedFeedTest)
	{
	public:

		TEST_METHOD(FeedFeed_DetailsContainsTitle) {
			TitleFeedFeed backing(L"The Title");
			std::wostringstream stream;
			::FeedFeed dut(& backing);

			dut.printDetails(stream);
			Assert::AreNotEqual(wstring::npos, stream.str().find(L"The Title"));
		}

		TEST_METHOD(FeedFeed_DetailsNotContainsUrlWhenSfalse) {
			DownloadUrlFeedFeed backing(L"Theurl", S_FALSE);
			std::wostringstream stream;
			::FeedFeed dut(&backing);

			dut.printDetails(stream);
			Assert::AreEqual(wstring::npos, stream.str().find(L"Theurl"));
		}

		TEST_METHOD(FeedFeed_DetailsContainsUrlWhenSok) {
			DownloadUrlFeedFeed backing(L"Theurl", S_OK);
			std::wostringstream stream;
			::FeedFeed dut(&backing);

			dut.printDetails(stream);
			Assert::AreNotEqual(wstring::npos, stream.str().find(L"Theurl"));
		}

		TEST_METHOD(FeedFeed_DetailsNotContainsImageWhenSfalse) {
			ImageFeedFeed backing(L"Theurl", S_FALSE);
			std::wostringstream stream;
			::FeedFeed dut(&backing);

			dut.printDetails(stream);
			Assert::AreEqual(wstring::npos, stream.str().find(L"Theurl"));
		}

		TEST_METHOD(FeedFeed_DetailsContainsImageWhenSok) {
			ImageFeedFeed backing(L"Theurl", S_OK);
			std::wostringstream stream;
			::FeedFeed dut(&backing);

			dut.printDetails(stream);
			Assert::AreNotEqual(wstring::npos, stream.str().find(L"Theurl"));
		}

		TEST_METHOD(FeedFeed_IsErrorIsFalse) {
			NotimplFeedFeed backing;
			::FeedFeed dut(& backing);
			Assert::IsFalse(dut.isError());
		}

		TEST_METHOD(FeedFeed_GetUrlWhenError) {
			UrlFeedFeed backing(L"Theurl", E_NOTIMPL);
			::FeedFeed dut(& backing);
			std::pair<HRESULT, std::wstring> exp(E_NOTIMPL, L"");
			Assert::IsTrue(exp == dut.getUrl());
		}

		TEST_METHOD(FeedFeed_GetUrlWhenOk) {
			UrlFeedFeed backing(L"Theurl", S_OK);
			::FeedFeed dut(& backing);
			std::pair<HRESULT, std::wstring> exp(S_OK, L"Theurl");
			Assert::IsTrue(exp == dut.getUrl());
		}

		TEST_METHOD(FeedFeed_GetContents_Empty) {
			std::vector<IFeedItem*> items;
			ChildrenFeedFeed backing(items);
			::FeedFeed dut(&backing);
			Assert::IsTrue(dut.getContents().empty());
		}

		TEST_METHOD(FeedFeed_GetContents_NonEmpty) {
			LocalIdFeedItem first(24), second(25), third(26);
			std::vector<IFeedItem*> items;
			items.push_back(&first);
			items.push_back(&second);
			items.push_back(&third);

			ChildrenFeedFeed backing(items);
			::FeedFeed dut(&backing);
			std::vector<wstring> res = dut.getContents();
			size_t EXPECTED_SIZE(3);
			Assert::AreEqual(EXPECTED_SIZE, res.size());
			Assert::AreEqual(L"24", res[0].c_str());
			Assert::AreEqual(L"25", res[1].c_str());
			Assert::AreEqual(L"26", res[2].c_str());
		}
	};
}
