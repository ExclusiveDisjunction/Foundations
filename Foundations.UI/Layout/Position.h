#pragma once

#include "..\UICommon.h"

namespace Core::UI
{
	/// <summary>
	/// Represents an absolute position, width, and height for a control.
	/// </summary>
	class UI_API Position
	{
	public:
		Position();
		Position(LONG x, LONG y, LONG width, LONG height);

		LONG X = 0l;
		LONG Y = 0l;
		LONG Width = 0l;
		LONG Height = 0l;
	};
}