// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Window::Window(const std::wstring &name, const std::wstring &title, DWORD style, DWORD exStyle) :
	handle(InitHandle(nullptr, name, title, style, exStyle)),
	window(nullptr),
	x(0),
	y(0),
	width(0),
	height(0),
	clientWidth(0),
	clientHeight(0),
	minWidth(0),
	minHeight(0),
	name(name),
	title(title),
	style(style),
	exStyle(exStyle),
	dpi(*this),
	theme(LightTheme(dpi)),
	close(false),
	status(0)
{
	AddMessages();
	SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

////////////////////////////////////////////////////////////////////////////

UI::Window::Window(const Window &window, const std::wstring &name, const std::wstring &title, DWORD style, DWORD exStyle) :
	handle(InitHandle(window.GetHandle(), name, title, style, exStyle)),
	window(&window),
	x(0),
	y(0),
	width(0),
	height(0),
	clientWidth(0),
	clientHeight(0),
	minWidth(0),
	minHeight(0),
	name(name),
	title(title),
	style(style),
	exStyle(exStyle),
	dpi(*this),
	theme(LightTheme(dpi)),
	close(false),
	status(0)
{
	AddMessages();
	SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

////////////////////////////////////////////////////////////////////////////

UI::Window::~Window()
{
	SetWindowLongPtrW(handle, GWLP_USERDATA, 0);
	DestroyWindow(handle);
	UnregisterClassW(name.c_str(), GetModuleHandleW(nullptr));
}

////////////////////////////////////////////////////////////////////////////

HWND UI::Window::GetHandle() const
{
	return handle;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetX() const
{
	return x;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetY() const
{
	return y;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetWidth() const
{
	return width;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetHeight() const
{
	return height;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetRight() const
{
	return x + width;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetBottom() const
{
	return y + height;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetClientWidth() const
{
	return clientWidth;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetClientHeight() const
{
	return clientHeight;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetMinWidth() const
{
	return minWidth;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::GetMinHeight() const
{
	return minHeight;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetPosition(int x, int y)
{
	return SetWindowPos(handle, nullptr, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetSize(int width, int height)
{
	return SetWindowPos(handle, nullptr, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetRightBottom(int right, int bottom)
{
	return SetSize(right - x, bottom - y);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetClientSize(int width, int height)
{
	RECT rect = {0, 0, width, height};

	if (!dpi.AdjustWindowRect(rect, style, exStyle, false))
		return false;

	return SetSize(rect.right - rect.left, rect.bottom - rect.top);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetMinSize(int minWidth, int minHeight)
{
	this->minWidth = minWidth;
	this->minHeight = minHeight;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetMinClientSize(int minWidth, int minHeight)
{
	RECT rect = {0, 0, minWidth, minHeight};

	if (!dpi.AdjustWindowRect(rect, style, exStyle, false))
		return false;

	return SetMinSize(rect.right - rect.left, rect.bottom - rect.top);
}

////////////////////////////////////////////////////////////////////////////

std::wstring UI::Window::GetName() const
{
	return name;
}

////////////////////////////////////////////////////////////////////////////

std::wstring UI::Window::GetTitle() const
{
	return title;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetTitle(const std::wstring &title)
{
	if (SendMessageW(handle, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(title.c_str())) != TRUE)
		return false;

	this->title = title;
	return true;
}

////////////////////////////////////////////////////////////////////////////

UI::Dpi UI::Window::GetDpi() const
{
	return dpi;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetIcon(const std::wstring &name, int maxDpi)
{
	Icon bigIcon(name, dpi.Get() <= maxDpi ? dpi.Scale(IsWindows10OrGreater() ? 24 : 32) : 256);
	SendMessageW(handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(bigIcon.GetHandle()));
	this->bigIcon = bigIcon;

	Icon smallIcon(name, dpi.Scale(16));
	SendMessageW(handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(smallIcon.GetHandle()));
	this->smallIcon = smallIcon;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Invalidate() const
{
	return RedrawWindow(handle, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE);
}

////////////////////////////////////////////////////////////////////////////

UI::Theme UI::Window::GetTheme() const
{
	return theme;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::SetTheme(const Theme &theme)
{
	SystemTheme systemTheme;
	systemTheme.SetDarkMode(*this, theme.IsDark);
	this->theme = theme;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::IsEnabled() const
{
	return IsWindowEnabled(handle);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Enable() const
{
	EnableWindow(handle, true);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Disable() const
{
	EnableWindow(handle, false);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Fit()
{
	RECT rect;

	if (!SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0))
		return false;

	int width = std::min<int>(this->width, rect.right - rect.left);
	int height = std::min<int>(this->height, rect.bottom - rect.top);
	return SetSize(width, height);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Center()
{
	RECT rect;

	if (!SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0))
		return false;

	int x = (rect.right - rect.left - width) / 2 + rect.left;
	int y = (rect.bottom - rect.top - height) / 2 + rect.top;
	return SetPosition(x, y);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Center(const Window &window)
{
	int x = (window.width - width) / 2 + window.x;
	int y = (window.height - height) / 2 + window.y;
	return SetPosition(x, y);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Show()
{
	ShowWindow(handle, SW_SHOW);
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Window::MessageLoop()
{
	if (window)
		window->Disable();

	close = false;

	while (!close)
	{
		MSG message;

		switch (GetMessageW(&message, nullptr, 0, 0))
		{
			case -1:
				return -1;

			case 0:
				return static_cast<int>(message.wParam);
		}

		if (!IsDialogMessageW(handle, &message))
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}

	if (window)
		window->Enable();

	return status;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::Close(int status)
{
	close = true;
	this->status = status;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::AddPreMessage(UINT message, MessageFunction function)
{
	preMessageList[message] = function;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::AddMessage(UINT message, MessageFunction function)
{
	messageList[message] = function;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::AddCommand(int command, CommandFunction function)
{
	commandList[command] = function;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &result)
{
	auto iterator = preMessageList.find(message);

	if (iterator != preMessageList.end())
		iterator->second(wParam, lParam, message);

	iterator = messageList.find(message);

	if (iterator == messageList.end())
		return false;

	result = iterator->second(wParam, lParam, message);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::ProcessCommand(int command)
{
	auto iterator = commandList.find(command);

	if (iterator == commandList.end())
		return false;

	iterator->second();
	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK UI::Window::WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window *window = reinterpret_cast<Window *>(GetWindowLongPtrW(handle, GWLP_USERDATA));

	if (window)
	{
		LRESULT result;

		if (window->ProcessMessage(message, wParam, lParam, result))
			return result;
	}

	return DefWindowProcW(handle, message, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(*this);
	RECT rect;
	GetClientRect(handle, &rect);
	dc.SetBrushColor(theme.WindowBackgroundColor);
	dc.FillRect(rect, dc.GetBrush());
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam)
{
	MINMAXINFO *info = reinterpret_cast<MINMAXINFO *>(lParam);
	info->ptMinTrackSize = {minWidth, minHeight};
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT *item = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);

	if (item->hwndItem)
	{
		Control *control = reinterpret_cast<Control *>(GetWindowLongPtrW(item->hwndItem, GWLP_USERDATA));

		if (control)
		{
			LRESULT result;

			if (control->ProcessMessage(WM_DRAWITEM, wParam, lParam, result))
				return result;
		}
	}

	return DefWindowProcW(handle, WM_DRAWITEM, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(handle, &rect);
	x = rect.left;
	y = rect.top;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	GetClientRect(handle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	return DefWindowProcW(handle, WM_WINDOWPOSCHANGED, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		Control *control = reinterpret_cast<Control *>(GetWindowLongPtrW(reinterpret_cast<HWND>(lParam), GWLP_USERDATA));

		if (control && control->ProcessCommand(HIWORD(wParam)))
			return 0;
	}
	else
	{
		if (ProcessCommand(LOWORD(wParam)))
			return 0;
	}

	return DefWindowProcW(handle, WM_COMMAND, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnCtlColor(WPARAM wParam, LPARAM lParam, UINT message)
{
	if (lParam)
	{
		Control *control = reinterpret_cast<Control *>(GetWindowLongPtrW(reinterpret_cast<HWND>(lParam), GWLP_USERDATA));

		if (control)
		{
			LRESULT result;

			if (control->ProcessMessage(message, wParam, lParam, result))
				return result;
		}
	}

	return DefWindowProcW(handle, message, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Window::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	dpi = Dpi(*this);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Window::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_PAINT, std::bind(&Window::OnPaint, this, _1, _2));
	AddMessage(WM_GETMINMAXINFO, std::bind(&Window::OnGetMinMaxInfo, this, _1, _2));
	AddMessage(WM_DRAWITEM, std::bind(&Window::OnDrawItem, this, _1, _2));
	AddPreMessage(WM_WINDOWPOSCHANGED, std::bind(&Window::OnWindowPosChanged, this, _1, _2));
	AddMessage(WM_COMMAND, std::bind(&Window::OnCommand, this, _1, _2));
	AddMessage(WM_CTLCOLOREDIT, std::bind(&Window::OnCtlColor, this, _1, _2, _3));
	AddMessage(WM_CTLCOLORLISTBOX, std::bind(&Window::OnCtlColor, this, _1, _2, _3));
	AddMessage(WM_CTLCOLORBTN, std::bind(&Window::OnCtlColor, this, _1, _2, _3));
	AddMessage(WM_CTLCOLORSTATIC, std::bind(&Window::OnCtlColor, this, _1, _2, _3));
	AddPreMessage(WM_DPICHANGED, std::bind(&Window::OnDpiChanged, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////

HWND UI::Window::InitHandle(HWND window, const std::wstring &name, const std::wstring &title, DWORD style, DWORD exStyle) const
{
	WNDCLASSEXW windowClass = {sizeof windowClass};
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = GetModuleHandleW(nullptr);
	windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	windowClass.lpszClassName = name.c_str();

	if (!RegisterClassExW(&windowClass))
		throw L"RegisterClassExW failed.";

	HWND handle = CreateWindowExW(exStyle, windowClass.lpszClassName, title.c_str(), style, 0, 0, 0, 0, window, nullptr, windowClass.hInstance, nullptr);

	if (!handle)
	{
		UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
		throw L"CreateWindowExW failed.";
	}

	return handle;
}

////////////////////////////////////////////////////////////////////////////
