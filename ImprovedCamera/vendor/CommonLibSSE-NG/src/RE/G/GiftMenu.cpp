#include "RE/G/GiftMenu.h"

namespace RE
{
	RefHandle GiftMenu::GetTargetRefHandle()
	{
		REL::Relocation<RefHandle*> handle{ RELOCATION_ID(519570, 406111) };
		return *handle;
	}
}
