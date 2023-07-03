#include "RE/C/ConeProjectile.h"

using namespace REL;

namespace RE
{
#ifdef SKYRIM_CROSS_VR
	void ConeProjectile::Unk_C2()
	{
		RelocateVirtual<decltype(&ConeProjectile::Unk_C2)>(0xC2, 0xC3, this);
	}
#endif
}
