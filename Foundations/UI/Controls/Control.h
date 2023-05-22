#pragma once

#include <Windows.h>
#include "..\UICommon.h"
#include "..\..\Color.h"
#include "..\..\Container.h"
#include "..\..\Str.h"

#if (!defined(_WIN32) and !defined(_WIN64)) or !defined(__cplusplus)
#error Win32 or Win64 is required, and the code must be compled in C++
#endif

namespace Core::UI::Controls
{
    String CORE_API GetText(HWND Window);
    void CORE_API SetText(HWND Window, String New);

    enum TextAlignment
    {
        TA_CenterAlignment,
        TA_LeftAlignment,
        TA_RightAlignment
    };

    struct CORE_API StyleSheet
    {
        StyleSheet()
        {
            Background = 0xFF000000;
            BorderBrush = 0xFF000000;
            BorderThickness = 0;
            Radius = 0;
        }

        AaColor Background;
        AaColor BorderBrush;
        AaColor BaseBackground;
        int BorderThickness;
        int Radius;
    };
    struct CORE_API TextStyleSheet
    {
        AaColor Foreground = 0xFFFFFFFF;
        String FontFamily;
        int FontSize = 12, CharSpacing = 0;
        bool Bold = false;
        bool Italic = false;
        bool Underline = false;

        TextAlignment Alignment = TextAlignment::TA_LeftAlignment;
    };

    class CORE_API Control
    {
    protected:
        HWND _Base = nullptr;
        bool IsEnabled = true;

        static LRESULT __stdcall CommonWndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

        virtual bool FocusOnClick() const { return true; }
    public:
        Control();
        virtual ~Control();

        virtual LRESULT KeyDown(WPARAM key);
        virtual LRESULT Create() { return 0; }
        virtual LRESULT Paint() { return 0; }
        virtual LRESULT OnDestroy() { return 0; }
        virtual LRESULT Click() { return 0; }
        virtual LRESULT DoubleClick() { return SendMessageW(GetParent(_Base), WM_LBUTTONDBLCLK, 0, 0); }
        virtual LRESULT RDoubleClick() { return SendMessageW(GetParent(_Base), WM_RBUTTONDBLCLK, 0, 0); }
        virtual LRESULT MouseUp() { return 0; }
        virtual LRESULT TimerTick(LONG ID) { return 0; }
        virtual LRESULT Char(WPARAM ID) { return 0; }
        virtual LRESULT Command(WPARAM ID, LPARAM lp) { return SendMessageW(GetParent(_Base), WM_COMMAND, ID, lp); }
        virtual LRESULT Size() { return 0; }
        virtual LRESULT GotFocus() { return 0; }
        virtual LRESULT LostFocus() { return 0; }
        virtual LRESULT RightClick() { return SendMessageW(GetParent(_Base), WM_RBUTTONDOWN, 0,0); }

        bool Enabled();
        void Enabled(bool New);

        virtual void Redraw();
        virtual void Move(int X, int Y, int Width, int Height);

        void Placement(RECT& WndRect) const;
        int XCoord() const;
        int YCoord() const;

        operator HWND()
        {
            return _Base;
        }
    };

    class CORE_API StyledControl : public Control
    {
    protected:
        StyleSheet _Style;
        TextStyleSheet _TextStyle;
    public:
        StyleSheet const& Style = _Style;
        TextStyleSheet const& TextStyle = _TextStyle;

        void Background(const AaColor& New);
        void BorderBrush(const AaColor& New);
        void BorderThickness(int New);
        void SetStyle(const StyleSheet& New);
        void SetTextStyle(const TextStyleSheet& New);
    };

    void CORE_API GetClientRect(Control* Source, LPRECT Dest);
    void CORE_API GetWindowRect(Control* Source, LPRECT Dest);
}