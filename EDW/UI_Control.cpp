// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Control::Control(const Window &window, const std::wstring &name, const std::wstring &text, DWORD style, DWORD exStyle) :
	handle(InitHandle(window, name, text, style, exStyle)),
	x(0),
	y(0),
	width(0),
	height(0),
	text(text),
	style(style),
	exStyle(exStyle),
	theme(window.GetTheme())
{
	OldWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc)));
	SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

////////////////////////////////////////////////////////////////////////////

UI::Control::~Control()
{
	SetWindowLongPtrW(handle, GWLP_USERDATA, 0);
	DestroyWindow(handle);
}

////////////////////////////////////////////////////////////////////////////

HWND UI::Control::GetHandle() const
{
	return handle;
}

////////////////////////////////////////////////////////////////////////////

int UI::Control::GetX() const
{
	return x;
}

////////////////////////////////////////////////////////////////////////////

int UI::Control::GetY() const
{
	return y;
}

////////////////////////////////////////////////////////////////////////////

int UI::Control::GetWidth() const
{
	return width;
}

////////////////////////////////////////////////////////////////////////////

int UI::Control::GetHeight() const
{
	return height;
}

////////////////////////////////////////////////////////////////////////////

int UI::Control::GetRight() const
{
	return x + width;
}

////////////////////////////////////////////////////////////////////////////

int UI::Control::GetBottom() const
{
	return y + height;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetPosition(int x, int y)
{
	if (!SetWindowPos(handle, nullptr, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE))
		return false;

	this->x = x;
	this->y = y;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetSize(int width, int height)
{
	if (!SetWindowPos(handle, nullptr, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE))
		return false;

	this->width = width;
	this->height = height;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetRightBottom(int right, int bottom)
{
	return SetSize(right - x, bottom - y);
}

////////////////////////////////////////////////////////////////////////////

std::wstring UI::Control::GetText() const
{
	return text;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetText(const std::wstring &text)
{
	if (SendMessageW(handle, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text.c_str())) != TRUE)
		return false;

	this->text = text;
	return true;
}

////////////////////////////////////////////////////////////////////////////

UI::Font UI::Control::GetFont() const
{
	return font;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetFont(const Font &font)
{
	SendMessageW(handle, WM_SETFONT, reinterpret_cast<WPARAM>(font.GetHandle()), true);
	this->font = font;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::Invalidate() const
{
	return RedrawWindow(handle, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE);
}

////////////////////////////////////////////////////////////////////////////

UI::Theme UI::Control::GetTheme() const
{
	return theme;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetTheme(const Theme &theme)
{
	SystemTheme systemTheme;
	systemTheme.SetDarkMode(*this, theme.IsDark);
	this->theme = theme;
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::IsEnabled() const
{
	return IsWindowEnabled(handle);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::Enable() const
{
	EnableWindow(handle, true);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::Disable() const
{
	EnableWindow(handle, false);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::IsFocused() const
{
	return GetFocus() == handle;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::IsFocusVisible() const
{
	return !(SendMessageW(handle, WM_QUERYUISTATE, 0, 0) & UISF_HIDEFOCUS);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::SetFocus()
{
	::SetFocus(handle);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::BeginUpdate()
{
	SendMessageW(handle, WM_SETREDRAW, false, 0);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::EndUpdate()
{
	SendMessageW(handle, WM_SETREDRAW, true, 0);
	return Invalidate();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::AddPreMessage(UINT message, MessageFunction function)
{
	preMessageList[message] = function;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::AddMessage(UINT message, MessageFunction function)
{
	messageList[message] = function;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::AddCommand(int command, CommandFunction function)
{
	commandList[command] = function;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Control::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &result)
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

bool UI::Control::ProcessCommand(int command)
{
	auto iterator = commandList.find(command);

	if (iterator == commandList.end())
		return false;

	iterator->second();
	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK UI::Control::WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Control *control = reinterpret_cast<Control *>(GetWindowLongPtrW(handle, GWLP_USERDATA));

	if (control)
	{
		LRESULT result;

		if (control->ProcessMessage(message, wParam, lParam, result))
			return result;

		return CallWindowProcW(control->OldWindowProc, handle, message, wParam, lParam);
	}

	return DefWindowProcW(handle, message, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

RECT UI::Control::DrawFillRect(const DC &dc, const RECT &rect, COLORREF textColor, COLORREF backgroundColor)
{
	dc.SetBrushColor(backgroundColor);
	dc.FillRect(rect, dc.GetBrush());
	dc.SetTextColor(textColor);
	dc.SetBackgroundColor(backgroundColor);
	return rect;
}

////////////////////////////////////////////////////////////////////////////

RECT UI::Control::DrawFrameRect(const DC &dc, const RECT &rect, COLORREF textColor, COLORREF backgroundColor, COLORREF borderColor, int borderSize)
{
	dc.SetBrushColor(borderColor);
	RECT innerRect = rect;
	InflateRect(&innerRect, -borderSize, -borderSize);
	dc.FrameRect(rect, innerRect, dc.GetBrush());
	dc.SetTextColor(textColor);
	dc.SetBackgroundColor(backgroundColor);
	return innerRect;
}

////////////////////////////////////////////////////////////////////////////

RECT UI::Control::DrawFrameFillRect(const DC &dc, const RECT &rect, COLORREF textColor, COLORREF backgroundColor, COLORREF borderColor, int borderSize)
{
	dc.SetBrushColor(borderColor);
	RECT innerRect = rect;
	InflateRect(&innerRect, -borderSize, -borderSize);
	dc.FrameRect(rect, innerRect, dc.GetBrush());
	dc.SetBrushColor(backgroundColor);
	dc.FillRect(innerRect, dc.GetBrush());
	dc.SetTextColor(textColor);
	dc.SetBackgroundColor(backgroundColor);
	return innerRect;
}

////////////////////////////////////////////////////////////////////////////

RECT UI::Control::DrawFrameFrameRect(const DC &dc, const RECT &outerRect, const RECT &innerRect, COLORREF textColor, COLORREF backgroundColor, COLORREF borderColor, int borderSize)
{
	dc.SetBrushColor(borderColor);
	RECT rect = outerRect;
	InflateRect(&rect, -borderSize, -borderSize);
	dc.FrameRect(outerRect, rect, dc.GetBrush());
	dc.SetBrushColor(backgroundColor);
	dc.FrameRect(rect, innerRect, dc.GetBrush());
	dc.SetTextColor(textColor);
	dc.SetBackgroundColor(backgroundColor);
	return innerRect;
}

////////////////////////////////////////////////////////////////////////////

HWND UI::Control::InitHandle(const Window &window, const std::wstring &name, const std::wstring &text, DWORD style, DWORD exStyle) const
{
	HWND handle = CreateWindowExW(exStyle, name.c_str(), text.c_str(), style | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, window.GetHandle(), nullptr, GetModuleHandleW(nullptr), nullptr);

	if (!handle)
		throw L"CreateWindowExW failed.";

	return handle;
}

////////////////////////////////////////////////////////////////////////////
