// Includes ////////////////////////////////////////////////////////////////

#include "EDW.h"

// Implementation //////////////////////////////////////////////////////////

namespace EDW
{
	struct DisplayImpl
	{
		// Functions ///////////////////////////////////////////////////////

		virtual std::unique_ptr<DisplayImpl> Clone() const = 0;
		virtual GpuType GetType() const = 0;
		virtual bool Compare(const std::unique_ptr<DisplayImpl> &impl) const = 0;

		bool Load();
		bool ReadData(unsigned char address, DisplayData &displayData);
		bool WriteData(unsigned char address, const DisplayData &oldDisplayData, const DisplayData &newDisplayData, bool fast);

		virtual bool ReadI2C(unsigned char address, std::basic_string<unsigned char> &data, int size) = 0;
		virtual bool WriteI2C(const std::basic_string<unsigned char> &data) = 0;

		// Variables ///////////////////////////////////////////////////////

		DisplayData edid;
		DisplayData displayId;

		////////////////////////////////////////////////////////////////////
	};

	struct AmdDisplayImpl : public DisplayImpl
	{
		// Functions ///////////////////////////////////////////////////////

		AmdDisplayImpl(int gpu, int output);

		static std::unique_ptr<DisplayImpl> Create(int gpu, int output);
		std::unique_ptr<DisplayImpl> Clone() const;
		GpuType GetType() const;
		bool Compare(const std::unique_ptr<DisplayImpl> &impl) const;

		bool ReadI2C(unsigned char address, std::basic_string<unsigned char> &data, int size);
		bool WriteI2C(const std::basic_string<unsigned char> &data);

		// Variables ///////////////////////////////////////////////////////

		Amd::Library amd;
		int gpu;
		int output;

		////////////////////////////////////////////////////////////////////
	};

	struct NvidiaDisplayImpl : public DisplayImpl
	{
		// Functions ///////////////////////////////////////////////////////

		NvidiaDisplayImpl(Nvidia::NvPhysicalGpuHandle gpu, Nvidia::NvU32 output);

		static std::unique_ptr<DisplayImpl> Create(Nvidia::NvPhysicalGpuHandle gpu, Nvidia::NvU32 output);
		std::unique_ptr<DisplayImpl> Clone() const;
		GpuType GetType() const;
		bool Compare(const std::unique_ptr<DisplayImpl> &impl) const;

		bool ReadI2C(unsigned char address, std::basic_string<unsigned char> &data, int size);
		bool WriteI2C(const std::basic_string<unsigned char> &data);

		// Variables ///////////////////////////////////////////////////////

		Nvidia::Library nvidia;
		Nvidia::NvPhysicalGpuHandle gpu;
		Nvidia::NvU32 output;

		////////////////////////////////////////////////////////////////////
	};
}

// Functions ///////////////////////////////////////////////////////////////

EDW::Display::Display() = default;

////////////////////////////////////////////////////////////////////////////

EDW::Display::Display(const Display &other) :
	impl(other.impl ? other.impl->Clone() : nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

EDW::Display &EDW::Display::operator=(const Display &other)
{
	impl = other.impl ? other.impl->Clone() : nullptr;
	return *this;
}

////////////////////////////////////////////////////////////////////////////

EDW::Display::~Display() = default;

////////////////////////////////////////////////////////////////////////////

EDW::Display::operator bool() const
{
	return impl.get();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::operator==(const Display &other) const
{
	if (impl == other.impl)
		return true;

	if (!impl || !other.impl)
		return false;

	return impl->Compare(other.impl);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::operator!=(const Display &other) const
{
	return !(*this == other);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::operator<(const Display &other) const
{
	std::string id1;
	impl->edid.GetId(id1);

	std::string id2;
	other.impl->edid.GetId(id2);

	if (id1.compare(id2) < 0)
		return true;

	if (id1.compare(id2) > 0)
		return false;

	std::string name1;
	impl->edid.GetName(name1);

	std::string name2;
	other.impl->edid.GetName(name2);

	if (name1.compare(name2) < 0)
		return true;

	if (name1.compare(name2) > 0)
		return false;

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::operator>(const Display &other) const
{
	return other < *this;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::operator<=(const Display &other) const
{
	return !(other < *this);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::operator>=(const Display &other) const
{
	return !(*this < other);
}

////////////////////////////////////////////////////////////////////////////

EDW::Display EDW::Display::CreateAmd(int gpu, int output)
{
	Display display;
	display.impl = AmdDisplayImpl::Create(gpu, output);
	return display;
}

////////////////////////////////////////////////////////////////////////////

EDW::Display EDW::Display::CreateNvidia(Nvidia::NvPhysicalGpuHandle gpu, Nvidia::NvU32 output)
{
	Display display;
	display.impl = NvidiaDisplayImpl::Create(gpu, output);
	return display;
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::Display::GetId(T &id)
{
	return impl->edid.GetId(id);
}

template bool EDW::Display::GetId(std::string &id);
template bool EDW::Display::GetId(std::wstring &id);

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::Display::GetName(T &name)
{
	return impl->edid.GetName(name);
}

template bool EDW::Display::GetName(std::string &name);
template bool EDW::Display::GetName(std::wstring &name);

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::ReadEdid(DisplayData &displayData)
{
	if (!impl->ReadData(0xA0, displayData))
		return false;

	impl->edid = displayData;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::ReadDisplayId(DisplayData &displayData)
{
	if (!impl->ReadData(0xA4, displayData))
		return false;

	impl->displayId = displayData;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::WriteEdid(const DisplayData &displayData, bool fast)
{
	if (!impl->WriteData(0xA0, impl->edid, displayData, fast))
		return false;

	impl->edid = displayData;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::Display::WriteDisplayId(const DisplayData &displayData, bool fast)
{
	if (!impl->WriteData(0xA4, impl->displayId, displayData, fast))
		return false;

	impl->displayId = displayData;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayImpl::Load()
{
	return ReadData(0xA0, edid);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayImpl::ReadData(unsigned char address, DisplayData &displayData)
{
	if (!WriteI2C({address, 0x00}))
		return false;

	Sleep(10);
	std::basic_string<unsigned char> data;

	if (!ReadI2C(address + 1, data, 256))
		return false;

	displayData = DisplayData(data);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayImpl::WriteData(unsigned char address, const DisplayData &oldDisplayData, const DisplayData &newDisplayData, bool fast)
{
	std::basic_string<unsigned char> oldData;
	oldDisplayData.GetData(oldData);

	std::basic_string<unsigned char> newData;
	newDisplayData.GetData(newData);

	if (fast)
	{
		for (int offset = (newDisplayData.GetSize() - 1) / 8 * 8; offset >= 0; offset -= 8)
		{
			if (offset < oldDisplayData.GetSize() && newData.substr(offset, 8) == oldData.substr(offset, 8))
				continue;

			std::basic_string<unsigned char> command;
			command.push_back(address);
			command.push_back(offset);
			command += newData.substr(offset, 8);

			for (int tries = 1; !WriteI2C(command); ++tries)
			{
				if (tries == 10)
					return false;

				Sleep(10);
			}

			Sleep(10);
		}
	}
	else
	{
		for (int offset = newDisplayData.GetSize() - 1; offset >= 0; --offset)
		{
			if (offset < oldDisplayData.GetSize() && newData[offset] == oldData[offset])
				continue;

			std::basic_string<unsigned char> command;
			command.push_back(address);
			command.push_back(offset);
			command.push_back(newData[offset]);

			for (int tries = 1; !WriteI2C(command); ++tries)
			{
				if (tries == 10)
					return false;

				Sleep(10);
			}

			Sleep(10);
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

EDW::AmdDisplayImpl::AmdDisplayImpl(int gpu, int output) :
	gpu(gpu),
	output(output)
{
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayImpl> EDW::AmdDisplayImpl::Create(int gpu, int output)
{
	std::unique_ptr<DisplayImpl> impl(new AmdDisplayImpl(gpu, output));

	if (!impl->Load())
		return nullptr;

	return impl;
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayImpl> EDW::AmdDisplayImpl::Clone() const
{
	std::unique_ptr<DisplayImpl> impl(new AmdDisplayImpl(*this));
	return impl;
}

////////////////////////////////////////////////////////////////////////////

EDW::GpuType EDW::AmdDisplayImpl::GetType() const
{
	return GpuType::Amd;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::AmdDisplayImpl::Compare(const std::unique_ptr<DisplayImpl> &impl) const
{
	if (GetType() != impl->GetType())
		return false;

	const AmdDisplayImpl *other = static_cast<AmdDisplayImpl *>(impl.get());

	if (gpu != other->gpu)
		return false;

	if (output != other->output)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::AmdDisplayImpl::ReadI2C(unsigned char address, std::basic_string<unsigned char> &data, int size)
{
	std::vector<unsigned char> buffer(size);

	if (amd.ADL_Display_DDCBlockAccess_Get(gpu, output, 0, 0, 1, &address, &size, buffer.data()) != Amd::ADL_OK)
		return false;

	data.assign(buffer.data(), buffer.size());
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::AmdDisplayImpl::WriteI2C(const std::basic_string<unsigned char> &data)
{
	int size = 0;

	if (amd.ADL_Display_DDCBlockAccess_Get(gpu, output, 0, 0, static_cast<int>(data.size()), data.data(), &size, nullptr) != Amd::ADL_OK)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

EDW::NvidiaDisplayImpl::NvidiaDisplayImpl(Nvidia::NvPhysicalGpuHandle gpu, Nvidia::NvU32 output) :
	gpu(gpu),
	output(output)
{
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayImpl> EDW::NvidiaDisplayImpl::Create(Nvidia::NvPhysicalGpuHandle gpu, Nvidia::NvU32 output)
{
	std::unique_ptr<DisplayImpl> impl(new NvidiaDisplayImpl(gpu, output));

	if (!impl->Load())
		return nullptr;

	return impl;
}

////////////////////////////////////////////////////////////////////////////

EDW::GpuType EDW::NvidiaDisplayImpl::GetType() const
{
	return GpuType::Nvidia;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::NvidiaDisplayImpl::Compare(const std::unique_ptr<DisplayImpl> &impl) const
{
	if (GetType() != impl->GetType())
		return false;

	const NvidiaDisplayImpl *other = static_cast<NvidiaDisplayImpl *>(impl.get());

	if (gpu != other->gpu)
		return false;

	if (output != other->output)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayImpl> EDW::NvidiaDisplayImpl::Clone() const
{
	std::unique_ptr<DisplayImpl> impl(new NvidiaDisplayImpl(*this));
	return impl;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::NvidiaDisplayImpl::ReadI2C(unsigned char address, std::basic_string<unsigned char> &data, int size)
{
	std::vector<unsigned char> buffer(size);
	Nvidia::NV_I2C_INFO info = {Nvidia::NV_I2C_INFO_VER};
	info.displayMask = output;
	info.bIsDDCPort = true;
	info.i2cDevAddress = address;
	info.pbI2cRegAddress = nullptr;
	info.regAddrSize = 0;
	info.pbData = buffer.data();
	info.cbSize = static_cast<Nvidia::NvU32>(buffer.size());
	info.i2cSpeed = Nvidia::NVAPI_I2C_SPEED_DEPRECATED;
	info.i2cSpeedKhz = Nvidia::NVAPI_I2C_SPEED_10KHZ;

	if (nvidia.NvAPI_I2CRead(gpu, &info) != Nvidia::NVAPI_OK)
		return false;

	data.assign(buffer.data(), buffer.size());
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::NvidiaDisplayImpl::WriteI2C(const std::basic_string<unsigned char> &data)
{
	Nvidia::NV_I2C_INFO info = {Nvidia::NV_I2C_INFO_VER};
	info.displayMask = output;
	info.bIsDDCPort = true;
	info.i2cDevAddress = data[0];
	info.pbI2cRegAddress = nullptr;
	info.regAddrSize = 0;
	info.pbData = const_cast<unsigned char *>(data.data()) + 1;
	info.cbSize = static_cast<Nvidia::NvU32>(data.size()) - 1;
	info.i2cSpeed = Nvidia::NVAPI_I2C_SPEED_DEPRECATED;
	info.i2cSpeedKhz = Nvidia::NVAPI_I2C_SPEED_10KHZ;

	if (nvidia.NvAPI_I2CWrite(gpu, &info) != Nvidia::NVAPI_OK)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////
