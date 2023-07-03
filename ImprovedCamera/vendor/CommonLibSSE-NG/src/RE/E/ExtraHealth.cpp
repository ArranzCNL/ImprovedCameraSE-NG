#include "RE/E/ExtraHealth.h"

namespace RE
{
	ExtraHealth::ExtraHealth() :
		ExtraHealth(1.0)
	{}

	ExtraHealth::ExtraHealth(float a_health) :
		BSExtraData(),
		health(a_health),
		pad14(0)
	{
		stl::emplace_vtable(this);
	}

	ExtraDataType ExtraHealth::GetType() const
	{
		return ExtraDataType::kHealth;
	}

	bool ExtraHealth::IsNotEqual(const BSExtraData* a_rhs) const
	{
		auto rhs = static_cast<const ExtraHealth*>(a_rhs);
		return health != rhs->health;
	}
}
