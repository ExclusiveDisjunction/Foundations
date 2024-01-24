#pragma once

#include "IOCommon.h"
#include <map>

namespace Core::IO
{
	class IO_API ElementList;
	template<typename ElementT>
	class ElementIterator;
	class IO_API ReferenceCore;
	class IO_API Reference;

	enum ElementState
	{
		ES_Modified = 1,
		ES_CanHaveChildren = 2
	};

	/// <summary>
	/// A node in the Element tree. This is the backbone that other types can be made from to hold data.
	/// </summary>
	class IO_API Element
	{
	private:
		Element* Parent = nullptr;
		Element* Next = nullptr, * Last = nullptr;

		std::string Type;
		unsigned int ID = 0;
		unsigned int State = 0;

		ElementList* Children = nullptr;
		ReferenceCore* RefCore = nullptr;
	protected:		
		Element(Element* Parent, bool CanHaveChildren);

	public:
		Element(Element* Parent) noexcept : Element(Parent, true) {}
		Element(const Element& Obj) noexcept = delete;
		Element(Element&& Obj) noexcept = delete;
		~Element();

		Element& operator=(const Element& Obj) noexcept = delete;
		Element& operator=(Element&& Obj) noexcept = delete;

		friend IO_API ElementList;
		friend ElementIterator<Element>;
		friend ElementIterator<const Element>;
		friend IO_API ReferenceCore;

		std::string getType() const noexcept;
		unsigned int getID() const noexcept;
		unsigned int getState() const noexcept;
		
		Reference getReference() noexcept;
		Reference getParent() const noexcept;
		Reference getNextSibling() const noexcept;
		Reference getPreviousSibling() const noexcept;
		ElementList& getChildren() const;
		bool SupportsChildren() const noexcept { return Children != nullptr && (State & ES_CanHaveChildren); }
	};
}