
#include"SplitStringIterator.h"

bool contains(const std::vector<wchar_t> coll, const wchar_t value) {
	return std::find(coll.cbegin(), coll.cend(), value) != coll.cend();
}


SplitStringIterator::SplitStringIterator(const SplitStringIterator& other) :
		backing(other.backing),
		delimiters(other.delimiters),
		quotes(other.quotes) {
	this->current_begin = this->backing.cbegin() + (other.current_begin - other.backing.cbegin());
	this->current_end = this->backing.cbegin() + (other.current_end - other.backing.cbegin());
}

SplitStringIterator& SplitStringIterator::operator++() {
	// find the start of the next segment
	bool quoteMode = false;
	while ((this->current_end != this->backing.cend()) &&
			(contains(this->delimiters, *(this->current_end)) ||
			contains(this->quotes, *(this->current_end)))) {
		quoteMode = contains(this->quotes, *(this->current_end));
		this->current_end++;
	}
	this->current_begin = this->current_end;

	// find the end of the segment
	while ((this->current_end != this->backing.cend()) &&
			!contains(( quoteMode ? this->quotes :  this->delimiters), *(this->current_end))) {
		this->current_end++;
	}

	return *this;
}

SplitStringIterator SplitStringIterator::operator++(int) {
	SplitStringIterator tmp(*this); this->operator++(); return tmp;
}

std::wstring SplitStringIterator::operator*() const {
	std::wstring retval(current_begin, current_end);
	return retval;
}

bool SplitStringIterator::operator==(const SplitStringIterator& other) const {
	return (
			this->isAtEnd() && other.isAtEnd()
		) || (
			this->backing == other.backing &&
			this->delimiters == other.delimiters &&
			this->quotes == other.quotes &&
			(this->backing.cbegin() - this->current_begin) == (other.backing.cbegin() - other.current_begin) &&
			(this->backing.cbegin() - this->current_end) == (other.backing.cbegin() - other.current_end)
		);
}

bool SplitStringIterator::operator!=(const SplitStringIterator& other) const {
	return !((*this) == other);
}

bool SplitStringIterator::isAtEnd() const {
	return (this->current_begin == this->backing.cend());
}

SplitStringIterator SplitStringIterator::end() {
	std::vector<wchar_t> empty;
	SplitStringIterator retval(L"", empty);
	return retval;
}

size_t SplitStringIterator::length() const {
	size_t retVal = 0;
	SplitStringIterator copy(*this);
	
	while (! copy.isAtEnd()) {
		retVal++;
		copy++;
	}
	return retVal;
}
