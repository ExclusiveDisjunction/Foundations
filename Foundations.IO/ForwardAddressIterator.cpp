#include "AddressBook.h"

#include "Reference.h"
#include "Component.h"

namespace Core::IO
{
	ForwardAddressIterator::ForwardAddressIterator(AddressRecord* Current)
	{
		this->_Current = Current;
		this->_RefObject = _Current ? _Current->Reference() : nullptr;
	}
	ForwardAddressIterator::~ForwardAddressIterator()
	{
		if (_RefObject)
			_RefObject->DeReference();
	}

	Reference* ForwardAddressIterator::Current() const
	{
		return _RefObject;
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

		if (_RefObject)
			_RefObject->DeReference();
		if (_Current)
			_RefObject = _Current->Reference();

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

		if (_RefObject)
			_RefObject->DeReference();
		if (_Current)
			_RefObject = _Current->Reference();

		return IsValid();
	}
}