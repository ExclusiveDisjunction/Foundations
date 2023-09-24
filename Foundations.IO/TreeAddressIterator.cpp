#include "AddressBook.h"

#include "Component.h"
#include "Reference.h"

namespace Core::IO
{
	TreeAddressIterator::TreeAddressIterator(AddressRecord* Current)
	{
		_Current = Current;
		_RefObject = _Current->Reference();
	}
	TreeAddressIterator::~TreeAddressIterator()
	{
		if (_RefObject)
			_RefObject->DeReference();
	}

	Reference* TreeAddressIterator::Current() const
	{
		return _RefObject;
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

		if (_RefObject)
			_RefObject->DeReference();
		if (_Current)
			_RefObject = _Current->Reference();

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

		if (_RefObject)
			_RefObject->DeReference();
		if (_Current)
			_RefObject = _Current->Reference();

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

		if (_RefObject)
			_RefObject->DeReference();
		if (_Current)
			_RefObject = _Current->Reference();

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

		if (_RefObject)
			_RefObject->DeReference();
		if (_Current)
			_RefObject = _Current->Reference();

		return IsValid();
	}
}