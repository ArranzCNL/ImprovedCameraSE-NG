#include "RE/T/TESActorBase.h"
#include "RE/RTTI.h"

namespace RE
{
	bool TESActorBase::IsLeveled() const
	{
		for (auto* base = skyrim_cast<TESActorBase*>(baseTemplateForm); base; base = skyrim_cast<TESActorBase*>(base->baseTemplateForm)) {
			if (base->Is(FormType::LeveledNPC)) {
				return true;
			}
		}
		return false;
	}
}
