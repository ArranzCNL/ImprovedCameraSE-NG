#pragma once

#include "RE/N/NiSmartPointer.h"

namespace RE
{
	class TESObjectREFR;

	struct TESEnterBleedoutEvent
	{
	public:
		// members
		NiPointer<TESObjectREFR> actor;  // 00
	};
	static_assert(sizeof(TESEnterBleedoutEvent) == 0x8);
}
