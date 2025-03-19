// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Static Variables ////////////////////////////////////////////////////////

HMODULE UI::Dpi::module = nullptr;
UI::Dpi::GetDpiForWindowFunction UI::Dpi::GetDpiForWindow = nullptr;
UI::Dpi::AdjustWindowRectExForDpiFunction UI::Dpi::AdjustWindowRectExForDpi = nullptr;
UI::Dpi::GetSystemMetricsForDpiFunction UI::Dpi::GetSystemMetricsForDpi = nullptr;

// Functions ///////////////////////////////////////////////////////////////

UI::Dpi::Dpi(const Window &window) :
	dpi(InitDpi(window))
{
}

////////////////////////////////////////////////////////////////////////////

UI::Dpi::Dpi(int dpi) :
	dpi(dpi)
{
}

////////////////////////////////////////////////////////////////////////////

int UI::Dpi::Get() const
{
	return dpi;
}

////////////////////////////////////////////////////////////////////////////

int UI::Dpi::Scale(int value) const
{
	return value * dpi / 96;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Dpi::AdjustWindowRect(RECT &rect, DWORD style, DWORD exStyle, bool menu) const
{
	if (AdjustWindowRectExForDpi)
		return AdjustWindowRectExForDpi(&rect, style, menu, exStyle, dpi);

	return AdjustWindowRectEx(&rect, style, menu, exStyle);
}

////////////////////////////////////////////////////////////////////////////

int UI::Dpi::GetSystemMetrics(int index) const
{
	if (GetSystemMetricsForDpi)
		return GetSystemMetricsForDpi(index, dpi);

	return ::GetSystemMetrics(index);
}

////////////////////////////////////////////////////////////////////////////

int UI::Dpi::InitDpi(const Window &window) const
{
	if (!module)
	{
		module = LoadLibraryW(L"user32.dll");

		if (!module)
			throw L"LoadLibraryW failed.";

		GetDpiForWindow = reinterpret_cast<GetDpiForWindowFunction>(GetProcAddress(module, "GetDpiForWindow"));
		AdjustWindowRectExForDpi = reinterpret_cast<AdjustWindowRectExForDpiFunction>(GetProcAddress(module, "AdjustWindowRectExForDpi"));
		GetSystemMetricsForDpi = reinterpret_cast<GetSystemMetricsForDpiFunction>(GetProcAddress(module, "GetSystemMetricsForDpi"));
	}

	if (GetDpiForWindow)
	{
		int dpi = GetDpiForWindow(window.GetHandle());

		if (dpi > 0)
			return dpi;
	}

	ScreenDC dc;
	return dc.GetDeviceCaps(LOGPIXELSX);
}

////////////////////////////////////////////////////////////////////////////
