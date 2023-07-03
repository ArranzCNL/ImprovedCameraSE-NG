#include "RE/E/ExtraCanTalkToPlayer.h"

namespace RE
{
	ExtraCanTalkToPlayer::ExtraCanTalkToPlayer() :
		ExtraCanTalkToPlayer(false)
	{}

	ExtraCanTalkToPlayer::ExtraCanTalkToPlayer(bool a_canTalk) :
		BSExtraData(),
		talk(a_canTalk),
		pad11(0),
		pad12(0),
		pad14(0)
	{
		stl::emplace_vtable(this);
	}

	ExtraDataType ExtraCanTalkToPlayer::GetType() const
	{
		return ExtraDataType::kCanTalkToPlayer;
	}
}
