#ifndef UI_MESSAGEBOX_H
#define UI_MESSAGEBOX_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Window.h"
#include "UI_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class MessageBox : public Window
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		MessageBox(const std::wstring &text, const std::wstring &title = L"", const std::wstring &button1Text = L"OK", const std::wstring &button2Text = L"", const std::wstring &button3Text = L"");
		MessageBox(const Window &window, const std::wstring &text, const std::wstring &title = L"", const std::wstring &button1Text = L"OK", const std::wstring &button2Text = L"", const std::wstring &button3Text = L"");

		////////////////////////////////////////////////////////////////////

	private:

		////////////////////////////////////////////////////////////////////
		// Private /////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		LRESULT OnClose(WPARAM wParam, LPARAM lParam);
		LRESULT OnSysColorChange(WPARAM wParam, LPARAM lParam);
		LRESULT OnSettingChange(WPARAM wParam, LPARAM lParam);
		LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
		LRESULT OnThemeChanged(WPARAM wParam, LPARAM lParam);
		bool AddMessages();
		void Button1Click();
		void Button2Click();
		void Button3Click();
		void EscKeyPress();
		bool AddCommands();
		bool InitializeTheme();
		bool InitializeControls();
		bool ScaleControls();

		// Variables ///////////////////////////////////////////////////////

		Controls::Label label;
		Controls::Button button1;
		Controls::Button button2;
		Controls::Button button3;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
