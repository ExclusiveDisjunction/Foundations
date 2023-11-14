#ifndef ITERATOR_TPP
#define ITERATOR_TPP

#include "Iterator.h"

namespace Core
{
	template<typename T, typename SegType>
	ListIterator<T, SegType>::ListIterator(SegType* This) : Current(This)
	{
	}

	template<typename T, typename SegType>
	ListIterator<T, SegType>::reference ListIterator<T, SegType>::operator*() const
	{
		return Current->Value;
	}
	template<typename T, typename SegType>
	ListIterator<T, SegType>::pointer ListIterator<T, SegType>::operator->()
	{
		return &Current->Value;
	}

	template<typename T, typename SegType>
	ListIterator<T, SegType> ListIterator<T, SegType>::operator++()
	{
		Current = Current->Next; return *this;
	}
	template<typename T, typename SegType>
	ListIterator<T, SegType> ListIterator<T, SegType>::operator++(int)
	{
		ListIterator<T, SegType> tmp = *this;
		++(*this);
		return tmp;
	}

	template<typename T, typename SegType>
	bool ListIterator<T, SegType>::operator==(const ListIterator<T, SegType>& Obj) const
	{
		return Current == Obj.Current;
	}
	template<typename T, typename SegType>
	bool ListIterator<T, SegType>::operator!= (const ListIterator<T, SegType>& Obj) const
	{
		return Current != Obj.Current;
	}

	template<typename T, typename SegType, typename Conv>
	BorrowedIterator<T, SegType, Conv>::BorrowedIterator(SegType* This, Conv* Conversion) : Current(This), Conversion(Conversion)
	{

	}

	template<typename T, typename SegType, typename Conv>
	BorrowedIterator<T, SegType, Conv>::reference BorrowedIterator<T, SegType, Conv>::operator*() const
	{
		return Conversion(Current->Value);
	}
	template<typename T, typename SegType, typename Conv>
	BorrowedIterator<T, SegType, Conv>::pointer BorrowedIterator<T, SegType, Conv>::operator->()
	{
		return &(Conversion(Current->Value));
	}

	template<typename T, typename SegType, typename Conv>
	BorrowedIterator<T, SegType, Conv> BorrowedIterator<T, SegType, Conv>::operator++()
	{
		Current = Current->Next; return *this;
	}
	template<typename T, typename SegType, typename Conv>
	BorrowedIterator<T, SegType, Conv>& BorrowedIterator<T, SegType, Conv>::operator++(int)
	{
		BorrowedIterator<T, SegType, Conv> tmp = *this;
		++(*this);
		return tmp;
	}

	template<typename T, typename SegType, typename Conv>
	bool BorrowedIterator<T, SegType, Conv>::operator==(const BorrowedIterator<T, SegType, Conv>& Obj) const
	{
		return Current == Obj.Current;
	}
	template<typename T, typename SegType, typename Conv>
	bool BorrowedIterator<T, SegType, Conv>::operator!=(const BorrowedIterator<T, SegType, Conv>& Obj) const
	{
		return Current != Obj.Current;
	}
}

#endif