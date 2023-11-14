#ifndef VECTOR_TPP
#define VECTOR_TPP

#include "Vector.h"

namespace Core
{
	template<typename T>
	Vector<T>::Vector()
	{
		First = 0;
		Last = 0;
		_Size = 0;
	}
	template<typename T>
	Vector<T>::Vector(const SequenceBase<T>*& Other) : Vector()
	{
		unsigned int Size = Other->Length();
		for (unsigned int i = 0; i < Size; i++)
			this->Add(Other->Item(i));
	}
	template<typename T>
	Vector<T>::Vector(const T* List, unsigned int Len) : Vector()
	{
		for (unsigned int i = 0; i < Len; i++)
			this->Add(List[i]);
	}
	template<typename T>
	Vector<T>::Vector(const std::initializer_list<T>& List) : Vector()
	{
		for (T Item : List)
			this->Add(Item);
	}
	template<typename T>
	Vector<T>::Vector(unsigned int Len) : Vector()
	{
		for (unsigned int i = 0u; i < Len; i++)
			Add(T());
	}
	template<typename T>
	Vector<T>::Vector(T& Obj) : Vector()
	{
		AddNewSegment(Obj);
	}
	template<typename T>
	Vector<T>::Vector(const T& Obj) : Vector()
	{
		AddNewSegment(Obj);
	}
	template<typename T>
	Vector<T>::Vector(const T& Obj, unsigned int Len) : Vector()
	{
		if (Len == 0)
			return;

		seg Current = First = new VectorSeg<T>(Obj);
		for (unsigned int i = 1; i < Len; i++)
		{
			seg Next = new VectorSeg<T>(Obj);
			Current->Next = Next;
			Next->Last = Current;
			Current = Next;
		}

		Last = Current;
	}
	template<typename T>
	Vector<T>::Vector(const std::vector<T>& Other) : Vector()
	{
		for (T&& Obj : Other)
			Add(Obj);
	}
	template<typename T>
	Vector<T>::~Vector()
	{
		Clear();
	}

	template<typename T>
	Vector<T>::seg Vector<T>::GetAtIndex(unsigned int Index) const
	{
		if (Index >= _Size)
			return nullptr;

		if (Index == 0 && _Size >= 1)
			return First;
		else if (Index == (_Size - 1) && _Size >= 1)
			return Last;

		Index++;
		seg Current = First;
		for (unsigned int i = 1; i < Index; i++)
			Current = Current->Next;

		return Current;
	}
	template<typename T>
	Vector<T>::seg Vector<T>::AddNewSegment(const T& Value)
	{
		if (_Size == 0)
		{
			First = Last = new VectorSeg<T>(Value);
			_Size++;
			return First;
		}
		else if (_Size == 1)
		{
			Last = new VectorSeg<T>(Value);
			First->Next = Last;
			Last->Last = First;

			_Size++;
			return Last;
		}

		seg last = this->Last;
		seg New = new VectorSeg<T>(Value);

		New->Last = last;

		if (_Size == 0)
			First = New;
		else
			last->Next = New;

		_Size++;
		this->Last = New;
		return New;
	}

	template<typename T>
	T& Vector<T>::operator[](unsigned int Index) const
	{
		if (Index >= _Size)
			throw "Out of range";

		return GetAtIndex(Index)->Value;
	}
	template<typename T>
	T& Vector<T>::Item(unsigned int Index) const
	{ 
		return operator[](Index); 
	}
	template<typename T>
	unsigned int Vector<T>::Length() const 
	{
		return _Size; 
	}

	template<typename T>
	void Vector<T>::Add(const T& Obj)
	{
		AddNewSegment(Obj);
	}
	template<typename T>
	void Vector<T>::AddList(const SequenceBase<T>* Other)
	{
		unsigned int Size = Other->Length();
		for (unsigned int i = 0; i < Size; i++)
			this->AddNewSegment(Other->Item(i));
	}
	template<typename T>
	void Vector<T>::InsertAt(unsigned int Index, const T& Value)
	{
		if (Index >= _Size)
			return;

		seg New = new VectorSeg<T>();
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
	template<typename T>
	bool Vector<T>::Remove(const T& Obj)
	{
		return RemoveAt(IndexOf(Obj));
	}
	template<typename T>
	bool Vector<T>::RemoveAt(unsigned int Index)
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
	template<typename T>
	bool Vector<T>::RemoveAllOf(T& Condition)
	{
		seg Current = First;
		bool Return = true;
		for (unsigned int i = 0; i < _Size && Current; i++)
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
	template<typename T>
	bool Vector<T>::RemoveDuplicates()
	{
		Vector<T> Refined;
		for (unsigned int i = 0; i < _Size; i++)
		{
			T& Item = operator[](i);
			if (!Refined.Contains(Item))
				Refined.Add(Item);
		}

		this->operator=(Refined);

		return true;
	}

	template<typename T>
	bool Vector<T>::Contains(const T& Obj) const
	{
		seg Current = First;

		for (unsigned int i = 0; i < _Size; i++)
		{
			if (Obj == Current->Value)
				return true;

			Current = Current->Next;
		}

		return false;
	}
	template<typename T>
	unsigned int Vector<T>::IndexOf(const T& Obj) const
	{
		for (unsigned int i = 0; i < _Size; i++)
		{
			if (Obj == operator[](i))
				return i;
		}

		return (unsigned int)4294967295;
	}

	template<typename T>
	void Vector<T>::Reverse()
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

		for (unsigned int i = 0; i < _Size; i++)
		{
			seg Temp = NewCurrent;
			NewCurrent = new VectorSeg<T>(Current->Value);
			LastItem = Temp;

			if (LastItem)
				LastItem->Next = NewCurrent;
			NewCurrent->Last = LastItem;

			Current = Current->Last;
		}

		Last = NewCurrent;

		Current = NewCurrent;
		for (unsigned int i = 0; i < _Size - 1; i++)
			Current = Current->Last;

		First = Current;
	}
	template<typename T>
	void Vector<T>::Clear()
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
		for (unsigned int i = 0; i < _Size; i++)
		{
			seg Temp = Current->Next;
			delete Current;
			Current = Temp;
		}

		_Size = 0;
		First = nullptr;
		Last = nullptr;
	}
	template<typename T>
	bool Vector<T>::IsEmpty() const
	{
		return _Size == 0;
	}

	template<typename T>
	Vector<T> Vector<T>::SubVector(unsigned int BeginIndex) const
	{
		if (_Size == 0)
			return Vector<T>();

		return SubVector(BeginIndex, _Size - 1);
	}
	template<typename T>
	Vector<T> Vector<T>::SubVector(unsigned int BeginIndex, unsigned int EndIndex) const
	{
		if (_Size == 0 || BeginIndex > EndIndex)
			return Vector<T>();

		if (BeginIndex >= _Size || EndIndex >= _Size)
			return Vector<T>(*this);

		unsigned int Size = (EndIndex - BeginIndex) + 1;

		seg Begin = GetAtIndex(BeginIndex);

		seg NewFirst = new VectorSeg<T>();
		NewFirst->Next = NewFirst->Last = 0;
		NewFirst->Value = Begin->Value;
		seg Last = NewFirst;
		seg Current = 0;
		for (unsigned int i = 1, j = BeginIndex + 1; i < Size; i++, j++)
		{
			Current = new VectorSeg<T>();
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

	template<typename T>
	ListIterator<T, VectorSeg<T>> Vector<T>::begin()
	{
		return !First ? end() : ListIterator<T, VectorSeg<T>>(First);
	}
	template<typename T>
	ListIterator<T, VectorSeg<T>> Vector<T>::end()
	{
		return ListIterator<T, VectorSeg<T>>(!Last ? nullptr : Last->Next);
	}
	template<typename T>
	const ListIterator<T, VectorSeg<T>> Vector<T>::begin() const 
	{ 
		return !First ? end() : ListIterator<T, VectorSeg<T>>(First);
	}
	template<typename T>
	const ListIterator<T, VectorSeg<T>> Vector<T>::end() const
	{ 
		return ListIterator<T, VectorSeg<T>>(!Last ? nullptr : Last->Next);
	}

	template<typename T>
	Vector<T> operator+(const Vector<T>& One, const T& Two)
	{
		Vector<T> Return = One;
		Return.Add(Two);

		return Return;
	}
	template<typename T>
	Vector<T> operator+(const Vector<T>& One, const Vector<T>& Two)
	{
		Vector<T> Return = One;
		const SequenceBase<T>* TwoPtr = static_cast<const SequenceBase<T>*>(&Two);
		Return.AddList(TwoPtr);

		return Return;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& Other)
	{
		if (this->_Size != 0)
			Clear();

		if (Other._Size > 0)
		{
			for (unsigned int i = 0; i < Other._Size; i++)
				Add(Other[i]);
		}

		return *this;
	}
	template<typename T>
	Vector<T>& Vector<T>::operator=(const T& Obj)
	{
		Clear();
		AddNewSegment(Obj);

		return *this;
	}

	template<typename T>
	bool Vector<T>::operator==(const Vector<T>& Other) const
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
	template<typename T>
	bool Vector<T>::operator!=(const Vector<T>& Other) const
	{
		Vector<T> One = *this;
		Vector<T> Two = Other;

		return !(One == Two);
	}

	template<typename T>
	Vector<T>::operator std::vector<T>() const
	{
		std::vector<T> Return;
		for (unsigned int i = 0; i < _Size; i++)
			Return.push_back(operator[](i));

		return Return;
	}
	template<typename T>
	void Vector<T>::FromStdVector(const std::vector<T>& Other)
	{
		this->Clear();

		for (T Item : Other)
			this->Add(Item);

		return *this;
	}
}

#endif