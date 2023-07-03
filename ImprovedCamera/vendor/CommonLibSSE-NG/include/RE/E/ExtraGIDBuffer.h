#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class ExtraGIDBuffer : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraGIDBuffer;
		inline static auto           EXTRADATATYPE = ExtraDataType::kGIDBuffer;

		~ExtraGIDBuffer() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;  // 01 - { return kGIDBuffer; }

		// members
		std::int8_t* buffer;  // 10
	};
	static_assert(sizeof(ExtraGIDBuffer) == 0x18);
}
