#pragma once

#include "RE/B/BGSLoadGameSubBuffer.h"
#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class ExtraSavedHavokData : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraSavedHavokData;
		inline static auto           EXTRADATATYPE = ExtraDataType::kSavedHavokData;

		~ExtraSavedHavokData() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;  // 01 - { return kSavedHavokData; }

		// members
		BGSLoadGameSubBuffer havokBuffer;  // 10
	};
	static_assert(sizeof(ExtraSavedHavokData) == 0x18);
}
