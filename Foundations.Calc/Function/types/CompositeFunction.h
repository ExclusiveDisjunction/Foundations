#pragma once

#include "..\structure\FunctionBase.h"

#include <iostream>

namespace Core::Function
{
	/// <summary>
	/// Serves as an intermediate abstract class between FunctionBase and derived types to simplify the implementation.
	/// </summary>
	class MATH_LIB CompositeFunction : public FunctionBase
	{
	public:
		CompositeFunction(unsigned int InputDim, unsigned int OutputDim) : FunctionBase(InputDim, OutputDim) {}
		CompositeFunction(const CompositeFunction& Obj) = delete;
		CompositeFunction(const CompositeFunction&& Obj) = delete;

		CompositeFunction& operator=(const CompositeFunction& Obj) = delete;
		CompositeFunction& operator=(const CompositeFunction&& Obj) = delete;

		unsigned int AllowedChildCount() const override { return UINT_MAX; }
		bool AllowsChildAppend() const override { return true; }
	};
}