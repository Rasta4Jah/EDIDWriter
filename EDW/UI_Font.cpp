// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Font::Font() :
	handle(InitHandle()),
	textMetrics(InitTextMetrics()),
	references(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

UI::Font::Font(const std::wstring &name, int size, int weight, bool italic, bool underline, bool strikeout) :
	handle(InitHandle(name, size, weight, italic, underline, strikeout)),
	textMetrics(InitTextMetrics()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::Font::Font(const Font &other) :
	handle(other.handle),
	textMetrics(other.textMetrics),
	references(other.references)
{
	if (references)
		++*references;
}

////////////////////////////////////////////////////////////////////////////

UI::Font &UI::Font::operator=(Font other)
{
	std::swap(handle, other.handle);
	std::swap(textMetrics, other.textMetrics);
	std::swap(references, other.references);
	return *this;
}

////////////////////////////////////////////////////////////////////////////

UI::Font::~Font()
{
	if (references)
	{
		--*references;

		if (*references <= 0)
		{
			DeleteObject(handle);
			delete references;
		}
	}
}

////////////////////////////////////////////////////////////////////////////

HFONT UI::Font::GetHandle() const
{
	return handle;
}

////////////////////////////////////////////////////////////////////////////

int UI::Font::GetWidth(const std::wstring &text) const
{
	ScreenDC dc;

	if (!dc.SelectObject(handle))
		return false;

	SIZE size;

	if (!dc.GetTextExtent(text, size))
		return false;

	return size.cx;
}

////////////////////////////////////////////////////////////////////////////

int UI::Font::GetHeight() const
{
	return textMetrics.tmHeight;
}

////////////////////////////////////////////////////////////////////////////

int UI::Font::GetAscent() const
{
	return textMetrics.tmAscent;
}

////////////////////////////////////////////////////////////////////////////

int UI::Font::GetDescent() const
{
	return textMetrics.tmDescent;
}

////////////////////////////////////////////////////////////////////////////

HFONT UI::Font::InitHandle() const
{
	HFONT handle = static_cast<HFONT>(GetStockObject(SYSTEM_FONT));

	if (!handle)
		throw L"GetStockObject failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////

HFONT UI::Font::InitHandle(const std::wstring &name, int size, int weight, bool italic, bool underline, bool strikeout) const
{
	HFONT handle = CreateFontW(size, 0, 0, 0, weight, italic, underline, strikeout, DEFAULT_CHARSET, 0, 0, 0, 0, name.c_str());

	if (!handle)
		throw L"CreateFontW failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////

TEXTMETRICW UI::Font::InitTextMetrics() const
{
	ScreenDC dc;

	if (!dc.SelectObject(handle))
	{
		DeleteObject(handle);
		throw L"SelectObject failed.";
	}

	TEXTMETRICW textMetrics;

	if (!dc.GetTextMetrics(textMetrics))
	{
		DeleteObject(handle);
		throw L"GetTextMetrics failed.";
	}

	return textMetrics;
}

////////////////////////////////////////////////////////////////////////////
