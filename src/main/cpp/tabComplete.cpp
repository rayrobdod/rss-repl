
#include "tabComplete.h"

using std::vector;
using std::wstring;


/**
 * Returns true if the first strlen(prefix) characters of string match prefix.
 * @pre parameters are not NULL
 */
bool isPrefix(const wstring string, const wstring prefix) {
	wstring::const_iterator i1 = string.cbegin();
	wstring::const_iterator i2 = prefix.cbegin();
	wstring::const_iterator end1 = string.cend();
	wstring::const_iterator end2 = prefix.cend();
	
	while (i1 != end1 && i2 != end2) {
		if (*i1 != *i2) {return false;}
		i1++; i2++;
	}
	return i2 == end2;
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
	vector<size_t> legal;
	for (size_t i = 0; i < completions.size(); i++) {
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
