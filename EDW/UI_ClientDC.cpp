// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::ClientDC::ClientDC(const Window &window) :
	DC(InitHandle(window.GetHandle())),
	window(window.GetHandle()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::ClientDC::ClientDC(const Control &control) :
	DC(InitHandle(control.GetHandle())),
	window(control.GetHandle()),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::ClientDC::ClientDC(const ClientDC &other) :
	DC(other),
	window(other.window),
	references(other.references)
{
	++*references;
}

////////////////////////////////////////////////////////////////////////////

UI::ClientDC &UI::ClientDC::operator=(ClientDC other)
{
	DC::operator=(other);
	std::swap(window, other.window);
	std::swap(references, other.references);
	return *this;
}

////////////////////////////////////////////////////////////////////////////

UI::ClientDC::~ClientDC()
{
	--*references;

	if (*references <= 0)
	{
		ReleaseDC(window, GetHandle());
		delete references;
	}
}

////////////////////////////////////////////////////////////////////////////

HDC UI::ClientDC::InitHandle(HWND window) const
{
	HDC handle = GetDC(window);

	if (!handle)
		throw L"GetDC failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////
