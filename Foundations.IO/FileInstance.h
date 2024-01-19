#pragma once

#include "IOCommon.h"
#include "Element.h"

namespace Core::IO
{	
	class IO_API FileInstance : public Element
	{
	private:
		std::fstream* fStream = nullptr;

		std::string Name;

	};
}