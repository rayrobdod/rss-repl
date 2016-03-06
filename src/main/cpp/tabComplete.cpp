

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
BOOL isPrefix(const wchar_t* const string, const wchar_t* const prefix) {
	const wchar_t* i1 = string;
	const wchar_t* i2 = prefix;
	
	while (*i1 != 0 && *i2 != 0) {
		if (*i1 != *i2) {return FALSE;}
		i1++; i2++;
	}
	return (*i2 == 0 ? TRUE : FALSE);
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
 * @return a newly malloc'd string that contains the tab completion
 */
wchar_t* tabComplete(
	const wchar_t* const current,
	const wchar_t** const completions_v, const int completions_c
) {
	vector<int> legal;
	for (int i = 0; i < completions_c; i++) {
		if (isPrefix(completions_v[i], current)) {
			legal.push_back(i);
		}
	}
	
	wchar_t* const retVal = (wchar_t*) malloc(MAX_COMPLETE_SIZE * sizeof(wchar_t));
	if (legal.size() == 0) {
		wcsncpy(retVal, current, MAX_COMPLETE_SIZE);
	} else if (legal.size() == 1) {
		wcsncpy(retVal, completions_v[legal[0]], MAX_COMPLETE_SIZE);
	} else {
		// TODO: figure out greatest common prefix of completions 
		wcsncpy(retVal, completions_v[legal[0]], MAX_COMPLETE_SIZE);
	}
	
	return retVal;
}
