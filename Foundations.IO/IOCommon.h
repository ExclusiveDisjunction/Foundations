#pragma once

#include "Container.h"
#include "Common.h"
#include "Str.h"

#ifdef FOUNDATIONSIO_EXPORTS 
#define FND_IO_API __declspec(dllexport)
#elif
#define FND_IO_API __declspec(dllimport)
#endif
