#pragma once

#include "IOCommon.h"
#include "ReductionLevel.h"

namespace Foundations::IO
{
	class CoreMember Component;
	class CoreMember AddressRecord;
	class CoreMember FileInstance;

	class CoreMember Reference
	{
	private:
		AddressRecord* Parent = nullptr;
		unsigned int RefCount = 0;

		Reference(AddressRecord* Record);
	public:
		Reference() = delete;
		Reference(const Reference& Obj) = delete;
		Reference(Reference&& Obj) = delete;
		~Reference();

		friend AddressRecord;

		Reference& operator=(const Reference& Obj) = delete;
		Reference& operator=(Reference&& Obj) = delete;

		void IncrementReference();
		void DeReference();

		ReductionLevel State() const;
		Component* Value(ReductionLevel LoadAs = ReductionLevel::Loaded);
	};
}