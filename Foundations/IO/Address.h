#pragma once

namespace Foundations::IO
{
	struct Address
	{
		unsigned long long FileID;
		unsigned long long SectionID;
		unsigned long long ComponentID;
	};
}