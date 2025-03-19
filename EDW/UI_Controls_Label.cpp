// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::Label::Label(const Window &window, const std::wstring &text, UINT format) :
	Control(window, L"STATIC", text, SS_OWNERDRAW, 0),
	format(format)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

UI::TextColors UI::Controls::Label::GetTextColors() const
{
	return theme.LabelTextColors;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Label::SetTextColors(const TextColors &colors)
{
	theme.LabelTextColors = colors;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Label::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	ClientDC dc(*this);
	RECT rect;
	GetClientRect(handle, &rect);

	if (!IsEnabled())
		DrawFillRect(dc, rect, theme.LabelTextColors.DisabledTextColor, theme.LabelTextColors.DisabledBackgroundColor);
	else
		DrawFillRect(dc, rect, theme.LabelTextColors.TextColor, theme.LabelTextColors.BackgroundColor);

	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, format);
	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Label::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.LabelTextColors.DisabledTextColor);
		dc.SetBackgroundColor(theme.LabelTextColors.DisabledBackgroundColor);
		dc.SetBrushColor(theme.LabelTextColors.DisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.LabelTextColors.TextColor);
		dc.SetBackgroundColor(theme.LabelTextColors.BackgroundColor);
		dc.SetBrushColor(theme.LabelTextColors.BackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Label::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_DRAWITEM, std::bind(&Label::OnDrawItem, this, _1, _2));
	AddMessage(WM_CTLCOLORSTATIC, std::bind(&Label::OnCtlColor, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
