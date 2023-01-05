#pragma once

#include <iostream>

#include "Types\BasicObject.h"
#include "Str.h"

#ifndef Mapping_Type_Def
#define Mapping_Type_Def

namespace Foundations
{
	template<typename TKey, typename TValue> class Mapping;
	class MappingB;

	template<typename TKey, typename TValue>
	class MappingPair
	{
	public:
		TKey Key;
		TValue Value;

		MappingPair(const TKey& _Key, const TValue& _Value) : Key(const_cast<TKey&>(_Key)), Value(const_cast<TValue&>(_Value)) {}
		MappingPair()
		{
			Key = TKey();
			Value = TValue();
		}
	};

	template<typename TKey, typename TValue>
	class MappingSegment
	{
	public:
		MappingSegment* Next = nullptr, * Last = nullptr;

		MappingPair<TKey, TValue> Root;
		TKey& Key = Root.Key;
		TValue& Value = Root.Value;

		MappingSegment(const TKey& _Key, const TValue& _Value) : Root(_Key, _Value), Next(nullptr), Last(nullptr) {}
		MappingSegment() : Root() {}

		operator MappingPair<TKey, TValue>& ()
		{
			return Root;
		}
	public:
		friend Mapping<TKey, TValue>;
	};

	template<typename TKey, typename TValue>
	class MappingIterator
	{
	private:
		MappingSegment<TKey, TValue>* Current;
	public:
		using iterator_category = std::forward_iterator_tag;
		using diffrence_type = std::ptrdiff_t;
		using value_type = MappingPair<TKey, TValue>;
		using pointer = value_type*;
		using reference = value_type&;

		MappingIterator(MappingSegment<TKey, TValue>* This) : Current(This) {}

		reference operator*() const { return Current->Root; }
		pointer operator->() { return &Current->Root; }

		// Prefix increment
		MappingIterator& operator++() { Current = Current->Next; return *this; }

		// Postfix increment
		MappingIterator operator++(int) { MappingIterator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const MappingIterator& a, const MappingIterator& b) { return a.Current == b.Current; };
		friend bool operator!= (const MappingIterator& a, const MappingIterator& b) { return a.Current != b.Current; };
	};

	template<typename TKey, typename TValue>
	class Mapping
	{
	private:
		using seg = MappingSegment<TKey, TValue>*;
		using pair = MappingPair<TKey, TValue>;
		using uint = unsigned int;

		seg GetAtIndex(uint Index) const
		{
			seg Return;

			if (_Size == 0 || Index >= _Size)
				Return = nullptr;
			else if (Index == 0)
				Return = First;
			else if (Index == 1 && _Size != 2)
				Return = First->Next;
			else if (Index == _Size - 1)
				Return = Last;
			else if (Index == _Size - 2)
				Return = Last->Last;
			else
			{
				Return = First;
				for (uint i = 0; i < Index; i++)
					Return = Return->Next;
			}

			return Return;
		}
		seg AddNewSegment(const TKey& Key, const TValue& Value)
		{
			seg Return = new MappingSegment<TKey, TValue>(Key, Value);

			if (_Size == 0)
			{
				First = Last = Return;
				_Size = 1;
			}
			else if (_Size == 1)
			{
				Last = Return;
				First->Next = Last;

				_Size++;
			}
			else
			{
				Last->Next = Return;
				Return->Last = Last;
				Last = Return;

				_Size++;
			}

			return Return;
		}

		seg First, Last;
		uint _Size;

	public:
		Mapping() : _Size(0), First(nullptr), Last(nullptr) {}
		Mapping(const MappingPair<TKey, TValue>& Obj)
		{
			Add(Obj);
		}
		Mapping(const Mapping<TKey, TValue>& Obj)
		{
			_Size = Obj._Size;
			for (uint i = 0; i < Obj.Size; i++)
				Add(Obj[i]);
		}
		Mapping(Mapping<TKey, TValue>&& Obj) noexcept
		{
			_Size = Obj._Size;
			First = Obj.First;
			Last = Obj.Last;

			Obj._Size = 0;
			Obj.First = Obj.Last = nullptr;
		}
		Mapping(const std::initializer_list<pair>& Obj)
		{
			for (pair Item : Obj)
				Add(Item);
		}
		~Mapping()
		{
			Clear();
		}

		Mapping<TKey, TValue>& operator=(const Mapping<TKey, TValue>& Obj)
		{
			Clear();
			_Size = Obj.Size;
			for (uint i = 0; i < Obj.Size; i++)
				Add(Obj[i]);

			return *this;
		}
		Mapping<TKey, TValue>& operator=(Mapping<TKey, TValue>&& Obj)
		{
			Clear();
			First = Obj.First;
			Last = Obj.Last;

			Obj._Size = 0;
			Obj.First = Obj.Last = nullptr;

			return *this;
		}

		const uint& Size = _Size;
		pair& operator[](uint Index) const
		{
			seg Return = GetAtIndex(Index);
			if (!Return)
				throw std::exception("The index provided was out of range.");

			return Return->Root;
		}
		TValue& operator[](const TKey& Key) const
		{
			seg Current = First;
			for (uint i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return Current->Value;
			}

			throw std::exception("The key provided is not in this mapping");
		}
		pair& Item(uint Index) const { return operator[](Index); }

		pair& Search(const TKey& For, bool& Found) const
		{
			for (seg Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Key == For)
				{
					Found = true;
					return Current->operator MappingPair<TKey, TValue>&();
				}
			}

			Found = false;
			MappingPair<TKey, TValue> Temp;
			return Temp;
		}
		pair& SearchValue(const TValue& For, bool& Found) const
		{
			for (seg Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Value == For)
				{
					Found = true;
					return Current->operator MappingPair<TKey, TValue>&();
				}
			}

			Found = false;
			MappingPair<TKey, TValue> Temp;
			return Temp;
		}

		void Add(const TKey& Key, const TValue& Value)
		{
			if (Contains(Key))
				return;

			AddNewSegment(Key, Value);
		}
		void Add(const pair& Pair)
		{
			if (Contains(Pair.Key))
				return;

			AddNewSegment(Pair.Key, Pair.Value);
		}
		bool Remove(const TKey& Key)
		{
			return RemoveAt(IndexOf(Key));
		}
		bool RemoveValue(const TValue& Value)
		{
			uint index = 0;
			for (seg current = First; current != nullptr; current = current->Next, index++)
			{
				if (current->Value == Value)
					break;
			}

			return RemoveAt(index);
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
			seg Last = Target->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;

			delete Target;
			_Size--;

			if (_Size == 0)
				First = this->Last = nullptr;

			return true;
		}
		void Clear()
		{
			if (_Size == 0 || !First || !Last)
				return;

			if (Size == 1)
			{
				delete First;

				First = Last = nullptr;
				_Size = 0;
				return;
			}
			else if (Size == 2)
			{
				delete First;
				delete Last;

				First = Last = nullptr;
				_Size = 0;
				return;
			}

			seg Current = this->First;
			for (uint i = 0; i < Size; i++)
			{
				seg Temp = Current->Next;
				delete Current;
				Current = Temp;
			}

			_Size = 0;
			First = Last = nullptr;
		}

		bool Contains(const TKey& Key)
		{
			seg Current = First;
			for (uint i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return true;
			}

			return false;
		}
		uint IndexOf(const TKey& Key)
		{
			seg Current = First;
			for (uint i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return i;
			}

			return UINT64_MAX;
		}

		MappingIterator<TKey, TValue> begin() { return !First ? end() : MappingIterator<TKey, TValue>(First); }
		MappingIterator<TKey, TValue> end() { return MappingIterator<TKey, TValue>(!Last ? nullptr : Last->Next); }
		const MappingIterator<TKey, TValue> begin() const { return !First ? end() : MappingIterator<TKey, TValue>(First); }
		const MappingIterator<TKey, TValue> end() const { return MappingIterator<TKey, TValue>(!Last ? nullptr : Last->Next); }
	};

	class MappingB : public BasicObject
	{
	private:
		using TKey = BasicObject*;
		using TValue = BasicObject*;
		using seg = MappingSegment<TKey, TValue>*;
		using pair = MappingPair<TKey, TValue>;
		using uint = unsigned int;

		seg GetAtIndex(uint Index) const
		{
			seg Return;

			if (_Size == 0 || Index >= _Size)
				Return = nullptr;
			else if (Index == 0)
				Return = First;
			else if (Index == 1 && _Size != 2)
				Return = First->Next;
			else if (Index == _Size - 1)
				Return = Last;
			else if (Index == _Size - 2)
				Return = Last->Last;
			else
			{
				Return = First;
				for (uint i = 0; i < Index; i++)
					Return = Return->Next;
			}

			return Return;
		}
		seg AddNewSegment(const TKey& Key, const TValue& Value)
		{
			seg Return = new MappingSegment<TKey, TValue>(Key, Value);

			if (_Size == 0)
			{
				First = Last = Return;
				_Size = 1;
			}
			else if (_Size == 1)
			{
				Last = Return;
				First->Next = Last;

				_Size++;
			}
			else
			{
				Last->Next = Return;
				Return->Last = Last;
				Last = Return;

				_Size++;
			}

			return Return;
		}

		seg First, Last;
		uint _Size;

	public:
		MappingB() : _Size(0), First(nullptr), Last(nullptr) {}
		MappingB(const MappingPair<TKey, TValue>& Obj)
		{
			Add(Obj);
		}
		MappingB(const std::initializer_list<pair>& Obj)
		{
			for (pair Item : Obj)
				Add(Item);
		}
		MappingB(const MappingB& Obj) = delete;
		MappingB(MappingB&& Obj) = delete;
		~MappingB()
		{
			Clear();
		}

		MappingB& operator=(const MappingB& Obj) = delete;
		MappingB& operator=(MappingB&& Obj) = delete;

		const uint& Size = _Size;
		pair& operator[](uint Index) const
		{
			seg Return = GetAtIndex(Index);
			if (!Return)
				throw std::exception("The index provided was out of range.");

			return Return->Root;
		}
		TValue& operator[](const TKey& Key) const
		{
			seg Current = First;
			for (uint i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return Current->Value;
			}

			throw std::exception("The key provided is not in this MappingB");
		}
		pair& Item(uint Index) const { return operator[](Index); }

		pair& Search(const TKey& For, bool& Found) const
		{
			for (seg Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Key == For)
				{
					Found = true;
					return Current->operator MappingPair<TKey, TValue>&();
				}
			}

			Found = false;
			MappingPair<TKey, TValue> Temp;
			return Temp;
		}
		pair& SearchValue(const TValue& For, bool& Found) const
		{
			for (seg Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Value == For)
				{
					Found = true;
					return Current->operator MappingPair<TKey, TValue>&();
				}
			}

			Found = false;
			MappingPair<TKey, TValue> Temp;
			return Temp;
		}

		void Add(const TKey& Key, const TValue& Value)
		{
			if (Contains(Key))
				return;

			AddNewSegment(Key, Value);
		}
		void Add(const pair& Pair)
		{
			if (Contains(Pair.Key))
				return;

			AddNewSegment(Pair.Key, Pair.Value);
		}
		bool Remove(const TKey& Key)
		{
			return RemoveAt(IndexOf(Key));
		}
		bool RemoveValue(const TValue& Value)
		{
			uint index = 0;
			for (seg current = First; current != nullptr; current = current->Next, index++)
			{
				if (current->Value == Value)
					break;
			}

			return RemoveAt(index);
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
			seg Last = Target->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;

			delete Target->Value;
			delete Target;
			_Size--;

			if (_Size == 0)
				First = this->Last = nullptr;

			return true;
		}
		void Clear()
		{
			if (_Size == 0 || !First || !Last)
				return;

			if (Size == 1)
			{
				delete First->Value;
				delete First;

				First = Last = nullptr;
				_Size = 0;
				return;
			}
			else if (Size == 2)
			{
				delete First->Value;
				delete First;
				delete Last->Value;
				delete Last;

				First = Last = nullptr;
				_Size = 0;
				return;
			}

			seg Current = this->First;
			for (uint i = 0; i < Size; i++)
			{
				seg Temp = Current->Next;
				delete Current;
				Current = Temp;
			}

			_Size = 0;
			First = Last = nullptr;
		}

		bool Contains(const TKey& Key)
		{
			seg Current = First;
			for (uint i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return true;
			}

			return false;
		}
		uint IndexOf(const TKey& Key)
		{
			seg Current = First;
			for (uint i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return i;
			}

			return (uint)UINT64_MAX;
		}

		//TODO:Finish MappingB & vector basic object inhertiance.

		String ToString() const override
		{
			if (Size == 0)
				return L"-M(){}";

			String KeyName = (String)First->Key->TypeName(), ValueName = (String)First->Value->TypeName();
			String Return = L"-M(" + KeyName + L',' + ValueName + L"){";

			Return += First->Key->ToString() + L',' + First->Value->ToString();
			for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
				Return += L',' + Current->Key->ToString() + L',' + Current->Value->ToString();

			Return += L"}";
			return Return;
		}
		String ToUIString() const override
		{ 
			if (Size == 0)
				return L"{ Collection is Empty. }";

			String Return = L"{ ";

			Return += First->Key->ToUIString() + L" : " + First->Value->ToUIString();
			for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
				Return += L", " + Current->Key->ToUIString() + L" : " + Current->Value->ToUIString();

			Return += L" }";
			return Return;
		}
		String TypeName() const override { return L"MAPPING"; }

		BasicObject* DefaultValue() const override { return new MappingB(); }
		BasicObject* Clone() const override
		{
			MappingB* Return = new MappingB();
			for (seg Current = First; Current != nullptr; Current = Current->Next)
			{
				BasicObject* NewK = Current->Key->Clone(), * NewV = Current->Value->Clone();
				Return->Add(NewK, NewV);
			}

			return Return;
		}
		bool OverrideFrom(BasicObject* Obj) override
		{
			MappingB* FirstConv = dynamic_cast<MappingB*>(Obj);
			if (FirstConv)
			{
				Clear();
				MappingB* Cloned = dynamic_cast<MappingB*>(FirstConv->Clone());
				if (Cloned)
				{
					First = Cloned->First;
					Last = Cloned->Last;
					_Size = Cloned->Size;

					Cloned->First = Cloned->Last = nullptr;
					Cloned->_Size = 0;
					delete Cloned;
					return true;
				}
				return false;
			}

			String* SecondConv = dynamic_cast<String*>(Obj);
			if (SecondConv)
			{
				this->FillFromString(*SecondConv);
				return true;
			}

			return false;
		}

		void FillFromString(const String& Obj) override
		{
			if (!Obj.BeginsWith(L"-M("))
				return;

			String UObj = Obj;
			UObj.RemoveAt(0); //-
			UObj.RemoveAt(0); //M
			UObj.RemoveAt(0); //(

			unsigned i = 0;
			for (i = 0; i < UObj.Length(); i++)
				if (UObj[i] == L')')
					break;

			String TypeStringRaw = UObj.SubString(0, i-1);
			StringList TypeStrings;
			if (TypeStringRaw == String())
				TypeStrings = { L"STRING", L"STRING" };
			else
				TypeStrings = TypeStringRaw.Split(',');

			if (TypeStrings.Size == 1)
				TypeStrings.Add(L"STRING");
			if (TypeStrings[0] == String())
				TypeStrings[0] = L"STRING";
			if (TypeStrings[1] == String())
				TypeStrings[1] = L"STRING";

			if (UObj[UObj.Length() - 1] != L'}')
				return;

			String Splined = UObj.SubString(i + 2, UObj.Length() - 2);
			if (Splined == String())
				return;
			StringList List = Splined.Split(L',');

			VectorIterator<String> Iterator = List.begin(), End = List.end();
			i = 0;
			MappingPair<BasicObject*, BasicObject*> Temp;
			for (i = 0; Iterator != End; i++, Iterator++)
			{
				if (i % 2) //Odd, value item
				{					
					Temp.Value = TypeRegistry::Retrive(TypeStrings[1]);
					Temp.Value->FillFromString(*Iterator);
					Add(Temp);
				}
				else //Even, key item
				{
					Temp.Key = TypeRegistry::Retrive(TypeStrings[0]);
					Temp.Key->FillFromString(*Iterator);
				}
			}
				
		}

		bool HasModifyer() const override { return true; }
		BasicObjectModifyer* ConstructModifyer() const override { return nullptr; }

		MappingIterator<BasicObject*, BasicObject*> begin() { return !First ? end() : MappingIterator<BasicObject*, BasicObject*>(First); }
		MappingIterator<BasicObject*, BasicObject*> end() { return MappingIterator<BasicObject*, BasicObject*>(!Last ? nullptr : Last->Next); }
		const MappingIterator<BasicObject*, BasicObject*> begin() const { return !First ? end() : MappingIterator<BasicObject*, BasicObject*>(First); }
		const MappingIterator<BasicObject*, BasicObject*> end() const { return MappingIterator<BasicObject*, BasicObject*>(!Last ? nullptr : Last->Next); }
	};
}

#endif // !Mapping_Type_Def
