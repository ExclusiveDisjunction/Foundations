#include "Address.h"

using namespace Core;

namespace IO
{
	Address::Address() : Address(L"NULL", { 0 })
	{
		
	}
	Address::Address(const Core::String& Tag, const Core::String& IDs)
	{
		this->Tag = Tag;

		Vector<String> Splits = IDs.Split(L'.');
		if (Splits.Size != 0)
		{
			IDLen = Splits.Size - 1;
			this->IDs = new int[IDLen];

			for (int i = 0; i < Splits.Size; i++)
				this->IDs[i] = Splits[i].ToInt();
		}
	}
	Address::Address(const Core::String& Tag, const int* IDs, size_t IDSize)
	{
		this->IDLen = IDSize;
		this->Tag = Tag;

		this->IDs = new int[this->IDLen];
		for (size_t i = 0; i < IDSize; i++)
			this->IDs[i] = IDs[i];
	}
	Address::Address(const Core::String& Tag, const std::initializer_list<int>& IDs)
	{
		this->Tag = Tag;

		IDLen = IDs.size();
		this->IDs = new int[IDLen];
		auto Iter = IDs.begin();
		for (size_t i = 0; i < IDLen && Iter != IDs.end(); i++, Iter++)
			this->IDs[i] = *Iter;
	}
	Address::~Address()
	{
		if (IDs)
			delete[] IDs;

		IDs = nullptr;
	}

	Core::String Address::ToString() const
	{
		String IDStr;
		for (size_t i = 0; i < IDLen; i++)
			IDStr += String(L'.') + IDs[i];

		return Tag + IDStr;
	}
	Address Address::FromString(const Core::String& Obj)
	{
		Vector<String> Splits = Obj.Split(L'.');
		if (Splits.Size < 2)
			return Address(); //Nothing to parse, null address.

		Address Return;
		Return.Tag = Splits[0];
		Return.IDLen = Splits.Size - 2; //One is for last element, two is for second to last index (first is used).
		Return.IDs = new int[Return.IDLen];

		for (int i = 1; i < Splits.Size; i++)
			Return.IDs[i - 1] = Splits[i].ToInt();

		return Return;
	}
	Address::operator Core::String() const
	{
		return this->ToString();
	}

	void ReStructureAddresses(Element* Parent)
	{
		
	}
}