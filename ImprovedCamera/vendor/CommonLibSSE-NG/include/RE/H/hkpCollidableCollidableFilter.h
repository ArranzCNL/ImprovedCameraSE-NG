#pragma once

namespace RE
{
	class hkpCollidable;

	class hkpCollidableCollidableFilter
	{
	public:
		inline static constexpr auto RTTI = RTTI_hkpCollidableCollidableFilter;

		virtual ~hkpCollidableCollidableFilter();  // 00

		// add
		virtual bool IsCollisionEnabled(const hkpCollidable& a_collidableA, const hkpCollidable& a_collidableB) const = 0;  // 01
	};
	static_assert(sizeof(hkpCollidableCollidableFilter) == 0x8);
}
