#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Includes ////////////////////////////////////////////////////////////////

#include "EDW.h"
#include "UI.h"

// Classes /////////////////////////////////////////////////////////////////

class MainWindow : public UI::Window
{
public:

	////////////////////////////////////////////////////////////////////////
	// Public //////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	// Functions ///////////////////////////////////////////////////////////

	MainWindow(const EDW::DisplayList &displayList);

	////////////////////////////////////////////////////////////////////////

private:

	////////////////////////////////////////////////////////////////////////
	// Private /////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	// Constants ///////////////////////////////////////////////////////////

	static const std::wstring Name;
	static const std::wstring Title;

	static const int DataTextBoxColumns;
	static const int DataTextBoxRows;

	static const std::wstring DefaultExtension;
	static const COMDLG_FILTERSPEC LoadFileTypes[];
	static const COMDLG_FILTERSPEC SaveFileTypes[];

	// Functions ///////////////////////////////////////////////////////////

	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnClose(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysColorChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnSettingChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnThemeChanged(WPARAM wParam, LPARAM lParam);
	bool AddMessages();
	bool RefreshGpuComboBox();
	bool RefreshDataTextBox();
	bool EnableButtons();
	bool DisableButtons();
	bool ProcessEdid(const std::wstring &title);
	bool ProcessDisplayId(const std::wstring &title);
	bool ProcessDisplayData(EDW::DisplayData &displayData, const std::wstring &title);
	void GpuComboBoxChange();
	void ReadEdidButtonClick();
	void ReadDisplayIdButtonClick();
	void SaveFileButtonClick();
	void LoadFileButtonClick();
	void WriteEdidButtonClick();
	void WriteDisplayIdButtonClick();
	void CloseButtonClick();
	void EnterKeyPress();
	void EscKeyPress();
	bool AddCommands();
	bool InitializeTheme();
	bool InitializeControls();
	bool ScaleControls(bool resize = false);

	// Variables ///////////////////////////////////////////////////////////

	UI::Controls::Label displayLabel;
	UI::Controls::ComboBox displayComboBox;
	UI::Controls::GroupBox dataGroupBox;
	UI::Controls::Button readEdidButton;
	UI::Controls::Button readDisplayIdButton;
	UI::Controls::Button saveFileButton;
	UI::Controls::TextBox dataTextBox;
	UI::Controls::Button loadFileButton;
	UI::Controls::Button writeEdidButton;
	UI::Controls::Button writeDisplayIdButton;
	UI::Controls::Button closeButton;

	EDW::DisplayList displayList;
	int displayIndex;
	EDW::Display display;
	EDW::DisplayData displayData;

	////////////////////////////////////////////////////////////////////////
};

////////////////////////////////////////////////////////////////////////////

#endif
