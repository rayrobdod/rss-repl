

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <wchar.h>
#include <vector>

using std::vector;
using std::wstring;
#define MAX_COMPLETE_SIZE 1028


/**
 * Returns true if the first strlen(prefix) characters of string match prefix.
 * @pre parameters are not NULL
 */
BOOL isPrefix(const wstring string, const wstring prefix) {
	wstring::const_iterator i1 = string.cbegin();
	wstring::const_iterator i2 = prefix.cbegin();
	wstring::const_iterator end1 = string.cend();
	wstring::const_iterator end2 = prefix.cend();
	
	while (i1 != end1 && i2 != end2) {
		if (*i1 != *i2) {return FALSE;}
		i1++; i2++;
	}
	return (i2 == end2 ? TRUE : FALSE);
}

int isPrefixTestCases(int argc, char** argv) {
	printf("TRUE: %s\n", (isPrefix(L"", L"") ? "TRUE" : "FALSE"));
	printf("TRUE: %s\n", (isPrefix(L"abc", L"") ? "TRUE" : "FALSE"));
	printf("FALS: %s\n", (isPrefix(L"", L"abc") ? "TRUE" : "FALSE"));
	printf("TRUE: %s\n", (isPrefix(L"abc", L"abc") ? "TRUE" : "FALSE"));
	printf("TRUE: %s\n", (isPrefix(L"abc", L"ab") ? "TRUE" : "FALSE"));
	printf("FALS: %s\n", (isPrefix(L"ab", L"abc") ? "TRUE" : "FALSE"));
	printf("FALS: %s\n", (isPrefix(L"abc", L"xy") ? "TRUE" : "FALSE"));
	return 0;
}

/**
 * @param current
 * @param completions_v, completions_c an array of possible completions
 * @return a string that contains the tab completion;
 * 		the first applicable completion; else current
 */
wstring tabComplete(
	const wstring current,
	const vector<wstring> completions
) {
	vector<int> legal;
	for (int i = 0; i < completions.size(); i++) {
		if (isPrefix(completions[i], current)) {
			legal.push_back(i);
		}
	}
	
	if (legal.size() == 0) {
		return current;
	} else if (legal.size() == 1) {
		return completions[legal[0]];
	} else {
		// TODO: figure out greatest common prefix of completions 
		return completions[legal[0]];
	}
}

int tabCompleteTestCase(int argc, char** argv) {
	vector<wstring> comps;
	comps.push_back(L"aaa");
	comps.push_back(L"abc");
	comps.push_back(L"bbb");
	comps.push_back(L"zzz");
	
	printf("aaa: %ls\n", tabComplete(L"", comps));
	printf("aaa: %ls\n", tabComplete(L"a", comps));
	printf("aaa: %ls\n", tabComplete(L"aa", comps));
	printf("aaa: %ls\n", tabComplete(L"aaa", comps));
	printf("aaaa: %ls\n", tabComplete(L"aaaa", comps));
	printf("abc: %ls\n", tabComplete(L"ab", comps));
	printf("bbb: %ls\n", tabComplete(L"b", comps));
	printf("zzz: %ls\n", tabComplete(L"z", comps));
	printf("x: %ls\n", tabComplete(L"x", comps));
	return 0;
}
