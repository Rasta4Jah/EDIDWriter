// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Function ////////////////////////////////////////////////////////////////

UI::MessageBox::MessageBox(const std::wstring &text, const std::wstring &title, const std::wstring &button1Text, const std::wstring &button2Text, const std::wstring &button3Text) :
	Window(L"MessageBox", title, WS_CAPTION | WS_SYSMENU, WS_EX_DLGMODALFRAME),
	label(*this, text, DT_NOPREFIX),
	button1(*this, button1Text),
	button2(*this, button2Text),
	button3(*this, button3Text)
{
	AddMessages();
	AddCommands();
	InitializeTheme();
	InitializeControls();
	ScaleControls();
	Center();
	Show();
}

////////////////////////////////////////////////////////////////////////////

UI::MessageBox::MessageBox(const Window &window, const std::wstring &text, const std::wstring &title, const std::wstring &button1Text, const std::wstring &button2Text, const std::wstring &button3Text) :
	Window(window, L"MessageBox", title, WS_CAPTION | WS_SYSMENU, WS_EX_DLGMODALFRAME),
	label(*this, text, DT_NOPREFIX),
	button1(*this, button1Text),
	button2(*this, button2Text),
	button3(*this, button3Text)
{
	AddMessages();
	AddCommands();
	InitializeTheme();
	InitializeControls();
	ScaleControls();
	Center(window);
	Show();
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::MessageBox::OnClose(WPARAM wParam, LPARAM lParam)
{
	Close(0);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::MessageBox::OnSysColorChange(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::MessageBox::OnSettingChange(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		std::wstring setting = reinterpret_cast<wchar_t *>(lParam);

		if (setting == L"ImmersiveColorSet")
			InitializeTheme();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::MessageBox::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	ScaleControls();
	RECT *rect = reinterpret_cast<RECT *>(lParam);
	SetPosition(rect->left, rect->top);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::MessageBox::OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

bool UI::MessageBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_CLOSE, std::bind(&MessageBox::OnClose, this, _1, _2));
	AddMessage(WM_SYSCOLORCHANGE, std::bind(&MessageBox::OnSysColorChange, this, _1, _2));
	AddMessage(WM_SETTINGCHANGE, std::bind(&MessageBox::OnSettingChange, this, _1, _2));
	AddMessage(WM_DPICHANGED, std::bind(&MessageBox::OnDpiChanged, this, _1, _2));
	AddMessage(WM_THEMECHANGED, std::bind(&MessageBox::OnThemeChanged, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////

void UI::MessageBox::Button1Click()
{
	Close(1);
}

////////////////////////////////////////////////////////////////////////////

void UI::MessageBox::Button2Click()
{
	Close(2);
}

////////////////////////////////////////////////////////////////////////////

void UI::MessageBox::Button3Click()
{
	Close(3);
}

////////////////////////////////////////////////////////////////////////////

void UI::MessageBox::EscKeyPress()
{
	Close(0);
}

////////////////////////////////////////////////////////////////////////////

bool UI::MessageBox::AddCommands()
{
	button1.AddCommand(BN_CLICKED, std::bind(&MessageBox::Button1Click, this));
	button2.AddCommand(BN_CLICKED, std::bind(&MessageBox::Button2Click, this));
	button3.AddCommand(BN_CLICKED, std::bind(&MessageBox::Button3Click, this));
	AddCommand(IDCANCEL, std::bind(&MessageBox::EscKeyPress, this));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::MessageBox::InitializeTheme()
{
	UI::Dpi dpi = GetDpi();
	UI::SystemTheme systemTheme;
	UI::Theme theme;

	if (systemTheme.IsHighContrast())
		theme = UI::HighContrastTheme(dpi);
	else if (systemTheme.IsDarkMode())
		theme = UI::DarkTheme(dpi);
	else
		theme = UI::LightTheme(dpi);

	SetTheme(theme);
	label.SetTheme(theme);
	button1.SetTheme(theme);
	button2.SetTheme(theme);
	button3.SetTheme(theme);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::MessageBox::InitializeControls()
{
	button1.SetFocus();

	if (button2.GetText() == L"")
		button2.Disable();

	if (button3.GetText() == L"")
		button3.Disable();

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::MessageBox::ScaleControls()
{
	UI::Theme theme = GetTheme();
	UI::Font font(theme.FontName, theme.FontSize);

	int labelPaddingWidth = theme.Spacing * 2;
	int labelPaddingHeight = theme.Spacing * 3;

	label.SetFont(font);
	label.SetPosition(labelPaddingWidth, labelPaddingHeight);
	label.SetSize(font.GetWidth(label.GetText()), font.GetHeight());

	int labelWidth = labelPaddingWidth + label.GetWidth() + labelPaddingWidth;
	int labelHeight = labelPaddingHeight + label.GetHeight() + labelPaddingHeight;

	int buttonsPaddingWidth = theme.Spacing * 2;
	int buttonsPaddingHeight = theme.Spacing * 2;
	int buttonsWidth = buttonsPaddingWidth * 2 - theme.Spacing;

	button1.SetFont(font);
	button1.SetSize(std::max(theme.ButtonWidth, theme.ButtonPadding * 2 + font.GetWidth(button1.GetText()) + theme.ButtonPadding * 2), theme.ButtonHeight);
	buttonsWidth += button1.GetWidth() + theme.Spacing;

	if (button2.IsEnabled())
	{
		button2.SetFont(font);
		button2.SetSize(std::max(theme.ButtonWidth, theme.ButtonPadding * 2 + font.GetWidth(button2.GetText()) + theme.ButtonPadding * 2), theme.ButtonHeight);
		buttonsWidth += button2.GetWidth() + theme.Spacing;
	}

	if (button3.IsEnabled())
	{
		button3.SetFont(font);
		button3.SetSize(std::max(theme.ButtonWidth, theme.ButtonPadding * 2 + font.GetWidth(button3.GetText()) + theme.ButtonPadding * 2), theme.ButtonHeight);
		buttonsWidth += button3.GetWidth() + theme.Spacing;
	}

	int buttonsHeight = theme.ButtonHeight + buttonsPaddingHeight;
	int clientWidth = std::max(labelWidth, buttonsWidth);
	int clientHeight = labelHeight + buttonsHeight;
	SetClientSize(clientWidth, clientHeight);

	int x = clientWidth - buttonsPaddingWidth + theme.Spacing;
	int y = labelHeight;

	if (button3.IsEnabled())
	{
		x -= button3.GetWidth() + theme.Spacing;
		button3.SetPosition(x, y);
	}

	if (button2.IsEnabled())
	{
		x -= button2.GetWidth() + theme.Spacing;
		button2.SetPosition(x, y);
	}

	x -= button1.GetWidth() + theme.Spacing;
	button1.SetPosition(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////
