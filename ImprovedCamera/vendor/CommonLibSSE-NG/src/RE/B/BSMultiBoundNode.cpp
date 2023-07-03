#include "RE/B/BSMultiBoundNode.h"

using namespace REL;

namespace RE
{
#if !defined(ENABLE_SKYRIM_VR) || (!defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR))
	BSMultiBoundRoom* BSMultiBoundNode::GetMultiBoundRoom()
	{
		return RelocateVirtual<decltype(&BSMultiBoundNode::GetMultiBoundRoom)>(0x3E, 0x3F, this);
	}

	bool BSMultiBoundNode::QPointWithin(const NiPoint3& a_point)
	{
		return RelocateVirtual<decltype(&BSMultiBoundNode::QPointWithin)>(0x3F, 0x40, this, a_point);
	}

	void BSMultiBoundNode::Unk_40()
	{
		RelocateVirtual<decltype(&BSMultiBoundNode::Unk_40)>(0x40, 0x41, this);
	}

	void BSMultiBoundNode::Unk_41()
	{
		RelocateVirtual<decltype(&BSMultiBoundNode::Unk_41)>(0x41, 0x42, this);
	}
#endif
}
