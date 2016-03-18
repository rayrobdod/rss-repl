#include "CppUnitTest.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/SplitStringIterator.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {
	template<> inline std::wstring ToString<::SplitStringIterator>(const ::SplitStringIterator& t) { RETURN_WIDE_STRING("Iterator"); }
	template<> inline std::wstring ToString<std::wstring::const_iterator>(const std::wstring::const_iterator& t) { RETURN_WIDE_STRING("Iterator"); }
} } }

namespace Tests
{
	std::vector<wchar_t> singleton(wchar_t a) {
		std::vector<wchar_t> retval;
		retval.push_back(a);
		return retval;
	}

	TEST_CLASS(SplitStringIterator)
	{
	public:

		TEST_METHOD(whenConstructed_thenPointsToFirstSection) {
			::SplitStringIterator dut(L"1 2 3 10 11", singleton(L' '));
			Assert::AreEqual(L"1", (*dut).c_str());
		}

		TEST_METHOD(whenFirstIncrement_thenPointsToSecondSection) {
			::SplitStringIterator dut(L"1 2 3 10 11", singleton(L' '));
			Assert::AreEqual(L"2", (*(++dut)).c_str());
		}

		TEST_METHOD(whenFirstIncrement_thenEqualsSimilar) {
			::SplitStringIterator first(L"1 2 3 10 11", singleton(L' '));
			::SplitStringIterator second(first);
			Assert::AreEqual(++first, ++second);
		}

		TEST_METHOD(whenIncrementedToEnd_thenComparesEqualToEnd) {
			::SplitStringIterator dut(L"first second", singleton(L' '));
			Assert::AreEqual(++(++dut), ::SplitStringIterator::end(), L"dut does not compare equal to the end iterator");
		}

		TEST_METHOD(whenEmptyConstructed_thenComparesEqualToEnd) {
			::SplitStringIterator dut(L"", singleton(L' '));
			Assert::AreEqual(dut, ::SplitStringIterator::end());
		}

		TEST_METHOD(whenEmptyConstructed_dereferencesToEmpty) {
			// dereferencing a end-of-thing is undefined behavior;
			// so I'm doing the no-error variant
			::SplitStringIterator dut(L"", singleton(L' '));
			Assert::AreEqual(L"", (*dut).c_str());
		}

		TEST_METHOD(iteration) {
			::SplitStringIterator dut(L"ABC/DEFG/HI", singleton(L'/'));
			std::vector<std::wstring> vals(dut, ::SplitStringIterator::end());

			Assert::AreEqual(L"ABC", vals[0].c_str());
			Assert::AreEqual(L"DEFG", vals[1].c_str());
			Assert::AreEqual(L"HI", vals[2].c_str());
		}

		TEST_METHOD(multipleDelimiters) {
			std::vector<wchar_t> delims;
			delims.push_back(L',');
			delims.push_back(L';');
			::SplitStringIterator dut(L"12,34,56;78;90,21", delims);
			std::vector<std::wstring> vals(dut, ::SplitStringIterator::end());

			Assert::AreEqual(L"12", vals[0].c_str());
			Assert::AreEqual(L"34", vals[1].c_str());
			Assert::AreEqual(L"56", vals[2].c_str());
			Assert::AreEqual(L"78", vals[3].c_str());
			Assert::AreEqual(L"90", vals[4].c_str());
			Assert::AreEqual(L"21", vals[5].c_str());
		}

		TEST_METHOD(quotes) {
			std::vector<wchar_t> delims;
			delims.push_back(L',');
			delims.push_back(L';');
			::SplitStringIterator dut(L"one two \"three and\" four", singleton(L' '), singleton(L'\"'));
			std::vector<std::wstring> vals(dut, ::SplitStringIterator::end());

			Assert::AreEqual(L"one", vals[0].c_str());
			Assert::AreEqual(L"two", vals[1].c_str());
			Assert::AreEqual(L"three and", vals[2].c_str());
			Assert::AreEqual(L"four", vals[3].c_str());
		}
	};
}
