// Includes ////////////////////////////////////////////////////////////////

#include "MainWindow.h"

// WinMain /////////////////////////////////////////////////////////////////

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ wchar_t *, _In_ int)
{
	try
	{
		EDW::DisplayList displayList;

		if (!displayList.Load())
			throw L"No compatible GPUs found.";

		if (displayList.GetCount() == 0)
			throw L"No compatible displays found.";

		MainWindow mainWindow(displayList);
		return mainWindow.MessageLoop();
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(nullptr, message, L"Error", MB_ICONERROR);
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////
