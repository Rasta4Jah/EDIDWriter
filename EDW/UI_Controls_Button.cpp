// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::Button::Button(const Window &window, const std::wstring &text, DWORD style, DWORD exStyle) :
	Control(window, L"BUTTON", text, style | BS_OWNERDRAW, exStyle),
	hovering(false),
	outlined(false)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Button::IsDefault() const
{
	return outlined;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Button::SetDefault(bool value)
{
	if (!IsEnabled())
		return false;

	outlined = value;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Button::Disable()
{
	SetDefault(false);
	return Control::Disable();
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT *item = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);
	DC dc(item->hDC);
	RECT rect = item->rcItem;

	if (!IsEnabled())
		rect = DrawFrameFillRect(dc, rect, theme.ButtonTextColors.DisabledTextColor, theme.ButtonTextColors.DisabledBackgroundColor, theme.ButtonBorderColors.DisabledBorderColor, theme.ButtonBorderSizes.DisabledBorderSize);
	else if (item->itemState & ODS_SELECTED)
		rect = DrawFrameFillRect(dc, rect, theme.ButtonTextColors.PushedTextColor, theme.ButtonTextColors.PushedBackgroundColor, theme.ButtonBorderColors.PushedBorderColor, theme.ButtonBorderSizes.PushedBorderSize);
	else if (hovering && (IsFocused() || outlined))
		rect = DrawFrameFillRect(dc, rect, theme.ButtonTextColors.HoverFocusTextColor, theme.ButtonTextColors.HoverFocusBackgroundColor, theme.ButtonBorderColors.HoverFocusBorderColor, theme.ButtonBorderSizes.HoverFocusBorderSize);
	else if (hovering)
		rect = DrawFrameFillRect(dc, rect, theme.ButtonTextColors.HoverTextColor, theme.ButtonTextColors.HoverBackgroundColor, theme.ButtonBorderColors.HoverBorderColor, theme.ButtonBorderSizes.HoverBorderSize);
	else if (IsFocused() || outlined)
		rect = DrawFrameFillRect(dc, rect, theme.ButtonTextColors.FocusTextColor, theme.ButtonTextColors.FocusBackgroundColor, theme.ButtonBorderColors.FocusBorderColor, theme.ButtonBorderSizes.FocusBorderSize);
	else
		rect = DrawFrameFillRect(dc, rect, theme.ButtonTextColors.TextColor, theme.ButtonTextColors.BackgroundColor, theme.ButtonBorderColors.BorderColor, theme.ButtonBorderSizes.BorderSize);

	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	if (theme.ButtonFocusRect && IsFocused() && IsFocusVisible())
	{
		rect = item->rcItem;
		InflateRect(&rect, -theme.ButtonFocusRectPadding, -theme.ButtonFocusRectPadding);
		dc.DrawFocusRect(rect);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnGetDlgCode(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_RETURN:
			return DLGC_WANTMESSAGE;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_GETDLGCODE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_RETURN:
			ProcessCommand(BN_CLICKED);
			return 0;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_KEYDOWN, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);
	dc.SetBrushColor(theme.WindowBackgroundColor);
	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if (!hovering)
	{
		TRACKMOUSEEVENT event = {sizeof event};
		event.dwFlags = TME_LEAVE;
		event.hwndTrack = handle;
		TrackMouseEvent(&event);

		hovering = true;
		Invalidate();
	}

	return CallWindowProcW(OldWindowProc, handle, WM_MOUSEMOVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnLButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	SendMessageW(handle, WM_LBUTTONDOWN, wParam, lParam);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Button::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	hovering = false;
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_MOUSELEAVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Button::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_DRAWITEM, std::bind(&Button::OnDrawItem, this, _1, _2));
	AddMessage(WM_GETDLGCODE, std::bind(&Button::OnGetDlgCode, this, _1, _2));
	AddMessage(WM_KEYDOWN, std::bind(&Button::OnKeyDown, this, _1, _2));
	AddMessage(WM_CTLCOLORBTN, std::bind(&Button::OnCtlColor, this, _1, _2));
	AddMessage(WM_MOUSEMOVE, std::bind(&Button::OnMouseMove, this, _1, _2));
	AddMessage(WM_LBUTTONDBLCLK, std::bind(&Button::OnLButtonDblClk, this, _1, _2));
	AddMessage(WM_MOUSELEAVE, std::bind(&Button::OnMouseLeave, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
