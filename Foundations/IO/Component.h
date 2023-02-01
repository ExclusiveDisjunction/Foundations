#pragma once

#include "IOCommon.h"

namespace Foundations::IO
{
	class CoreMember AddressRecord;
	class CoreMember Reference;
	class CoreMember FileInstance;

	class CoreMember Component
	{
	public:
		Component() = delete;
		Component(AddressRecord* Record);
		Component(const Component& Obj) = delete;
		Component(Component&& Obj) = delete;
		~Component();

		Component& operator=(const Component& Obj) = delete;
		Component& operator=(Component&& Obj) = delete;
	};
}