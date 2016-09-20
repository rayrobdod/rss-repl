#include "CppUnitTest.h"
#include "../../main/cpp/FeedElement_Null.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {

	TEST_CLASS(FeedElement_Null) {
	public:

		TEST_METHOD(FeedError_GetContents_IsEmpty) {
			::ErrorFeedElement dut(L"Err");
			Assert::IsTrue(dut.getContents().empty());
		}

		TEST_METHOD(FeedError_GetPath_IsERROR) {
			::ErrorFeedElement dut(L"Err");
			Assert::AreEqual(L"ERROR", dut.getPath().c_str());
		}

		TEST_METHOD(FeedError_IsError_ReturnTrue) {
			::ErrorFeedElement dut(L"Err");
			Assert::IsTrue(dut.isError());
		}

		TEST_METHOD(FeedError_MarkAsRead_NOTIMPL) {
			::ErrorFeedElement dut(L"Err");
			Assert::AreEqual(E_NOTIMPL, dut.markAsRead());
		}

		TEST_METHOD(FeedError_getAttachment_NOTIMPL) {
			::ErrorFeedElement dut(L"Err");
			Assert::AreEqual(E_NOTIMPL, dut.getAttachmentFile().first);
		}

		TEST_METHOD(FeedError_getUrl_NOTIMPL) {
			::ErrorFeedElement dut(L"Err");
			Assert::AreEqual(E_NOTIMPL, dut.getUrl().first);
		}

		TEST_METHOD(FeedError_printDetails) {
			std::wstring exp(L"Error Message");
			::ErrorFeedElement dut(exp);
			std::wostringstream buffer;
			dut.printDetails(buffer);

			Assert::AreEqual(exp, buffer.str());
		}

		TEST_METHOD(FeedError_printContents) {
			std::wstring exp(L"Error Message");
			::ErrorFeedElement dut(exp);
			std::wostringstream buffer;
			dut.printContents(false, buffer);

			Assert::AreEqual(exp, buffer.str());
		}

		TEST_METHOD(FeedError_getParent) {
			std::wstring exp(L"Error Message");
			::ErrorFeedElement dut(exp);
			::FeedElement* res = dut.followPath(L"..");
			std::wostringstream buffer1, buffer2;
			dut.printDetails(buffer1);
			res->printDetails(buffer2);

			Assert::AreEqual(buffer1.str(), buffer2.str());

			delete res;
		}

		TEST_METHOD(FeedError_getChild) {
			std::wstring exp(L"Error Message");
			::ErrorFeedElement dut(exp);
			::FeedElement* res = dut.followPath(L"asdfsdf");
			std::wostringstream buffer1, buffer2;
			dut.printDetails(buffer1);
			res->printDetails(buffer2);

			Assert::AreEqual(buffer1.str(), buffer2.str());

			delete res;
		}

	};
}
