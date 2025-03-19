// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::GroupBox::GroupBox(const Window &window, const std::wstring &text, DWORD style, DWORD exStyle) :
	Control(window, L"BUTTON", text, style | BS_GROUPBOX, exStyle)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetPosition(int x, int y)
{
	if (!SetWindowPos(handle, nullptr, x, y - font.GetHeight() / 2, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE))
		return false;

	this->x = x;
	this->y = y;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetSize(int width, int height)
{
	if (!SetWindowPos(handle, nullptr, 0, 0, width, height + font.GetHeight() / 2, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE))
		return false;

	this->width = width;
	this->height = height;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetFont(const Font &font)
{
	if (!Control::SetFont(font))
		return false;

	if (!SetPosition(x, y))
		return false;

	if (!SetSize(width, height))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::GroupBox::GetTextColors() const
{
	return theme.GroupBoxTextColors;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetTextColors(const TextColors &colors)
{
	theme.GroupBoxTextColors = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::GroupBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(*this);
	RECT rect;
	GetClientRect(handle, &rect);
	rect.top += font.GetHeight() / 2;

	if (!IsEnabled())
		DrawFrameRect(dc, rect, theme.GroupBoxTextColors.DisabledTextColor, theme.GroupBoxTextColors.DisabledBackgroundColor, theme.GroupBoxBorderColors.DisabledBorderColor, theme.GroupBoxBorderSizes.DisabledBorderSize);
	else
		DrawFrameRect(dc, rect, theme.GroupBoxTextColors.TextColor, theme.GroupBoxTextColors.BackgroundColor, theme.GroupBoxBorderColors.BorderColor, theme.GroupBoxBorderSizes.BorderSize);

	GetClientRect(handle, &rect);
	rect.left += font.GetHeight() / 2;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::GroupBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.GroupBoxTextColors.DisabledTextColor);
		dc.SetBackgroundColor(theme.GroupBoxTextColors.DisabledBackgroundColor);
		dc.SetBrushColor(theme.GroupBoxTextColors.DisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.GroupBoxTextColors.TextColor);
		dc.SetBackgroundColor(theme.GroupBoxTextColors.BackgroundColor);
		dc.SetBrushColor(theme.GroupBoxTextColors.BackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_PAINT, std::bind(&GroupBox::OnPaint, this, _1, _2));
	AddMessage(WM_CTLCOLORSTATIC, std::bind(&GroupBox::OnCtlColor, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
