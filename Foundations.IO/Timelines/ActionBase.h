#pragma once

#include "..\IOCommon.h"
#include "ActionID.h"

namespace Core::IO::Timelines
{
	class CORE_API Timeline;

	class CORE_API ActionBase
	{
	private:
		Timeline* _Owner = nullptr;
		ActionBase* _Next = nullptr, * _Previous = nullptr, * _Top = nullptr;

		void DeleteChain();

		ActionBase(Timeline* Owner);
		ActionBase(Timeline* Owner, ActionBase* Next, ActionBase* Previous);
	protected:

	public:
		~ActionBase();
		ActionBase(const ActionBase& Obj) noexcept = delete;
		ActionBase(ActionBase&& Obj) noexcept = delete;

		friend Timeline;

		ActionBase& operator=(const ActionBase& Obj) noexcept = delete;
		ActionBase& operator=(ActionBase&& Obj) noexcept = delete;

		Timeline* const& Owner = _Owner;
		ActionBase* const& Next = _Next;
		ActionBase* const& Previous = _Previous;
		ActionBase* const Top = _Top;

		ActionID ID;

		virtual bool ApplyChanges() = 0;
	};
}