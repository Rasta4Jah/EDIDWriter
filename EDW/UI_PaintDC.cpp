// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::PaintDC::PaintDC(const Window &window) :
	DC(InitHandle(window.GetHandle(), paint)),
	window(window.GetHandle()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::PaintDC::PaintDC(const Control &control) :
	DC(InitHandle(control.GetHandle(), paint)),
	window(control.GetHandle()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::PaintDC::PaintDC(const PaintDC &other) :
	DC(other),
	window(other.window),
	paint(other.paint),
	references(other.references)
{
	++*references;
}

////////////////////////////////////////////////////////////////////////////

UI::PaintDC &UI::PaintDC::operator=(PaintDC other)
{
	DC::operator=(other);
	std::swap(window, other.window);
	std::swap(paint, other.paint);
	std::swap(references, other.references);
	return *this;
}

////////////////////////////////////////////////////////////////////////////

UI::PaintDC::~PaintDC()
{
	--*references;

	if (*references <= 0)
	{
		EndPaint(window, &paint);
		delete references;
	}
}

////////////////////////////////////////////////////////////////////////////

HDC UI::PaintDC::InitHandle(HWND window, PAINTSTRUCT &paint) const
{
	HDC handle = BeginPaint(window, &paint);

	if (!handle)
		throw L"BeginPaint failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////
