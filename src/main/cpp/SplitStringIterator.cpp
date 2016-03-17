
#include"SplitStringIterator.h"

SplitStringIterator::SplitStringIterator(std::wstring a, wchar_t d) :
	backing(a) {
	this->delimiters.push_back(d);
	this->current_end = backing.cbegin();
	++(*this);
}

SplitStringIterator::SplitStringIterator(const SplitStringIterator& other) :
		backing(other.backing),
		delimiters(other.delimiters) {
	this->current_begin = this->backing.cbegin() + (other.current_begin - other.backing.cbegin());
	this->current_end = this->backing.cbegin() + (other.current_end - other.backing.cbegin());
}

SplitStringIterator& SplitStringIterator::operator++() {
	// find the start of the next segment
	while (this->current_end != this->backing.cend() &&
			(find(delimiters.cbegin(), delimiters.cend(), *(this->current_end)) != delimiters.cend())) {
		this->current_end++;
	}
	this->current_begin = this->current_end;

	// find the end of the segment
	while (this->current_end != this->backing.cend() &&
			!(find(delimiters.cbegin(), delimiters.cend(), *(this->current_end)) != delimiters.cend())) {
		this->current_end++;
	}

	return *this;
}

SplitStringIterator SplitStringIterator::operator++(int) {
	SplitStringIterator tmp(*this); this->operator++(); return tmp;
}

std::wstring SplitStringIterator::operator*() {
	std::wstring retval(current_begin, current_end);
	return retval;
}

bool SplitStringIterator::operator==(const SplitStringIterator& other) const {
	return (
			this->isAtEnd() && other.isAtEnd()
		) || (
			this->backing == other.backing &&
			this->delimiters == other.delimiters &&
			this->current_begin == other.current_begin &&
			this->current_end == other.current_end
		);
}

bool SplitStringIterator::operator!=(const SplitStringIterator& other) const {
	return !((*this) == other);
}

bool SplitStringIterator::isAtEnd() const {
	return (this->current_begin == this->backing.cend());
}

SplitStringIterator SplitStringIterator::end() {
	SplitStringIterator retval(L"", L' ');
	return retval;
}
