#pragma once

#include "..\..\StdCalc.h"
#include "..\..\MathVector.h"

namespace Core::Calc::Function
{
	class MATH_LIB FunctionIterator;

	class MATH_LIB FunctionBase
	{
	private:		
		const unsigned _Input;
		const unsigned _Output;

		FunctionBase* Parent = nullptr;
		FunctionBase* Next = nullptr, * Prev = nullptr;
		FunctionBase* FirstCh = nullptr, * LastCh = nullptr;
		unsigned CHCount = 0;

	protected:
		FunctionBase(unsigned int InputDim, unsigned int OutputDim);

		void AssignParent(FunctionBase* Obj);
		bool PushChild(FunctionBase* Obj);
		bool PopChild(FunctionBase* Obj, bool DeleteFunc = true);
		void ClearChildren();

	public:
		FunctionBase(const FunctionBase& Obj) = delete;
		FunctionBase(FunctionBase&& Obj) = delete;
		virtual ~FunctionBase();

		friend FunctionIterator;

		using iterator = FunctionIterator;
		using const_iterator = const FunctionIterator;

		FunctionBase& operator=(const FunctionBase& Obj) = delete;
		FunctionBase& operator=(FunctionBase&& Obj) = delete;

		double A = 1.0;

		bool CanHaveChildren() const { return AllowedChildCount() != 0; }
		virtual unsigned AllowedChildCount() const = 0;
		virtual bool AllowsChildAppend() const = 0;
		void RemoveParent();

		bool AddChild(FunctionBase* Obj);
		bool RemoveChild(FunctionBase* Child, bool Delete = true);

		iterator ChildAt(unsigned i) noexcept;
		const_iterator ChildAt(unsigned i) const noexcept;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

		unsigned InputDim() const;
		unsigned OutputDim() const;

		virtual MathVector Evaluate(const MathVector& X, bool& Exists) const = 0;

		virtual bool ComparesTo(FunctionBase* const& Obj) const;
		virtual bool EquatesTo(FunctionBase* const& Obj) const;
		virtual FunctionBase* Clone() const = 0;

		virtual FunctionBase& operator-();
	};
}