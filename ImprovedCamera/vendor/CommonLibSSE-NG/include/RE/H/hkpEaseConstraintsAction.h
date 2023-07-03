#pragma once

#include "RE/H/hkpArrayAction.h"

namespace RE
{
	class hkpEaseConstraintsAction : public hkpArrayAction
	{
	public:
		inline static constexpr auto RTTI = RTTI_hkpArrayAction;

		virtual ~hkpEaseConstraintsAction() override;  // 00

		// add
		virtual void Unk_03(void) override;  // 03
		virtual void Unk_07(void) override;  // 07

		// members
		float                           duration;             // 40
		float                           timePassed;           // 44
		hkArray<hkpConstraintInstance*> originalConstraints;  // 48
		hkArray<float>                  originalLimits;       // 58
	};
	static_assert(sizeof(hkpEaseConstraintsAction) == 0x68);
}
