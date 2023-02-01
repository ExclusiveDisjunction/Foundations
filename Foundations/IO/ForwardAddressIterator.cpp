#include "AddressBook.h"

#include "Reference.h"
#include "Component.h"

namespace Foundations::IO
{
	ForwardAddressIterator::ForwardAddressIterator(AddressRecord* Current)
	{
		this->_Current = Current;
	}

	Reference* ForwardAddressIterator::Current() const
	{
		return _Current ? _Current->Reference() : nullptr;
	}
	bool ForwardAddressIterator::IsValid() const
	{
		return _Current != nullptr;
	}
	bool ForwardAddressIterator::Advance()
	{
		if (_Current)
		{
			if (_Current->_NextF != nullptr)
				_Current = _Current->_NextF;
			else
				return false;
		}

		return IsValid();
	}
	bool ForwardAddressIterator::Retreat()
	{
		if (_Current)
		{
			if (_Current->_PrevF != nullptr)
				_Current = _Current->_PrevF;
			else
				return false;
		}

		return IsValid();
	}
}