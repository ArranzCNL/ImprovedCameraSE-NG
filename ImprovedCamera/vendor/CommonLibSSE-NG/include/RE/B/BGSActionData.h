#pragma once

#include "RE/A/ActionInput.h"

namespace RE
{
	class BGSActionData : public ActionInput
	{
	public:
		inline static constexpr auto RTTI = RTTI_BGSActionData;
		inline static constexpr auto VTABLE = VTABLE_BGSActionData;

		virtual ~BGSActionData() override;  // 00

		// add
		virtual BGSActionData* Clone() const;  // 04
		virtual bool           Process();      // 05 - { return false; }

		// members
		BSFixedString unk28;  // 08
		BSFixedString unk30;  // 30
		uint64_t      unk38;  // 38
		uint64_t      unk40;  // 40
		uint64_t      unk48;  // 48
		uint64_t      unk50;  // 50
		uint64_t      unk58;  // 58
	};
	static_assert(sizeof(BGSActionData) == 0x60);
}
