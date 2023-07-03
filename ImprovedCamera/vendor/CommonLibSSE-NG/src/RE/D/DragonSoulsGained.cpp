#include "RE/D/DragonSoulsGained.h"

namespace RE
{
	BSTEventSource<DragonSoulsGained::Event>* DragonSoulsGained::GetEventSource()
	{
		using func_t = decltype(&DragonSoulsGained::GetEventSource);
		REL::Relocation<func_t> func{ RELOCATION_ID(37571, 38520) };
		return func();
	}
}
