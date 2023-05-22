#pragma once

#include "IOCommon.h"

namespace Core::IO
{
	class CORE_API AddressRecord;
	class CORE_API Reference;
	class CORE_API FileInstance;

	class CORE_API Component
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