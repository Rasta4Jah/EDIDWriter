// Includes ////////////////////////////////////////////////////////////////

#include "EDW.h"

// Implementation //////////////////////////////////////////////////////////

namespace EDW
{
	namespace Amd
	{
		struct LibraryImpl
		{
			// Typedefs ////////////////////////////////////////////////////

			typedef void *(__stdcall *ADL_MAIN_MALLOC_CALLBACK)(int);
			typedef int (*ADL_Main_Control_Create_Function)(ADL_MAIN_MALLOC_CALLBACK, int);
			typedef int (*ADL_Adapter_NumberOfAdapters_Get_Function)(int *);
			typedef int (*ADL_Adapter_AdapterInfo_Get_Function)(AdapterInfo *, int);
			typedef int (*ADL_Display_DisplayInfo_Get_Function)(int, int *, ADLDisplayInfo **, int);
			typedef int (*ADL_Display_EdidData_Get_Function)(int, int, ADLDisplayEDIDData *);
			typedef int (*ADL_Display_DDCBlockAccess_Get_Function)(int, int, int, int, int, const unsigned char *, int *, unsigned char *);
			typedef int (*ADL_Main_Control_Destroy_Function)();

			// Functions ///////////////////////////////////////////////////

			LibraryImpl();
			~LibraryImpl();

			static std::unique_ptr<LibraryImpl> Create();
			template <class T>
			bool GetFunction(T &function, const char *name);
			static void *__stdcall ADL_Main_Memory_Alloc(int size);
			bool Load();

			// Variables ///////////////////////////////////////////////////

			HMODULE library;
			ADL_Main_Control_Create_Function ADL_Main_Control_Create;
			ADL_Adapter_NumberOfAdapters_Get_Function ADL_Adapter_NumberOfAdapters_Get;
			ADL_Adapter_AdapterInfo_Get_Function ADL_Adapter_AdapterInfo_Get;
			ADL_Display_DisplayInfo_Get_Function ADL_Display_DisplayInfo_Get;
			ADL_Display_EdidData_Get_Function ADL_Display_EdidData_Get;
			ADL_Display_DDCBlockAccess_Get_Function ADL_Display_DDCBlockAccess_Get;
			ADL_Main_Control_Destroy_Function ADL_Main_Control_Destroy;
			bool loaded;

			////////////////////////////////////////////////////////////////
		};
	}
}

// Static Variables ////////////////////////////////////////////////////////

std::unique_ptr<EDW::Amd::LibraryImpl> EDW::Amd::Library::impl = nullptr;
int EDW::Amd::Library::references = 0;

// Functions ///////////////////////////////////////////////////////////////

EDW::Amd::Library::Library()
{
	if (!impl)
		impl = LibraryImpl::Create();

	++references;
}

////////////////////////////////////////////////////////////////////////////

EDW::Amd::Library::Library(const Library &other)
{
	++references;
}

////////////////////////////////////////////////////////////////////////////

EDW::Amd::Library::~Library()
{
	--references;

	if (references <= 0)
		impl.reset();
}

////////////////////////////////////////////////////////////////////////////

EDW::Amd::Library::operator bool() const
{
	return impl.get();
}

////////////////////////////////////////////////////////////////////////////

int EDW::Amd::Library::ADL_Adapter_NumberOfAdapters_Get(int *lpNumAdapters)
{
	return impl->ADL_Adapter_NumberOfAdapters_Get(lpNumAdapters);
}

////////////////////////////////////////////////////////////////////////////

int EDW::Amd::Library::ADL_Adapter_AdapterInfo_Get(AdapterInfo *lpInfo, int iInputSize)
{
	return impl->ADL_Adapter_AdapterInfo_Get(lpInfo, iInputSize);
}

////////////////////////////////////////////////////////////////////////////

int EDW::Amd::Library::ADL_Display_DisplayInfo_Get(int iAdapterIndex, int *lpNumDisplays, ADLDisplayInfo **lppInfo, int iForceDetect)
{
	return impl->ADL_Display_DisplayInfo_Get(iAdapterIndex, lpNumDisplays, lppInfo, iForceDetect);
}

////////////////////////////////////////////////////////////////////////////

int EDW::Amd::Library::ADL_Display_EdidData_Get(int iAdapterIndex, int iDisplayIndex, ADLDisplayEDIDData *lpEDIDData)
{
	return impl->ADL_Display_EdidData_Get(iAdapterIndex, iDisplayIndex, lpEDIDData);
}

////////////////////////////////////////////////////////////////////////////

int EDW::Amd::Library::ADL_Display_DDCBlockAccess_Get(int iAdapterIndex, int iDisplayIndex, int iOption, int iCommandIndex, int iSendMsgLen, const unsigned char *lpucSendMsgBuf, int *lpulRecvMsgLen, unsigned char *lpucRecvMsgBuf)
{
	return impl->ADL_Display_DDCBlockAccess_Get(iAdapterIndex, iDisplayIndex, iOption, iCommandIndex, iSendMsgLen, lpucSendMsgBuf, lpulRecvMsgLen, lpucRecvMsgBuf);
}

////////////////////////////////////////////////////////////////////////////

EDW::Amd::LibraryImpl::LibraryImpl() :
	library(nullptr),
	loaded(false)
{
}

////////////////////////////////////////////////////////////////////////////

EDW::Amd::LibraryImpl::~LibraryImpl()
{
	if (loaded)
		ADL_Main_Control_Destroy();

	if (library)
		FreeLibrary(library);
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::Amd::LibraryImpl> EDW::Amd::LibraryImpl::Create()
{
	std::unique_ptr<LibraryImpl> impl(new LibraryImpl);

	if (!impl->Load())
		return nullptr;

	return impl;
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::Amd::LibraryImpl::GetFunction(T &function, const char *name)
{
	function = reinterpret_cast<T>(GetProcAddress(library, name));
	return function;
}

////////////////////////////////////////////////////////////////////////////

void *EDW::Amd::LibraryImpl::ADL_Main_Memory_Alloc(int size)
{
	return std::malloc(size);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Amd::LibraryImpl::Load()
{
	library = LoadLibraryW(L"atiadlxx.dll");

	if (!library)
	{
		library = LoadLibraryW(L"atiadlxy.dll");

		if (!library)
			return false;
	}

	if (!GetFunction(ADL_Main_Control_Create, "ADL_Main_Control_Create"))
		return false;

	if (!GetFunction(ADL_Adapter_NumberOfAdapters_Get, "ADL_Adapter_NumberOfAdapters_Get"))
		return false;

	if (!GetFunction(ADL_Adapter_AdapterInfo_Get, "ADL_Adapter_AdapterInfo_Get"))
		return false;

	if (!GetFunction(ADL_Display_DisplayInfo_Get, "ADL_Display_DisplayInfo_Get"))
		return false;

	if (!GetFunction(ADL_Display_EdidData_Get, "ADL_Display_EdidData_Get"))
		return false;

	if (!GetFunction(ADL_Display_DDCBlockAccess_Get, "ADL_Display_DDCBlockAccess_Get"))
		return false;

	if (!GetFunction(ADL_Main_Control_Destroy, "ADL_Main_Control_Destroy"))
		return false;

	if (ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1) != ADL_OK)
		return false;

	loaded = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////
