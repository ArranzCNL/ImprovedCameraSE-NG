#include "RE/N/NiExtraData.h"

#include "RE/M/MemoryManager.h"

namespace RE
{
	const BSFixedString& NiExtraData::GetName() const
	{
		return name;
	}

	void NiExtraData::SetName(const BSFixedString& a_name)
	{
		name = a_name;
	}

	NiExtraData* NiExtraData::Create(std::size_t a_size, std::uintptr_t a_vtbl)
	{
		auto memory = malloc(a_size);
		std::memset(memory, 0, a_size);
		reinterpret_cast<std::uintptr_t*>(memory)[0] = a_vtbl;
		return static_cast<NiExtraData*>(memory);
	}
}
