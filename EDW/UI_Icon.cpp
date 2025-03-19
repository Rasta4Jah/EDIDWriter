// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Icon::Icon() :
	handle(nullptr),
	references(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

UI::Icon::Icon(const std::wstring &name, int size) :
	handle(InitHandle(name, size)),
	references(new int(1))
{
}

////////////////////////////////////////////////////////////////////////////

UI::Icon::Icon(const Icon &other) :
	handle(other.handle),
	references(other.references)
{
	if (references)
		++*references;
}

////////////////////////////////////////////////////////////////////////////

UI::Icon &UI::Icon::operator=(Icon other)
{
	std::swap(handle, other.handle);
	std::swap(references, other.references);
	return *this;
}

////////////////////////////////////////////////////////////////////////////

UI::Icon::~Icon()
{
	if (references)
	{
		--*references;

		if (*references <= 0)
		{
			DestroyIcon(handle);
			delete references;
		}
	}
}

////////////////////////////////////////////////////////////////////////////

HICON UI::Icon::GetHandle() const
{
	return handle;
}

////////////////////////////////////////////////////////////////////////////

HICON UI::Icon::InitHandle(const std::wstring &name, int size) const
{
	HICON handle = static_cast<HICON>(LoadImageW(GetModuleHandleW(nullptr), name.c_str(), IMAGE_ICON, size, size, 0));

	if (!handle)
		throw L"LoadImageW failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////
