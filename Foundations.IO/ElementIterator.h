#ifndef ELEMENTITERATOR_H
#define ELEMENTITERATOR_H

#include "IOCommon.h"
#include "Element.h"
#include "Constraints.h"

namespace Core::IO
{
	template<ElementSimilar ElementT>
	class IO_API ElementIterator
	{
	private:
		ElementT* Current = nullptr;
	public:
		ElementIterator(ElementT* Current);

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::size_t;
		using value_type = ElementT*;
		using pointer = ElementT**;
		using reference = ElementT*&;

		reference operator*();
		pointer operator->();

		ElementIterator& operator++();
		ElementIterator operator++(int);
		ElementIterator& operator--();
		ElementIterator operator--(int);

		difference_type operator-(const ElementIterator& Obj) const;
		ElementIterator operator+(int diffnum) const;
		ElementIterator& operator+=(int diffnum);
		ElementIterator operator-(int diffnum) const;
		ElementIterator& operator-=(int diffnum);

		bool operator==(const ElementIterator& Obj) const;
		bool operator!=(const ElementIterator& Obj) const;
	};
}

#include "ElementIterator.cpp"

#endif