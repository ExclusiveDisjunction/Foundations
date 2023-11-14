#pragma once

#include "IOCommon.h"

namespace IO
{
	class Address
	{
	private:
		int* IDs = nullptr;
		size_t IDLen = 0;
		Core::String Tag;
	public:
		Address(); //Null Address
		Address(const Core::String& Tag, const Core::String& IDs);
		Address(const Core::String& Tag, const int* IDs, size_t IDSize);
		Address(const Core::String& Tag, const std::initializer_list<int>& IDs);
		Address(const Address& Obj);
		Address(Address&& Obj) noexcept;
		~Address();

		Address& operator=(const Address& Obj);
		Address& operator=(Address&& Obj) noexcept;

		Core::String ToString() const;
		static Address FromString(const Core::String& Obj);
		operator Core::String() const;
	};

	/// <summary>
	/// Taking a tree starting at the Parent, re-order the ID's so that all of the ID's line up in order.
	/// </summary>
	/// <param name="Parent>The root node to start adjusting IDs</param>
	void ReStructureAddresses(Element* Parent);
}