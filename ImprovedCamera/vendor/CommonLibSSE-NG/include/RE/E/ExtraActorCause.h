#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/N/NiSmartPointer.h"

namespace RE
{
	class ActorCause;

	class ExtraActorCause : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraActorCause;
		inline static auto           EXTRADATATYPE = ExtraDataType::kActorCause;

		~ExtraActorCause() override;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override;  // 01 - { return kActorCause; }

		// members
		NiPointer<ActorCause> actorCause;  // 10
	};
	static_assert(sizeof(ExtraActorCause) == 0x18);
}
