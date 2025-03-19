// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::ComboBox::ComboBox(const Window &window, DWORD style, DWORD exStyle) :
	Control(window, L"COMBOBOX", L"", style | CBS_OWNERDRAWFIXED, exStyle),
	listHandle(InitListHandle()),
	hovering(false)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetSize(int width, int height, int listHeight)
{
	if (SendMessageW(handle, CB_SETITEMHEIGHT, -1, static_cast<LPARAM>(height) - 6) == CB_ERR)
		return false;

	return Control::SetSize(width, height + listHeight);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetRightBottom(int right, int bottom, int listBottom)
{
	return SetSize(right - x, bottom - y, listBottom - bottom);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetFont(const Font &font)
{
	if (SendMessageW(handle, CB_SETITEMHEIGHT, 0, font.GetHeight()) == CB_ERR)
		return false;

	return Control::SetFont(font);
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::ComboBox::GetTextColors() const
{
	return theme.ComboBoxListTextColors;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetTextColors(const TextColors &colors)
{
	theme.ComboBoxListTextColors = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::ComboBox::GetTextColors(int index) const
{
	if (!IsValidIndex(index))
		return theme.ComboBoxListTextColors;

	return textColors[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetTextColors(int index, const TextColors &colors)
{
	if (!IsValidIndex(index))
		return false;

	textColors[index] = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ComboBox::GetCount() const
{
	return static_cast<int>(text.size());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::IsValidIndex(int index) const
{
	return index >= 0 && index < GetCount();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Add(const std::wstring &value)
{
	SendMessageW(handle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(value.c_str()));
	text.push_back(value);
	textColors.push_back(theme.ComboBoxListTextColors);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Insert(int index, const std::wstring &value)
{
	if (index == GetCount())
		return Add(value);

	if (!IsValidIndex(index))
		return false;

	SendMessageW(handle, CB_INSERTSTRING, index, reinterpret_cast<LPARAM>(value.c_str()));
	text.insert(text.begin() + index, value);
	textColors.insert(textColors.begin() + index, theme.ComboBoxListTextColors);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::GetText(int index, std::wstring &value) const
{
	if (!IsValidIndex(index))
		return false;

	value = text[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Delete(int index)
{
	if (!IsValidIndex(index))
		return false;

	SendMessageW(handle, CB_DELETESTRING, index, 0);
	text.erase(text.begin() + index);
	textColors.erase(textColors.begin() + index);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Clear()
{
	if (GetCount() <= 0)
		return false;

	SendMessageW(handle, CB_RESETCONTENT, 0, 0);
	text.clear();
	textColors.clear();
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ComboBox::GetIndex() const
{
	return static_cast<int>(SendMessageW(handle, CB_GETCURSEL, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetIndex(int index)
{
	return SendMessageW(handle, CB_SETCURSEL, index, 0) != CB_ERR;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Deselect()
{
	SetIndex(-1);
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ComboBox::GetTopIndex() const
{
	return static_cast<int>(SendMessageW(handle, CB_GETTOPINDEX, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetTopIndex(int index)
{
	return SendMessageW(handle, CB_SETTOPINDEX, index, 0) != CB_ERR;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::IsDroppedDown() const
{
	return SendMessageW(handle, CB_GETDROPPEDSTATE, 0, 0);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_SETFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_KILLFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

void UI::Controls::ComboBox::DrawComboBox()
{
	WindowDC dc(*this);
	RECT rect;
	GetClientRect(handle, &rect);

	if (!IsEnabled())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColors.DisabledTextColor, theme.ComboBoxTextColors.DisabledBackgroundColor, theme.ComboBoxBorderColors.DisabledBorderColor, theme.ComboBoxBorderSizes.DisabledBorderSize);
	else if (IsDroppedDown())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColors.PushedTextColor, theme.ComboBoxTextColors.PushedBackgroundColor, theme.ComboBoxBorderColors.PushedBorderColor, theme.ComboBoxBorderSizes.PushedBorderSize);
	else if (hovering && IsFocused())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColors.HoverFocusTextColor, theme.ComboBoxTextColors.HoverFocusBackgroundColor, theme.ComboBoxBorderColors.HoverFocusBorderColor, theme.ComboBoxBorderSizes.HoverFocusBorderSize);
	else if (hovering)
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColors.HoverTextColor, theme.ComboBoxTextColors.HoverBackgroundColor, theme.ComboBoxBorderColors.HoverBorderColor, theme.ComboBoxBorderSizes.HoverBorderSize);
	else if (IsFocused())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColors.FocusTextColor, theme.ComboBoxTextColors.FocusBackgroundColor, theme.ComboBoxBorderColors.FocusBorderColor, theme.ComboBoxBorderSizes.FocusBorderSize);
	else
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColors.TextColor, theme.ComboBoxTextColors.BackgroundColor, theme.ComboBoxBorderColors.BorderColor, theme.ComboBoxBorderSizes.BorderSize);

	RECT buttonRect;
	GetClientRect(handle, &buttonRect);
	buttonRect.left = buttonRect.right - theme.ComboBoxButtonWidth;

	if (theme.IsDark)
		dc.InvertRect(buttonRect);

	HTHEME htheme = OpenThemeData(handle, L"COMBOBOX");

	if (htheme)
	{
		DrawThemeBackground(htheme, dc.GetHandle(), CP_DROPDOWNBUTTONRIGHT, CBXSR_NORMAL, &buttonRect, nullptr);
		CloseThemeData(htheme);
	}
	else
	{
		const int DFCS_MENUARROWDOWN = 16;
		DrawFrameControl(dc.GetHandle(), &buttonRect, DFC_MENU, DFCS_TRANSPARENT | DFCS_MENUARROWDOWN);
	}

	if (theme.IsDark)
		dc.InvertRect(buttonRect);

	std::wstring text;
	GetText(GetIndex(), text);
	rect.left = theme.Padding;
	rect.right = buttonRect.left + 1 - theme.Padding;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);

	if (theme.ComboBoxFocusRect && IsFocused() && IsFocusVisible() && !IsDroppedDown())
	{
		GetClientRect(handle, &rect);
		rect.right = buttonRect.left + 1;
		InflateRect(&rect, -theme.ComboBoxFocusRectPadding, -theme.ComboBoxFocusRectPadding);
		dc.DrawFocusRect(rect);
	}
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(*this);
	DrawComboBox();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT *item = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);

	if (item->itemState & ODS_COMBOBOXEDIT)
	{
		DrawComboBox();
		return true;
	}

	DC dc(item->hDC);
	RECT rect = item->rcItem;
	TextColors textColors = GetTextColors(item->itemID);

	if (item->itemState & ODS_SELECTED)
		DrawFillRect(dc, rect, textColors.SelectedTextColor, textColors.SelectedBackgroundColor);
	else
		DrawFillRect(dc, rect, textColors.TextColor, textColors.BackgroundColor);

	std::wstring text;
	GetText(item->itemID, text);
	rect.left = theme.Padding - 1;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);

	if (theme.ComboBoxListFocusRect && (item->itemState & ODS_FOCUS) && !(item->itemState & ODS_NOFOCUSRECT))
	{
		rect = item->rcItem;
		InflateRect(&rect, -theme.ComboBoxListFocusRectPadding, -theme.ComboBoxListFocusRectPadding);
		dc.DrawFocusRect(rect);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);
	dc.SetTextColor(theme.ComboBoxListTextColors.TextColor);
	dc.SetBackgroundColor(theme.ComboBoxListTextColors.BackgroundColor);
	dc.SetBrushColor(theme.ComboBoxListTextColors.BackgroundColor);
	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnMouseMove(WPARAM wParam, LPARAM lParam)
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

LRESULT UI::Controls::ComboBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	hovering = false;

	if (!IsDroppedDown())
		Invalidate();

	return CallWindowProcW(OldWindowProc, handle, WM_MOUSELEAVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SETFOCUS, std::bind(&ComboBox::OnSetFocus, this, _1, _2));
	AddMessage(WM_KILLFOCUS, std::bind(&ComboBox::OnKillFocus, this, _1, _2));
	AddMessage(WM_PAINT, std::bind(&ComboBox::OnPaint, this, _1, _2));
	AddMessage(WM_DRAWITEM, std::bind(&ComboBox::OnDrawItem, this, _1, _2));
	AddMessage(WM_CTLCOLORLISTBOX, std::bind(&ComboBox::OnCtlColor, this, _1, _2));
	AddMessage(WM_MOUSEMOVE, std::bind(&ComboBox::OnMouseMove, this, _1, _2));
	AddMessage(WM_MOUSELEAVE, std::bind(&ComboBox::OnMouseLeave, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////

HWND UI::Controls::ComboBox::InitListHandle() const
{
	COMBOBOXINFO info = {sizeof info};

	if (!GetComboBoxInfo(handle, &info))
		throw L"GetComboBoxInfo failed.";

	return info.hwndList;
}

////////////////////////////////////////////////////////////////////////////
