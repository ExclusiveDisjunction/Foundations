#pragma once

#include "IOCommon.h"

namespace Foundations::IO
{
	class CoreMember Component;
	class CoreMember AddressRecord;
	class CoreMember FileInstance;

	class CoreMember Reference
	{
	public:
		Reference() = delete;
		Reference(AddressRecord* Record);
		Reference(const Reference& Obj) = delete;
		Reference(Reference&& Obj) = delete;
		~Reference();

		Reference& operator=(const Reference& Obj) = delete;
		Reference& operator=(Reference&& Obj) = delete;
	};
}