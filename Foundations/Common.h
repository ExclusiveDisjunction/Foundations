#pragma once

#ifdef CORE_EXPORTS
#define CoreMember _declspec(dllexport)
#else
#define CoreMember _declspec(dllimport)
#endif