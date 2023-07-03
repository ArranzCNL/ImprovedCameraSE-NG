#include "RE/B/BGSIdleCollection.h"
#include "RE/T/TESIdleForm.h"

namespace RE
{
	void BGSIdleCollection::CopyIdles(const std::vector<TESIdleForm*>& a_copiedData)
	{
		auto oldData = idles;

		auto newSize = a_copiedData.size();
		auto newData = calloc<TESIdleForm*>(newSize);
		std::ranges::copy(a_copiedData, newData);

		idleCount = static_cast<std::int8_t>(newSize);
		idles = newData;

		free(oldData);
	}

	bool BGSIdleCollection::AddIdle(TESIdleForm* a_idle)
	{
		if (!GetIndex(a_idle)) {
			std::vector<TESIdleForm*> copiedData{ idles, idles + idleCount };
			copiedData.push_back(a_idle);
			CopyIdles(copiedData);
			return true;
		}
		return false;
	}

	BGSIdleCollection* BGSIdleCollection::Create()
	{
		auto idleCollection = malloc<BGSIdleCollection>();
		if (idleCollection) {
			idleCollection->Ctor();
		}
		return idleCollection;
	}

	std::optional<std::uint32_t> BGSIdleCollection::GetIndex(TESIdleForm* a_idle) const
	{
		std::optional<std::uint32_t> index = std::nullopt;
		if (idles) {
			for (std::int8_t i = 0; i < idleCount; ++i) {
				if (idles[i] == a_idle) {
					index = i;
					break;
				}
			}
		}
		return index;
	}

	bool BGSIdleCollection::RemoveIdle(TESIdleForm* a_idle)
	{
		auto index = GetIndex(a_idle);
		if (index) {
			std::vector<TESIdleForm*> copiedData{ idles, idles + idleCount };
			copiedData.erase(copiedData.cbegin() + *index);
			CopyIdles(copiedData);
			return true;
		}
		return false;
	}
}
