#ifndef UI_CONTROL_H
#define UI_CONTROL_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Font.h"
#include "UI_Theme.h"
#include "UI_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class Window;
	class DC;

	class Control
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Typedefs ////////////////////////////////////////////////////////

		typedef std::function<LRESULT(WPARAM, LPARAM, UINT)> MessageFunction;
		typedef std::function<void()> CommandFunction;

		// Functions ///////////////////////////////////////////////////////

		Control(const Window &window, const std::wstring &name, const std::wstring &text, DWORD style, DWORD exStyle);
		Control(const Control &) = delete;
		Control &operator=(const Control &) = delete;
		virtual ~Control();

		virtual HWND GetHandle() const;
		virtual int GetX() const;
		virtual int GetY() const;
		virtual int GetWidth() const;
		virtual int GetHeight() const;
		virtual int GetRight() const;
		virtual int GetBottom() const;
		virtual bool SetPosition(int x, int y);
		virtual bool SetSize(int width, int height);
		virtual bool SetRightBottom(int right, int bottom);
		virtual std::wstring GetText() const;
		virtual bool SetText(const std::wstring &text);
		virtual Font GetFont() const;
		virtual bool SetFont(const Font &font);
		virtual bool Invalidate() const;
		virtual Theme GetTheme() const;
		virtual bool SetTheme(const Theme &theme);
		virtual bool IsEnabled() const;
		virtual bool Enable() const;
		virtual bool Disable() const;
		virtual bool IsFocused() const;
		virtual bool IsFocusVisible() const;
		virtual bool SetFocus();
		virtual bool BeginUpdate();
		virtual bool EndUpdate();

		virtual bool AddPreMessage(UINT message, MessageFunction function);
		virtual bool AddMessage(UINT message, MessageFunction function);
		virtual bool AddCommand(int command, CommandFunction function);
		virtual bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &result);
		virtual bool ProcessCommand(int command);

		////////////////////////////////////////////////////////////////////

	protected:

		////////////////////////////////////////////////////////////////////
		// Protected ///////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Static Functions ////////////////////////////////////////////////

		static LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

		// Functions ///////////////////////////////////////////////////////

		RECT DrawFillRect(const DC &dc, const RECT &rect, COLORREF textColor, COLORREF backgroundColor);
		RECT DrawFrameRect(const DC &dc, const RECT &rect, COLORREF textColor, COLORREF backgroundColor, COLORREF borderColor, int borderSize);
		RECT DrawFrameFillRect(const DC &dc, const RECT &rect, COLORREF textColor, COLORREF backgroundColor, COLORREF borderColor, int borderSize);
		RECT DrawFrameFrameRect(const DC &dc, const RECT &outerRect, const RECT &innerRect, COLORREF textColor, COLORREF backgroundColor, COLORREF borderColor, int borderSize);

		// Init Functions //////////////////////////////////////////////////

		HWND InitHandle(const Window &window, const std::wstring &name, const std::wstring &text, DWORD style, DWORD exStyle) const;

		// Variables ///////////////////////////////////////////////////////

		HWND handle;
		int x;
		int y;
		int width;
		int height;
		std::wstring text;
		DWORD style;
		DWORD exStyle;
		Font font;
		Theme theme;

		std::unordered_map<int, MessageFunction> preMessageList;
		std::unordered_map<int, MessageFunction> messageList;
		std::unordered_map<int, CommandFunction> commandList;

		WNDPROC OldWindowProc;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
