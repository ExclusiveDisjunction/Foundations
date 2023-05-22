#pragma once

#include "Control.h"

namespace Core::UI::Controls
{
	class CORE_API ScrollViewer : public Control
	{
	private:
		StyleSheet _Style;
		Control* Viewer;

		int HScrollUnit, VScrollUnit;
		int ScrollWidth, ClientWidth, ClientHeight;
		int XCoord, YCoord;

		SCROLLINFO si;

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
	public:
		ScrollViewer(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins, StyleSheet Style);
		ScrollViewer(int X, int Y, int Width, int Height, Control* Parent, HINSTANCE ins, StyleSheet Style) : ScrollViewer(X, Y, Width, Height, *Parent, ins, Style) {}

		void SetViewer(Control* New)
		{
			Viewer = New;
			Reset();
		}

		LRESULT Paint() override;
		LRESULT HScroll(WPARAM wp);
		LRESULT VScroll(WPARAM wp);
		LRESULT MouseWheel(WPARAM wp);
		LRESULT HMouseWheel(WPARAM wp);
		LRESULT Size() override;

		void Reset();
	};
}