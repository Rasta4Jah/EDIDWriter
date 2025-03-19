// Includes ////////////////////////////////////////////////////////////////

#include "EDW.h"

// Implementations /////////////////////////////////////////////////////////

namespace EDW
{
	struct DisplayDataImpl
	{
		// Constants ///////////////////////////////////////////////////////

		static const int MaxSize = 256;
		static const int MaxLoadSize = MaxSize + 1;

		// Functions ///////////////////////////////////////////////////////

		DisplayDataImpl() = default;
		DisplayDataImpl(const std::basic_string<unsigned char> &data);

		static std::unique_ptr<DisplayDataImpl> Create();
		static std::unique_ptr<DisplayDataImpl> Create(const std::basic_string<unsigned char> &data);
		std::unique_ptr<DisplayDataImpl> Clone() const;

		bool GetData(std::basic_string<unsigned char> &data) const;
		int GetSize() const;
		int GetOriginalSize() const;
		int GetReportedSize() const;
		int GetDisplayIdBlockSize(int block) const;
		bool Trim();
		template <class T> bool GetId(T &id) const;
		template <class T> bool GetEdidId(T &id) const;
		template <class T> bool GetDisplayIdId(T &id, unsigned char tag) const;
		template <class T> bool GetName(T &name) const;
		template <class T> bool GetEdidName(T &name, unsigned char tag) const;
		template <class T> bool GetDisplayIdName(T &id, unsigned char tag) const;
		template <class T> bool GetText(T &text, int columns = 0) const;
		template <class T> bool Load(const T &name);
		bool LoadBin(std::ifstream &stream);
		bool LoadDat(std::ifstream &stream);
		bool LoadTxt(std::ifstream &stream);
		template <class T> bool SaveBin(const T &name) const;
		template <class T> bool SaveDat(const T &name) const;
		template <class T> bool SaveTxt(const T &name, int columns = 16) const;
		bool IsEdid() const;
		bool IsValidEdidHeader() const;
		bool IsCorruptedEdidHeader() const;
		bool FixEdidHeader();
		bool IsValidEdidExtensionBlock(int block) const;
		bool IsValidEdidExtensionBlocks() const;
		bool FixEdidExtensionBlocks();
		bool IsValidEdidChecksums() const;
		bool FixEdidChecksums();
		bool IsDisplayId() const;
		bool IsValidDisplayIdChecksums() const;
		bool FixDisplayIdChecksums();
		bool IsValidChecksum(int offset, int size, unsigned char add = 0) const;
		bool FixChecksum(int offset, int size);

		static int FromHex(int value);
		static int ToHex(int value);

		// Variables ///////////////////////////////////////////////////////

		std::basic_string<unsigned char> data;
		int originalSize;

		////////////////////////////////////////////////////////////////////
	};
}

// Functions ///////////////////////////////////////////////////////////////

EDW::DisplayData::DisplayData() :
	impl(DisplayDataImpl::Create())
{
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayData::DisplayData(const std::basic_string<unsigned char> &data) :
	impl(DisplayDataImpl::Create(data))
{
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayData::DisplayData(const DisplayData &other) :
	impl(other.impl ? other.impl->Clone() : nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayData &EDW::DisplayData::operator=(const DisplayData &other)
{
	impl = other.impl ? other.impl->Clone() : nullptr;
	return *this;
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayData::~DisplayData() = default;

////////////////////////////////////////////////////////////////////////////

EDW::DisplayData::operator bool() const
{
	return impl.get();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::operator==(const DisplayData &other) const
{
	int size = std::min(impl->GetSize(), other.impl->GetSize());
	return impl->data.compare(0, size, other.impl->data, 0, size) == 0;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::operator!=(const DisplayData &other) const
{
	return !(*this == other);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::GetData(std::basic_string<unsigned char> &data) const
{
	return impl->GetData(data);
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayData::GetSize() const
{
	return impl->GetSize();
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayData::GetOriginalSize() const
{
	return impl->GetOriginalSize();
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayData::GetReportedSize() const
{
	return impl->GetReportedSize();
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::GetId(T &id) const
{
	return impl->GetId(id);
}

template bool EDW::DisplayData::GetId(std::string &id) const;
template bool EDW::DisplayData::GetId(std::wstring &id) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::GetName(T &name) const
{
	return impl->GetName(name);
}

template bool EDW::DisplayData::GetName(std::string &name) const;
template bool EDW::DisplayData::GetName(std::wstring &name) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::GetText(T &text, int columns) const
{
	return impl->GetText(text, columns);
}

template bool EDW::DisplayData::GetText(std::string &text, int columns) const;
template bool EDW::DisplayData::GetText(std::wstring &text, int columns) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::Load(const T &file)
{
	return impl->Load(file);
}

template bool EDW::DisplayData::Load(const std::string &file);
template bool EDW::DisplayData::Load(const std::wstring &file);

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::SaveBin(const T &file) const
{
	return impl->SaveBin(file);
}

template bool EDW::DisplayData::SaveBin(const std::string &file) const;
template bool EDW::DisplayData::SaveBin(const std::wstring &file) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::SaveDat(const T &file) const
{
	return impl->SaveDat(file);
}

template bool EDW::DisplayData::SaveDat(const std::string &file) const;
template bool EDW::DisplayData::SaveDat(const std::wstring &file) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayData::SaveTxt(const T &file, int columns) const
{
	return impl->SaveTxt(file);
}

template bool EDW::DisplayData::SaveTxt(const std::string &file, int columns) const;
template bool EDW::DisplayData::SaveTxt(const std::wstring &file, int columns) const;

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::IsEdid() const
{
	return impl->IsEdid();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::IsValidEdidHeader() const
{
	return impl->IsValidEdidHeader();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::FixEdidHeader()
{
	return impl->FixEdidHeader();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::IsValidEdidExtensionBlocks() const
{
	return impl->IsValidEdidExtensionBlocks();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::FixEdidExtensionBlocks()
{
	return impl->FixEdidExtensionBlocks();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::IsValidEdidChecksums() const
{
	return impl->IsValidEdidChecksums();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::FixEdidChecksums()
{
	return impl->FixEdidChecksums();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::IsDisplayId() const
{
	return impl->IsDisplayId();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::IsValidDisplayIdChecksums() const
{
	return impl->IsValidDisplayIdChecksums();
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayData::FixDisplayIdChecksums()
{
	return impl->FixDisplayIdChecksums();
}

////////////////////////////////////////////////////////////////////////////

EDW::DisplayDataImpl::DisplayDataImpl(const std::basic_string<unsigned char> &data) :
	data(data)
{
	Trim();
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayDataImpl> EDW::DisplayDataImpl::Create()
{
	std::unique_ptr<DisplayDataImpl> impl(new DisplayDataImpl);
	return impl;
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayDataImpl> EDW::DisplayDataImpl::Create(const std::basic_string<unsigned char> &data)
{
	std::unique_ptr<DisplayDataImpl> impl(new DisplayDataImpl(data));
	return impl;
}

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<EDW::DisplayDataImpl> EDW::DisplayDataImpl::Clone() const
{
	std::unique_ptr<DisplayDataImpl> impl(new DisplayDataImpl(*this));
	return impl;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::GetData(std::basic_string<unsigned char> &data) const
{
	data = this->data;
	return true;
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayDataImpl::GetSize() const
{
	return static_cast<int>(data.size());
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayDataImpl::GetOriginalSize() const
{
	return originalSize;
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayDataImpl::GetReportedSize() const
{
	if (IsEdid())
		return data[126] * 128 + 128;

	if (IsDisplayId())
	{
		int lastBlockSize = GetDisplayIdBlockSize(data[3]);

		if (lastBlockSize < 5)
			lastBlockSize = 256;

		return data[3] * 256 + lastBlockSize;
	}

	return GetSize();
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayDataImpl::GetDisplayIdBlockSize(int block) const
{
	if (block < 0)
		return 0;

	int offset = block * 256;
	int end = offset + 5;

	if (GetSize() < end)
		return 0;

	if (data[offset] < 0x10)
		return 0;

	int index = offset + 1;
	int size = data[index] + 5;

	if (size > 256)
		return 0;

	end = offset + size;

	if (GetSize() < end)
		return 0;

	return size;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::Trim()
{
	int size = GetReportedSize();

	if (GetSize() > size)
		data.resize(size);

	originalSize = GetSize();

	if (GetSize() > MaxSize)
		data.resize(MaxSize);

	return true;
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetId(T &id) const
{
	if (IsEdid())
	{
		if (GetEdidId(id))
			return true;
	}
	else if (IsDisplayId())
	{
		if (data[0] < 0x20)
		{
			if (GetDisplayIdId(id, 0x00))
				return true;

			if (GetDisplayIdId(id, 0x20))
				return true;
		}
		else
		{
			if (GetDisplayIdId(id, 0x20))
				return true;

			if (GetDisplayIdId(id, 0x00))
				return true;
		}
	}

	return false;
}

template bool EDW::DisplayDataImpl::GetId(std::string &id) const;
template bool EDW::DisplayDataImpl::GetId(std::wstring &id) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetEdidId(T &id) const
{
	if (GetSize() < 128)
		return false;

	id.clear();
	id.push_back(64 | (data[8] >> 2 & 31));
	id.push_back(64 | (data[8] << 3 & 24) | (data[9] >> 5 & 7));
	id.push_back(64 | (data[9] & 31));
	id.push_back(ToHex(data[11] >> 4));
	id.push_back(ToHex(data[11] & 15));
	id.push_back(ToHex(data[10] >> 4));
	id.push_back(ToHex(data[10] & 15));
	return true;
}

template bool EDW::DisplayDataImpl::GetEdidId(std::string &id) const;
template bool EDW::DisplayDataImpl::GetEdidId(std::wstring &id) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetDisplayIdId(T &id, unsigned char tag) const
{
	int end = GetDisplayIdBlockSize(0) - 1;

	for (int index = 6; index < end; index += data[index] + 3)
	{
		int offset = index - 2;
		const unsigned char *byte = &data[offset];

		if (byte[0] == tag && byte[2] >= 5)
		{
			id.clear();

			if (tag == 0x00 && std::isgraph(byte[3]) && std::isgraph(byte[4]) && std::isgraph(byte[5]))
			{
				id.push_back(byte[3]);
				id.push_back(byte[4]);
				id.push_back(byte[5]);
			}
			else
			{
				id.push_back(ToHex(byte[3] >> 4));
				id.push_back(ToHex(byte[3] & 15));
				id.push_back(ToHex(byte[4] >> 4));
				id.push_back(ToHex(byte[4] & 15));
				id.push_back(ToHex(byte[5] >> 4));
				id.push_back(ToHex(byte[5] & 15));
			}

			id.push_back(ToHex(byte[7] >> 4));
			id.push_back(ToHex(byte[7] & 15));
			id.push_back(ToHex(byte[6] >> 4));
			id.push_back(ToHex(byte[6] & 15));
			return true;
		}
	}

	return false;
}

template bool EDW::DisplayDataImpl::GetDisplayIdId(std::string &id, unsigned char tag) const;
template bool EDW::DisplayDataImpl::GetDisplayIdId(std::wstring &id, unsigned char tag) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetName(T &name) const
{
	if (IsEdid())
	{
		if (GetEdidName(name, 0xFC))
			return true;

		if (GetEdidName(name, 0xFE))
			return true;
	}
	else if (IsDisplayId())
	{
		if (data[0] < 0x20)
		{
			if (GetDisplayIdName(name, 0x00))
				return true;

			if (GetDisplayIdName(name, 0x20))
				return true;
		}
		else
		{
			if (GetDisplayIdName(name, 0x20))
				return true;

			if (GetDisplayIdName(name, 0x00))
				return true;
		}
	}

	return false;
}

template bool EDW::DisplayDataImpl::GetName(std::string &name) const;
template bool EDW::DisplayDataImpl::GetName(std::wstring &name) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetEdidName(T &name, unsigned char tag) const
{
	if (GetSize() < 128)
		return false;

	for (int slot = 0; slot < 4; ++slot)
	{
		int offset = 54 + slot * 18;
		const unsigned char *byte = &data[offset];

		if (byte[0] == 0x00 && byte[1] == 0x00 && byte[2] == 0x00 && byte[3] == tag && byte[4] == 0x00)
		{
			name.clear();
			int size = 13;
			int begin = 5;
			int end = begin + size;

			for (int index = begin; index < end && byte[index] != 0x0A && byte[index] != 0x00; ++index)
				name.push_back(byte[index]);

			return true;
		}
	}

	return false;
}

template bool EDW::DisplayDataImpl::GetEdidName(std::string &name, unsigned char tag) const;
template bool EDW::DisplayDataImpl::GetEdidName(std::wstring &name, unsigned char tag) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetDisplayIdName(T &name, unsigned char tag) const
{
	int end = GetDisplayIdBlockSize(0) - 1;

	for (int index = 6; index < end; index += data[index] + 3)
	{
		int offset = index - 2;
		const unsigned char *byte = &data[offset];

		if (byte[0] == tag && byte[2] >= 12)
		{
			name.clear();
			int size = std::min<int>(byte[14], byte[2] - 12);
			int begin = 15;
			int end = begin + size;

			for (int index = begin; index < end; ++index)
				name.push_back(byte[index]);

			return true;
		}
	}

	return false;
}

template bool EDW::DisplayDataImpl::GetDisplayIdName(std::string &name, unsigned char tag) const;
template bool EDW::DisplayDataImpl::GetDisplayIdName(std::wstring &name, unsigned char tag) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::GetText(T &text, int columns) const
{
	text.clear();

	for (int index = 0; index < GetSize(); ++index)
	{
		if (index > 0)
		{
			if (columns > 0 && index % columns == 0)
			{
				text.push_back('\r');
				text.push_back('\n');
			}
			else
			{
				text.push_back(' ');
			}
		}

		text.push_back(ToHex(data[index] >> 4));
		text.push_back(ToHex(data[index] & 15));
	}

	return true;
}

template bool EDW::DisplayDataImpl::GetText(std::string &text, int columns) const;
template bool EDW::DisplayDataImpl::GetText(std::wstring &text, int columns) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::Load(const T &file)
{
	std::ifstream stream(file, std::ios::binary);

	if (!stream)
		return false;

	if (!LoadTxt(stream))
	if (!LoadDat(stream))
	if (!LoadBin(stream))
	{
		stream.close();
		return false;
	}

	stream.close();
	Trim();
	return true;
}

template bool EDW::DisplayDataImpl::Load(const std::string &file);
template bool EDW::DisplayDataImpl::Load(const std::wstring &file);

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::LoadBin(std::ifstream &stream)
{
	stream.clear();
	stream.seekg(0);

	if (!stream)
		return false;

	std::vector<unsigned char> buffer(MaxLoadSize);
	stream.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

	if (!stream && !stream.eof())
		return false;

	data.assign(buffer.data(), static_cast<int>(stream.gcount()));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::LoadDat(std::ifstream &stream)
{
	stream.clear();
	stream.seekg(0);

	if (!stream)
		return false;

	std::string type;
	stream >> type;

	if (!stream)
		return false;

	std::transform(type.begin(), type.end(), type.begin(), toupper);

	if (type != "EDID" && type != "DISPLAYID" && type != "DATA")
		return false;

	stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (!stream)
		return false;

	std::vector<unsigned char> buffer;

	for (int index = 0; index < MaxLoadSize; ++index)
	{
		if (index % 16 == 0)
		{
			int offset;
			stream >> std::hex >> offset >> std::dec;

			if (!stream)
				break;

			if (offset != index)
				return false;

			stream >> std::ws;
			int delimiter = stream.get();

			if (!stream)
				return false;

			if (delimiter != '|')
				return false;
		}

		stream >> std::ws;
		int hex1 = stream.get();

		if (!stream)
		{
			if (index % 16 == 0)
				return false;

			break;
		}

		int hex2 = stream.get();

		if (!stream)
			return false;

		if (!std::isxdigit(hex1) || !std::isxdigit(hex2))
			return false;

		buffer.push_back(FromHex(hex1) << 4 | FromHex(hex2));
	}

	if (!stream && !stream.eof())
		return false;

	if (buffer.empty())
		return false;

	data.assign(buffer.data(), buffer.size());
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::LoadTxt(std::ifstream &stream)
{
	stream.clear();
	stream.seekg(0);

	if (!stream)
		return false;

	std::vector<unsigned char> buffer;

	for (int index = 0; index < MaxLoadSize; ++index)
	{
		stream >> std::ws;
		int hex1 = stream.get();

		if (!stream)
			break;

		int hex2 = stream.get();

		if (!stream)
			return false;

		if (hex1 == '0' && (hex2 == 'x' || hex2 == 'X'))
		{
			hex1 = stream.get();
			hex2 = stream.get();

			if (!stream)
				return false;
		}

		if (!std::isxdigit(hex1) || !std::isxdigit(hex2))
			return false;

		buffer.push_back(FromHex(hex1) << 4 | FromHex(hex2));
		int delimiter = stream.get();

		if (!stream)
			break;

		if (!std::ispunct(delimiter))
			stream.unget();
	}

	if (!stream && !stream.eof())
		return false;

	if (buffer.empty())
		return false;

	data.assign(buffer.data(), buffer.size());
	return true;
}

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::SaveBin(const T &file) const
{
	std::ofstream stream(file, std::ios::binary);

	if (!stream)
		return false;

	stream.write(reinterpret_cast<const char *>(data.data()), data.size());
	stream.close();
	return stream.good();
}

template bool EDW::DisplayDataImpl::SaveBin(const std::string &file) const;
template bool EDW::DisplayDataImpl::SaveBin(const std::wstring &file) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::SaveDat(const T &file) const
{
	std::ofstream stream(file, std::ios::binary);

	if (!stream)
		return false;

	if (IsEdid())
		stream << "EDID BYTES:\r\n";
	else if (IsDisplayId())
		stream << "DISPLAYID BYTES:\r\n";
	else
		stream << "DATA BYTES:\r\n";

	if (GetSize() <= 256)
	{
		stream << "0x   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n";
		stream << "    ------------------------------------------------\r\n";
	}
	else
	{
		stream << "0x     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n";
		stream << "      ------------------------------------------------\r\n";
	}

	int columns = 16;

	for (int index = 0; index < GetSize(); ++index)
	{
		if (index % columns == 0)
		{
			if (index > 0)
			{
				stream.put('\r');
				stream.put('\n');
			}

			if (GetSize() > 256)
			{
				stream.put(ToHex(index >> 12 & 15));
				stream.put(ToHex(index >> 8 & 15));
			}

			stream.put(ToHex(index >> 4 & 15));
			stream.put(ToHex(index & 15));
			stream << " | ";
		}
		else
		{
			stream.put(' ');
		}

		stream.put(ToHex(data[index] >> 4));
		stream.put(ToHex(data[index] & 15));
	}

	stream.put('\r');
	stream.put('\n');
	stream.close();
	return stream.good();
}

template bool EDW::DisplayDataImpl::SaveDat(const std::string &file) const;
template bool EDW::DisplayDataImpl::SaveDat(const std::wstring &file) const;

////////////////////////////////////////////////////////////////////////////

template <class T>
bool EDW::DisplayDataImpl::SaveTxt(const T &file, int columns) const
{
	std::ofstream stream(file, std::ios::binary);

	if (!stream)
		return false;

	for (int index = 0; index < GetSize(); ++index)
	{
		if (index > 0)
		{
			if (columns > 0 && index % columns == 0)
			{
				stream.put('\r');
				stream.put('\n');
			}
			else
			{
				stream.put(' ');
			}
		}

		stream.put(ToHex(data[index] >> 4));
		stream.put(ToHex(data[index] & 15));
	}

	stream.put('\r');
	stream.put('\n');
	stream.close();
	return stream.good();
}

template bool EDW::DisplayDataImpl::SaveTxt(const std::string &file, int columns) const;
template bool EDW::DisplayDataImpl::SaveTxt(const std::wstring &file, int columns) const;

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsEdid() const
{
	if (GetSize() < 128)
		return false;

	if (!IsValidEdidHeader() && !IsCorruptedEdidHeader())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsValidEdidHeader() const
{
	if (GetSize() < 128)
		return false;

	if (data.compare(0, 8, {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00}) != 0)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsCorruptedEdidHeader() const
{
	if (GetSize() < 128)
		return false;

	if (data[18] != 1 || data[19] > 4)
		return false;

	return IsValidChecksum(8, 120, 6);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::FixEdidHeader()
{
	if (GetSize() < 128)
		return false;

	data.replace(0, 8, {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00});
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsValidEdidExtensionBlock(int block) const
{
	if (block < 1)
		return false;

	int offset = block * 128;
	int end = offset + 128;

	if (GetSize() < end)
		return false;

	if (data.compare(offset, 8, {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00}) == 0)
		return false;

	std::basic_string<unsigned char> zero(127, 0x00);

	if (data.compare(offset, 127, zero) == 0)
		return false;

	std::basic_string<unsigned char> compare(127, 0xFF);

	if (data.compare(offset, 127, compare) == 0)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsValidEdidExtensionBlocks() const
{
	if (GetSize() < 128)
		return false;

	int blocks = std::min(data[126] + 1, (GetSize() + 127) / 128);

	for (int block = 1; block < blocks; ++block)
	{
		if (!IsValidEdidExtensionBlock(block))
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::FixEdidExtensionBlocks()
{
	if (GetSize() < 128)
		return false;

	int blocks = std::min(data[126] + 1, (GetSize() + 127) / 128);

	for (int block = blocks - 1; block >= 1; --block)
	{
		if (!IsValidEdidExtensionBlock(block))
		{
			int offset = block * 128;
			data.erase(offset, 128);
		}
	}

	data[126] = GetSize() / 128 - 1;
	return FixChecksum(0, 128);
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsValidEdidChecksums() const
{
	if (GetSize() < 128)
		return false;

	if (!IsValidChecksum(0, 128))
		return false;

	int blocks = std::min(data[126] + 1, GetSize() / 128);

	for (int block = 1; block < blocks; ++block)
	{
		if (!IsValidEdidExtensionBlock(block))
			continue;

		if (!IsValidChecksum(block * 128, 128))
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::FixEdidChecksums()
{
	if (GetSize() < 128)
		return false;

	if (!FixChecksum(0, 128))
		return false;

	int blocks = std::min(data[126] + 1, GetSize() / 128);

	for (int block = 1; block < blocks; ++block)
	{
		if (!IsValidEdidExtensionBlock(block))
			continue;

		if (!FixChecksum(block * 128, 128))
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsDisplayId() const
{
	int size = GetDisplayIdBlockSize(0);

	if (size < 5)
		return false;

	int end = size - 1;
	int bytes = 5;

	for (int index = 6; index < end; index += data[index] + 3)
		bytes += data[index] + 3;

	if (bytes > size)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsValidDisplayIdChecksums() const
{
	if (GetSize() < 5)
		return false;

	int blocks = std::min(data[3] + 1, (GetSize() + 255) / 256);

	for (int block = 0; block < blocks; ++block)
	{
		int size = GetDisplayIdBlockSize(block);

		if (size < 5)
			continue;

		if (!IsValidChecksum(block * 256, size))
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::FixDisplayIdChecksums()
{
	if (GetSize() < 5)
		return false;

	int blocks = std::min(data[3] + 1, (GetSize() + 255) / 256);

	for (int block = 0; block < blocks; ++block)
	{
		int size = GetDisplayIdBlockSize(block);

		if (size < 5)
			continue;

		if (!FixChecksum(block * 256, size))
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::IsValidChecksum(int offset, int size, unsigned char add) const
{
	if (offset < 0 || size < 1)
		return false;

	int end = offset + size - 1;

	if (GetSize() <= end)
		return false;

	unsigned char checksum = add;

	for (int index = offset; index < end; ++index)
		checksum -= data[index];

	if (data[end] != checksum)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool EDW::DisplayDataImpl::FixChecksum(int offset, int size)
{
	if (offset < 0 || size < 1)
		return false;

	int end = offset + size - 1;

	if (GetSize() <= end)
		return false;

	unsigned char checksum = 0;

	for (int index = offset; index < end; ++index)
		checksum -= data[index];

	data[end] = checksum;
	return true;
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayDataImpl::FromHex(int value)
{
	if (value >= '0' && value <= '9')
		return value - '0';

	if (value >= 'A' && value <= 'F')
		return value - 'A' + 10;

	if (value >= 'a' && value <= 'f')
		return value - 'a' + 10;

	return 0;
}

////////////////////////////////////////////////////////////////////////////

int EDW::DisplayDataImpl::ToHex(int value)
{
	if (value < 10)
		return value + '0';

	return value + 'A' - 10;
}

////////////////////////////////////////////////////////////////////////////
