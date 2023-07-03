#include "RE/T/TESObjectSTAT.h"

namespace RE
{
	bool TESObjectSTAT::HasTreeLOD() const
	{
		return (GetFormFlags() & RecordFlags::kHasTreeLOD) != 0;
	}

	bool TESObjectSTAT::IsSkyObject() const
	{
		return (GetFormFlags() & RecordFlags::kIsSkyObject) != 0;
	}

	bool TESObjectSTAT::IsSnowObject() const
	{
		return data.flags.all(TESObjectSTATData::Flag::kConsideredSnow);
	}
}
