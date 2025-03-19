// Includes ////////////////////////////////////////////////////////////////

#include "EDW.h"

// Implementation //////////////////////////////////////////////////////////

namespace EDW
{
	struct DisplayListImpl
	{
		// Functions ///////////////////////////////////////////////////////

		static std::unique_ptr<DisplayListImpl> Create();
		std::unique_ptr<DisplayListImpl> Clone() const;

		bool LoadAmd();
		bool LoadNvidia();

		// Variables ///////////////////////////////////////////////////////

		Amd::Library amd;
		Nvidia::Library nvidia;
		std::vector<Display> displayList;

		////////////////////////////////////////////////////////////////////
	};
}

// Functions ///////////////////////////////////////////////////////////////

EDW::DisplayList::DisplayList() :
	impl(DisplayListImpl::Create())
{
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayList::DisplayList(const DisplayList &other) :
	impl(other.impl ? other.impl->Clone() : nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayList &EDW::DisplayList::operator=(const DisplayList &other)
{
	impl = other.impl ? other.impl->Clone() : nullptr;
	return *this;
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayList::~DisplayList() = default;

////////////////////////////////////////////////////////////////////////////

EDW::DisplayList::operator bool() const
{
	return impl.get();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayList::Load()
{
	impl->displayList.clear();
	bool status = impl->LoadAmd() + impl->LoadNvidia();
	std::stable_sort(impl->displayList.begin(), impl->displayList.end());
	return status;
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayList::GetCount() const
{
	return static_cast<int>(impl->displayList.size());
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayList::IsValidIndex(int index) const
{
	return index >= 0 && index < GetCount();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayList::Get(int index, Display &display) const
{
	if (!IsValidIndex(index))
		return false;

	display = impl->displayList[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayList::Set(int index, const Display &display)
{
	if (!IsValidIndex(index))
		return false;

	if (display != impl->displayList[index])
		return false;

	impl->displayList[index] = display;
	return true;
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayListImpl> EDW::DisplayListImpl::Create()
{
	std::unique_ptr<DisplayListImpl> impl(new DisplayListImpl);
	return impl;
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayListImpl> EDW::DisplayListImpl::Clone() const
{
	std::unique_ptr<DisplayListImpl> impl(new DisplayListImpl(*this));
	return impl;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayListImpl::LoadAmd()
{
	if (!amd)
		return false;

	int gpuCount;

	if (amd.ADL_Adapter_NumberOfAdapters_Get(&gpuCount) != Amd::ADL_OK)
		return false;

	if (gpuCount <= 0)
		return false;

	std::vector<Amd::AdapterInfo> gpuList(gpuCount);
	int size = sizeof(Amd::AdapterInfo) * gpuCount;

	if (amd.ADL_Adapter_AdapterInfo_Get(gpuList.data(), size) != Amd::ADL_OK)
		return false;

	for (int gpuIndex = 0; gpuIndex < gpuCount; ++gpuIndex)
	{
		int index;

		for (index = 0; index < gpuIndex; ++index)
		{
			if (std::strcmp(gpuList[gpuIndex].strDriverPathExt, gpuList[index].strDriverPathExt) == 0)
				break;
		}

		if (index < gpuIndex)
			continue;

		int outputCount;
		Amd::ADLDisplayInfo *outputList = nullptr;

		if (amd.ADL_Display_DisplayInfo_Get(gpuList[gpuIndex].iAdapterIndex, &outputCount, &outputList, false) != Amd::ADL_OK)
			continue;

		for (int outputIndex = 0; outputIndex < outputCount; ++outputIndex)
		{
			Display display = Display::CreateAmd(gpuList[gpuIndex].iAdapterIndex, outputList[outputIndex].displayID.iDisplayLogicalIndex);

			if (display)
				displayList.push_back(display);
		}

		std::free(outputList);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayListImpl::LoadNvidia()
{
	if (!nvidia)
		return false;

	Nvidia::NvPhysicalGpuHandle gpuList[Nvidia::NVAPI_MAX_PHYSICAL_GPUS];
	Nvidia::NvU32 gpuCount;

	if (nvidia.NvAPI_EnumPhysicalGPUs(gpuList, &gpuCount) != Nvidia::NVAPI_OK)
		return false;

	if (gpuCount <= 0)
		return false;

	for (Nvidia::NvU32 gpuIndex = 0; gpuIndex < gpuCount; ++gpuIndex)
	{
		Nvidia::NvU32 outputMask;

		if (nvidia.NvAPI_GPU_GetConnectedOutputs(gpuList[gpuIndex], &outputMask) != Nvidia::NVAPI_OK)
			continue;

		for (int outputIndex = 0; outputIndex < Nvidia::NVAPI_MAX_HEADS_PER_GPU; ++outputIndex)
		{
			Nvidia::NvU32 output = 1 << outputIndex;

			if (output & outputMask)
			{
				Display display = Display::CreateNvidia(gpuList[gpuIndex], output);

				if (display)
					displayList.push_back(display);
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
