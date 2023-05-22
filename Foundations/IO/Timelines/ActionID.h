#pragma once

#include "..\IOCommon.h"

namespace Core::IO::Timelines
{
	class CORE_API Timeline;

	struct ActionID
	{
	private:
		unsigned long long _PositionID = 0;
		unsigned long long _BranchID = 0;

	public:
		ActionID(unsigned long long PositionID = 0, unsigned long long BranchID = 0) : _PositionID(PositionID), _BranchID(BranchID) {}

		friend Timeline;

		unsigned long long PositionID() const { return _PositionID; }
		unsigned long long BranchID() const { return _BranchID; }
	};
}