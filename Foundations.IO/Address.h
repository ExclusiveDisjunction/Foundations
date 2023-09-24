#pragma once

namespace Core::IO
{
	struct Address
	{
		unsigned long long FileID;
		unsigned long long SectionID;
		unsigned long long ComponentID;
	};
}