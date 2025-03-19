// Includes ////////////////////////////////////////////////////////////////

#include "EDW.h"

// Implementation //////////////////////////////////////////////////////////

namespace EDW
{
	namespace Nvidia
	{
		struct LibraryImpl
		{
			// Typedefs ////////////////////////////////////////////////////

			typedef void *(*NvAPI_QueryInterface_Function)(NvU32);
			typedef NvAPI_Status (*NvAPI_Initialize_Function)();
			typedef NvAPI_Status (*NvAPI_EnumPhysicalGPUs_Function)(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount);
			typedef NvAPI_Status (*NvAPI_GPU_GetConnectedOutputs_Function)(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pOutputsMask);
			typedef NvAPI_Status (*NvAPI_GPU_GetEDID_Function)(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID *pEDID);
			typedef NvAPI_Status (*NvAPI_I2CRead_Function)(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo);
			typedef NvAPI_Status (*NvAPI_I2CWrite_Function)(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo);
			typedef NvAPI_Status (*NvAPI_Unload_Function)();

			// Functions ///////////////////////////////////////////////////

			LibraryImpl();
			~LibraryImpl();

			static std::unique_ptr<LibraryImpl> Create();
			template <class T>
			bool GetFunction(T &function, const char *name);
			template <class T>
			bool GetFunction(T &function, NvU32 id);
			bool Load();

			// Variables ///////////////////////////////////////////////////

			HMODULE library;
			NvAPI_QueryInterface_Function NvAPI_QueryInterface;
			NvAPI_Initialize_Function NvAPI_Initialize;
			NvAPI_EnumPhysicalGPUs_Function NvAPI_EnumPhysicalGPUs;
			NvAPI_GPU_GetConnectedOutputs_Function NvAPI_GPU_GetConnectedOutputs;
			NvAPI_GPU_GetEDID_Function NvAPI_GPU_GetEDID;
			NvAPI_I2CRead_Function NvAPI_I2CRead;
			NvAPI_I2CWrite_Function NvAPI_I2CWrite;
			NvAPI_Unload_Function NvAPI_Unload;
			bool loaded;

			////////////////////////////////////////////////////////////////
		};
	}
}

// Static Variables ////////////////////////////////////////////////////////

std::unique_ptr<EDW::Nvidia::LibraryImpl> EDW::Nvidia::Library::impl = nullptr;
int EDW::Nvidia::Library::references = 0;

// Functions ///////////////////////////////////////////////////////////////

EDW::Nvidia::Library::Library()
{
	if (!impl)
		impl = LibraryImpl::Create();

	++references;
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::Library::Library(const Library &other)
{
	++references;
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::Library::~Library()
{
	--references;

	if (references <= 0)
		impl.reset();
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::Library::operator bool() const
{
	return impl.get();
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::NvAPI_Status EDW::Nvidia::Library::NvAPI_EnumPhysicalGPUs(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount)
{
	return impl->NvAPI_EnumPhysicalGPUs(nvGPUHandle, pGpuCount);
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::NvAPI_Status EDW::Nvidia::Library::NvAPI_GPU_GetConnectedOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pOutputsMask)
{
	return impl->NvAPI_GPU_GetConnectedOutputs(hPhysicalGpu, pOutputsMask);
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::NvAPI_Status EDW::Nvidia::Library::NvAPI_GPU_GetEDID(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID *pEDID)
{
	return impl->NvAPI_GPU_GetEDID(hPhysicalGpu, displayOutputId, pEDID);
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::NvAPI_Status EDW::Nvidia::Library::NvAPI_I2CRead(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo)
{
	return impl->NvAPI_I2CRead(hPhysicalGpu, pI2cInfo);
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::NvAPI_Status EDW::Nvidia::Library::NvAPI_I2CWrite(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo)
{
	return impl->NvAPI_I2CWrite(hPhysicalGpu, pI2cInfo);
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::LibraryImpl::LibraryImpl() :
	library(nullptr),
	loaded(false)
{
}

////////////////////////////////////////////////////////////////////////////

EDW::Nvidia::LibraryImpl::~LibraryImpl()
{
	if (loaded)
		NvAPI_Unload();

	if (library)
		FreeLibrary(library);
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::Nvidia::LibraryImpl> EDW::Nvidia::LibraryImpl::Create()
{
	std::unique_ptr<LibraryImpl> impl(new LibraryImpl);

	if (!impl->Load())
		return nullptr;

	return impl;
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::Nvidia::LibraryImpl::GetFunction(T &function, const char *name)
{
	function = reinterpret_cast<T>(GetProcAddress(library, name));
	return function;
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::Nvidia::LibraryImpl::GetFunction(T &function, NvU32 id)
{
	function = reinterpret_cast<T>(NvAPI_QueryInterface(id));
	return function;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Nvidia::LibraryImpl::Load()
{
	library = LoadLibraryW(L"nvapi.dll");

	if (!library)
	{
		library = LoadLibraryW(L"nvapi64.dll");

		if (!library)
			return false;
	}

	if (!GetFunction(NvAPI_QueryInterface, "nvapi_QueryInterface"))
		return false;

	if (!GetFunction(NvAPI_Initialize, 0x0150E828))
		return false;

	if (!GetFunction(NvAPI_EnumPhysicalGPUs, 0xE5AC921F))
		return false;

	if (!GetFunction(NvAPI_GPU_GetConnectedOutputs, 0x1730BFC9))
		return false;

	if (!GetFunction(NvAPI_GPU_GetEDID, 0x37D32E69))
		return false;

	if (!GetFunction(NvAPI_I2CRead, 0x2FDE12C5))
		return false;

	if (!GetFunction(NvAPI_I2CWrite, 0xE812EB07))
		return false;

	if (!GetFunction(NvAPI_Unload, 0xD22BDD7E))
		return false;

	if (NvAPI_Initialize() != NVAPI_OK)
		return false;

	loaded = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////
