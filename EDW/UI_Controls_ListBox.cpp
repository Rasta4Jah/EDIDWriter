// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::ListBox::ListBox(const Window &window, DWORD style, DWORD exStyle) :
	Control(window, L"LISTBOX", L"", style | LBS_OWNERDRAWFIXED, exStyle),
	hovering(false)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetFont(const Font &font)
{
	if (SendMessageW(handle, LB_SETITEMHEIGHT, 0, font.GetHeight()) == LB_ERR)
		return false;

	return Control::SetFont(font);
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::ListBox::GetTextColors() const
{
	return theme.ListBoxTextColors;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetTextColors(const TextColors &colors)
{
	theme.ListBoxTextColors = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::ListBox::GetTextColors(int index) const
{
	if (!IsValidIndex(index))
		return theme.ListBoxTextColors;

	return textColors[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetTextColors(int index, const TextColors &colors)
{
	if (!IsValidIndex(index))
		return false;

	textColors[index] = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ListBox::GetCount() const
{
	return static_cast<int>(text.size());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::IsValidIndex(int index) const
{
	return index >= 0 && index < GetCount();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Add(const std::wstring &value)
{
	SendMessageW(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(value.c_str()));
	text.push_back(value);
	textColors.push_back(theme.ListBoxTextColors);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Insert(int index, const std::wstring &value)
{
	if (index == GetCount())
		return Add(value);

	if (!IsValidIndex(index))
		return false;

	SendMessageW(handle, LB_INSERTSTRING, index, reinterpret_cast<LPARAM>(value.c_str()));
	text.insert(text.begin() + index, value);
	textColors.insert(textColors.begin() + index, theme.ListBoxTextColors);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::GetText(int index, std::wstring &value) const
{
	if (!IsValidIndex(index))
		return false;

	value = text[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Delete(int index)
{
	if (!IsValidIndex(index))
		return false;

	SendMessageW(handle, LB_DELETESTRING, index, 0);
	text.erase(text.begin() + index);
	textColors.erase(textColors.begin() + index);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Clear()
{
	if (GetCount() <= 0)
		return false;

	SendMessageW(handle, LB_RESETCONTENT, 0, 0);
	text.clear();
	textColors.clear();
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ListBox::GetIndex() const
{
	return static_cast<int>(SendMessageW(handle, LB_GETCURSEL, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetIndex(int index)
{
	return SendMessageW(handle, LB_SETCURSEL, index, 0) != LB_ERR;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Deselect()
{
	SetIndex(-1);
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ListBox::GetTopIndex() const
{
	return static_cast<int>(SendMessageW(handle, LB_GETTOPINDEX, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetTopIndex(int index)
{
	return SendMessageW(handle, LB_SETTOPINDEX, index, 0) != LB_ERR;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_SETFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_KILLFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	if (GetCount() <= 0)
	{
		PaintDC dc(*this);
		RECT rect;
		GetClientRect(handle, &rect);

		if (!IsEnabled())
			DrawFillRect(dc, rect, theme.ListBoxGrayTextColors.DisabledTextColor, theme.ListBoxGrayTextColors.DisabledBackgroundColor);
		else
			DrawFillRect(dc, rect, theme.ListBoxGrayTextColors.TextColor, theme.ListBoxGrayTextColors.BackgroundColor);

		rect.left = theme.Padding - theme.ListBoxPadding;
		dc.SelectObject(font.GetHandle());
		dc.DrawText(Control::text, rect, DT_NOPREFIX);

		if (theme.ListBoxFocusRect && IsFocused() && IsFocusVisible())
		{
			dc.SetTextColor(theme.ListBoxTextColors.TextColor);
			dc.SetBackgroundColor(theme.ListBoxTextColors.BackgroundColor);
			GetClientRect(handle, &rect);
			rect.bottom = font.GetHeight();
			InflateRect(&rect, -theme.ListBoxFocusRectPadding, -theme.ListBoxFocusRectPadding);
			dc.DrawFocusRect(rect);
		}

		return 0;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_PAINT, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	if (GetCount() <= 0)
		return true;

	DRAWITEMSTRUCT *item = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);
	DC dc(item->hDC);
	RECT rect = item->rcItem;
	TextColors textColors = GetTextColors(item->itemID);

	if (!IsEnabled())
		DrawFillRect(dc, rect, textColors.DisabledTextColor, textColors.DisabledBackgroundColor);
	else if (item->itemState & ODS_SELECTED)
		DrawFillRect(dc, rect, textColors.SelectedTextColor, textColors.SelectedBackgroundColor);
	else
		DrawFillRect(dc, rect, textColors.TextColor, textColors.BackgroundColor);

	std::wstring text;
	GetText(item->itemID, text);
	rect.left = theme.Padding - theme.ListBoxPadding;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);

	if (theme.ListBoxFocusRect && (item->itemState & ODS_FOCUS) && !(item->itemState & ODS_NOFOCUSRECT))
	{
		rect = item->rcItem;
		InflateRect(&rect, -theme.ListBoxFocusRectPadding, -theme.ListBoxFocusRectPadding);
		dc.DrawFocusRect(rect);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnNcCalcSize(WPARAM wParam, LPARAM lParam)
{
	CallWindowProcW(OldWindowProc, handle, WM_NCCALCSIZE, wParam, lParam);

	if (wParam)
	{
		NCCALCSIZE_PARAMS *size = reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
		InflateRect(&size->rgrc[0], -theme.ListBoxPadding, -theme.ListBoxPadding);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnNcPaint(WPARAM wParam, LPARAM lParam)
{
	WindowDC dc(*this);
	RECT rect;
	GetWindowRect(handle, &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;
	RECT clientRect = rect;
	InflateRect(&clientRect, -theme.ListBoxPadding, -theme.ListBoxPadding);
	CallWindowProcW(OldWindowProc, handle, WM_NCPAINT, wParam, lParam);

	if (!IsEnabled())
		DrawFrameFrameRect(dc, rect, clientRect, theme.ListBoxTextColors.DisabledTextColor, theme.ListBoxTextColors.DisabledBackgroundColor, theme.ListBoxBorderColors.DisabledBorderColor, theme.ListBoxBorderSizes.DisabledBorderSize);
	else if (hovering && IsFocused())
		DrawFrameFrameRect(dc, rect, clientRect, theme.ListBoxTextColors.TextColor, theme.ListBoxTextColors.BackgroundColor, theme.ListBoxBorderColors.HoverFocusBorderColor, theme.ListBoxBorderSizes.HoverFocusBorderSize);
	else if (hovering)
		DrawFrameFrameRect(dc, rect, clientRect, theme.ListBoxTextColors.TextColor, theme.ListBoxTextColors.BackgroundColor, theme.ListBoxBorderColors.HoverBorderColor, theme.ListBoxBorderSizes.HoverBorderSize);
	else if (IsFocused())
		DrawFrameFrameRect(dc, rect, clientRect, theme.ListBoxTextColors.TextColor, theme.ListBoxTextColors.BackgroundColor, theme.ListBoxBorderColors.FocusBorderColor, theme.ListBoxBorderSizes.FocusBorderSize);
	else
		DrawFrameFrameRect(dc, rect, clientRect, theme.ListBoxTextColors.TextColor, theme.ListBoxTextColors.BackgroundColor, theme.ListBoxBorderColors.BorderColor, theme.ListBoxBorderSizes.BorderSize);

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.ListBoxTextColors.DisabledTextColor);
		dc.SetBackgroundColor(theme.ListBoxTextColors.DisabledBackgroundColor);
		dc.SetBrushColor(theme.ListBoxTextColors.DisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.ListBoxTextColors.TextColor);
		dc.SetBackgroundColor(theme.ListBoxTextColors.BackgroundColor);
		dc.SetBrushColor(theme.ListBoxTextColors.BackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnMouseMove(WPARAM wParam, LPARAM lParam)
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

LRESULT UI::Controls::ListBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	hovering = false;
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_MOUSELEAVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SETFOCUS, std::bind(&ListBox::OnSetFocus, this, _1, _2));
	AddMessage(WM_KILLFOCUS, std::bind(&ListBox::OnKillFocus, this, _1, _2));
	AddMessage(WM_PAINT, std::bind(&ListBox::OnPaint, this, _1, _2));
	AddMessage(WM_DRAWITEM, std::bind(&ListBox::OnDrawItem, this, _1, _2));
	AddMessage(WM_NCCALCSIZE, std::bind(&ListBox::OnNcCalcSize, this, _1, _2));
	AddMessage(WM_NCPAINT, std::bind(&ListBox::OnNcPaint, this, _1, _2));
	AddMessage(WM_CTLCOLORLISTBOX, std::bind(&ListBox::OnCtlColor, this, _1, _2));
	AddMessage(WM_MOUSEMOVE, std::bind(&ListBox::OnMouseMove, this, _1, _2));
	AddMessage(WM_MOUSELEAVE, std::bind(&ListBox::OnMouseLeave, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
