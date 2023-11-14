#pragma once

#include <iostream>
#include <vector>

#include "Iterator.h"
#include "SequenceBase.h"

#define VectorDef

namespace Core
{
	template<typename T>
	class VectorSeg
	{
	public:
		VectorSeg() : Next(nullptr), Last(nullptr) {}
		VectorSeg(const T& Value) : Value(Value), Next(nullptr), Last(nullptr) {}

		VectorSeg<T>* Next = nullptr;
		VectorSeg<T>* Last = nullptr;
		T Value = T();
	};

	template<typename T>
	class CORE_API Vector : public SequenceBase<T>
	{
	public:
		Vector();
		Vector(const SequenceBase<T>*& Other);
		Vector(const T* List, unsigned int Len);
		Vector(const std::initializer_list<T>& List);
		Vector(unsigned int Len);
		Vector(T& Obj);
		Vector(const T& Obj);
		Vector(const T& Obj, unsigned int Len);
		Vector(const std::vector<T>& Other);
		virtual ~Vector();

		using seg = VectorSeg<T>*;
		using iter = ListIterator<T, VectorSeg<T>>;

		T& operator[](unsigned int Index) const override;
		T& Item(unsigned int Index) const override;
		const unsigned int& Size = _Size;
		unsigned int Length() const override;

		void Add(const T& Obj) override;
		void AddList(const SequenceBase<T>* Other) override;
		void InsertAt(unsigned int Index, const T& Value) override;
		bool Remove(const T& Obj) override;
		bool RemoveAt(unsigned int Index) override;
		bool RemoveAllOf(T& Condition) override;
		bool RemoveDuplicates() override;

		bool Contains(const T& Obj) const override;
		template<typename Condition>
		bool Search(Condition Cond) const
		{
			if (_Size == 0)
				return false;
			else if (_Size == 1)
				return Cond(First->Value);

			for (seg Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Cond(Current->Value))
					return true;
			}

			return false;
		}
		template<typename Condition>
		T Find(Condition Cond) const
		{
			if (_Size == 0)
				return T();

			if (_Size == 1)
			{
				if (Cond(First->Value))
					return First->Value;
				else
					return T();
			}

			seg Current = First;
			for (unsigned int i = 0; i < _Size; i++)
			{
				if (Cond(Current->Value))
					return Current->Value;

				Current = Current->Next;
			}

			return T();
		}
		unsigned int IndexOf(const T& Obj) const;

		void Reverse() override;
		void Clear() override;
		bool IsEmpty() const override;

		Vector<T> SubVector(unsigned int BeginIndex) const;
		Vector<T> SubVector(unsigned int BeginIndex, unsigned int EndIndex) const;
		template<typename ReturnT, typename Conversion>
		Vector<ReturnT> ConvertVector(Conversion Conv)
		{
			Vector<ReturnT> Return;
			seg Current = First;
			for (unsigned int i = 0; i < _Size; i++)
			{
				Return.Add(Conv(Current->Value));
				Current = Current->Next;
			}
		}

		iter begin();
		iter end();
		const iter begin() const;
		const iter end() const;
		template<typename ConvType, typename ConvFunction>
		std::pair<BorrowedIterator<ConvType, seg, ConvFunction>, BorrowedIterator<ConvType, seg, ConvFunction>> Borrow(ConvFunction* Conversion)
		{
			using BItter = BorrowedIterator<ConvType, seg, ConvFunction>;
			if (Size == 0)
				return std::pair<BItter, BItter>(BItter(nullptr, Conversion), BItter(nullptr, Conversion));

			BItter First(First, Conversion);
			BItter Last(Last, Conversion);

			return std::pair<BItter, BItter>(First, Last);
		}

		friend Vector<T> operator+(const Vector<T>& One, const T& Two);
		friend Vector<T> operator+(const Vector<T>& One, const Vector<T>& Two);

		Vector<T>& operator=(const Vector<T>& Other);
		Vector<T>& operator=(const T& Obj);

		bool operator==(const Vector<T>& Other) const;
		bool operator!=(const Vector<T>& Other) const;

		operator std::vector<T>() const;
		void FromStdVector(const std::vector<T>& Other);
	protected:
		seg First;
		seg Last;
		unsigned int _Size;

		seg GetAtIndex(unsigned int Index) const;
		seg AddNewSegment(const T& Value);
	};

}

#include "Vector.tpp"