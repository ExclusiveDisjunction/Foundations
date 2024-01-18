#pragma once

#include "IOCommon.h"
#include "Constraints.h"
#include <map>

namespace Core::IO
{
	class IO_API FileInstance;
	class IO_API ElementList;
	template<ElementSimilar ElementT>
	class IO_API ElementIterator;

	class IO_API Element
	{
	private:
		Element* Parent = nullptr;
		Element* Next = nullptr, * Last = nullptr;

		std::string Type;
		unsigned int ID = 0;
		bool IsModified = false;

	protected:		
		Element(Element* Parent, bool CanHaveChildren) noexcept;
		ElementList* Children = nullptr;
		bool CanHaveChildren = true;

		virtual void GetAttributes(const std::map<std::string, std::string>& Out);
	public:
		Element(Element* Parent) noexcept : Element(Parent, true) {}
		Element(const Element& Obj) noexcept = delete;
		Element(Element&& Obj) noexcept = delete;

		Element& operator=(const Element& Obj) noexcept = delete;
		Element& operator=(Element&& Obj) noexcept = delete;

		friend IO_API ElementList;
		friend IO_API ElementIterator<Element>;

		std::string getType() const;
		unsigned int getID() const;
		
		Element* getParent() const;
		Element* getNextSibling() const;
		Element* getPreviousSibling() const;
		ElementList* getChildren() const;
	};
}