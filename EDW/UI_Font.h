#ifndef UI_FONT_H
#define UI_FONT_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class Font
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		Font();
		Font(const std::wstring &name, int size, int weight = FW_NORMAL, bool italic = false, bool underline = false, bool strikeout = false);
		Font(const Font &other);
		Font &operator=(Font other);
		~Font();

		HFONT GetHandle() const;
		int GetWidth(const std::wstring &text) const;
		int GetHeight() const;
		int GetAscent() const;
		int GetDescent() const;

		////////////////////////////////////////////////////////////////////

	private:

		////////////////////////////////////////////////////////////////////
		// Private /////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Init Functions //////////////////////////////////////////////////

		HFONT InitHandle() const;
		HFONT InitHandle(const std::wstring &name, int size, int weight, bool italic, bool underline, bool strikeout) const;
		TEXTMETRICW InitTextMetrics() const;

		// Variables ///////////////////////////////////////////////////////

		HFONT handle;
		TEXTMETRICW textMetrics;
		int *references;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
