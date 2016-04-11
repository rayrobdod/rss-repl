#include "CppUnitTest.h"
#include "../../main/cpp/FeedElement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::wstring;

namespace Tests
{

	TEST_CLASS(FeedElement_Shared)
	{
	public:

		TEST_METHOD(getRootFolder) {
			FeedElement* res = ::getRootFolder();
			
			Assert::AreEqual<wstring>(L"", res->getPath());
			Assert::IsFalse(res->isError());
			
			FeedElement* parent = res->followPath(L"..");
			Assert::IsTrue(parent->isError());
			delete parent;
			delete res;
		}
		
		TEST_METHOD(downloadStatus2String) {
			Assert::AreEqual<wstring>(L"Not Attempted", ::downloadStatus2String(FDS_NONE));
		}
	};
}
