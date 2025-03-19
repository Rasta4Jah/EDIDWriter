#ifndef EDW_DISPLAY_H
#define EDW_DISPLAY_H

// Includes ////////////////////////////////////////////////////////////////

#include "EDW_DisplayData.h"
#include "EDW_NvidiaLibrary.h"
#include "EDW_Common.h"

// Enums ///////////////////////////////////////////////////////////////////

namespace EDW
{
	enum class GpuType
	{
		Amd,
		Nvidia
	};
}

// Classes /////////////////////////////////////////////////////////////////

namespace EDW
{
	class Display
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		Display();
		Display(const Display &other);
		Display &operator=(const Display &other);
		~Display();

		explicit operator bool() const;

		bool operator==(const Display &other) const;
		bool operator!=(const Display &other) const;
		bool operator<(const Display &other) const;
		bool operator>(const Display &other) const;
		bool operator<=(const Display &other) const;
		bool operator>=(const Display &other) const;

		static Display CreateAmd(int gpu, int output);
		static Display CreateNvidia(Nvidia::NvPhysicalGpuHandle gpu, Nvidia::NvU32 output);

		template <class T> bool GetId(T &id);
		template <class T> bool GetName(T &name);
		bool ReadEdid(DisplayData &displayData);
		bool ReadDisplayId(DisplayData &displayData);
		bool WriteEdid(const DisplayData &displayData, bool fast = true);
		bool WriteDisplayId(const DisplayData &displayData, bool fast = true);

		////////////////////////////////////////////////////////////////////

	private:
		std::unique_ptr<struct DisplayImpl> impl;
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
