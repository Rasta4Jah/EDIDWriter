// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::DC::DC(HDC handle, bool restore) :
	handle(handle),
	state(restore ? SaveDC(handle) : 0),
	references(restore ? new int(1) : nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

UI::DC::DC(WPARAM wParam, bool restore) :
	handle(reinterpret_cast<HDC>(wParam)),
	state(restore ? SaveDC(handle) : 0),
	references(restore ? new int(1) : nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

UI::DC::DC(const DC &other) :
	handle(other.handle),
	state(other.state),
	references(other.references)
{
	if (references)
		++*references;
}

////////////////////////////////////////////////////////////////////////////

UI::DC &UI::DC::operator=(DC other)
{
	std::swap(handle, other.handle);
	std::swap(state, other.state);
	std::swap(references, other.references);
	return *this;
}

////////////////////////////////////////////////////////////////////////////

UI::DC::~DC()
{
	if (references)
	{
		--*references;

		if (*references <= 0)
		{
			RestoreDC(handle, state);
			delete references;
		}
	}
}

////////////////////////////////////////////////////////////////////////////

HDC UI::DC::GetHandle() const
{
	return handle;
}

////////////////////////////////////////////////////////////////////////////

bool UI::DC::DrawFocusRect(const RECT &rect) const
{
	return ::DrawFocusRect(handle, &rect);
}

////////////////////////////////////////////////////////////////////////////

int UI::DC::DrawText(const std::wstring &text, RECT &rect, UINT format) const
{
	return ::DrawTextW(handle, text.c_str(), static_cast<int>(text.size()), &rect, format);
}

////////////////////////////////////////////////////////////////////////////

bool UI::DC::FillRect(const RECT &rect, HBRUSH brush) const
{
	return ::FillRect(handle, &rect, brush);
}

////////////////////////////////////////////////////////////////////////////

bool UI::DC::FrameRect(const RECT &outerRect, const RECT &innerRect, HBRUSH brush) const
{
	RECT rect;
	rect.left = outerRect.left;
	rect.top = outerRect.top;
	rect.right = outerRect.right;
	rect.bottom = innerRect.top;
	::FillRect(handle, &rect, brush);
	rect.left = outerRect.left;
	rect.top = innerRect.top;
	rect.right = innerRect.left;
	rect.bottom = innerRect.bottom;
	::FillRect(handle, &rect, brush);
	rect.left = innerRect.right;
	rect.top = innerRect.top;
	rect.right = outerRect.right;
	rect.bottom = innerRect.bottom;
	::FillRect(handle, &rect, brush);
	rect.left = outerRect.left;
	rect.top = innerRect.bottom;
	rect.right = outerRect.right;
	rect.bottom = outerRect.bottom;
	::FillRect(handle, &rect, brush);
	return true;
}

////////////////////////////////////////////////////////////////////////////

HBRUSH UI::DC::GetBrush() const
{
	return static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
}

////////////////////////////////////////////////////////////////////////////

int UI::DC::GetDeviceCaps(int index) const
{
	return ::GetDeviceCaps(handle, index);
}

////////////////////////////////////////////////////////////////////////////

bool UI::DC::GetTextExtent(const std::wstring &text, SIZE &size) const
{
	return GetTextExtentPoint32W(handle, text.c_str(), static_cast<int>(text.size()), &size);
}

////////////////////////////////////////////////////////////////////////////

bool UI::DC::GetTextMetrics(TEXTMETRICW &textMetrics) const
{
	return ::GetTextMetricsW(handle, &textMetrics);
}

////////////////////////////////////////////////////////////////////////////

bool UI::DC::InvertRect(const RECT &rect) const
{
	return ::InvertRect(handle, &rect);
}

////////////////////////////////////////////////////////////////////////////

HGDIOBJ UI::DC::SelectObject(HGDIOBJ object) const
{
	return ::SelectObject(handle, object);
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::DC::SetBackgroundColor(COLORREF color) const
{
	return SetBkColor(handle, color);
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::DC::SetBrushColor(COLORREF color) const
{
	return SetDCBrushColor(handle, color);
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::DC::SetTextColor(COLORREF color) const
{
	return ::SetTextColor(handle, color);
}

////////////////////////////////////////////////////////////////////////////
