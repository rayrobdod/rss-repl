
#include <vector>
#include <string>
#include <iterator>



/** Splits a string with an iterator-style interface */
class SplitStringIterator : public std::iterator<std::input_iterator_tag, std::wstring> {
public:
	/**
	* @param back the string to split into substrings
	* @param delim the delimiters to split the string by
	*/
	template<class collection_d> SplitStringIterator(const std::wstring back, const collection_d delim);
	/**
	* @param back the string to split into substrings
	* @param delim the delimiters to split the string by
	* @param q quote characters
	*/
	template<class collection_d, class collection_q> SplitStringIterator(const std::wstring back, const collection_d delim, const collection_q quot);
	/** copy constructor */
	SplitStringIterator(const SplitStringIterator& other);
	bool operator==(const SplitStringIterator& other) const;
	bool operator!=(const SplitStringIterator& other) const;
	std::wstring operator*() const;
	SplitStringIterator& operator++();
	SplitStringIterator operator++(int);
	static SplitStringIterator end();
private:
	const std::wstring backing;
	const std::vector<wchar_t> delimiters;
	const std::vector<wchar_t> quotes;
	std::wstring::const_iterator current_begin;
	std::wstring::const_iterator current_end;
private:
	bool isAtEnd() const;
};





template<class collection_d> SplitStringIterator::SplitStringIterator(
		const std::wstring back,
		const collection_d delim
) :
		backing(back),
		delimiters(delim.cbegin(), delim.cend()) {
	this->current_end = backing.cbegin();
	++(*this);
}

template<class collection_d, class collection_q> SplitStringIterator::SplitStringIterator(
		const std::wstring back,
		const collection_d delim,
		const collection_q quot
) :
		backing(back),
		delimiters(delim.cbegin(), delim.cend()),
		quotes(quot.cbegin(), quot.cend()) {
	this->current_end = backing.cbegin();
	++(*this);
}
