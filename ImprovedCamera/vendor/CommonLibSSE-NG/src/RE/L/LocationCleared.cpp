#include "RE/L/LocationCleared.h"

namespace RE
{
	BSTEventSource<LocationCleared::Event>* LocationCleared::GetEventSource()
	{
		using func_t = decltype(&LocationCleared::GetEventSource);
		REL::Relocation<func_t> func{ RELOCATION_ID(18046, 18435) };
		return func();
	}
}
