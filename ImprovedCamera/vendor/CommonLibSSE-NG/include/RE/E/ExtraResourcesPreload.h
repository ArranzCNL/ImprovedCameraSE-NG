#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class ExtraResourcesPreload : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraResourcesPreload;
		inline static auto           EXTRADATATYPE = ExtraDataType::kResourcesPreload;

		~ExtraResourcesPreload() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;  // 01 - { return kResourcesPreload; }

		// members
		void* unk10;  // 10
	};
	static_assert(sizeof(ExtraResourcesPreload) == 0x18);
}
