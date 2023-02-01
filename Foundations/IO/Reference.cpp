#include "Reference.h"

#include "AddressRecord.h"
#include "Component.h"

namespace Foundations::IO
{
	Reference::Reference(AddressRecord* Record)
	{
		if (!Record)
			throw std::exception("The record provided cannot be null.");

		if (Record->_Ref != nullptr)
			throw std::exception("The record already has a reference object attached to it.");

		Parent = Record;
		RefCount = 0;
	}
	Reference::~Reference()
	{
		RefCount = 0;
		if (Parent)
			Parent->_Ref = nullptr;

		Parent = nullptr;
	}

	void Reference::IncrementReference()
	{
		RefCount++;
	}
	void Reference::DeReference()
	{
		RefCount = RefCount == 0 ? 0 : RefCount - 1;

		if (RefCount == 0)
		{
			delete this;
			return;
		}
	}

	ReductionLevel Reference::State() const
	{
		return !Parent ? ReductionLevel::Flat : Parent->State();
	}
	Component* Reference::Value(ReductionLevel LoadAs)
	{
		if (!Parent)
			return nullptr;

		Parent->Reduce(LoadAs);
		return Parent->_Data;
	}
}