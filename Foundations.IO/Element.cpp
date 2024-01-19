#include "Element.h"

#include "ElementList.h"
#include "Reference.h"

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
	Element::~Element()
	{
		if (Parent && Parent->Children)
		{
			ElementIterator<Element> iter(this, Parent->Children);
			Parent->Children->detach(iter);
		}
		if (Children)
			delete Children;
		if (RefCore)
			RefCore->Unbind();
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

	Reference Element::getReference() noexcept
	{
		if (!RefCore)
			RefCore = new ReferenceCore(this);

		return Reference(RefCore);
	}
	Reference Element::getParent() const noexcept
	{
		if (!Parent)
			return Reference(nullptr);

		return Parent->getReference();
	}
	Reference Element::getNextSibling() const noexcept
	{
		if (!Next)
			return Reference(nullptr);

		return Next->getReference();
	}
	Reference Element::getPreviousSibling() const noexcept
	{
		if (!Last)
			return Reference(nullptr);

		return Last->getReference();
	}
	ElementList& Element::getChildren() const
	{
		if (!SupportsChildren())
			throw std::logic_error("ERROR: Element has no children OR does not support children.");

		return *Children;
	}
}