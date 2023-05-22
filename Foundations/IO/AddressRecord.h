#pragma once

#include "IOCommon.h"
#include "Address.h"
#include "ReductionLevel.h"

namespace Core::IO
{
	class CORE_API AddressBook;
	class CORE_API Component;
	class CORE_API FileInstance;
	class CORE_API Reference;

	class CORE_API ForwardAddressIterator;
	class CORE_API TreeAddressIterator;

	class CORE_API AddressRecord
	{
	private:
		AddressBook* _Owner;
		
		AddressRecord* _NextF = nullptr, *_PrevF = nullptr; //The next and previous siblings in the forward list.
		AddressRecord* _ParentT = nullptr, * _NextT = nullptr, * _PrevT = nullptr; //The parent of the node, and then the next and previous sibling.
		AddressRecord* _FirstCT = nullptr, *_LastCT = nullptr; //The first and last child of the node.

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
		friend Reference;

		void Reduce(ReductionLevel Level);
		ReductionLevel State() const;

		Address ID() const { return _ID; }
		Component* const& Data = _Data;
		Reference* Reference();
	};
}