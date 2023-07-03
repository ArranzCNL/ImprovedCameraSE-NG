#pragma once

#include "RE/B/BGSLoadGameSubBuffer.h"
#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class ExtraSavedAnimation : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraSavedAnimation;
		inline static auto           EXTRADATATYPE = ExtraDataType::kSavedAnimation;

		~ExtraSavedAnimation() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;  // 01 - { return kSavedAnimation; }

		// members
		BGSLoadGameSubBuffer animationBuffer;  // 10
	};
	static_assert(sizeof(ExtraSavedAnimation) == 0x18);
}
