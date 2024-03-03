#include "FunctionIterator.h"

#include "FunctionBase.h"

namespace Core::Calc::Function
{
	FunctionIterator::FunctionIterator(FunctionBase* obj, const FunctionBase* host) : Curr(obj), ofList(host) 
	{
	}

	using iterator_tag = FunctionIterator::iterator_tag;
	using difference_type = FunctionIterator::difference_type;
	using value_type = FunctionIterator::value_type;
	using pointer = FunctionIterator::pointer;
	using reference = FunctionIterator::reference;

	reference FunctionIterator::operator*()
	{
		return *Curr;
	}
	const reference FunctionIterator::operator*() const
	{
		return *Curr;
	}
	pointer FunctionIterator::operator->() const
	{
		return Curr;
	}

	FunctionIterator& FunctionIterator::operator++()
	{
		return (*this) += 1;
	}
	FunctionIterator FunctionIterator::operator++(int)
	{
		FunctionIterator temp(*this);
		(*this) += 1;
		return temp;
	}
	FunctionIterator& FunctionIterator::operator--()
	{
		return (*this) -= 1;
	}
	FunctionIterator FunctionIterator::operator--(int)
	{
		FunctionIterator temp(*this);
		(*this) -= 1;
		return temp;
	}

	FunctionIterator FunctionIterator::operator+(difference_type diff) const
	{
		return (FunctionIterator(Curr, ofList) += diff);
	}
	FunctionIterator& FunctionIterator::operator+=(difference_type diff)
	{
		while (Curr)
			Curr = Curr->Next;

		return *this;
	}
	FunctionIterator FunctionIterator::operator-(difference_type diff) const
	{
		return (FunctionIterator(Curr, ofList) -= diff);
	}
	FunctionIterator& FunctionIterator::operator-=(difference_type diff)
	{
		if (ofList && !Curr && diff > 0) //If current is nullptr, we assume that we are at the end of the list.
		{
			Curr = ofList->LastCh;
			diff--;
		}

		while (Curr)
			Curr = Curr->Prev;

		return *this;
	}

	bool FunctionIterator::operator==(const FunctionIterator& obj) const
	{
		return Curr == obj.Curr && ofList == obj.ofList;
	}
	bool FunctionIterator::operator!=(const FunctionIterator& obj) const
	{
		return Curr != obj.Curr || ofList != obj.ofList;
	}
}