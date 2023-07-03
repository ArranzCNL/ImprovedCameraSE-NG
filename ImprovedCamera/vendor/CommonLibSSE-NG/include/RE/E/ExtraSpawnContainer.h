#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class ExtraSpawnContainer : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraSpawnContainer;
		inline static auto           EXTRADATATYPE = ExtraDataType::kSpawnContainer;

		~ExtraSpawnContainer() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;                             // 01 - { return kSpawnContainer; }
		bool          IsNotEqual(const BSExtraData* a_rhs) const override;  // 02 - { return container != a_rhs->container; }

		// members
		ObjectRefHandle container;  // 10
		std::uint32_t   pad14;      // 14
	};
	static_assert(sizeof(ExtraSpawnContainer) == 0x18);
}
