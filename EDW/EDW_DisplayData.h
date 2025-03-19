#ifndef EDW_DISPLAYDATA_H
#define EDW_DISPLAYDATA_H

// Includes ////////////////////////////////////////////////////////////////

#include "EDW_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace EDW
{
	class DisplayData
	{
	public:
		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		DisplayData();
		DisplayData(const std::basic_string<unsigned char> &data);
		DisplayData(const DisplayData &other);
		DisplayData &operator=(const DisplayData &other);
		~DisplayData();

		explicit operator bool() const;
		bool operator==(const DisplayData &other) const;
		bool operator!=(const DisplayData &other) const;

		bool GetData(std::basic_string<unsigned char> &data) const;
		int GetSize() const;
		int GetOriginalSize() const;
		int GetReportedSize() const;
		template <class T> bool GetId(T &id) const;
		template <class T> bool GetName(T &name) const;
		template <class T> bool GetText(T &text, int columns = 0) const;
		template <class T> bool Load(const T &name);
		template <class T> bool SaveBin(const T &name) const;
		template <class T> bool SaveDat(const T &name) const;
		template <class T> bool SaveTxt(const T &name, int columns = 16) const;
		bool IsEdid() const;
		bool IsValidEdidHeader() const;
		bool FixEdidHeader();
		bool IsValidEdidExtensionBlocks() const;
		bool FixEdidExtensionBlocks();
		bool IsValidEdidChecksums() const;
		bool FixEdidChecksums();
		bool IsDisplayId() const;
		bool IsValidDisplayIdChecksums() const;
		bool FixDisplayIdChecksums();

		////////////////////////////////////////////////////////////////////

	private:
		std::unique_ptr<struct DisplayDataImpl> impl;
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
