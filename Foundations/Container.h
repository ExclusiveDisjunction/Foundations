#pragma once

#include <iostream>

#include "Str.h"
#include "Types\BasicObject.h"
#include "Types\TypeRegistry.h"

#ifndef Vector_Type_Def
#define Vector_Type_Def

namespace Core
{
	template<typename T>
	class VectorIterator;
	template<typename T>
	class Vector;

	class VectorB;

	template<typename T>
	class ContainerSegment
	{
	public:
		ContainerSegment() : Next(nullptr), Last(nullptr) {}
		ContainerSegment(const T& Value) : Value(Value), Next(nullptr), Last(nullptr) {}

		ContainerSegment<T>* Next = nullptr;
		ContainerSegment<T>* Last = nullptr;
		T Value = T();

		friend Vector<T>;
		friend VectorB;
		friend VectorIterator<T>;
	};

	using uint = unsigned int;

	template<typename T>
	class VectorIterator
	{
	private:
		ContainerSegment<T>* Current;
	public:
		using iterator_category = std::forward_iterator_tag;
		using diffrence_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		VectorIterator(ContainerSegment<T>* This) : Current(This) {}

		reference operator*() const { return Current->Value; }
		pointer operator->() { return &Current->Value; }

		// Prefix increment
		VectorIterator& operator++() { Current = Current->Next; return *this; }

		// Postfix increment
		VectorIterator operator++(int) { VectorIterator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const VectorIterator& a, const VectorIterator& b) { return a.Current == b.Current; };
		friend bool operator!= (const VectorIterator& a, const VectorIterator& b) { return a.Current != b.Current; };
	};

	template<typename T>
	class Vector
	{
	private:
		using ThisType = Vector<T>;
		using seg = ContainerSegment<T>*;
		using uint = unsigned int;

		seg First;
		seg Last;
		uint _Size;

		seg GetAtIndex(uint Index) const
		{
			if (Index >= _Size)
				return nullptr;

			if (Index == 0 && _Size >= 1)
				return First;
			else if (Index == (_Size - 1) && _Size >= 1)
				return Last;

			Index++;
			seg Current = First;
			for (uint i = 1; i < Index; i++)
				Current = Current->Next;

			return Current;
		}
		seg AddNewSegment(const T& Value)
		{
			if (_Size == 0)
			{
				First = Last = new ContainerSegment<T>(Value);
				_Size++;
				return First;
			}
			else if (_Size == 1)
			{
				Last = new ContainerSegment<T>(Value);
				First->Next = Last;
				Last->Last = First;

				_Size++;
				return Last;
			}

			seg last = this->Last;
			seg New = new ContainerSegment<T>(Value);

			New->Last = last;

			if (_Size == 0)
				First = New;
			else
				last->Next = New;

			_Size++;
			this->Last = New;
			return New;
		}

	public:
		Vector()
		{
			First = 0;
			Last = 0;
			_Size = 0;
		}
		Vector(const Vector<T>& Other) : Vector()
		{
			for (uint i = 0; i < Other._Size; i++)
				this->Add(Other[i]);
		}
		Vector(const T* List, uint Len) : Vector()
		{
			for (uint i = 0; i < Len; i++)
				this->Add(List[i]);
		}
		Vector(const std::initializer_list<T>& List) : Vector()
		{
			for (T Item : List)
				this->Add(Item);
		}
		Vector(uint Len) : Vector()
		{
			for (uint i = 0u; i < Len; i++)
				Add(T());
		}
		Vector(T& Obj) : Vector()
		{
			AddNewSegment(Obj);
		}
		Vector(const T& Obj) : Vector()
		{
			AddNewSegment(Obj);
		}
		Vector(const T& Obj, uint Len) : Vector()
		{
			if (Len == 0)
				return;

			seg Current = First = new ContainerSegment<T>(Obj);
			for (uint i = 1; i < Len; i++)
			{
				seg Next = new ContainerSegment<T>(Obj);
				Current->Next = Next;
				Next->Last = Current;
				Current = Next;
			}

			Last = Current;
		}
		~Vector()
		{
			Clear();
		}

#ifdef _VECTOR_
		Vector(const std::vector<T>& Other) : Vector()
		{
			for (T&& Obj : Other)
				Add(Obj);
		}
#endif

		T& operator[](uint Index) const
		{
			if (Index >= _Size)
				throw "Out of range";

			return GetAtIndex(Index)->Value;
		}
		const uint& Size = _Size;

		void Add(const T& Obj)
		{
			AddNewSegment(Obj);
		}
		void Add(const Vector<T>& Other)
		{
			for (uint i = 0; i < Other._Size; i++)
				this->AddNewSegment(Other[i]);
		}
		void InsertAt(uint Index, const T& Value)
		{
			if (Index >= _Size)
				return;

			seg New = new ContainerSegment<T>();
			New->Value = Value;

			if (Index == 0)
			{
				New->Next = First;
				New->Last = nullptr;
				First->Last = New;

				First = New;
				_Size++;

				return;
			}

			seg At = GetAtIndex(Index);
			if (At->Next == nullptr)
			{
				At->Next = New;
				New->Last = At;
				New->Next = nullptr;

				_Size++;
				return;
			}

			seg Next = At->Next;
			At->Next = New;
			Next->Last = New;

			New->Next = Next;
			New->Last = At;
			_Size++;
		}
		bool Remove(const T& Obj)
		{
			return RemoveAt(IndexOf(Obj));
		}
		bool RemoveAt(uint Index)
		{
			if (Index >= _Size)
				return false;

			if (Index == 0) //Delete First Index
			{
				seg Second = First->Next;
				delete First;
				if (Second)
					Second->Last = 0;

				First = Second;
				_Size--;
				if (_Size == 0)
					First = Last = nullptr;
				return true;
			}
			else if (Index == (_Size - 1)) //Delete Last Index
			{
				seg SecondLast = Last->Last;
				delete Last;
				if (SecondLast)
					SecondLast->Next = 0;

				Last = SecondLast;
				_Size--;

				if (_Size == 0)
					First = Last = nullptr;
				return true;
			}

			seg Target = GetAtIndex(Index);
			seg Next = Target->Next;
			seg last = Target->Last;

			if (last)
				last->Next = Next;
			Next->Last = last;

			delete Target;
			_Size--;

			if (_Size == 0)
				First = this->Last = nullptr;

			return true;
		}
		bool RemoveAllOf(T& Condition)
		{
			seg Current = First;
			bool Return = true;
			for (uint i = 0; i < _Size && Current; i++)
			{
				if (Current->Value == Condition)
				{
					seg Last = Current->Last;
					seg Next = Current->Next;

					delete Current;
					if (Last == nullptr)
						First = Next;
					else if (Next == nullptr)
						this->Last = Last;

					if (Last)
						Last->Next = Next;
					if (Next)
						Next->Last = Last;

					Current = Next;
					_Size--;
					continue;
				}
				Current = Current->Next;
			}

			return Return;
		}
		bool RemoveDuplicates()
		{
			Vector<T> Refined;
			for (uint i = 0; i < _Size; i++)
			{
				T& Item = operator[](i);
				if (!Refined.Contains(Item))
					Refined.Add(Item);
			}

			this->operator=(Refined);

			return true;
		}

		bool Contains(const T& Obj) const
		{
			seg Current = First;

			for (uint i = 0; i < _Size; i++)
			{
				if (Obj == Current->Value)
					return true;

				Current = Current->Next;
			}

			return false;
		}
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
			for (uint i = 0; i < _Size; i++)
			{
				if (Cond(Current->Value))
					return Current->Value;

				Current = Current->Next;
			}

			return T();
		}
		uint IndexOf(const T& Obj) const
		{
			for (uint i = 0; i < _Size; i++)
			{
				if (Obj == operator[](i))
					return i;
			}

			return (uint)4294967295;
		}

		void Reverse()
		{
			if (_Size == 0 || _Size == 1) //For Single Size lists, and empty lists, do nothing.
				return;
			if (_Size == 2)
			{
				seg Temp = First;
				First = Last;
				Last = Temp;

				First->Last = nullptr;
				First->Next = Last;
				Last->Next = nullptr;
				Last->Last = First;

				return;
			}

			seg LastItem = nullptr;
			seg NewCurrent = nullptr;
			seg Current = Last;

			for (uint i = 0; i < _Size; i++)
			{
				seg Temp = NewCurrent;
				NewCurrent = new ContainerSegment<T>(Current->Value);
				LastItem = Temp;

				if (LastItem)
					LastItem->Next = NewCurrent;
				NewCurrent->Last = LastItem;

				Current = Current->Last;
			}

			Last = NewCurrent;

			Current = NewCurrent;
			for (uint i = 0; i < _Size - 1; i++)
				Current = Current->Last;

			First = Current;
		}
		void Clear()
		{
			if (_Size == 0 || !First || !Last)
				return;

			if (_Size == 1)
			{
				delete First;

				_Size = 0;
				First = nullptr;
				Last = nullptr;
				return;
			}
			else if (_Size == 2)
			{
				delete First;
				delete Last;

				_Size = 0;
				First = nullptr;
				Last = nullptr;
				return;
			}

			seg Current = this->First;
			for (uint i = 0; i < _Size; i++)
			{
				seg Temp = Current->Next;
				delete Current;
				Current = Temp;
			}

			_Size = 0;
			First = nullptr;
			Last = nullptr;
		}
		bool IsEmpty() const
		{
			return _Size == 0;
		}

		Vector<T> SubVector(uint BeginIndex) const
		{
			if (_Size == 0)
				return Vector<T>();

			return SubVector(BeginIndex, _Size - 1);
		}
		Vector<T> SubVector(uint BeginIndex, uint EndIndex) const
		{
			if (_Size == 0 || BeginIndex > EndIndex)
				return Vector<T>();

			if (BeginIndex >= _Size || EndIndex >= _Size)
				return Vector<T>(*this);

			uint Size = (EndIndex - BeginIndex) + 1;

			seg Begin = GetAtIndex(BeginIndex);

			seg NewFirst = new ContainerSegment<T>();
			NewFirst->Next = NewFirst->Last = 0;
			NewFirst->Value = Begin->Value;
			seg Last = NewFirst;
			seg Current = 0;
			for (uint i = 1, j = BeginIndex + 1; i < Size; i++, j++)
			{
				Current = new ContainerSegment<T>();
				Current->Last = Last;
				Current->Next = 0;
				Current->Value = operator[](j);
				Last->Next = Current;

				Last = Current;
			}

			Vector<T> Return = Vector<T>();
			Return._Size = Size;
			Return.First = NewFirst;
			Return.Last = Current;

			return Return;
		}
		template<typename ReturnT, typename Conversion>
		Vector<ReturnT> ConvertVector(Conversion Conv)
		{
			Vector<ReturnT> Return;
			seg Current = First;
			for (uint i = 0; i < _Size; i++)
			{
				Return.Add(Conv(Current->Value));
				Current = Current->Next;
			}
		}

		VectorIterator<T> begin() { return !First ? end() : VectorIterator<T>(First); }
		VectorIterator<T> end() { return VectorIterator<T>(!Last ? nullptr : Last->Next); }
		const VectorIterator<T> begin() const { return !First ? end() : VectorIterator<T>(First); }
		const VectorIterator<T> end() const { return VectorIterator<T>(!Last ? nullptr : Last->Next); }

		friend Vector<T> operator+(const Vector<T>& One, const T& Two)
		{
			Vector<T> Return = One;
			Return.Add(Two);

			return Return;
		}
		friend Vector<T> operator+(const Vector<T>& One, const Vector<T>& Two)
		{
			Vector<T> Return = One;
			Return.Add(Two);

			return Return;
		}

		Vector<T>& operator=(const Vector<T>& Other)
		{
			if (this->_Size != 0)
				Clear();

			if (Other._Size > 0)
			{
				for (uint i = 0; i < Other._Size; i++)
					Add(Other[i]);
			}

			return *this;
		}
		Vector<T>& operator=(const T& Obj)
		{
			Clear();
			AddNewSegment(Obj);

			return *this;
		}

		bool operator==(const Vector<T>& Other) const
		{
			Vector<T> One = *this;
			Vector<T> Two = Other;

			if (One._Size != Two._Size)
				return false;

			if (One._Size == 0 && Two._Size == 0)
				return true;

			bool Return = true;
			for (unsigned int i = 0; i < One._Size; i++)
				Return &= One[i] == Two[i];

			return Return;
		}
		bool operator!=(const Vector<T>& Other) const
		{
			Vector<T> One = *this;
			Vector<T> Two = Other;

			return !(One == Two);
		}

#ifdef _VECTOR_
		operator std::vector<T>() const
		{
			std::vector<T> Return;
			for (uint i = 0; i < _Size; i++)
				Return.push_back(operator[](i));

			return Return;
		}

		void FromStdVector(const std::vector<T>& Other)
		{
			this->Clear();

			for (T Item : Other)
				this->Add(Item);

			return *this;
		}
#endif
	};

	using IntList = Vector<int>;
	using LongList = Vector<long long>;
	template<typename T>
	using List = Vector<T>;

	class VectorB : public BasicObject
	{
	private:
		using ThisType = VectorB;
		using seg = ContainerSegment<BasicObject*>*;
		using uint = unsigned int;

		seg First;
		seg Last;
		uint _Size;

		seg GetAtIndex(uint Index) const
		{
			if (Index >= _Size)
				return nullptr;

			if (Index == 0 && _Size >= 1)
				return First;
			else if (Index == (_Size - 1) && _Size >= 1)
				return Last;

			Index++;
			seg Current = First;
			for (uint i = 1; i < Index; i++)
				Current = Current->Next;

			return Current;
		}
		seg AddNewSegment(BasicObject*& Value)
		{
			if (_Size == 0)
			{
				First = Last = new ContainerSegment<BasicObject*>(Value);
				_Size++;
				return First;
			}
			else if (_Size == 1)
			{
				Last = new ContainerSegment<BasicObject*>(Value);
				First->Next = Last;
				Last->Last = First;

				_Size++;
				return Last;
			}

			seg last = this->Last;
			seg New = new ContainerSegment<BasicObject*>(Value);

			New->Last = last;

			if (_Size == 0)
				First = New;
			else
				last->Next = New;

			_Size++;
			this->Last = New;
			return New;
		}

	public:
		VectorB()
		{
			First = 0;
			Last = 0;
			_Size = 0;
		}
		VectorB(const VectorB& Other) : VectorB()
		{
			for (uint i = 0; i < Other._Size; i++)
				this->Add(Other[i]);
		}
		VectorB(BasicObject** List, uint Len) : VectorB()
		{
			for (uint i = 0; i < Len; i++)
				this->Add(List[i]);
		}
		VectorB(const std::initializer_list<BasicObject*>& List) : VectorB()
		{
			for (BasicObject* Item : List)
				this->Add(Item);
		}
		VectorB(const Vector<BasicObject*>& List) : _Size(0), First(nullptr), Last(nullptr)
		{
			for (BasicObject* Item : List)
				this->Add(Item);
		}
		template<typename T, typename Conversion>
		VectorB(const Vector<T>& List, Conversion Conv) : _Size(0)
		{
			for (T Item : List)
			{
				BasicObject* Converted = Conv(Item);
				this->Add(Converted);
			}
		}
		VectorB(BasicObject* Obj) : VectorB()
		{
			AddNewSegment(Obj);
		}
		~VectorB()
		{
			Clear();
		}

#ifdef _VECTOR_
		VectorB(const std::vector<BasicObject*>& Other) : VectorB()
		{
			for (BasicObject* Obj : Other)
				Add(Obj);
		}
#endif

		BasicObject*& operator[](uint Index) const
		{
			if (Index >= _Size)
				throw "Out of range";

			return GetAtIndex(Index)->Value;
		}
		const uint& Size = _Size;

		void Add(BasicObject* Obj)
		{
			AddNewSegment(Obj);
		}
		void Add(const VectorB& Other)
		{
			for (uint i = 0; i < Other._Size; i++)
				this->AddNewSegment(Other[i]);
		}
		void InsertAt(uint Index, BasicObject* Value)
		{
			if (Index >= _Size)
				return;

			seg New = new ContainerSegment<BasicObject*>();
			New->Value = Value;

			if (Index == 0)
			{
				New->Next = First;
				New->Last = nullptr;
				First->Last = New;

				First = New;
				_Size++;

				return;
			}

			seg At = GetAtIndex(Index);
			if (At->Next == nullptr)
			{
				At->Next = New;
				New->Last = At;
				New->Next = nullptr;

				_Size++;
				return;
			}

			seg Next = At->Next;
			At->Next = New;
			Next->Last = New;

			New->Next = Next;
			New->Last = At;
			_Size++;
		}
		bool Remove(BasicObject* Obj)
		{
			return RemoveAt(IndexOf(Obj));
		}
		bool RemoveAt(uint Index)
		{
			if (Index >= _Size)
				return false;

			if (Index == 0) //Delete First Index
			{
				seg Second = First->Next;
				delete First->Value;
				delete First;
				if (Second)
					Second->Last = 0;

				First = Second;
				_Size--;
				if (_Size == 0)
					First = Last = nullptr;
				return true;
			}
			else if (Index == (_Size - 1)) //Delete Last Index
			{
				seg SecondLast = Last->Last;
				delete Last->Value;
				delete Last;
				if (SecondLast)
					SecondLast->Next = 0;

				Last = SecondLast;
				_Size--;

				if (_Size == 0)
					First = Last = nullptr;
				return true;
			}

			seg Target = GetAtIndex(Index);
			seg Next = Target->Next;
			seg last = Target->Last;

			if (last)
				last->Next = Next;
			Next->Last = last;

			delete Target->Value;
			delete Target;
			_Size--;

			if (_Size == 0)
				First = this->Last = nullptr;

			return true;
		}
		bool RemoveAllOf(BasicObject*& Condition)
		{
			seg Current = First;
			bool Return = true;
			for (uint i = 0; i < _Size && Current; i++)
			{
				if (Current->Value == Condition)
				{
					seg Last = Current->Last;
					seg Next = Current->Next;

					delete Current->Value;
					delete Current;
					if (Last == nullptr)
						First = Next;
					else if (Next == nullptr)
						this->Last = Last;

					if (Last)
						Last->Next = Next;
					if (Next)
						Next->Last = Last;

					Current = Next;
					_Size--;
					continue;
				}
				Current = Current->Next;
			}

			return Return;
		}
		bool RemoveDuplicates()
		{
			VectorB Refined;
			for (uint i = 0; i < _Size; i++)
			{
				BasicObject*& Item = operator[](i);
				if (!Refined.Contains(Item))
					Refined.Add(Item);
			}

			Clear();
			this->operator=(Refined);

			return true;
		}

		bool Contains(BasicObject* Obj) const
		{
			seg Current = First;

			for (uint i = 0; i < _Size; i++)
			{
				if (Obj == Current->Value)
					return true;

				Current = Current->Next;
			}

			return false;
		}
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
		BasicObject* Find(Condition Cond) const
		{
			if (_Size == 0)
				return nullptr;

			if (_Size == 1)
			{
				if (Cond(First->Value))
					return First->Value;
				else
					return nullptr;
			}

			seg Current = First;
			for (uint i = 0; i < _Size; i++)
			{
				if (Cond(Current->Value))
					return Current->Value;

				Current = Current->Next;
			}

			return nullptr;
		}
		uint IndexOf(BasicObject* Obj) const
		{
			for (uint i = 0; i < _Size; i++)
			{
				if (Obj == operator[](i))
					return i;
			}

			return (uint)4294967295;
		}

		void Reverse()
		{
			if (_Size == 0 || _Size == 1) //For Single Size lists, and empty lists, do nothing.
				return;
			if (_Size == 2)
			{
				seg Temp = First;
				First = Last;
				Last = Temp;

				First->Last = nullptr;
				First->Next = Last;
				Last->Next = nullptr;
				Last->Last = First;

				return;
			}

			seg LastItem = nullptr;
			seg NewCurrent = nullptr;
			seg Current = Last;

			for (uint i = 0; i < _Size; i++)
			{
				seg Temp = NewCurrent;
				NewCurrent = new ContainerSegment<BasicObject*>(Current->Value);
				LastItem = Temp;

				if (LastItem)
					LastItem->Next = NewCurrent;
				NewCurrent->Last = LastItem;

				Current = Current->Last;
			}

			Last = NewCurrent;

			Current = NewCurrent;
			for (uint i = 0; i < _Size - 1; i++)
				Current = Current->Last;

			First = Current;
		}
		void Clear()
		{
			if (_Size == 0 || !First || !Last)
				return;

			if (_Size == 1)
			{
				delete First->Value;
				delete First;

				_Size = 0;
				First = nullptr;
				Last = nullptr;
				return;
			}
			else if (_Size == 2)
			{
				delete First->Value;
				delete First;
				delete Last->Value;
				delete Last;

				_Size = 0;
				First = nullptr;
				Last = nullptr;
				return;
			}

			seg Current = this->First;
			for (uint i = 0; i < _Size; i++)
			{
				seg Temp = Current->Next;
				delete Current->Value;
				delete Current;
				Current = Temp;
			}

			_Size = 0;
			First = nullptr;
			Last = nullptr;
		}
		bool IsEmpty() const
		{
			return _Size == 0;
		}

		VectorB SubVector(uint BeginIndex) const
		{
			if (_Size == 0)
				return VectorB();

			return SubVector(BeginIndex, _Size - 1);
		}
		VectorB SubVector(uint BeginIndex, uint EndIndex) const
		{
			if (_Size == 0 || BeginIndex > EndIndex)
				return VectorB();

			if (BeginIndex >= _Size || EndIndex >= _Size)
				return VectorB(*this);

			uint Size = (EndIndex - BeginIndex) + 1;

			seg Begin = GetAtIndex(BeginIndex);

			seg NewFirst = new ContainerSegment<BasicObject*>();
			NewFirst->Next = NewFirst->Last = 0;
			NewFirst->Value = Begin->Value;
			seg Last = NewFirst;
			seg Current = 0;
			for (uint i = 1, j = BeginIndex + 1; i < Size; i++, j++)
			{
				Current = new ContainerSegment<BasicObject*>();
				Current->Last = Last;
				Current->Next = 0;
				Current->Value = operator[](j);
				Last->Next = Current;

				Last = Current;
			}

			VectorB Return = VectorB();
			Return._Size = Size;
			Return.First = NewFirst;
			Return.Last = Current;

			return Return;
		}
		template<typename ReturnT, typename Conversion>
		Vector<ReturnT> ConvertVector(Conversion Conv)
		{
			Vector<ReturnT> Return;
			seg Current = First;
			for (uint i = 0; i < _Size; i++)
			{
				Return.Add(Conv(Current->Value));
				Current = Current->Next;
			}
		}

		String ToString() const override
		{
			if (Size == 0)
				return L"-V(){}";

			String Name = (String)First->Value->TypeName();
			String Return = L"-V(" + Name + L"){";

			if (Size == 0)
				return (Return + L'}');


			Return += static_cast<BasicObject*>(First->Value)->ToString();
			for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
				Return += L',' + Current->Value->ToString();

			Return += L"}";
			return Return;
		}
		String ToUIString() const override
		{
			if (Size == 0)
				return L"{ Collection is Empty. }";

			String Return = L"{ ";

			Return += First->Value->ToUIString();
			for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
				Return += L", " + Current->Value->ToUIString();

			Return += L" }";
			return Return;
		}
		String TypeName() const override { return L"VECTOR"; }

		BasicObject* DefaultValue() const override { return new VectorB(); }
		BasicObject* Clone() const override
		{
			VectorB* Return = new VectorB();
			for (seg Current = First; Current != nullptr; Current = Current->Next)
				Return->Add(Current->Value->Clone());

			return Return;
		}
		bool OverrideFrom(BasicObject* Obj) override
		{
			VectorB* Conv1 = dynamic_cast<VectorB*>(Obj);
			if (Conv1)
			{
				*this = *Conv1;
				return true;
			}

			String* Conv2 = dynamic_cast<String*>(Obj);

			if (Conv2)
				this->FillFromString(*Conv2);
			else
				return false;

			return true;
		}

		void FillFromString(const String& Obj) override
		{
			if (!Obj.BeginsWith(L"-V("))
				return;

			String UObj = Obj; //Removes Const qualifer
			UObj.RemoveAt(0); //-
			UObj.RemoveAt(0); //V
			UObj.RemoveAt(0); //(

			unsigned i = 0;
			for (i = 0; i < UObj.Length(); i++)
				if (UObj[i] == L')')
					break;

			String TypeString = UObj.SubString(0, i-1);
			if (TypeString == String())
				TypeString = L"STRING";

			if (UObj[UObj.Length() - 1] != L'}')
				return;

			String Splined = UObj.SubString(i+2, UObj.Length() - 2);
			if (Splined == String())
				return;

			StringList List = Splined.Split(L',');

			//Assumes that BasicObject* is a pointer to a class that inherits from BasicObject.
			for (String Item : List)
			{
				BasicObject* Template = TypeRegistry::Retrive(TypeString);
				Template->FillFromString(Item);
				Add(Template);
			}
		}

		bool HasModifyer() const override { return true; }
		BasicObjectModifyer* ConstructModifyer() const override { return nullptr; }

		VectorIterator<BasicObject*> begin() { return !First ? end() : VectorIterator<BasicObject*>(First); }
		VectorIterator<BasicObject*> end() { return VectorIterator<BasicObject*>(!Last ? nullptr : Last->Next); }
		const VectorIterator<BasicObject*> begin() const { return !First ? end() : VectorIterator<BasicObject*>(First); }
		const VectorIterator<BasicObject*> end() const { return VectorIterator<BasicObject*>(!Last ? nullptr : Last->Next); }

		friend VectorB operator+(const VectorB& One, BasicObject* Two)
		{
			VectorB Return = One;
			Return.Add(Two);

			return Return;
		}
		friend VectorB operator+(const VectorB& One, const VectorB& Two)
		{
			VectorB Return = One;
			Return.Add(Two);

			return Return;
		}

		VectorB& operator=(VectorB& Other)
		{
			if (this->_Size != 0)
				Clear();

			if (Other._Size > 0)
			{
				for (uint i = 0; i < Other._Size; i++)
					Add(Other[i]);
			}

			return *this;
		}
		VectorB& operator=(const VectorB& Other)
		{
			if (this->_Size != 0)
				Clear();

			if (Other._Size > 0)
			{
				for (uint i = 0; i < Other._Size; i++)
					Add(Other[i]);
			}

			return *this;
		}
		VectorB& operator=(BasicObject* Obj)
		{
			Clear();
			AddNewSegment(Obj);

			return *this;
		}

		bool operator==(const VectorB& Other) const
		{
			VectorB One = *this;
			VectorB Two = Other;

			if (One._Size != Two._Size)
				return false;

			if (One._Size == 0 && Two._Size == 0)
				return true;

			bool Return = true;
			for (unsigned int i = 0; i < One._Size; i++)
				Return &= One[i] == Two[i];

			return Return;
		}
		bool operator!=(const VectorB& Other) const
		{
			VectorB One = *this;
			VectorB Two = Other;

			return !(One == Two);
		}

#ifdef _VECTOR_
		operator std::vector<BasicObject*>() const
		{
			std::vector<BasicObject*> Return;
			for (uint i = 0; i < _Size; i++)
				Return.push_back(operator[](i));

			return Return;
		}

		VectorB& operator=(const std::vector<BasicObject*>& Other)
		{
			this->Clear();

			for (BasicObject* Item : Other)
				this->Add(Item);

			return *this;
		}
		VectorB& operator=(const Vector<BasicObject*>& Other)
		{
			this->Clear();

			for (BasicObject* Item : Other)
				this->Add(Item);

			return *this;
		}
#endif
	};
}

#endif