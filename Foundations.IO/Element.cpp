#include "Element.h"

#include "ElementList.h"

namespace Core::IO
{
	Element::Element(Element* Parent, bool CanHaveChildren)
	{
		if (!Parent || !Parent->SupportsChildren())
			throw std::logic_error("ERROR: Parent is nullptr, OR Parent does not support children.");

		Parent->getChildren().push_back(this);

		if (CanHaveChildren)
		{
			Children = new ElementList(this);
			State |= ElementState::ES_CanHaveChildren;
		}
	}

	std::string Element::getType() const noexcept
	{
		return Type;
	}
	unsigned int Element::getID() const noexcept
	{
		return ID;
	}
	unsigned int Element::getState() const noexcept
	{
		return State;
	}

	ElementList& Element::getChildren() const
	{
		if (!SupportsChildren())
			throw std::logic_error("ERROR: Element has no children OR does not support children.");

		return *Children;
	}
}