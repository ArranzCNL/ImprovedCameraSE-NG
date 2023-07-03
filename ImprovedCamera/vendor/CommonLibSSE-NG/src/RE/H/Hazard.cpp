#include "RE/H/Hazard.h"

using namespace REL;

namespace RE
{
#ifdef SKYRIM_CROSS_VR
	void Hazard::Initialize()
	{
		RelocateVirtual<decltype(&Hazard::Initialize)>(0xA2, 0xA3, this);
	}

	void Hazard::Unk_A3()
	{
		RelocateVirtual<decltype(&Hazard::Unk_A3)>(0xA3, 0xA4, this);
	}

	bool Hazard::IsPermanent() const
	{
		return RelocateVirtual<decltype(&Hazard::IsPermanent)>(0xA4, 0xA5, this);
	}
#endif
}
