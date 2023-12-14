#pragma once

#include "..\UICommon.h"
#include "Mapping.h"
#include <vector>

#include "StyleType.h"

namespace Core::UI
{
	class CORE_API StyleSchematicElement
	{
	private:
		String PropertyName;
		StyleType* Type = nullptr;
		StyleType* DefaultValue = nullptr;
	public:
		StyleSchematicElement(const String& Name, StyleType* TypeSignature, StyleType* DefaultValue = nullptr);
		StyleSchematicElement(const StyleSchematicElement& Obj) noexcept;
		~StyleSchematicElement();

		StyleSchematicElement& operator=(const StyleSchematicElement& Obj) noexcept;

		String GetName() const;
		String TypeName() const;
		String DefaultValueStr() const;
	};
	class CORE_API StyleSchematic
	{
	private:
		std::vector<StyleSchematicElement*> Properties;
	public:
		StyleSchematic(const std::vector<StyleSchematicElement*>& Schemas);
		StyleSchematic(const StyleSchematic& Obj) noexcept;
		~StyleSchematic();

		StyleSchematic& operator=(const StyleSchematic& Obj);

		StyleSchematicElement& operator[](const String& Name);
		
		void AppendProperty(StyleSchematicElement* Schematic);
		void AppendManyProperties(const std::vector<StyleSchematicElement*>& Schemas);
		bool RemoveProperty(String Name);
	};

	
}