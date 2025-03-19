#ifndef EDW_NVIDIALIBRARY_H
#define EDW_NVIDIALIBRARY_H

// Includes ////////////////////////////////////////////////////////////////

#include "EDW_Common.h"

// Typedefs ////////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Nvidia
	{
		typedef int NvAPI_Status;
		typedef unsigned char NvU8;
		typedef unsigned NvU32;
		typedef struct NvPhysicalGpuHandle__ { int unused; } *NvPhysicalGpuHandle;
	}
}

// Enums ///////////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Nvidia
	{
		enum NV_I2C_SPEED
		{
			NVAPI_I2C_SPEED_DEFAULT,
			NVAPI_I2C_SPEED_3KHZ,
			NVAPI_I2C_SPEED_10KHZ,
			NVAPI_I2C_SPEED_33KHZ,
			NVAPI_I2C_SPEED_100KHZ,
			NVAPI_I2C_SPEED_200KHZ,
			NVAPI_I2C_SPEED_400KHZ,
		};
	}
}

// Constants ///////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Nvidia
	{
		const NvAPI_Status NVAPI_OK = 0;
		const int NVAPI_MAX_PHYSICAL_GPUS = 64;
		const int NVAPI_MAX_HEADS_PER_GPU = 32;
		const int NV_EDID_DATA_SIZE = 256;
		const NvU32 NVAPI_I2C_SPEED_DEPRECATED = 0xFFFF;
	}
}

// Structs /////////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Nvidia
	{
		struct NV_EDID
		{
			NvU32 version;
			NvU8 EDID_Data[NV_EDID_DATA_SIZE];
			NvU32 sizeofEDID;
			NvU32 edidId;
			NvU32 offset;
		};

		const int NV_EDID_VER = sizeof(NV_EDID) | (3 << 16);

		struct NV_I2C_INFO
		{
			NvU32 version;
			NvU32 displayMask;
			NvU8 bIsDDCPort;
			NvU8 i2cDevAddress;
			NvU8 *pbI2cRegAddress;
			NvU32 regAddrSize;
			NvU8 *pbData;
			NvU32 cbSize;
			NvU32 i2cSpeed;
			NV_I2C_SPEED i2cSpeedKhz;
			//NvU8 portId;
			//NvU32 bIsPortIdSet;
		};

		const int NV_I2C_INFO_VER = sizeof(NV_I2C_INFO) | (2 << 16);
	}
}

// Classes /////////////////////////////////////////////////////////////////

namespace EDW
{
	namespace Nvidia
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

			NvAPI_Status NvAPI_EnumPhysicalGPUs(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount);
			NvAPI_Status NvAPI_GPU_GetConnectedOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pOutputsMask);
			NvAPI_Status NvAPI_GPU_GetEDID(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID *pEDID);
			NvAPI_Status NvAPI_I2CRead(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo);
			NvAPI_Status NvAPI_I2CWrite(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo);

			////////////////////////////////////////////////////////////////

		private:
			static std::unique_ptr<struct LibraryImpl> impl;
			static int references;
		};
	}
}

////////////////////////////////////////////////////////////////////////////

#endif
