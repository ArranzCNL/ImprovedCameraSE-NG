#include "RE/E/ExtraCannotWear.h"

namespace RE
{
	ExtraCannotWear::ExtraCannotWear() :
		BSExtraData()
	{
		stl::emplace_vtable(this);
	}

	ExtraDataType ExtraCannotWear::GetType() const
	{
		return ExtraDataType::kCannotWear;
	}
}
