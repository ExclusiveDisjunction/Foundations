#include "AddressBook.h"

#include "Component.h"
#include "Reference.h"

namespace Foundations::IO
{
	TreeAddressIterator::TreeAddressIterator(AddressRecord* Current)
	{
		_Current = Current;
	}

	Reference* TreeAddressIterator::Current() const
	{
		return _Current->Reference();
	}
	bool TreeAddressIterator::IsValid() const
	{
		return _Current != nullptr;
	}

	bool TreeAddressIterator::Advance()
	{
		if (_Current)
		{
			if (!_Current->_NextT)
				_Current = _Current->_ParentT == nullptr ? nullptr : _Current;
			else
				_Current = _Current->_NextT;
		}

		return IsValid();
	}
	bool TreeAddressIterator::Retreat()
	{
		if (_Current)
		{
			if (!_Current->_PrevT)
				_Current = _Current->_ParentT == nullptr ? nullptr : _Current;
			else
				_Current = _Current->_PrevT;
		}

		return IsValid();
	}

	bool TreeAddressIterator::Accend()
	{
		if (_Current)
		{
			if (_Current->_ParentT != nullptr)
				_Current = _Current->_ParentT;
			else
				return false;
		}

		return IsValid();
	}
	bool TreeAddressIterator::Decend()
	{
		if (_Current)
		{
			if (_Current->_FirstCT != nullptr)
				_Current = _Current->_FirstCT;
			else
				return false;
		}

		return IsValid();
	}
}