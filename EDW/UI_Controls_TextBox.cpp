// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////
UI::Controls::TextBox::TextBox(const Window &window, DWORD style, DWORD exStyle) :
	Control(window, L"EDIT", L"", style, exStyle),
	hovering(false),
	updating(false)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::SetFont(const Font &font)
{
	if (!Control::SetFont(font))
		return false;

	SendMessageW(handle, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 0);
	return true;
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::TextBox::GetTextColors() const
{
	return theme.TextBoxTextColors;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::SetTextColors(const TextColors &colors)
{
	theme.TextBoxTextColors = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::TextBox::GetMaxLength() const
{
	return static_cast<int>(SendMessageW(handle, EM_GETLIMITTEXT, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::SetMaxLength(int length)
{
	SendMessageW(handle, EM_SETLIMITTEXT, length, 0);
	return true;
}

////////////////////////////////////////////////////////////////////////////

std::wstring UI::Controls::TextBox::GetText() const
{
	LRESULT size = SendMessageW(handle, WM_GETTEXTLENGTH, 0, 0);

	if (size <= 0)
		return L"";

	++size;
	std::vector<wchar_t> buffer(size);

	if (SendMessageW(handle, WM_GETTEXT, size, reinterpret_cast<LPARAM>(buffer.data())) <= 0)
		return L"";

	return buffer.data();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::SetText(const std::wstring &text)
{
	updating = true;
	SendMessageW(handle, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text.c_str()));
	updating = false;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::GetInteger(int &value) const
{
	std::wstringstream stream(GetText());
	stream >> value;

	if (!stream)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::SetInteger(int value)
{
	return SetText(std::to_wstring(value));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::Clear()
{
	return SetText(L"");
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::Select(int begin, int end)
{
	SendMessageW(handle, EM_SETSEL, begin, end);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::ProcessCommand(int command)
{
	switch (command)
	{
		case EN_CHANGE:
		case EN_UPDATE:
			if (updating)
				return false;

			break;
	}

	return Control::ProcessCommand(command);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_SETFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_KILLFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	if (!IsEnabled())
	{
		PaintDC dc(*this);
		RECT rect;
		GetClientRect(handle, &rect);
		DrawFillRect(dc, rect, theme.TextBoxTextColors.DisabledTextColor, theme.TextBoxTextColors.DisabledBackgroundColor);
		std::wstring text = GetText();
		dc.SelectObject(font.GetHandle());
		dc.DrawText(text, rect, DT_NOPREFIX);
		return 0;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_PAINT, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnNcCalcSize(WPARAM wParam, LPARAM lParam)
{
	CallWindowProcW(OldWindowProc, handle, WM_NCCALCSIZE, wParam, lParam);

	if (wParam)
	{
		NCCALCSIZE_PARAMS *size = reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
		int y = style & ES_MULTILINE ? theme.Padding : (size->rgrc[0].bottom - size->rgrc[0].top - font.GetHeight()) / 2;
		InflateRect(&size->rgrc[0], -theme.Padding, -y);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnNcPaint(WPARAM wParam, LPARAM lParam)
{
	WindowDC dc(*this);
	RECT rect;
	GetWindowRect(handle, &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;
	RECT clientRect = rect;
	int y = (rect.bottom - rect.top - font.GetHeight()) / 2;
	InflateRect(&clientRect, -theme.Padding, -y);

	if (!IsEnabled())
		DrawFrameFrameRect(dc, rect, clientRect, theme.TextBoxTextColors.DisabledTextColor, theme.TextBoxTextColors.DisabledBackgroundColor, theme.TextBoxBorderColors.DisabledBorderColor, theme.TextBoxBorderSizes.DisabledBorderSize);
	else if (hovering && IsFocused())
		DrawFrameFrameRect(dc, rect, clientRect, theme.TextBoxTextColors.TextColor, theme.TextBoxTextColors.BackgroundColor, theme.TextBoxBorderColors.HoverFocusBorderColor, theme.TextBoxBorderSizes.HoverFocusBorderSize);
	else if (hovering)
		DrawFrameFrameRect(dc, rect, clientRect, theme.TextBoxTextColors.TextColor, theme.TextBoxTextColors.BackgroundColor, theme.TextBoxBorderColors.HoverBorderColor, theme.TextBoxBorderSizes.HoverBorderSize);
	else if (IsFocused())
		DrawFrameFrameRect(dc, rect, clientRect, theme.TextBoxTextColors.TextColor, theme.TextBoxTextColors.BackgroundColor, theme.TextBoxBorderColors.FocusBorderColor, theme.TextBoxBorderSizes.FocusBorderSize);
	else
		DrawFrameFrameRect(dc, rect, clientRect, theme.TextBoxTextColors.TextColor, theme.TextBoxTextColors.BackgroundColor, theme.TextBoxBorderColors.BorderColor, theme.TextBoxBorderSizes.BorderSize);

	Invalidate();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnGetDlgCode(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_TAB:
			return 0;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_GETDLGCODE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnChar(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case 1:
			Select();
			return 0;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_CHAR, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.TextBoxTextColors.DisabledTextColor);
		dc.SetBackgroundColor(theme.TextBoxTextColors.DisabledBackgroundColor);
		dc.SetBrushColor(theme.TextBoxTextColors.DisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.TextBoxTextColors.TextColor);
		dc.SetBackgroundColor(theme.TextBoxTextColors.BackgroundColor);
		dc.SetBrushColor(theme.TextBoxTextColors.BackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::TextBox::OnMouseMove(WPARAM wParam, LPARAM lParam)
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

LRESULT UI::Controls::TextBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	hovering = false;
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_MOUSELEAVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::TextBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SETFOCUS, std::bind(&TextBox::OnSetFocus, this, _1, _2));
	AddMessage(WM_KILLFOCUS, std::bind(&TextBox::OnKillFocus, this, _1, _2));
	AddMessage(WM_PAINT, std::bind(&TextBox::OnPaint, this, _1, _2));
	AddMessage(WM_NCCALCSIZE, std::bind(&TextBox::OnNcCalcSize, this, _1, _2));
	AddMessage(WM_NCPAINT, std::bind(&TextBox::OnNcPaint, this, _1, _2));
	AddMessage(WM_GETDLGCODE, std::bind(&TextBox::OnGetDlgCode, this, _1, _2));
	AddMessage(WM_CHAR, std::bind(&TextBox::OnChar, this, _1, _2));
	AddMessage(WM_CTLCOLOREDIT, std::bind(&TextBox::OnCtlColor, this, _1, _2));
	AddMessage(WM_CTLCOLORSTATIC, std::bind(&TextBox::OnCtlColor, this, _1, _2));
	AddMessage(WM_MOUSEMOVE, std::bind(&TextBox::OnMouseMove, this, _1, _2));
	AddMessage(WM_MOUSELEAVE, std::bind(&TextBox::OnMouseLeave, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
