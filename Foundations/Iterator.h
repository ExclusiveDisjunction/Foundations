#pragma once

#include <iostream>

namespace Core
{
	template<typename T, typename SegType>
	class ListIterator
	{
	private:
		SegType* Current;
	public:
		using iterator_category = std::forward_iterator_tag;
		using diffrence_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		ListIterator(SegType* This);

		reference operator*() const;
		pointer operator->();

		ListIterator<T, SegType> operator++();
		ListIterator<T, SegType> operator++(int);

		bool operator==(const ListIterator<T, SegType>& Obj) const;
		bool operator!= (const ListIterator<T, SegType>& Obj) const;
	};

	template<typename T, typename SegType, typename Conv>
	class BorrowedIterator
	{
	private:
		Conv* Conversion;
		SegType* Current;

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		BorrowedIterator(SegType* This, Conv* Conversion);

		reference operator*() const;
		pointer operator->();

		BorrowedIterator<T, SegType, Conv> operator++();
		BorrowedIterator<T, SegType, Conv>& operator++(int);

		bool operator==(const BorrowedIterator<T, SegType, Conv>& Obj) const;
		bool operator!=(const BorrowedIterator<T, SegType, Conv>& Obj) const;
	};
}

#include "Iterator.tpp"