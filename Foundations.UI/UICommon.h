#pragma once

#include "Controls\Control.h"
#include "Common.h"
#include "Color.h"

#include "Str.h"

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

namespace Core::UI
{
	static Color Background1;
	static Color Background2;

	static Color Accent1;
	static Color Accent2;
	static Color Accent3;
	static Color Accent4;

	static Color Gray1;
	static Color Gray2;
	static Color Gray3;
	static Color FontColor;
	static Color EditorGray;

	static HICON ProgramIcon;
	static HCURSOR ProgramCursor;
	static String StandardFontName;
	static String TitleFontName;


}