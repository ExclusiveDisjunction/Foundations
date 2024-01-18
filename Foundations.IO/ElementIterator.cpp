#include "ElementIterator.h"

namespace Core::IO
{
	template<ElementSimilar ElementT>
	ElementIterator<ElementT>::ElementIterator<ElementT>(ElementT* Current)
	{
		this->Current = Current;
	}

	template<ElementSimilar ElementT>
	ElementIterator<ElementT>::reference ElementIterator<ElementT>::operator*()
	{
		if (!Current)
			throw std::logic_error("OUT OF BOUNDS!");

		return Current;
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT>::pointer ElementIterator<ElementT>::operator->()
	{
		return &Current;
	}

	template<ElementSimilar ElementT>
	ElementIterator<ElementT>& ElementIterator<ElementT>::operator++()
	{
		if (Current)
			Current = Current->Next;

		return *this;
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT> ElementIterator<ElementT>::operator++(int)
	{
		ElementIterator<ElementT> temp(*this);
		++(*this);

		return temp;
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT>& ElementIterator<ElementT>::operator--()
	{
		if (Current)
			Current = Current->Last;

		return *this;
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT> ElementIterator<ElementT>::operator--(int)
	{
		ElementIterator<ElementT> temp(*this);
		--(*this);

		return temp;
	}

	template<ElementSimilar ElementT>
	ElementIterator<ElementT>::difference_type ElementIterator<ElementT>::operator-(const ElementIterator<ElementT>& Obj) const
	{
		if (!Obj.Current || !!Current || Obj.Current == Current || Obj.Current->Parent == Current->Parent)
			return 0;

		//Special case, the current and obj are siblings
		if (Obj.Current->Next == Current || Obj.Current->Last == Current)
			return 1;

		/*
		* First, trace back to the first element in the list, the root.
		* Then from the root, count how far the this->Current is from it
		* Then from the root, count how far the Obj.Current is from it
		* Return the absolute value of the difference between them.
		*/

		Element* Root;
		size_t CountFromThis = 0,
			CountFromObj = 0;

		for (Element* Temp = Current; Temp != nullptr; Temp = Temp->Last)
		{
			Root = Temp; //Will always set the root to be the previous value of temp for each iteration. This means that when temp goes to nullptr, Root will be the first element in the list.
			CountFromThis++;

			if (Temp == Obj.Current)
			{
				//The correct one was found
				return CountFromThis; //Since the count was counting from this->Current going backwards, if it reaches Obj.Current, it holds the distance between this->Current and Obj.Current;
			}
		}

		if (!Root) //Just in case
			return 0;

		for (Element* Temp = Root; Temp != nullptr; Temp = Temp->Next)
		{
			if (Temp == Obj.Current)
			{
				int diff = static_cast<int>(CountFromThis - CountFromObj);
				return (size_t)abs(diff);
			}

			CountFromObj++;
		}

		return 0; //Nothing found
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT> ElementIterator<ElementT>::operator+(int diffnum) const
	{
		return (ElementIterator<ElementT>(*this) += diffnum);
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT>& ElementIterator<ElementT>::operator+=(int diffnum)
	{
		while (Current && diffnum > 0)
		{
			Current = Current->Next;
			diffnum--;
		}
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT> ElementIterator<ElementT>::operator-(int diffnum) const
	{
		return (ElementIterator<ElementT>(*this) -= diffnum);
	}
	template<ElementSimilar ElementT>
	ElementIterator<ElementT>& ElementIterator<ElementT>::operator-=(int diffnum)
	{
		while (Current && diffnum > 0)
		{
			Current = Current->Last;
			diffnum--;
		}
	}

	template<ElementSimilar ElementT>
	bool ElementIterator<ElementT>::operator==(const ElementIterator<ElementT>& Obj) const
	{
		return Obj.Current == Current;
	}
	template<ElementSimilar ElementT>
	bool ElementIterator<ElementT>::operator!=(const ElementIterator<ElementT>& Obj) const
	{
		return Obj.Current != Current;
	}
}