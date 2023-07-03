#pragma once

#include "RE/N/NiTimeController.h"

namespace RE
{
	class BSSimpleScaleController : public NiTimeController
	{
	public:
		inline static constexpr auto RTTI = RTTI_BSSimpleScaleController;
		inline static auto           Ni_RTTI = NiRTTI_BSSimpleScaleController;

		~BSSimpleScaleController() override;  // 00

		// override (NiTimeController)
		const NiRTTI*      GetRTTI() const override;                           // 02
		NiObject*          CreateClone(NiCloningProcess& a_cloning) override;  // 17
		void               Update(float a_time) override;                      // 27
		[[nodiscard]] bool TargetIsRequiredType() const override;              // 2E

		// members
		float scale3D;            // 48
		float refScale;           // 4C
		float scaleAnimDuration;  // 50
	};
	static_assert(sizeof(BSSimpleScaleController) == 0x58);
}
