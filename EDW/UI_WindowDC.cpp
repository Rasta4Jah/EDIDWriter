// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::WindowDC::WindowDC(const Window &window) :
	DC(InitHandle(window.GetHandle())),
	window(window.GetHandle()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::WindowDC::WindowDC(const Control &control) :
	DC(InitHandle(control.GetHandle())),
	window(control.GetHandle()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::WindowDC::WindowDC(const WindowDC &other) :
	DC(other),
	window(other.window),
	references(other.references)
{
	++*references;
}

////////////////////////////////////////////////////////////////////////////

UI::WindowDC &UI::WindowDC::operator=(WindowDC other)
{
	DC::operator=(other);
	std::swap(window, other.window);
	std::swap(references, other.references);
	return *this;
}

////////////////////////////////////////////////////////////////////////////

UI::WindowDC::~WindowDC()
{
	--*references;

	if (*references <= 0)
	{
		ReleaseDC(window, GetHandle());
		delete references;
	}
}

////////////////////////////////////////////////////////////////////////////

HDC UI::WindowDC::InitHandle(HWND window) const
{
	HDC handle = GetWindowDC(window);

	if (!handle)
		throw L"GetWindowDC failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////
