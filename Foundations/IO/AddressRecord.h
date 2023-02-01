#pragma once

#include "IOCommon.h"
#include "Address.h"

namespace Foundations::IO
{
	class CoreMember AddressBook;
	class CoreMember Component;
	class CoreMember FileInstance;
	class CoreMember Reference;

	class CoreMember ForwardAddressIterator;
	class CoreMember TreeAddressIterator;

	class CoreMember AddressRecord
	{
	private:
		AddressBook* _Owner;
		
		AddressRecord* _NextF = nullptr, *_PrevF = nullptr;
		AddressRecord* _ParentT = nullptr, * _NextT = nullptr, * _PrevT = nullptr;
		AddressRecord* _FirstCT = nullptr, _LastCT = nullptr;

		Address _ID;
		Component* _Data;
		Reference* _Ref;

	public:
		AddressRecord() = delete;
		AddressRecord(AddressBook* Owner, AddressRecord* NextF, AddressRecord* PrevF, AddressRecord* ParentT, AddressRecord* NextT, AddressRecord* PrevT);
		AddressRecord(const AddressRecord& Obj) = delete;
		AddressRecord(AddressRecord&& Obj) noexcept = delete;
		~AddressRecord();

		AddressRecord& operator=(const AddressRecord& Obj) = delete;
		AddressRecord& operator=(AddressRecord&& Obj) = delete;

		friend AddressBook;
		friend ForwardAddressIterator;
		friend TreeAddressIterator;

		Address ID() const { return _ID; }
		Component* const& Data = _Data;
		Reference* Reference()
		{

		}
	};
}