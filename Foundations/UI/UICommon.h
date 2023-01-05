#pragma once

#include "Controls\Control.h"
#include "..\Common.h"
#include "..\Color.h"
#include "..\Container.h"
#include "..\Str.h"

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

namespace Foundations::UI
{
	static AaColor Background1;
	static AaColor Background2;

	static AaColor Accent1;
	static AaColor Accent2;
	static AaColor Accent3;
	static AaColor Accent4;

	static AaColor Gray1;
	static AaColor Gray2;
	static AaColor Gray3;
	static AaColor FontColor;
	static AaColor EditorGray;

	static HICON ProgramIcon;
	static HCURSOR ProgramCursor;
	static String StandardFontName;
	static String TitleFontName;


}