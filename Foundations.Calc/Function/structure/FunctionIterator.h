#pragma once

#include <iostream>
#include <string>
#include "..\..\StdCalc.h"

namespace Core::Calc::Function
{
	class MATH_LIB FunctionBase;

	class MATH_LIB FunctionIterator
	{
	private:
		FunctionBase* Curr;
		const FunctionBase* ofList;
		unsigned pos;
	public:
		FunctionIterator(FunctionBase* obj, const FunctionBase* host, unsigned p = UINT_MAX);

		using iterator_category = std::random_access_iterator_tag;
		using iterator_tag = std::random_access_iterator_tag;
		using difference_type = std::size_t;
		using value_type = FunctionBase;
		using pointer = value_type*;
		using reference = value_type&;

		reference operator*();
		const reference operator*() const;
		pointer operator->() const;

		difference_type operator-(const FunctionIterator& obj) const;
		FunctionIterator& operator[](difference_type diff);

		FunctionIterator& operator++();
		FunctionIterator operator++(int);
		FunctionIterator& operator--();
		FunctionIterator operator--(int);

		FunctionIterator operator+(difference_type diff) const;
		FunctionIterator& operator+=(difference_type diff);
		FunctionIterator operator-(difference_type diff) const;
		FunctionIterator& operator-=(difference_type diff);

		bool operator<(const FunctionIterator& obj) const;
		bool operator>(const FunctionIterator& obj) const;
		bool operator<=(const FunctionIterator& obj) const;
		bool operator>=(const FunctionIterator& obj) const;

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

		bool canDecrement() const;
		reference operator*();
		const reference operator*() const;
		pointer operator->() const;

		difference_type operator-(const CastedFunctionIterator<T>& obj) const;
		CastedFunctionIterator<T>& operator[](difference_type diff);

		CastedFunctionIterator<T>& operator++();
		CastedFunctionIterator<T> operator++(int);
		CastedFunctionIterator<T>& operator--();
		CastedFunctionIterator<T> operator--(int);

		CastedFunctionIterator<T> operator+(difference_type diff) const;
		CastedFunctionIterator<T>& operator+=(difference_type diff);
		CastedFunctionIterator<T> operator-(difference_type diff) const;
		CastedFunctionIterator<T>& operator-=(difference_type diff);

		bool operator<(const CastedFunctionIterator<T>& obj) const;
		bool operator>(const CastedFunctionIterator<T>& obj) const;
		bool operator<=(const CastedFunctionIterator<T>& obj) const;
		bool operator>=(const CastedFunctionIterator<T>& obj) const;

		bool operator==(const CastedFunctionIterator<T>& obj) const;
		bool operator!=(const CastedFunctionIterator<T>& obj) const;
	};
}

#include "CastedFunctionIterator.tpp"