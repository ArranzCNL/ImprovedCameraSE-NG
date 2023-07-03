#include "RE/A/ActorKill.h"

namespace RE
{
	BSTEventSource<ActorKill::Event>* ActorKill::GetEventSource()
	{
		using func_t = decltype(&ActorKill::GetEventSource);
		REL::Relocation<func_t> func{ RELOCATION_ID(37390, 38338) };
		return func();
	}
}
