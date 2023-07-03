#include "RE/M/MissileProjectile.h"

using namespace REL;

namespace RE
{
#ifdef SKYRIM_CROSS_VR
	void MissileProjectile::Unk_C2()
	{
		RelocateVirtual<decltype(&MissileProjectile::Unk_C2)>(0xC2, 0xC3, this);
	}

	void MissileProjectile::Unk_C3()
	{
		RelocateVirtual<decltype(&MissileProjectile::Unk_C3)>(0xC3, 0xC4, this);
	}
#endif
}
