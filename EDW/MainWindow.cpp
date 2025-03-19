// Includes ////////////////////////////////////////////////////////////////

#include "MainWindow.h"

// Constants ///////////////////////////////////////////////////////////////

const std::wstring MainWindow::Name = L"MainWindow";
const std::wstring MainWindow::Title = L"EDID/DisplayID Writer Beta by ToastyX";

const int MainWindow::DataTextBoxColumns = 16;
const int MainWindow::DataTextBoxRows = 16;

const std::wstring MainWindow::DefaultExtension = L"bin";

const COMDLG_FILTERSPEC MainWindow::LoadFileTypes[] =
{
	{ L"BIN/DAT Files", L"*.bin;*.dat" },
	{ L"TXT Files", L"*.txt" },
	{ L"All Files", L"*.*" }
};

const COMDLG_FILTERSPEC MainWindow::SaveFileTypes[] =
{
	{ L"BIN Files", L"*.bin" },
	{ L"DAT Files", L"*.dat" },
	{ L"TXT Files", L"*.txt" }
};

// Functions ///////////////////////////////////////////////////////////////

MainWindow::MainWindow(const EDW::DisplayList &displayList) :
	Window(Name, Title, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
	displayLabel(*this, L"Display:"),
	displayComboBox(*this),
	dataGroupBox(*this, L" Data "),
	readEdidButton(*this, L"Read EDID"),
	readDisplayIdButton(*this, L"Read DisplayID"),
	saveFileButton(*this, L"Save file..."),
	dataTextBox(*this, ES_READONLY | UI::Controls::TextBox::MultiLineStyle),
	loadFileButton(*this, L"Load file..."),
	writeEdidButton(*this, L"Write EDID"),
	writeDisplayIdButton(*this, L"Write DisplayID"),
	closeButton(*this, L"Close"),
	displayList(displayList),
	displayIndex(-1)
{
	AddMessages();
	AddCommands();
	InitializeTheme();
	InitializeControls();
	ScaleControls(true);
	Center();
	Show();
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnSize(WPARAM wParam, LPARAM lParam)
{
	ScaleControls();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnClose(WPARAM wParam, LPARAM lParam)
{
	CloseButtonClick();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnSysColorChange(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnSettingChange(WPARAM wParam, LPARAM lParam)
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

LRESULT MainWindow::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	ScaleControls(true);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SIZE, std::bind(&MainWindow::OnSize, this, _1, _2));
	AddMessage(WM_CLOSE, std::bind(&MainWindow::OnClose, this, _1, _2));
	AddMessage(WM_SYSCOLORCHANGE, std::bind(&MainWindow::OnSysColorChange, this, _1, _2));
	AddMessage(WM_SETTINGCHANGE, std::bind(&MainWindow::OnSettingChange, this, _1, _2));
	AddMessage(WM_DPICHANGED, std::bind(&MainWindow::OnDpiChanged, this, _1, _2));
	AddMessage(WM_THEMECHANGED, std::bind(&MainWindow::OnThemeChanged, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshGpuComboBox()
{
	EDW::Display display;
	displayComboBox.BeginUpdate();
	displayComboBox.Clear();

	for (int index = 0; displayList.Get(index, display); ++index)
	{
		std::wstring id;
		display.GetId(id);

		std::wstring name;
		display.GetName(name);

		if (name.empty())
			name = L"Unknown Display";

		if (id.empty())
			displayComboBox.Add(name);
		else
			displayComboBox.Add(id + L" - " + name);
	}

	displayComboBox.SetIndex(displayIndex);
	displayComboBox.EndUpdate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshDataTextBox()
{
	std::wstring text;
	displayData.GetText(text, DataTextBoxColumns);
	dataTextBox.SetText(text);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::EnableButtons()
{
	saveFileButton.Enable();
	writeEdidButton.Enable();
	writeDisplayIdButton.Enable();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::DisableButtons()
{
	saveFileButton.Disable();
	writeEdidButton.Disable();
	writeDisplayIdButton.Disable();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::ProcessEdid(const std::wstring &title)
{
	if (!displayData.IsValidEdidHeader())
	{
		if (MessageBoxW(GetHandle(), L"EDID header is corrupted. Fix?", title.c_str(), MB_ICONWARNING | MB_YESNO) != IDYES)
			return false;

		displayData.FixEdidHeader();
		RefreshDataTextBox();
	}

	if (!displayData.IsValidEdidExtensionBlocks())
	{
		if (MessageBoxW(GetHandle(), L"EDID extension block is invalid. Remove?", title.c_str(), MB_ICONWARNING | MB_YESNO) == IDYES)
		{
			displayData.FixEdidExtensionBlocks();
			RefreshDataTextBox();
		}
	}

	if (!displayData.IsValidEdidChecksums())
	{
		if (MessageBoxW(GetHandle(), L"EDID checksum is invalid. Fix?", title.c_str(), MB_ICONWARNING | MB_YESNO) == IDYES)
		{
			displayData.FixEdidChecksums();
			RefreshDataTextBox();
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::ProcessDisplayId(const std::wstring &title)
{
	if (!displayData.IsValidDisplayIdChecksums())
	{
		if (MessageBoxW(GetHandle(), L"DisplayID checksum is invalid. Fix?", title.c_str(), MB_ICONWARNING | MB_YESNO) == IDYES)
		{
			displayData.FixDisplayIdChecksums();
			RefreshDataTextBox();
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::ProcessDisplayData(EDW::DisplayData &displayData, const std::wstring &title)
{
	bool isEdid = displayData.IsEdid();
	bool isDisplayId = displayData.IsDisplayId();

	if (!isEdid && !isDisplayId)
	{
		if (MessageBoxW(GetHandle(), L"File does not contain a valid EDID or DisplayID. Load anyway?", title.c_str(), MB_ICONWARNING | MB_YESNO) != IDYES)
			return false;

		this->displayData = displayData;
		RefreshDataTextBox();
		EnableButtons();
		return true;
	}

	std::wstring id1;
	displayData.GetId(id1);

	std::wstring id2;
	display.GetId(id2);

	if (id1.size() == id2.size() && id1 != id2)
	{
		if (MessageBoxW(GetHandle(), L"Product ID does not match display. Load anyway?", title.c_str(), MB_ICONWARNING | MB_YESNO) != IDYES)
			return false;
	}

	this->displayData = displayData;
	RefreshDataTextBox();
	EnableButtons();

	if (displayData.GetSize() < displayData.GetOriginalSize())
	{
		std::wstring size = std::to_wstring(displayData.GetSize());
		std::wstring message = L"File contains more than " + size + L" bytes.\nOnly the first " + size + L" bytes were loaded.";
		MessageBoxW(GetHandle(), message.c_str(), title.c_str(), MB_ICONWARNING);
	}

	if (isEdid)
		ProcessEdid(title);
	else if (isDisplayId)
		ProcessDisplayId(title);

	return true;
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::GpuComboBoxChange()
{
	displayIndex = displayComboBox.GetIndex();

	if (!displayList.Get(displayIndex, display))
		return;
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::ReadEdidButtonClick()
{
	try
	{
		if (!display.ReadEdid(displayData))
			throw L"Failed to communicate with display.";

		RefreshDataTextBox();
		EnableButtons();

		if (displayData.GetSize() < displayData.GetReportedSize())
		{
			std::wstring size = std::to_wstring(displayData.GetSize());
			std::wstring message = L"Data contains more than " + size + L" bytes.\nOnly the first " + size + L" bytes were read.";
			MessageBoxW(GetHandle(), message.c_str(), L"Read EDID", MB_ICONWARNING);
		}

		if (displayData.IsEdid())
			ProcessEdid(L"Read EDID");
		else if (displayData.IsDisplayId())
			ProcessDisplayId(L"Read EDID");
		else
			MessageBoxW(GetHandle(), L"Data is not a valid EDID or DisplayID.", L"Read EDID", MB_ICONWARNING);
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Read EDID", MB_ICONERROR);
	}
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::ReadDisplayIdButtonClick()
{
	try
	{
		if (!display.ReadDisplayId(displayData))
			throw L"Failed to communicate with display.";

		RefreshDataTextBox();
		EnableButtons();

		if (displayData.GetSize() < displayData.GetReportedSize())
		{
			std::wstring size = std::to_wstring(displayData.GetSize());
			std::wstring message = L"Data contains more than " + size + L" bytes.\nOnly the first " + size + L" bytes were read.";
			MessageBoxW(GetHandle(), message.c_str(), L"Read DisplayID", MB_ICONWARNING);
		}

		if (displayData.IsDisplayId())
			ProcessDisplayId(L"Read DisplayID");
		else
			MessageBoxW(GetHandle(), L"Data is not a valid DisplayID.", L"Read DisplayID", MB_ICONWARNING);
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Read DisplayID", MB_ICONERROR);
	}
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::SaveFileButtonClick()
{
	bool initialized = false;
	IFileSaveDialog *saveDialog = nullptr;
	IShellItem *result = nullptr;
	wchar_t *file = nullptr;

	try
	{
		if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
			throw L"CoInitializeEx failed.";

		initialized = true;

		if (!SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&saveDialog))))
			throw L"CoCreateInstance failed.";

		if (!SUCCEEDED(saveDialog->SetTitle(L"Save")))
			throw L"IFileSaveDialog::SetTitle failed.";

		if (!SUCCEEDED(saveDialog->SetOkButtonLabel(L"Save")))
			throw L"IFileSaveDialog::SetOkButtonLabel failed.";

		if (!SUCCEEDED(saveDialog->SetDefaultExtension(DefaultExtension.c_str())))
			throw L"IFileSaveDialog::SetDefaultExtension failed.";

		if (!SUCCEEDED(saveDialog->SetFileTypes(ARRAYSIZE(SaveFileTypes), SaveFileTypes)))
			throw L"IFileSaveDialog::SetFileTypes failed.";

		if (SUCCEEDED(saveDialog->Show(GetHandle())))
		{
			if (!SUCCEEDED(saveDialog->GetResult(&result)))
				throw L"IFileSaveDialog::GetResult failed.";

			if (!SUCCEEDED(result->GetDisplayName(SIGDN_FILESYSPATH, &file)))
				throw L"IShellItem::GetDisplayName failed.";

			UINT format;

			if (!SUCCEEDED(saveDialog->GetFileTypeIndex(&format)))
				throw L"IFileSaveDialog::GetFileTypeIndex failed.";

			bool status;

			switch (format)
			{
				case 1:
					status = displayData.SaveBin<std::wstring>(file);
					break;

				case 2:
					status = displayData.SaveDat<std::wstring>(file);
					break;

				case 3:
					status = displayData.SaveTxt<std::wstring>(file);
					break;

				default:
					status = false;
					break;
			}

			if (!status)
				throw L"Failed to save file.";
		}
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Save file", MB_ICONERROR);
	}

	if (file)
		CoTaskMemFree(file);

	if (result)
		result->Release();

	if (saveDialog)
		saveDialog->Release();

	if (initialized)
		CoUninitialize();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::LoadFileButtonClick()
{
	bool initialized = false;
	IFileOpenDialog *openDialog = nullptr;
	IShellItem *result = nullptr;
	wchar_t *file = nullptr;

	try
	{
		if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
			throw L"CoInitializeEx failed.";

		initialized = true;

		if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&openDialog))))
			throw L"CoCreateInstance failed.";

		if (!SUCCEEDED(openDialog->SetTitle(L"Load")))
			throw L"IFileSaveDialog::SetTitle failed.";

		if (!SUCCEEDED(openDialog->SetOkButtonLabel(L"Load")))
			throw L"IFileSaveDialog::SetOkButtonLabel failed.";

		if (!SUCCEEDED(openDialog->SetDefaultExtension(DefaultExtension.c_str())))
			throw L"IFileSaveDialog::SetDefaultExtension failed.";

		if (!SUCCEEDED(openDialog->SetFileTypes(ARRAYSIZE(LoadFileTypes), LoadFileTypes)))
			throw L"IFileSaveDialog::SetFileTypes failed.";

		if (SUCCEEDED(openDialog->Show(GetHandle())))
		{
			if (!SUCCEEDED(openDialog->GetResult(&result)))
				throw L"IFileSaveDialog::GetResult failed.";

			if (!SUCCEEDED(result->GetDisplayName(SIGDN_FILESYSPATH, &file)))
				throw L"IShellItem::GetDisplayName failed.";

			EDW::DisplayData displayData;

			if (!displayData.Load<std::wstring>(file))
				throw L"Failed to load file.";

			if (displayData.GetSize() <= 0)
				throw L"File is empty.";

			ProcessDisplayData(displayData, L"Load file");
		}
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Load file", MB_ICONERROR);
	}

	if (file)
		CoTaskMemFree(file);

	if (result)
		result->Release();

	if (openDialog)
		openDialog->Release();

	if (initialized)
		CoUninitialize();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::WriteEdidButtonClick()
{
	try
	{
		EDW::DisplayData oldDisplayData;

		if (!display.ReadEdid(oldDisplayData))
			throw L"Failed to communicate with display.";

		if (displayData == oldDisplayData)
		{
			MessageBoxW(GetHandle(), L"Data already matches display.", L"Write EDID", 0);
			return;
		}

		std::wstring message;

		if (displayData.IsEdid())
		{
			if (!displayData.IsValidEdidChecksums())
				message = L"EDID checksum is invalid.\n";
		}
		else if (displayData.IsDisplayId())
		{
			if (!displayData.IsValidDisplayIdChecksums())
				message = L"DisplayID checksum is invalid.\n";
		}
		else
		{
			message = L"Data is not a valid EDID or DisplayID.\n";
		}

		int icon = 0;

		if (!message.empty())
			icon = MB_ICONWARNING;

		message += L"Are you sure you want to write the data to the display?";

		if (MessageBoxW(GetHandle(), message.c_str(), L"Write EDID", icon | MB_YESNO) != IDYES)
			return;

		SetCursor(LoadCursorW(nullptr, IDC_WAIT));

		if (!display.WriteEdid(displayData))
			throw L"Failed to write data.";

		EDW::DisplayData newDisplayData;

		if (!display.ReadEdid(newDisplayData))
			throw L"Failed to read data.";

		if (newDisplayData == oldDisplayData)
			throw L"Display is write-protected.";

		if (newDisplayData != displayData)
		{
			if (!display.WriteEdid(displayData, false))
				throw L"Failed to write data.";

			if (!display.ReadEdid(newDisplayData))
				throw L"Failed to read data.";

			if (newDisplayData != displayData)
				throw L"Failed to verify data.";
		}

		displayList.Set(displayIndex, display);
		RefreshGpuComboBox();
		MessageBoxW(GetHandle(), L"Data written successfully.", L"Write EDID", 0);
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Write EDID", MB_ICONERROR);
	}
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::WriteDisplayIdButtonClick()
{
	try
	{
		EDW::DisplayData oldDisplayData;

		if (!display.ReadDisplayId(oldDisplayData))
			throw L"Failed to communicate with display.";

		if (displayData == oldDisplayData)
		{
			MessageBoxW(GetHandle(), L"Data already matches display.", L"Write DisplayID", 0);
			return;
		}

		std::wstring message;

		if (displayData.IsDisplayId())
		{
			if (!displayData.IsValidDisplayIdChecksums())
				message = L"DisplayID checksum is invalid.\n";
		}
		else
		{
			message = L"Data is not a valid DisplayID.\n";
		}

		int icon = 0;

		if (!message.empty())
			icon = MB_ICONWARNING;

		message += L"Are you sure you want to write the data to the display?";

		if (MessageBoxW(GetHandle(), message.c_str(), L"Write DisplayID", icon | MB_YESNO) != IDYES)
			return;

		SetCursor(LoadCursorW(nullptr, IDC_WAIT));

		if (!display.WriteDisplayId(displayData))
			throw L"Failed to write data.";

		EDW::DisplayData newDisplayData;

		if (!display.ReadDisplayId(newDisplayData))
			throw L"Failed to read data.";

		if (newDisplayData == oldDisplayData)
			throw L"Display is write-protected.";

		if (newDisplayData != displayData)
		{
			if (!display.WriteDisplayId(displayData, false))
				throw L"Failed to write data.";

			if (!display.ReadDisplayId(newDisplayData))
				throw L"Failed to read data.";

			if (newDisplayData != displayData)
				throw L"Failed to verify data.";
		}

		MessageBoxW(GetHandle(), L"Data written successfully.", L"Write DisplayID", 0);
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Write DisplayID", MB_ICONERROR);
	}
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::CloseButtonClick()
{
	Close(0);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::EnterKeyPress()
{
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::EscKeyPress()
{
	CloseButtonClick();
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::AddCommands()
{
	displayComboBox.AddCommand(CBN_SELCHANGE, std::bind(&MainWindow::GpuComboBoxChange, this));
	readEdidButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::ReadEdidButtonClick, this));
	readDisplayIdButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::ReadDisplayIdButtonClick, this));
	saveFileButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::SaveFileButtonClick, this));
	loadFileButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::LoadFileButtonClick, this));
	writeEdidButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::WriteEdidButtonClick, this));
	writeDisplayIdButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::WriteDisplayIdButtonClick, this));
	closeButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::CloseButtonClick, this));
	AddCommand(IDOK, std::bind(&MainWindow::EnterKeyPress, this));
	AddCommand(IDCANCEL, std::bind(&MainWindow::EscKeyPress, this));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::InitializeTheme()
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
	displayLabel.SetTheme(theme);
	displayComboBox.SetTheme(theme);
	dataGroupBox.SetTheme(theme);
	readEdidButton.SetTheme(theme);
	readDisplayIdButton.SetTheme(theme);
	saveFileButton.SetTheme(theme);
	dataTextBox.SetTheme(theme);
	loadFileButton.SetTheme(theme);
	writeEdidButton.SetTheme(theme);
	writeDisplayIdButton.SetTheme(theme);
	closeButton.SetTheme(theme);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::InitializeControls()
{
	RefreshGpuComboBox();
	dataTextBox.SetText(L"No data");
	DisableButtons();
	displayComboBox.SetIndex(0);
	GpuComboBoxChange();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::ScaleControls(bool resize)
{
	UI::Dpi dpi = GetDpi();
	UI::Theme theme = GetTheme();
	UI::Font font(theme.FontName, theme.FontSize);
	UI::Font fixedFont(L"Consolas", theme.FontSize);

	const int DataTextBoxWidth = theme.Padding + fixedFont.GetWidth(L"0") * (DataTextBoxColumns * 3 - 1) + theme.Padding;
	const int DataTextBoxHeight = theme.Padding + fixedFont.GetHeight() * DataTextBoxRows + theme.Padding;

	const int ReadButtonPadding = (DataTextBoxWidth - font.GetWidth(readEdidButton.GetText()) - theme.LineSpacing - font.GetWidth(readDisplayIdButton.GetText()) - theme.LineSpacing - font.GetWidth(saveFileButton.GetText())) / 6;
	const int ReadEdidButtonWidth = ReadButtonPadding + font.GetWidth(readEdidButton.GetText()) + ReadButtonPadding;
	const int ReadDisplayIdButtonWidth = ReadButtonPadding + font.GetWidth(readDisplayIdButton.GetText()) + ReadButtonPadding;
	const int SaveFileButtonWidth = DataTextBoxWidth - ReadEdidButtonWidth - theme.LineSpacing - ReadDisplayIdButtonWidth - theme.LineSpacing;

	const int WriteButtonPadding = (DataTextBoxWidth - font.GetWidth(loadFileButton.GetText()) - theme.LineSpacing - font.GetWidth(writeEdidButton.GetText()) - theme.LineSpacing - font.GetWidth(writeDisplayIdButton.GetText())) / 6;
	const int WriteEdidButtonWidth = WriteButtonPadding + font.GetWidth(writeEdidButton.GetText()) + WriteButtonPadding;
	const int WriteDisplayIdButtonWidth = WriteButtonPadding + font.GetWidth(writeDisplayIdButton.GetText()) + WriteButtonPadding;
	const int LoadFileButtonWidth = DataTextBoxWidth - theme.LineSpacing - WriteEdidButtonWidth - theme.LineSpacing - WriteDisplayIdButtonWidth;

	SetIcon(L"ICON", 192);

	displayLabel.SetFont(font);
	displayLabel.SetPosition(theme.Spacing, theme.Spacing);
	displayLabel.SetSize(font.GetWidth(displayLabel.GetText()), theme.LineHeight);

	displayComboBox.SetFont(font);
	displayComboBox.SetPosition(displayLabel.GetRight() + theme.LabelSpacing, displayLabel.GetY());
	displayComboBox.SetRightBottom(GetClientWidth() - theme.Spacing, displayComboBox.GetY() + theme.LineHeight, GetClientHeight() - theme.Spacing);

	dataGroupBox.SetFont(font);
	dataGroupBox.SetPosition(displayLabel.GetX(), displayComboBox.GetY() + theme.LineHeight + theme.Spacing + theme.Spacing);
	{
		readEdidButton.SetFont(font);
		readEdidButton.SetPosition(dataGroupBox.GetX() + theme.GroupBoxBorderSizes.BorderSize + theme.Spacing, dataGroupBox.GetY() + theme.GroupBoxBorderSizes.BorderSize + theme.Spacing + theme.Spacing);
		readEdidButton.SetSize(ReadEdidButtonWidth, theme.LineHeight);

		readDisplayIdButton.SetFont(font);
		readDisplayIdButton.SetPosition(readEdidButton.GetRight() + theme.LineSpacing, readEdidButton.GetY());
		readDisplayIdButton.SetSize(ReadDisplayIdButtonWidth, theme.LineHeight);

		saveFileButton.SetFont(font);
		saveFileButton.SetPosition(readDisplayIdButton.GetRight() + theme.LineSpacing, readDisplayIdButton.GetY());
		saveFileButton.SetSize(SaveFileButtonWidth, theme.LineHeight);

		dataTextBox.SetFont(fixedFont);
		dataTextBox.SetPosition(readEdidButton.GetX(), readEdidButton.GetBottom() + theme.Spacing);
		dataTextBox.SetSize(DataTextBoxWidth, DataTextBoxHeight);

		loadFileButton.SetFont(font);
		loadFileButton.SetPosition(dataTextBox.GetX(), dataTextBox.GetBottom() + theme.Spacing);
		loadFileButton.SetSize(LoadFileButtonWidth, theme.LineHeight);

		writeEdidButton.SetFont(font);
		writeEdidButton.SetPosition(loadFileButton.GetRight() + theme.LineSpacing, loadFileButton.GetY());
		writeEdidButton.SetSize(WriteEdidButtonWidth, theme.LineHeight);

		writeDisplayIdButton.SetFont(font);
		writeDisplayIdButton.SetPosition(writeEdidButton.GetRight() + theme.LineSpacing, writeEdidButton.GetY());
		writeDisplayIdButton.SetSize(WriteDisplayIdButtonWidth, theme.LineHeight);
	}
	dataGroupBox.SetRightBottom(writeDisplayIdButton.GetRight() + theme.Spacing + theme.GroupBoxBorderSizes.BorderSize, writeDisplayIdButton.GetBottom() + theme.Spacing + theme.GroupBoxBorderSizes.BorderSize);

	closeButton.SetFont(font);
	closeButton.SetPosition(dataGroupBox.GetRight() - theme.ButtonWidth, dataGroupBox.GetBottom() + theme.Spacing);
	closeButton.SetSize(theme.ButtonWidth, theme.ButtonHeight);

	if (resize)
		SetClientSize(closeButton.GetRight() + theme.Spacing, closeButton.GetBottom() + theme.Spacing);

	return true;
}

////////////////////////////////////////////////////////////////////////////
