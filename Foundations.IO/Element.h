#pragma once

#include "IOCommon.h"
#include "AttributesWriter.h"
#include <map>

namespace Core::IO
{
	class IO_API ElementList;
	template<typename ElementT>
	class ElementIterator;
	class IO_API ReferenceCore;
	class IO_API Reference;
	class IO_API FileInstance;

	enum ElementState
	{
		ES_Modified = 1,
		ES_CanHaveChildren = 2,
		ES_Loaded = 4
	};

	/// <summary>
	/// A node in the Element tree. This is the backbone that other types can be made from to hold data.
	/// </summary>
	class IO_API Element
	{
	private:
		Element* Parent = nullptr;
		Element* Next = nullptr, * Last = nullptr;
		FileInstance* ParentFile = nullptr;

		std::string Type;
		unsigned int ID = 0;
		unsigned int State = 0;

		ElementList* Children = nullptr;
		ReferenceCore* RefCore = nullptr;

		std::streampos Begin = 0, End = 0;
		std::streampos HBegin = 0, AttrBegin = 0, HEnd = 0;
	protected:		
		Element(FileInstance* Instance, Element* Parent, bool CanHaveChildren);
		Element(Element* Parent, bool CanHaveChildren);

		virtual void WriteAttributes(AttributesWriter& out) const {
			out.WriteKey("Name") << "Your \"Mother\"";
		} //Element in base class (unloaded) do not output any properties, rather, the system will copy the attributes over for it.


	public:
		Element(Element* Parent) noexcept : Element(Parent, true) {}
		Element(Element& Parent) noexcept : Element(&Parent, true) {}
		Element(const Element& Obj) noexcept = delete;
		Element(Element&& Obj) noexcept = delete;
		~Element();

		Element& operator=(const Element& Obj) noexcept = delete;
		Element& operator=(Element&& Obj) noexcept = delete;

		friend IO_API ElementList;
		friend ElementIterator<Element>;
		friend ElementIterator<const Element>;
		friend IO_API ReferenceCore;
		friend IO_API FileInstance;

		std::pair<std::streampos, std::streampos> GetPositionInFile() const { return std::make_pair(Begin, End); }
		std::pair<std::streampos, std::streampos> GetHeaderPositionInFile() const { return std::make_pair(HBegin, HEnd); }
		std::pair<std::streampos, std::streampos> GetAttributesPositionInFile() const { return std::make_pair(AttrBegin, HEnd); }

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