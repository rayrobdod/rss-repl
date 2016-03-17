
#include <vector>
#include <cwchar>
#include <string>
#include <iterator>

/**  */
class SplitStringIterator : public std::iterator<std::input_iterator_tag, std::wstring> {
public:
	template<class collection> SplitStringIterator(std::wstring a, collection d);
	SplitStringIterator(std::wstring a, wchar_t delimieter);
	SplitStringIterator(const SplitStringIterator& other);
	bool operator==(const SplitStringIterator& other) const;
	bool operator!=(const SplitStringIterator& other) const;
	std::wstring operator*();
	SplitStringIterator& operator++();
	SplitStringIterator operator++(int);
	static SplitStringIterator end();
private:
	const std::wstring backing;
	std::vector<wchar_t> delimiters;
	std::wstring::const_iterator current_begin;
	std::wstring::const_iterator current_end;
private:
	bool isAtEnd() const;
};

template<class collection> SplitStringIterator::SplitStringIterator(std::wstring a, collection d) :
		backing(a),
		delimiters(d.cbegin(), d.cend()) {
	this->current_end = backing.cbegin();
	++(*this);
}
