#include "Position.h"

namespace Core::UI
{
	Position::Position() : Position(0, 0, 0, 0)
	{

	}
	Position::Position(LONG x, LONG y, LONG width, LONG height) : X(x), Y(y), Width(width), Height(height)
	{

	}
}