// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Theme::Theme(const Dpi &dpi)
{
	FontName = L"Tahoma";
	FontSize = dpi.Scale(16);

	Spacing = dpi.Scale(8);
	LabelSpacing = dpi.Scale(4);
	Padding = dpi.Scale(4);
	LineHeight = Padding + FontSize + Padding - 1;
	LineSpacing = dpi.Scale(2);
	ButtonPadding = Padding * 2;
	ButtonWidth = ButtonPadding + FontSize * 4 + ButtonPadding;
	ButtonHeight = LineHeight;
}

////////////////////////////////////////////////////////////////////////////

UI::LightTheme::LightTheme(const Dpi &dpi) :
	Theme(dpi)
{
	IsDark = false;

	UI::TextColors TextColors;
	TextColors.TextColor = RGB(0, 0, 0);
	TextColors.BackgroundColor = RGB(255, 255, 255);
	TextColors.SelectedTextColor = RGB(255, 255, 255);
	TextColors.SelectedBackgroundColor = RGB(0, 120, 215);
	TextColors.DisabledTextColor = RGB(131, 131, 131);
	TextColors.DisabledBackgroundColor = RGB(204, 204, 204);

	UI::TextColors GrayTextColors = TextColors;
	GrayTextColors.TextColor = RGB(131, 131, 131);
	GrayTextColors.SelectedTextColor = RGB(204, 204, 204);
	GrayTextColors.SelectedBackgroundColor = RGB(131, 131, 131);

	UI::TextColors InvalidTextColors = TextColors;
	InvalidTextColors.TextColor = RGB(255, 0, 0);

	UI::TextColors WindowTextColors = TextColors;
	WindowTextColors.TextColor = RGB(0, 0, 0);
	WindowTextColors.BackgroundColor = RGB(240, 240, 240);
	WindowTextColors.DisabledBackgroundColor = WindowTextColors.BackgroundColor;

	UI::BorderColors BorderColors;
	BorderColors.BorderColor = RGB(173, 173, 173);
	BorderColors.FocusBorderColor = BorderColors.BorderColor;
	BorderColors.HoverBorderColor = BorderColors.BorderColor;
	BorderColors.HoverFocusBorderColor = BorderColors.BorderColor;
	BorderColors.PushedBorderColor = BorderColors.BorderColor;
	BorderColors.DisabledBorderColor = RGB(191, 191, 191);

	UI::BorderSizes BorderSizes;
	BorderSizes.BorderSize = dpi.Scale(1);
	BorderSizes.FocusBorderSize = BorderSizes.BorderSize;
	BorderSizes.HoverBorderSize = BorderSizes.BorderSize;
	BorderSizes.HoverFocusBorderSize = BorderSizes.BorderSize;
	BorderSizes.PushedBorderSize = BorderSizes.BorderSize;
	BorderSizes.DisabledBorderSize = BorderSizes.BorderSize;

	WindowBackgroundColor = WindowTextColors.BackgroundColor;

	ButtonTextColors.TextColor = RGB(0, 0, 0);
	ButtonTextColors.FocusTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.HoverTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.HoverFocusTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.PushedTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.DisabledTextColor = RGB(131, 131, 131);

	ButtonTextColors.BackgroundColor = RGB(225, 225, 225);
	ButtonTextColors.FocusBackgroundColor = ButtonTextColors.BackgroundColor;
	ButtonTextColors.HoverBackgroundColor = RGB(229, 241, 251);
	ButtonTextColors.HoverFocusBackgroundColor = ButtonTextColors.HoverBackgroundColor;
	ButtonTextColors.PushedBackgroundColor = RGB(204, 228, 247);
	ButtonTextColors.DisabledBackgroundColor = RGB(204, 204, 204);

	ButtonBorderColors = BorderColors;
	ButtonBorderColors.FocusBorderColor = TextColors.SelectedBackgroundColor;
	ButtonBorderColors.HoverBorderColor = ButtonBorderColors.FocusBorderColor;
	ButtonBorderColors.HoverFocusBorderColor = ButtonBorderColors.FocusBorderColor;
	ButtonBorderColors.PushedBorderColor = RGB(0, 84, 153);
	ButtonBorderSizes = BorderSizes;
	ButtonBorderSizes.FocusBorderSize = ButtonBorderSizes.BorderSize * 2;
	ButtonFocusRect = true;
	ButtonFocusRectPadding = ButtonBorderSizes.BorderSize * 2;

	ListBoxTextColors = TextColors;
	ListBoxGrayTextColors = GrayTextColors;
	ListBoxBorderColors = BorderColors;
	ListBoxBorderSizes = BorderSizes;
	ListBoxPadding = ListBoxBorderSizes.FocusBorderSize;
	ListBoxFocusRect = true;
	ListBoxFocusRectPadding = 0;

	ComboBoxButtonWidth = dpi.GetSystemMetrics(SM_CXVSCROLL);
	ComboBoxTextColors = ButtonTextColors;
	ComboBoxBorderColors = ButtonBorderColors;
	ComboBoxBorderColors.FocusBorderColor = BorderColors.FocusBorderColor;
	ComboBoxBorderSizes = BorderSizes;
	ComboBoxFocusRect = true;
	ComboBoxFocusRectPadding = Padding - 1;

	ComboBoxListTextColors = ListBoxTextColors;
	ComboBoxListGrayTextColors = ListBoxGrayTextColors;
	ComboBoxListFocusRect = ListBoxFocusRect;
	ComboBoxListFocusRectPadding = ListBoxFocusRectPadding;

	GroupBoxTextColors = WindowTextColors;
	GroupBoxBorderColors = BorderColors;
	GroupBoxBorderSizes = BorderSizes;

	LabelTextColors = WindowTextColors;

	TextBoxTextColors = TextColors;
	TextBoxInvalidTextColors = InvalidTextColors;
	TextBoxBorderColors = ButtonBorderColors;
	TextBoxBorderColors.HoverBorderColor = RGB(23, 23, 23);
	TextBoxBorderSizes = BorderSizes;
}

////////////////////////////////////////////////////////////////////////////

UI::DarkTheme::DarkTheme(const Dpi &dpi) :
	Theme(dpi)
{
	IsDark = true;

	UI::TextColors TextColors;
	TextColors.TextColor = RGB(255, 255, 255);
	TextColors.BackgroundColor = RGB(0, 0, 0);
	TextColors.SelectedTextColor = RGB(255, 255, 255);
	TextColors.SelectedBackgroundColor = RGB(0, 120, 215);
	TextColors.DisabledTextColor = RGB(131, 131, 131);
	TextColors.DisabledBackgroundColor = RGB(51, 51, 51);

	UI::TextColors GrayTextColors = TextColors;
	GrayTextColors.TextColor = RGB(131, 131, 131);
	GrayTextColors.SelectedTextColor = RGB(51, 51, 51);
	GrayTextColors.SelectedBackgroundColor = RGB(131, 131, 131);

	UI::TextColors InvalidTextColors = TextColors;
	InvalidTextColors.TextColor = RGB(255, 0, 0);

	UI::TextColors WindowTextColors = TextColors;
	WindowTextColors.TextColor = RGB(255, 255, 255);
	WindowTextColors.BackgroundColor = RGB(0, 0, 0);
	WindowTextColors.DisabledBackgroundColor = WindowTextColors.BackgroundColor;

	UI::BorderColors BorderColors;
	BorderColors.BorderColor = RGB(155, 155, 155);
	BorderColors.FocusBorderColor = BorderColors.BorderColor;
	BorderColors.HoverBorderColor = BorderColors.BorderColor;
	BorderColors.HoverFocusBorderColor = BorderColors.BorderColor;
	BorderColors.PushedBorderColor = BorderColors.BorderColor;
	BorderColors.DisabledBorderColor = RGB(64, 64, 64);

	UI::BorderSizes BorderSizes;
	BorderSizes.BorderSize = dpi.Scale(1);
	BorderSizes.FocusBorderSize = BorderSizes.BorderSize;
	BorderSizes.HoverBorderSize = BorderSizes.BorderSize;
	BorderSizes.HoverFocusBorderSize = BorderSizes.BorderSize;
	BorderSizes.PushedBorderSize = BorderSizes.BorderSize;
	BorderSizes.DisabledBorderSize = BorderSizes.BorderSize;

	WindowBackgroundColor = WindowTextColors.BackgroundColor;

	ButtonTextColors.TextColor = RGB(255, 255, 255);
	ButtonTextColors.FocusTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.HoverTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.HoverFocusTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.PushedTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.DisabledTextColor = RGB(131, 131, 131);

	ButtonTextColors.BackgroundColor = RGB(51, 51, 51);
	ButtonTextColors.FocusBackgroundColor = ButtonTextColors.BackgroundColor;
	ButtonTextColors.HoverBackgroundColor = RGB(69, 69, 69);
	ButtonTextColors.HoverFocusBackgroundColor = ButtonTextColors.HoverBackgroundColor;
	ButtonTextColors.PushedBackgroundColor = RGB(102, 102, 102);
	ButtonTextColors.DisabledBackgroundColor = ButtonTextColors.BackgroundColor;

	ButtonBorderColors = BorderColors;
	ButtonBorderColors.FocusBorderColor = RGB(255, 255, 255);
	ButtonBorderSizes = BorderSizes;
	ButtonFocusRect = true;
	ButtonFocusRectPadding = ButtonBorderSizes.BorderSize * 2;

	ListBoxTextColors = TextColors;
	ListBoxGrayTextColors = GrayTextColors;
	ListBoxBorderColors = BorderColors;
	ListBoxBorderSizes = BorderSizes;
	ListBoxPadding = ListBoxBorderSizes.FocusBorderSize;
	ListBoxFocusRect = true;
	ListBoxFocusRectPadding = 0;

	ComboBoxButtonWidth = dpi.GetSystemMetrics(SM_CXVSCROLL);
	ComboBoxTextColors = ButtonTextColors;
	ComboBoxBorderColors = ButtonBorderColors;
	ComboBoxBorderColors.FocusBorderColor = BorderColors.FocusBorderColor;
	ComboBoxBorderSizes = BorderSizes;
	ComboBoxFocusRect = true;
	ComboBoxFocusRectPadding = Padding - 1;

	ComboBoxListTextColors = ListBoxTextColors;
	ComboBoxListGrayTextColors = ListBoxGrayTextColors;
	ComboBoxListFocusRect = ListBoxFocusRect;
	ComboBoxListFocusRectPadding = ListBoxFocusRectPadding;

	GroupBoxTextColors = WindowTextColors;
	GroupBoxBorderColors = BorderColors;
	GroupBoxBorderSizes = BorderSizes;

	LabelTextColors = WindowTextColors;

	TextBoxTextColors = TextColors;
	TextBoxInvalidTextColors = InvalidTextColors;
	TextBoxBorderColors = ButtonBorderColors;
	TextBoxBorderColors.HoverBorderColor = RGB(200, 200, 200);
	TextBoxBorderSizes = BorderSizes;
}

////////////////////////////////////////////////////////////////////////////

UI::HighContrastTheme::HighContrastTheme(const Dpi &dpi) :
	Theme(dpi)
{
	IsDark = false;

	UI::TextColors TextColors;
	TextColors.TextColor = GetSysColor(COLOR_WINDOWTEXT);
	TextColors.BackgroundColor = GetSysColor(COLOR_WINDOW);
	TextColors.SelectedTextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
	TextColors.SelectedBackgroundColor = GetSysColor(COLOR_HIGHLIGHT);
	TextColors.DisabledTextColor = GetSysColor(COLOR_GRAYTEXT);
	TextColors.DisabledBackgroundColor = GetSysColor(COLOR_WINDOW);

	UI::TextColors GrayTextColors = TextColors;
	GrayTextColors.TextColor = GetSysColor(COLOR_GRAYTEXT);
	GrayTextColors.SelectedTextColor = GetSysColor(COLOR_WINDOW);
	GrayTextColors.SelectedBackgroundColor = GetSysColor(COLOR_GRAYTEXT);

	UI::TextColors InvalidTextColors = TextColors;
	InvalidTextColors.TextColor = RGB(255, 0, 0);

	UI::TextColors WindowTextColors = TextColors;
	WindowTextColors.TextColor = GetSysColor(COLOR_BTNTEXT);
	WindowTextColors.BackgroundColor = GetSysColor(COLOR_BTNFACE);
	WindowTextColors.DisabledBackgroundColor = WindowTextColors.BackgroundColor;

	UI::BorderColors BorderColors;
	BorderColors.BorderColor = GetSysColor(COLOR_BTNTEXT);
	BorderColors.FocusBorderColor = BorderColors.BorderColor;
	BorderColors.HoverBorderColor = BorderColors.BorderColor;
	BorderColors.HoverFocusBorderColor = BorderColors.BorderColor;
	BorderColors.PushedBorderColor = BorderColors.BorderColor;
	BorderColors.DisabledBorderColor = GetSysColor(COLOR_GRAYTEXT);

	UI::BorderSizes BorderSizes;
	BorderSizes.BorderSize = dpi.Scale(1);
	BorderSizes.FocusBorderSize = BorderSizes.BorderSize;
	BorderSizes.HoverBorderSize = BorderSizes.BorderSize;
	BorderSizes.HoverFocusBorderSize = BorderSizes.BorderSize;
	BorderSizes.PushedBorderSize = BorderSizes.BorderSize;
	BorderSizes.DisabledBorderSize = BorderSizes.BorderSize;

	WindowBackgroundColor = WindowTextColors.BackgroundColor;

	ButtonTextColors.TextColor = GetSysColor(COLOR_BTNTEXT);
	ButtonTextColors.FocusTextColor = TextColors.SelectedTextColor;
	ButtonTextColors.HoverTextColor = ButtonTextColors.FocusTextColor;
	ButtonTextColors.HoverFocusTextColor = ButtonTextColors.FocusTextColor;
	ButtonTextColors.PushedTextColor = ButtonTextColors.TextColor;
	ButtonTextColors.DisabledTextColor = GetSysColor(COLOR_GRAYTEXT);

	ButtonTextColors.BackgroundColor = GetSysColor(COLOR_BTNFACE);
	ButtonTextColors.FocusBackgroundColor = TextColors.SelectedBackgroundColor;
	ButtonTextColors.HoverBackgroundColor = ButtonTextColors.FocusBackgroundColor;
	ButtonTextColors.HoverFocusBackgroundColor = ButtonTextColors.FocusBackgroundColor;
	ButtonTextColors.PushedBackgroundColor = ButtonTextColors.BackgroundColor;
	ButtonTextColors.DisabledBackgroundColor = ButtonTextColors.BackgroundColor;

	ButtonBorderColors = BorderColors;
	ButtonBorderSizes = BorderSizes;
	ButtonFocusRect = true;
	ButtonFocusRectPadding = ButtonBorderSizes.BorderSize * 2;

	ListBoxTextColors = TextColors;
	ListBoxGrayTextColors = GrayTextColors;
	ListBoxBorderColors = BorderColors;
	ListBoxBorderSizes = BorderSizes;
	ListBoxPadding = ListBoxBorderSizes.FocusBorderSize;
	ListBoxFocusRect = true;
	ListBoxFocusRectPadding = 0;

	ComboBoxButtonWidth = dpi.GetSystemMetrics(SM_CXVSCROLL);
	ComboBoxTextColors = ButtonTextColors;
	ComboBoxTextColors.FocusTextColor = ComboBoxTextColors.TextColor;
	ComboBoxTextColors.HoverTextColor = ComboBoxTextColors.TextColor;
	ComboBoxTextColors.HoverFocusTextColor = ComboBoxTextColors.TextColor;
	ComboBoxTextColors.FocusBackgroundColor = ComboBoxTextColors.BackgroundColor;
	ComboBoxTextColors.HoverBackgroundColor = ComboBoxTextColors.BackgroundColor;
	ComboBoxTextColors.HoverFocusBackgroundColor = ComboBoxTextColors.BackgroundColor;
	ComboBoxBorderColors = ButtonBorderColors;
	ComboBoxBorderColors.HoverBorderColor = GetSysColor(COLOR_HOTLIGHT);
	ComboBoxBorderColors.HoverFocusBorderColor = ComboBoxBorderColors.HoverBorderColor;
	ComboBoxBorderColors.PushedBorderColor = ComboBoxBorderColors.HoverBorderColor;
	ComboBoxBorderSizes = BorderSizes;
	ComboBoxFocusRect = true;
	ComboBoxFocusRectPadding = Padding - 1;

	ComboBoxListTextColors = ListBoxTextColors;
	ComboBoxListGrayTextColors = ListBoxGrayTextColors;
	ComboBoxListFocusRect = ListBoxFocusRect;
	ComboBoxListFocusRectPadding = ListBoxFocusRectPadding;

	GroupBoxTextColors = WindowTextColors;
	GroupBoxBorderColors = BorderColors;
	GroupBoxBorderSizes = BorderSizes;

	LabelTextColors = WindowTextColors;

	TextBoxTextColors = TextColors;
	TextBoxInvalidTextColors = InvalidTextColors;
	TextBoxBorderColors = ButtonBorderColors;
	TextBoxBorderColors.FocusBorderColor = GetSysColor(COLOR_HOTLIGHT);
	TextBoxBorderColors.HoverBorderColor = TextBoxBorderColors.FocusBorderColor;
	TextBoxBorderColors.HoverFocusBorderColor = TextBoxBorderColors.FocusBorderColor;
	TextBoxBorderSizes = BorderSizes;
}

////////////////////////////////////////////////////////////////////////////
