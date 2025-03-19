#ifndef UI_CONTROLS_TEXTBOX_H
#define UI_CONTROLS_TEXTBOX_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Control.h"
#include "UI_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class Window;
	class Font;

	namespace Controls
	{
		class TextBox : public Control
		{
		public:

			////////////////////////////////////////////////////////////////////
			// Public //////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			// Constants ///////////////////////////////////////////////////////

			static const DWORD DefaultStyle = WS_TABSTOP | ES_AUTOHSCROLL;
			static const DWORD MultiLineStyle = WS_TABSTOP | ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE;
			static const DWORD DefaultExStyle = 0;

			// Functions ///////////////////////////////////////////////////////

			TextBox(const Window &window, DWORD style = DefaultStyle, DWORD exStyle = DefaultExStyle);

			bool SetFont(const Font &font);
			TextColors GetTextColors() const;
			bool SetTextColors(const TextColors &colors);
			int GetMaxLength() const;
			bool SetMaxLength(int length);
			std::wstring GetText() const;
			bool SetText(const std::wstring &text);
			bool GetInteger(int &value) const;
			bool SetInteger(int value);
			bool Clear();
			bool Select(int begin = 0, int end = -1);

			bool ProcessCommand(int command);

			////////////////////////////////////////////////////////////////////

		private:

			////////////////////////////////////////////////////////////////////
			// Private /////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			// Functions ///////////////////////////////////////////////////////

			LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
			LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
			LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
			LRESULT OnNcCalcSize(WPARAM wParam, LPARAM lParam);
			LRESULT OnNcPaint(WPARAM wParam, LPARAM lParam);
			LRESULT OnGetDlgCode(WPARAM wParam, LPARAM lParam);
			LRESULT OnChar(WPARAM wParam, LPARAM lParam);
			LRESULT OnCtlColor(WPARAM wParam, LPARAM lParam);
			LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
			LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
			bool AddMessages();

			// Variables ///////////////////////////////////////////////////////

			bool hovering;
			bool updating;

			////////////////////////////////////////////////////////////////////
		};
	}
}

////////////////////////////////////////////////////////////////////////////

#endif
