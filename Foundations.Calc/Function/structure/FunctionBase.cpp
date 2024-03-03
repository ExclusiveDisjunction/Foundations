#include "FunctionBase.h"

#include "FunctionIterator.h"

namespace Core::Calc::Function
{
	FunctionBase::FunctionBase(unsigned int Input, unsigned int Output) : _Input(Input), _Output(Output)
	{
		if (Input == 0 || Output == 0)
			throw std::exception("The input or output cannot be zero.");
	}
	FunctionBase::~FunctionBase()
	{
		ClearChildren();
	}

	void FunctionBase::AssignParent(FunctionBase* Obj)
	{
		if (Obj == nullptr)
			return;

		if (Obj->Parent != nullptr)
			Obj->Parent->RemoveChild(Obj, false);

		Obj->Parent = this;
		PushChild(Obj);
	}
	bool FunctionBase::PushChild(FunctionBase* Obj)
	{
		if (!Obj)
			return false;

		if (!LastCh)
		{
			FirstCh = LastCh = Obj;
			Obj->Next = nullptr;
			Obj->Prev = nullptr;
		}
		else
		{
			Obj->Prev = LastCh;
			LastCh = LastCh->Next = Obj;
		}

		CHCount++;	
		return true;
	}
	bool FunctionBase::PopChild(FunctionBase* Obj, bool Delete)
	{
		if (!Obj || Obj->Parent != this)
			return false;

		if (FirstCh == Obj && LastCh == Obj)
			FirstCh = LastCh = nullptr;
		else if (FirstCh == Obj)
			FirstCh = FirstCh->Next;
		else if (LastCh == Obj)
			LastCh = LastCh->Prev;
		else
		{
			FunctionBase* Next = Obj->Next, * Prev = Obj->Prev;
			if (Next)
				Next->Prev = Prev;
			if (Prev)
				Prev->Next = Next;
		}

		CHCount--;
		if (Delete)
			delete Obj;
		return true;
	}
	void FunctionBase::ClearChildren()
	{
		FunctionBase* curr = FirstCh;
		while (curr)
		{
			FunctionBase* next = curr->Next;
			delete curr;
			curr = next;
		}

		FirstCh = LastCh = nullptr;
		CHCount = 0;
	}
	void FunctionBase::RemoveParent()
	{
		if (!Parent)
			return;

		Parent->PopChild(this, false);
	}
	bool FunctionBase::AddChild(FunctionBase* Obj)
	{
		if (!Obj)
			return false;

		return PushChild(Obj);
	}
	bool FunctionBase::RemoveChild(FunctionBase* Child, bool Delete)
	{
		try
		{
			return PopChild(Child, Delete);
		}
		catch (std::logic_error& e)
		{
			throw e;
		}
	}

	FunctionBase::iterator FunctionBase::ChildAt(unsigned i) noexcept
	{
		if (i >= CHCount)
			return end();

		return begin() + i;
	}
	FunctionBase::const_iterator FunctionBase::ChildAt(unsigned i) const noexcept
	{
		if (i >= CHCount)
			return end();

		return begin() + i;
	}

	FunctionBase::iterator FunctionBase::begin()
	{
		return !FirstCh ? end() : iterator(FirstCh, this);
	}
	FunctionBase::iterator FunctionBase::end()
	{
		return iterator(nullptr, this);
	}
	FunctionBase::const_iterator FunctionBase::begin() const
	{
		return !FirstCh ? end() : const_iterator(FirstCh, this);
	}
	FunctionBase::const_iterator FunctionBase::end() const
	{
		return const_iterator(nullptr, this);
	}

	unsigned FunctionBase::InputDim() const
	{
		return _Input;
	}
	unsigned FunctionBase::OutputDim() const
	{
		return _Output;
	}

	bool FunctionBase::ComparesTo(FunctionBase* const& Obj) const
	{
		if (!Obj)
			return false;

		bool Return = Obj->_Input == _Input && Obj->_Output == _Output && Obj->AllowedChildCount() == AllowedChildCount();
		if (Return && AllowedChildCount() != 0)
		{
			FunctionBase* currT = FirstCh,
				* currO = Obj->FirstCh;
			for (; currT && currO; currT = currT->Next, currO = currO->Next)
			{
				Return &= currT->ComparesTo(currO);
				if (!Return)
					return false;
			
			}
		}

		return true;
	}
	bool FunctionBase::EquatesTo(FunctionBase* const& Obj) const
	{
		if (!Obj)
			return false;

		bool Return = Obj->_Input == _Input && Obj->_Output == _Output && Obj->AllowedChildCount() == AllowedChildCount();
		if (Return && AllowedChildCount() != 0)
		{
			FunctionBase* currT = FirstCh,
				* currO = Obj->FirstCh;
			for (; currT && currO; currT = currT->Next, currO = currO->Next)
			{
				Return &= currT->EquatesTo(currO);
				if (!Return)
					return false;

			}
		}

		return true;
	}

	FunctionBase& FunctionBase::operator-()
	{
		A = -A; 
		return *this;
	}
}