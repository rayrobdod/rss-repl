#ifndef TABCOMPLETE_H
#define TABCOMPLETE_H

#include <string>
#include <vector>

/**
 * Returns true if the first strlen(prefix) characters of string match prefix.
 * @pre parameters are not NULL
 */
bool _declspec(dllexport) isPrefix(const std::wstring string, const std::wstring prefix);

/**
 * @param current
 * @param completions_v, completions_c an array of possible completions
 * @return a string that contains the tab completion;
 * 		the first applicable completion; else current
 */
std::wstring tabComplete(
	const std::wstring current,
	const std::vector<std::wstring> completions
);

#endif        //  #ifndef TABCOMPLETE_H
