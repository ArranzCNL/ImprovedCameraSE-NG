#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class ExtraTerminalState : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraTerminalState;
		inline static auto           EXTRADATATYPE = ExtraDataType::kTerminalState;

		~ExtraTerminalState() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;  // 01 - { return kTerminalState; }

		// members
		std::uint64_t unk10;  // 10
	};
	static_assert(sizeof(ExtraTerminalState) == 0x18);
}
