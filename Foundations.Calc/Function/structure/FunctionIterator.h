#pragma once

#include <iostream>
#include "..\..\StdCalc.h"

namespace Core::Calc::Function
{
	class MATH_LIB FunctionBase;

	class MATH_LIB FunctionIterator
	{
	private:
		FunctionBase* Curr;
		const FunctionBase* ofList;
	public:
		FunctionIterator(FunctionBase* obj, const FunctionBase* host);

		using iterator_tag = std::bidirectional_iterator_tag;
		using difference_type = std::size_t;
		using value_type = FunctionBase;
		using pointer = value_type*;
		using reference = value_type&;

		reference operator*();
		const reference operator*() const;
		pointer operator->() const;

		FunctionIterator& operator++();
		FunctionIterator operator++(int);
		FunctionIterator& operator--();
		FunctionIterator operator--(int);

		FunctionIterator operator+(difference_type diff) const;
		FunctionIterator& operator+=(difference_type diff);
		FunctionIterator operator-(difference_type diff) const;
		FunctionIterator& operator-=(difference_type diff);

		bool operator==(const FunctionIterator& obj) const;
		bool operator!=(const FunctionIterator& obj) const;
	};

	template<typename T> requires std::is_base_of<FunctionBase, T>::value
	class CastedFunctionIterator
	{
	private:
		FunctionIterator iter;
	public:
		CastedFunctionIterator(const FunctionIterator i) : iter(i) {}

		using iterator_tag = FunctionIterator::iterator_tag;
		using difference_type = FunctionIterator::difference_type;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		reference operator*()
		{
			return static_cast<T&>(*iter);
		}
		const reference operator*() const
		{
			return static_cast<const T&>(*iter);
		}
		pointer operator->() const
		{
			return dynamic_cast<pointer>(iter.operator->());
		}

		CastedFunctionIterator<T>& operator++()
		{
			++iter;
			return *this;
		}
		CastedFunctionIterator<T> operator++(int)
		{
			CastedFunctionIterator<T> Return(*this);
			iter++;
			return *this;
		}
		CastedFunctionIterator<T>& operator--()
		{
			--iter;
			return *this;
		}
		CastedFunctionIterator<T> operator--(int)
		{
			CastedFunctionIterator<T> Return(*this);
			iter--;
			return *this;
		}

		CastedFunctionIterator<T> operator+(difference_type diff) const
		{
			return (CastedFunctionIterator<T>(*this) += diff);
		}
		CastedFunctionIterator<T>& operator+=(difference_type diff)
		{
			iter += diff;
			return *this;
		}
		CastedFunctionIterator<T> operator-(difference_type diff) const
		{
			return (CastedFunctionIterator<T>(*this) -= diff);
		}
		CastedFunctionIterator<T>& operator-=(difference_type diff)
		{
			iter += diff;
			return *this;
		}

		bool operator==(const CastedFunctionIterator<T>& obj) const { return iter == obj.iter; }
		bool operator!=(const CastedFunctionIterator<T>& obj) const { return iter != obj.iter; }
	};
}