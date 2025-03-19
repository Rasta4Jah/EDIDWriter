// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Constants ///////////////////////////////////////////////////////////////

const int UI::SystemTheme::DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
const int UI::SystemTheme::DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 = 19;

// Static Variables ////////////////////////////////////////////////////////

HMODULE UI::SystemTheme::module = nullptr;
UI::SystemTheme::ShouldAppsUseDarkModeFunction UI::SystemTheme::ShouldAppsUseDarkMode = nullptr;
UI::SystemTheme::SetPreferredAppModeFunction UI::SystemTheme::SetPreferredAppMode = nullptr;

// Functions ///////////////////////////////////////////////////////////////

UI::SystemTheme::SystemTheme()
{
	if (IsWindows10OrGreater() && !module)
	{
		module = LoadLibraryW(L"uxtheme.dll");

		if (!module)
			throw L"LoadLibraryW failed.";

		ShouldAppsUseDarkMode = reinterpret_cast<ShouldAppsUseDarkModeFunction>(GetProcAddress(module, MAKEINTRESOURCEA(132)));
		SetPreferredAppMode = reinterpret_cast<SetPreferredAppModeFunction>(GetProcAddress(module, MAKEINTRESOURCEA(135)));
	}
}

////////////////////////////////////////////////////////////////////////////

bool UI::SystemTheme::IsHighContrast() const
{
	HIGHCONTRASTW highContrast = {sizeof highContrast};

	if (!SystemParametersInfoW(SPI_GETHIGHCONTRAST, 0, &highContrast, 0))
		return false;

	return highContrast.dwFlags & HCF_HIGHCONTRASTON;
}

////////////////////////////////////////////////////////////////////////////

bool UI::SystemTheme::IsDarkMode() const
{
	if (!IsWindows10OrGreater())
		return false;

	if (SetPreferredAppMode)
		SetPreferredAppMode(PreferredAppMode::AllowDark);

	if (!ShouldAppsUseDarkMode)
		return false;

	if (!ShouldAppsUseDarkMode())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::SystemTheme::SetDarkMode(const Window &window, bool enable) const
{
	if (!IsWindows10OrGreater())
		return false;

	if (SetPreferredAppMode)
		SetPreferredAppMode(enable ? PreferredAppMode::ForceDark : PreferredAppMode::ForceLight);

	int value = enable;

	if (!SUCCEEDED(DwmSetWindowAttribute(window.GetHandle(), DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof value)))
	if (!SUCCEEDED(DwmSetWindowAttribute(window.GetHandle(), DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1, &value, sizeof value)))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::SystemTheme::SetDarkMode(const Control &control, bool enable) const
{
	if (!IsWindows10OrGreater())
		return false;

	if (!SUCCEEDED(SetWindowTheme(control.GetHandle(), enable ? L"DarkMode_Explorer" : nullptr, nullptr)))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////
