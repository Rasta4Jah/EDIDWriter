#ifndef UI_CONTROLS_LISTBOX_H
#define UI_CONTROLS_LISTBOX_H

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
		class ListBox : public Control
		{
		public:

			////////////////////////////////////////////////////////////////////
			// Public //////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			// Constants ///////////////////////////////////////////////////////

			static const DWORD DefaultStyle = WS_VSCROLL | WS_TABSTOP | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY;
			static const DWORD DefaultExStyle = 0;

			// Functions ///////////////////////////////////////////////////////

			ListBox(const Window &window, DWORD style = DefaultStyle, DWORD exStyle = DefaultExStyle);

			bool SetFont(const Font &font);
			TextColors GetTextColors() const;
			bool SetTextColors(const TextColors &colors);
			TextColors GetTextColors(int index) const;
			bool SetTextColors(int index, const TextColors &colors);
			int GetCount() const;
			bool IsValidIndex(int index) const;
			bool Add(const std::wstring &value);
			bool Insert(int index, const std::wstring &value);
			bool GetText(int index, std::wstring &value) const;
			bool Delete(int index);
			bool Clear();
			int GetIndex() const;
			bool SetIndex(int index);
			bool Deselect();
			int GetTopIndex() const;
			bool SetTopIndex(int index);

			////////////////////////////////////////////////////////////////////

		private:

			////////////////////////////////////////////////////////////////////
			// Private /////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			// Functions ///////////////////////////////////////////////////////

			LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
			LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
			LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
			LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
			LRESULT OnNcCalcSize(WPARAM wParam, LPARAM lParam);
			LRESULT OnNcPaint(WPARAM wParam, LPARAM lParam);
			LRESULT OnCtlColor(WPARAM wParam, LPARAM lParam);
			LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
			LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
			bool AddMessages();

			// Variables ///////////////////////////////////////////////////////

			std::vector<std::wstring> text;
			std::vector<TextColors> textColors;
			bool hovering;

			////////////////////////////////////////////////////////////////////
		};
	}
}

////////////////////////////////////////////////////////////////////////////

#endif
