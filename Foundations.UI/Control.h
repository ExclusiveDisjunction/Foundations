#pragma once

#include "UICommon.h"

//Dll Entry Point
bool __stdcall DllMain(HINSTANCE, DWORD, LPVOID);

namespace Core::UI
{
    String CORE_API GetText(HWND Window);
    void CORE_API SetText(HWND Window, String New);

    enum TextAlignment
    {
        TA_CenterAlignment,
        TA_LeftAlignment,
        TA_RightAlignment
    };

    class CORE_API Control
    {
    private:
        static LRESULT __stdcall CommonWndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
        static bool RegisterAtom(HINSTANCE ins);
        static ATOM _ThisAtom;

    protected:
        HWND _Base = nullptr;
        bool IsEnabled = true;

        ATOM GetAtom() const;

        virtual bool FocusOnClick() const { return true; }
        virtual void ProcessExtraMessage(UINT Message, WPARAM wp, LPARAM lp);
    public:
        Control(HINSTANCE ins);
        virtual ~Control();

        // !! IMPORTANT !! The DllMain function is responsible for cleaning starting and closing out the ATOM.
        friend bool __stdcall ::DllMain(HINSTANCE, DWORD, LPVOID);

        //Properties
        D2D1_COLOR_F Background;
        D2D1_COLOR_F 

        //Basic Management
        virtual void OnPaint() { }
        virtual void OnDestroy() { }

        //User Input
        virtual void OnKeyDown(WPARAM key);
        virtual void OnChar(wchar_t Character, int RepeatCount, int ScanCode, bool ExtendedKey, bool PrevStatePressed, bool IsBeingReleased) { }
        virtual void OnClick() {  }
        virtual void OnDoubleClick() { SendMessage(GetParent(_Base), WM_LBUTTONDBLCLK, 0, 0); }
        virtual void OnRDoubleClick() { SendMessage(GetParent(_Base), WM_RBUTTONDBLCLK, 0, 0); }
        virtual void OnMouseUp() { }
        virtual void OnRightClick() { SendMessage(GetParent(_Base), WM_RBUTTONDOWN, 0, 0); }

        //Windows Events
        virtual void OnTimerTick(long TimerID) { }
        virtual void OnSize();
        virtual void OnMove();
        virtual void GotFocus() { }
        virtual void LostFocus() { }
        virtual void OnCommand(WPARAM ID, LPARAM lp) { SendMessage(GetParent(_Base), WM_COMMAND, ID, lp); }

        bool Enabled();
        void Enabled(bool New);

        virtual void Redraw();

        explicit operator HWND() const
        {
            return _Base;
        }
    };
}