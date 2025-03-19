#ifndef UI_THEME_H
#define UI_THEME_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Common.h"

// Structs /////////////////////////////////////////////////////////////////

namespace UI
{
	class Dpi;

	struct TextColors
	{
		COLORREF TextColor;
		COLORREF BackgroundColor;
		COLORREF SelectedTextColor;
		COLORREF SelectedBackgroundColor;
		COLORREF DisabledTextColor;
		COLORREF DisabledBackgroundColor;
	};

	struct ButtonTextColors
	{
		COLORREF TextColor;
		COLORREF FocusTextColor;
		COLORREF HoverTextColor;
		COLORREF HoverFocusTextColor;
		COLORREF PushedTextColor;
		COLORREF DisabledTextColor;

		COLORREF BackgroundColor;
		COLORREF FocusBackgroundColor;
		COLORREF HoverBackgroundColor;
		COLORREF HoverFocusBackgroundColor;
		COLORREF PushedBackgroundColor;
		COLORREF DisabledBackgroundColor;
	};

	struct BorderColors
	{
		COLORREF BorderColor;
		COLORREF FocusBorderColor;
		COLORREF HoverBorderColor;
		COLORREF HoverFocusBorderColor;
		COLORREF PushedBorderColor;
		COLORREF DisabledBorderColor;
	};

	struct BorderSizes
	{
		int BorderSize;
		int FocusBorderSize;
		int HoverBorderSize;
		int HoverFocusBorderSize;
		int PushedBorderSize;
		int DisabledBorderSize;
	};

	struct Theme
	{
		std::wstring FontName;
		int FontSize;

		int Spacing;
		int LabelSpacing;
		int Padding;
		int LineHeight;
		int LineSpacing;
		int ButtonPadding;
		int ButtonWidth;
		int ButtonHeight;

		bool IsDark;

		COLORREF WindowBackgroundColor;

		UI::ButtonTextColors ButtonTextColors;
		UI::BorderColors ButtonBorderColors;
		UI::BorderSizes ButtonBorderSizes;
		bool ButtonFocusRect;
		int ButtonFocusRectPadding;

		UI::TextColors ListBoxTextColors;
		UI::TextColors ListBoxGrayTextColors;
		UI::BorderColors ListBoxBorderColors;
		UI::BorderSizes ListBoxBorderSizes;
		int ListBoxPadding;
		bool ListBoxFocusRect;
		int ListBoxFocusRectPadding;

		int ComboBoxButtonWidth;
		UI::ButtonTextColors ComboBoxTextColors;
		UI::BorderColors ComboBoxBorderColors;
		UI::BorderSizes ComboBoxBorderSizes;
		bool ComboBoxFocusRect;
		int ComboBoxFocusRectPadding;

		UI::TextColors ComboBoxListTextColors;
		UI::TextColors ComboBoxListGrayTextColors;
		bool ComboBoxListFocusRect;
		int ComboBoxListFocusRectPadding;

		UI::TextColors GroupBoxTextColors;
		UI::BorderColors GroupBoxBorderColors;
		UI::BorderSizes GroupBoxBorderSizes;

		UI::TextColors LabelTextColors;

		UI::TextColors TextBoxTextColors;
		UI::TextColors TextBoxInvalidTextColors;
		UI::BorderColors TextBoxBorderColors;
		UI::BorderSizes TextBoxBorderSizes;

		Theme() = default;
		Theme(const Dpi &dpi);
	};

	struct LightTheme : public Theme
	{
		LightTheme(const Dpi &dpi);
	};

	struct DarkTheme : public Theme
	{
		DarkTheme(const Dpi &dpi);
	};

	struct HighContrastTheme : public Theme
	{
		HighContrastTheme(const Dpi &dpi);
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
