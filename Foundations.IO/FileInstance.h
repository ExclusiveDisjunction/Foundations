#pragma once

#include "IOCommon.h"
#include "Element.h"

namespace Core::UI
{
	class IO_API FileInstance : 
	{
	private:
		std::fstream* fStream = nullptr;

		std::string Name;

	}
}