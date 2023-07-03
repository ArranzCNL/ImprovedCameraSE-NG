#include "RE/N/NiFloatsExtraData.h"

#include "RE/N/NiTCollection.h"

namespace RE
{
	NiFloatsExtraData* NiFloatsExtraData::Create(const BSFixedString& a_name, const std::vector<float>& a_floats)
	{
		auto data = NiExtraData::Create<NiFloatsExtraData>();
		if (data) {
			data->name = a_name;

			auto size = static_cast<std::uint32_t>(a_floats.size());
			data->size = size;

			data->value = NiAlloc<float>(size);
			std::memcpy(data->value, a_floats.data(), size * sizeof(float));
		}
		return data;
	}

	std::optional<std::uint32_t> NiFloatsExtraData::GetIndex(float a_element) const
	{
		if (value) {
			for (std::uint32_t i = 0; i < size; i++) {
				if (value[i] == a_element) {
					return i;
				}
			}
		}
		return std::nullopt;
	}

	bool NiFloatsExtraData::Insert(float a_element)
	{
		if (!GetIndex(a_element)) {
			auto oldData = value;

			value = NiAlloc<float>(++size);

			if (oldData) {
				for (std::uint32_t i = 0; i < size - 1; i++) {
					value[i] = oldData[i];
				}
				NiFree(oldData);
			}

			value[size - 1] = a_element;

			return true;
		}

		return false;
	}

	bool NiFloatsExtraData::Remove(float a_element)
	{
		if (auto index = GetIndex(a_element); index) {
			auto oldData = value;

			value = NiAlloc<float>(--size);

			for (std::uint32_t i = 0; i < size + 1; i++) {
				if (i != *index) {
					value[i] = oldData[i];
				}
			}
			NiFree(oldData);

			return true;
		}

		return false;
	}
}
