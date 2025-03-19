#ifndef EDW_AMDLIBRARY_H
#define EDW_AMDLIBRARY_H

// Includes ////////////////////////////////////////////////////////////////

#include "EDW_Common.h"

// Constants ///////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Amd
	{
		const int ADL_OK = 0;
		const int ADL_MAX_PATH = 256;
		const int ADL_MAX_EDIDDATA_SIZE = 256;
	}
}

// Structs /////////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Amd
	{
		struct AdapterInfo
		{
			int iSize;
			int iAdapterIndex;
			char strUDID[ADL_MAX_PATH];
			int iBusNumber;
			int iDeviceNumber;
			int iFunctionNumber;
			int iVendorID;
			char strAdapterName[ADL_MAX_PATH];
			char strDisplayName[ADL_MAX_PATH];
			int iPresent;
			int iExist;
			char strDriverPath[ADL_MAX_PATH];
			char strDriverPathExt[ADL_MAX_PATH];
			char strPNPString[ADL_MAX_PATH];
			int iOSDisplayIndex;
		};

		struct ADLDisplayID
		{
			int iDisplayLogicalIndex;
			int iDisplayPhysicalIndex;
			int iDisplayLogicalAdapterIndex;
			int iDisplayPhysicalAdapterIndex;
		};

		struct ADLDisplayInfo
		{
			ADLDisplayID displayID;
			int iDisplayControllerIndex;
			char strDisplayName[ADL_MAX_PATH];
			char strDisplayManufacturerName[ADL_MAX_PATH];
			int iDisplayType;
			int iDisplayOutputType;
			int iDisplayConnector;
			int iDisplayInfoMask;
			int iDisplayInfoValue;
		};

		struct ADLDisplayEDIDData
		{
			int iSize;
			int iFlag;
			int iEDIDSize;
			int iBlockIndex;
			char cEDIDData[ADL_MAX_EDIDDATA_SIZE];
			int iReserved[4];
		};
	}
}

// Classes /////////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Amd
	{
		class Library
		{
		public:

			////////////////////////////////////////////////////////////////
			// Public //////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////

			// Functions ///////////////////////////////////////////////////

			Library();
			Library(const Library &other);
			~Library();

			explicit operator bool() const;

			int ADL_Adapter_NumberOfAdapters_Get(int *lpNumAdapters);
			int ADL_Adapter_AdapterInfo_Get(AdapterInfo *lpInfo, int iInputSize);
			int ADL_Display_DisplayInfo_Get(int iAdapterIndex, int *lpNumDisplays, ADLDisplayInfo **lppInfo, int iForceDetect);
			int ADL_Display_EdidData_Get(int iAdapterIndex, int iDisplayIndex, ADLDisplayEDIDData *lpEDIDData);
			int ADL_Display_DDCBlockAccess_Get(int iAdapterIndex, int iDisplayIndex, int iOption, int iCommandIndex, int iSendMsgLen, const unsigned char *lpucSendMsgBuf, int *lpulRecvMsgLen, unsigned char *lpucRecvMsgBuf);

			////////////////////////////////////////////////////////////////

		private:
			static std::unique_ptr<struct LibraryImpl> impl;
			static int references;
		};
	}
}

////////////////////////////////////////////////////////////////////////////

#endif
