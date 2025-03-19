// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Constants ///////////////////////////////////////////////////////////////

const int UI::Clipboard::MaxTries = 20;
const int UI::Clipboard::Delay = 20;

// Functions ///////////////////////////////////////////////////////////////

UI::Clipboard::Clipboard(const Window &window) :
	window(window.GetHandle())
{
}

////////////////////////////////////////////////////////////////////////////

bool UI::Clipboard::GetText(std::wstring &text) const
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
		return false;

	if (!OpenClipboard())
		return false;

	HGLOBAL memory = GetClipboardData(CF_UNICODETEXT);

	if (!memory)
	{
		CloseClipboard();
		return false;
	}

	wchar_t *buffer = static_cast<wchar_t *>(GlobalLock(memory));

	if (!buffer)
	{
		CloseClipboard();
		return false;
	}

	text = buffer;
	GlobalUnlock(memory);
	CloseClipboard();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Clipboard::SetText(const std::wstring &text) const
{
	if (!OpenClipboard())
		return false;

	if (!EmptyClipboard())
		return false;

	SIZE_T size = (text.size() + 1) * sizeof(wchar_t);
	HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, size);

	if (!memory)
	{
		CloseClipboard();
		return false;
	}

	wchar_t *buffer = static_cast<wchar_t *>(GlobalLock(memory));

	if (!buffer)
	{
		GlobalFree(memory);
		CloseClipboard();
		return false;
	}

	std::memcpy(buffer, text.c_str(), size);
	GlobalUnlock(memory);

	if (!SetClipboardData(CF_UNICODETEXT, memory))
	{
		GlobalFree(memory);
		CloseClipboard();
		return false;
	}

	CloseClipboard();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Clipboard::OpenClipboard() const
{
	for (int tries = 1; !::OpenClipboard(window); ++tries)
	{
		if (tries >= MaxTries)
			return false;

		Sleep(Delay);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
