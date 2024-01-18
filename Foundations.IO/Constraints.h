#pragma once

namespace Core::IO
{
	template<typename T>
	concept ElementSimilar = requires(T a)
	{
		a.Next&& a.Last&& a.Parent;
	};
}