#ifndef UI_WINDOW_H
#define UI_WINDOW_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Dpi.h"
#include "UI_Icon.h"
#include "UI_Theme.h"
#include "UI_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class Window
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Typedefs ////////////////////////////////////////////////////////

		typedef std::function<LRESULT(WPARAM, LPARAM, UINT)> MessageFunction;
		typedef std::function<void()> CommandFunction;

		// Functions ///////////////////////////////////////////////////////

		Window(const std::wstring &name, const std::wstring &title, DWORD style = 0, DWORD exStyle = 0);
		Window(const Window &window, const std::wstring &name, const std::wstring &title, DWORD style = 0, DWORD exStyle = 0);
		Window(const Window &) = delete;
		Window &operator=(const Window &) = delete;
		virtual ~Window();

		HWND GetHandle() const;
		int GetX() const;
		int GetY() const;
		int GetWidth() const;
		int GetHeight() const;
		int GetRight() const;
		int GetBottom() const;
		int GetClientWidth() const;
		int GetClientHeight() const;
		int GetMinWidth() const;
		int GetMinHeight() const;
		bool SetPosition(int x, int y);
		bool SetSize(int width, int height);
		bool SetRightBottom(int right, int bottom);
		bool SetClientSize(int width, int height);
		bool SetMinSize(int minWidth, int minHeight);
		bool SetMinClientSize(int minWidth, int minHeight);
		std::wstring GetName() const;
		std::wstring GetTitle() const;
		bool SetTitle(const std::wstring &title);
		Dpi GetDpi() const;
		bool SetIcon(const std::wstring &name, int maxDpi);
		bool Invalidate() const;
		Theme GetTheme() const;
		bool SetTheme(const Theme &theme);
		bool IsEnabled() const;
		bool Enable() const;
		bool Disable() const;
		bool Fit();
		bool Center();
		bool Center(const Window &window);
		bool Show();
		int MessageLoop();
		bool Close(int status);

		bool AddPreMessage(UINT message, MessageFunction function);
		bool AddMessage(UINT message, MessageFunction function);
		bool AddCommand(int command, CommandFunction function);
		bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &result);
		bool ProcessCommand(int command);

		////////////////////////////////////////////////////////////////////

	private:

		////////////////////////////////////////////////////////////////////
		// Private /////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Static Functions ////////////////////////////////////////////////

		static LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

		// Functions ///////////////////////////////////////////////////////

		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
		LRESULT OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam);
		LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
		LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		LRESULT OnCtlColor(WPARAM wParam, LPARAM lParam, UINT message);
		LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
		bool AddMessages();

		// Init Functions //////////////////////////////////////////////////

		HWND InitHandle(HWND window, const std::wstring &name, const std::wstring &title, DWORD style, DWORD exStyle) const;

		// Variables ///////////////////////////////////////////////////////

		HWND handle;
		const Window *window;
		int x;
		int y;
		int width;
		int height;
		int clientWidth;
		int clientHeight;
		int minWidth;
		int minHeight;
		std::wstring name;
		std::wstring title;
		DWORD style;
		DWORD exStyle;
		Dpi dpi;
		Icon bigIcon;
		Icon smallIcon;
		Theme theme;
		bool close;
		int status;

		std::unordered_map<int, MessageFunction> preMessageList;
		std::unordered_map<int, MessageFunction> messageList;
		std::unordered_map<int, CommandFunction> commandList;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
