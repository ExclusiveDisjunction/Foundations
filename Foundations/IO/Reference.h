#pragma once

#include "IOCommon.h"
#include "ReductionLevel.h"

namespace Core::IO
{
	class CORE_API Component;
	class CORE_API AddressRecord;
	class CORE_API FileInstance;

	class CORE_API Reference
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