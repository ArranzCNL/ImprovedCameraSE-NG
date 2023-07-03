#include "RE/P/Projectile.h"
#include "RE/F/FormTraits.h"

using namespace REL;

namespace RE
{
#ifdef SKYRIM_CROSS_VR
	bool Projectile::IsMissileProjectile()
	{
		return RelocateVirtual<bool(Projectile*)>(0xA2, 0xA3, this);
	}

	bool Projectile::IsGrenadeProjectile()
	{
		return RelocateVirtual<bool(Projectile*)>(0xA3, 0xA4, this);
	}

	bool Projectile::IsFlameProjectile()
	{
		return RelocateVirtual<bool(Projectile*)>(0xA4, 0xA5, this);
	}

	bool Projectile::IsBeamProjectile()
	{
		return RelocateVirtual<bool(Projectile*)>(0xA5, 0xA6, this);
	}

	void Projectile::Unk_A6()
	{
		RelocateVirtual<void(Projectile*)>(0xA6, 0xA7, this);
	}

	bool Projectile::IsBarrierProjectile()
	{
		return RelocateVirtual<bool(Projectile*)>(0xA7, 0xA8, this);
	}

	void Projectile::OnKill()
	{
		RelocateVirtual<void(Projectile*)>(0xA8, 0xA9, this);
	}

	void Projectile::Process3D()
	{
		RelocateVirtual<void(Projectile*)>(0xA9, 0xAA, this);
	}

	void Projectile::PostLoad3D(NiAVObject* a_root)
	{
		RelocateVirtual<void(Projectile*, NiAVObject*)>(0xAA, 0xAB, this, a_root);
	}

	void Projectile::UpdateImpl(float a_delta)
	{
		RelocateVirtual<decltype(&Projectile::UpdateImpl)>(0xAB, 0xAC, this, a_delta);
	}

	bool Projectile::ProcessImpacts()
	{
		return RelocateVirtual<bool(Projectile*)>(0xAC, 0xAD, this);
	}

	void Projectile::Update3D()
	{
		RelocateVirtual<void(Projectile*)>(0xAD, 0xAE, this);
	}

	void Projectile::Unk_AE()
	{
		RelocateVirtual<void(Projectile*)>(0xAE, 0xAF, this);
	}

	float Projectile::GetPowerSpeedMult() const
	{
		return RelocateVirtual<float(const Projectile*)>(0xAF, 0xB0, this);
	}

	float Projectile::GetWeaponSpeedMult() const
	{
		return RelocateVirtual<float(const Projectile*)>(0xB0, 0xB1, this);
	}

	bool Projectile::GetStopMainSoundAfterImpact()
	{
		return RelocateVirtual<bool(Projectile*)>(0xB1, 0xB2, this);
	}

	void Projectile::ReportHavokDeactivation()
	{
		RelocateVirtual<void(Projectile*)>(0xB2, 0xB3, this);
	}

	bool Projectile::TurnOff(Actor* a_owner, bool a_noDeactivateSound)
	{
		return RelocateVirtual<bool(Projectile*, Actor*, bool)>(0xB3, 0xB4, this, a_owner, a_noDeactivateSound);
	}

	bool Projectile::IsPermanent() const
	{
		return RelocateVirtual<decltype(&Projectile::IsPermanent)>(0xB4, 0xB5, this);
	}

	float Projectile::GetGravity()
	{
		return RelocateVirtual<float(Projectile*)>(0xB5, 0xB6, this);
	}

	void Projectile::CleanUpPointersOnDisable()
	{
		RelocateVirtual<void(Projectile*)>(0xB6, 0xB7, this);
	}

	bool Projectile::RunTargetPick()
	{
		return RelocateVirtual<bool(Projectile*)>(0xB7, 0xB8, this);
	}

	bool Projectile::GetKillOnCollision()
	{
		return RelocateVirtual<bool(Projectile*)>(0xB8, 0xB9, this);
	}

	bool Projectile::ShouldBeLimited()
	{
		return RelocateVirtual<bool(Projectile*)>(0xB9, 0xBA, this);
	}

	bool Projectile::TargetsWholeBody()
	{
		return RelocateVirtual<bool(Projectile*)>(0xBA, 0xBB, this);
	}

	std::uint32_t Projectile::GetCollisionGroup()
	{
		return RelocateVirtual<std::uint32_t(Projectile*)>(0xBB, 0xBC, this);
	}

	bhkShape* Projectile::GetCollisionShape()
	{
		return RelocateVirtual<bhkShape*(Projectile*)>(0xBC, 0xBD, this);
	}

	void Projectile::AddImpact(TESObjectREFR* a_ref, const NiPoint3& a_targetLoc, const NiPoint3& a_velocity, hkpCollidable* a_collidable, std::int32_t a_arg6, std::uint32_t a_arg7)
	{
		RelocateVirtual<decltype(&Projectile::AddImpact)>(0xBD, 0xBE, this, a_ref, a_targetLoc, a_velocity, a_collidable, a_arg6, a_arg7);
	}

	bool Projectile::HandleHits(hkpCollidable* a_collidable)
	{
		return RelocateVirtual<bool(Projectile*, hkpCollidable*)>(0xBE, 0xBF, this, a_collidable);
	}

	void Projectile::OnTriggerEnter()
	{
		RelocateVirtual<void(Projectile*)>(0xBF, 0xC0, this);
	}

	void Projectile::Handle3DLoaded()
	{
		RelocateVirtual<decltype(&Projectile::Handle3DLoaded)>(0xC0, 0xC1, this);
	}

	bool Projectile::ShouldUseDesiredTarget()
	{
		return RelocateVirtual<bool(Projectile*)>(0xC1, 0xC2, this);
	}
#endif

	Projectile::Manager* Projectile::Manager::GetSingleton()
	{
		REL::Relocation<Projectile::Manager**> singleton{ RELOCATION_ID(514313, 400473) };
		return *singleton;
	}

	float Projectile::GetHeight() const
	{
		auto obj = GetObjectReference();
		auto projectile = obj ? obj->As<BGSProjectile>() : nullptr;

		return projectile ? projectile->data.collisionRadius * 2 : 0.0f;
	}

	float Projectile::GetSpeed() const
	{
		auto obj = GetObjectReference();
		auto projectile = obj ? obj->As<BGSProjectile>() : nullptr;

		if (!projectile) {
			return 0.0f;
		}

		return projectile->data.speed * GetPowerSpeedMult() * GetWeaponSpeedMult() * GetProjectileRuntimeData().speedMult;
	}

	BSPointerHandle<Projectile>* Projectile::Launch(BSPointerHandle<Projectile>* a_result, LaunchData& a_data) noexcept
	{
		using func_t = decltype(&Projectile::Launch);
		REL::Relocation<func_t> func{ RELOCATION_ID(42928, 44108) };
		return func(a_result, a_data);
	}
}
