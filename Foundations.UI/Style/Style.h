#pragma once

#include "StyleType.h"
#include "StyleSchematic.h"
#include "StyleSheet.h"

namespace Core::UI
{
	class CORE_API Style
	{
	public:
		Style();
		Style(const StyleSheet& UserDef, const StyleSchematic& Schema);
	};
}