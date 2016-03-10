#include "CppUnitTest.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/tabComplete.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(TabComplete)
	{
	public:

		TEST_METHOD(isPrefix_emptyIsPrefixOfEmpty) { Assert::IsTrue(::isPrefix(L"", L"")); }
		TEST_METHOD(isPrefix_emptyIsPrefixOfAny) { Assert::IsTrue(::isPrefix(L"abc", L"")); }
		TEST_METHOD(isPrefix_nothingIsPrefixOfEmpty) { Assert::IsFalse(::isPrefix(L"", L"abc")); }
		TEST_METHOD(isPrefix_anyIsPrefixOfItself) { Assert::IsTrue(::isPrefix(L"abc", L"abc")); }
		TEST_METHOD(isPrefix) {
			Assert::IsTrue(::isPrefix(L"abc", L"ab"));
			Assert::IsFalse(::isPrefix(L"ab", L"abc"));
			Assert::IsFalse(::isPrefix(L"abc", L"xy"));
			Assert::IsFalse(::isPrefix(L"abc", L"ax"));
		}



		TEST_METHOD (tabComplete) {
			vector<wstring> comps;
			comps.push_back(L"aaa");
			comps.push_back(L"abc");
			comps.push_back(L"bbb");
			comps.push_back(L"zzz");

			Assert::AreEqual(L"aaa", ::tabComplete(L"", comps).c_str());
			Assert::AreEqual(L"aaa", ::tabComplete(L"a", comps).c_str());
			Assert::AreEqual(L"aaa", ::tabComplete(L"aa", comps).c_str());
			Assert::AreEqual(L"aaa", ::tabComplete(L"aaa", comps).c_str());
			Assert::AreEqual(L"aaaa", ::tabComplete(L"aaaa", comps).c_str());
			Assert::AreEqual(L"abc", ::tabComplete(L"ab", comps).c_str());
			Assert::AreEqual(L"bbb", ::tabComplete(L"b", comps).c_str());
			Assert::AreEqual(L"zzz", ::tabComplete(L"z", comps).c_str());
			Assert::AreEqual(L"x", ::tabComplete(L"x", comps).c_str());
		}
	};
}
