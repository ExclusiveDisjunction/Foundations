#pragma once

#include "StyleType.h"

namespace Core::UI
{
	class CORE_API StyleSheet
	{
	private:
		Mapping<String, StyleType*> Values;
	public:
		StyleSheet(const StyleSheet& Obj);
		StyleSheet(StyleSheet&& Obj);
		~StyleSheet();
	};
}