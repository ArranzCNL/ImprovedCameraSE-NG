#include "RE/P/PlayerCharacter.h"

#include "RE/T/TESObjectREFR.h"

using namespace REL;

namespace RE
{
	void PlayerCharacter::PlayerSkills::AdvanceLevel(bool a_addThreshold)
	{
		using func_t = decltype(&PlayerCharacter::PlayerSkills::AdvanceLevel);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::PlayerSkills::AdvanceLevel };
		return func(this, a_addThreshold);
	}

	PlayerCharacter* PlayerCharacter::GetSingleton()
	{
		REL::Relocation<NiPointer<PlayerCharacter>*> singleton{ Offset::PlayerCharacter::Singleton };
		return singleton->get();
	}

	void PlayerCharacter::ActivatePickRef()
	{
		using func_t = decltype(&PlayerCharacter::ActivatePickRef);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::ActivatePickRef };
		return func(this);
	}

	void PlayerCharacter::AddPlayerAddItemEvent(TESObject* a_object, TESForm* a_owner, TESObjectREFR* a_container, AQUIRE_TYPE a_type)
	{
		using func_t = decltype(&PlayerCharacter::AddPlayerAddItemEvent);
		REL::Relocation<func_t> func{ RELOCATION_ID(39384, 40456) };
		return func(this, a_object, a_owner, a_container, a_type);
	}

	bool PlayerCharacter::AttemptPickpocket(TESObjectREFR* a_containerRef, InventoryEntryData* a_entry, std::int32_t a_number, bool a_fromContainer)
	{
		using func_t = decltype(&PlayerCharacter::AttemptPickpocket);
		REL::Relocation<func_t> func{ RELOCATION_ID(39568, 40654) };
		return func(this, a_containerRef, a_entry, a_number, a_fromContainer);
	}

	bool PlayerCharacter::CenterOnCell(const char* a_cellName)
	{
		return CenterOnCell_Impl(a_cellName, nullptr);
	}

	bool PlayerCharacter::CenterOnCell(RE::TESObjectCELL* a_cell)
	{
		return CenterOnCell_Impl(nullptr, a_cell);
	}

	NiPointer<Actor> PlayerCharacter::GetActorDoingPlayerCommand() const
	{
        if SKYRIM_REL_CONSTEXPR (REL::Module::IsVR()) {
            return REL::RelocateMember<ActorHandle>(this, 0, 0xE8C).get();
        } else {
            return REL::RelocateMemberIfNewer<ActorHandle>(SKSE::RUNTIME_SSE_1_6_629, this, 0x894, 0x89C).get();
        }
	}

	float PlayerCharacter::GetArmorValue(InventoryEntryData* a_form)
	{
		using func_t = decltype(&PlayerCharacter::GetArmorValue);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::GetArmorValue };
		return func(this, a_form);
	}

	float PlayerCharacter::GetDamage(InventoryEntryData* a_form)
	{
		using func_t = decltype(&PlayerCharacter::GetDamage);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::GetDamage };
		return func(this, a_form);
	}

	NiPointer<TESObjectREFR> PlayerCharacter::GetGrabbedRef()
	{
		if SKYRIM_REL_CONSTEXPR (Module::IsVR()) {
			return nullptr;
		} else {
			return REL::RelocateMemberIfNewer<ObjectRefHandle>(SKSE::RUNTIME_SSE_1_6_629, this, 0x8C8, 0x8D0).get();
		}
	}

	std::int32_t PlayerCharacter::GetItemCount(TESBoundObject* a_object)
	{
		using func_t = decltype(&PlayerCharacter::GetItemCount);
		REL::Relocation<func_t> func{ RELOCATION_ID(19275, 19701) };
		return func(this, a_object);
	}

	std::uint32_t PlayerCharacter::GetNumTints(std::uint32_t a_tintType)
	{
		using func_t = decltype(&PlayerCharacter::GetNumTints);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::GetNumTints };
		return func(this, a_tintType);
	}

	TintMask* PlayerCharacter::GetOverlayTintMask(TintMask* a_original)
	{
		if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
			return nullptr;
		} else {
			auto* tryOverlayTintMasks = REL::RelocateMemberIfNewer<BSTArray<TintMask*>*>(SKSE::RUNTIME_SSE_1_6_629, this, 0xB28, 0xB30);
			if (!tryOverlayTintMasks) {
				return nullptr;
			}

			auto& tintMasksValue = REL::RelocateMemberIfNewer<BSTArray<TintMask*>>(SKSE::RUNTIME_SSE_1_6_629, this, 0xB10, 0xB18);
			for (std::uint32_t i = 0; i < tintMasksValue.size(); ++i) {
				if (tintMasksValue[i] == a_original) {
					return i < tryOverlayTintMasks->size() ? (*tryOverlayTintMasks)[i] : nullptr;
				}
			}

			return nullptr;
		}
	}

	BSTArray<TintMask*>* PlayerCharacter::GetTintList()
	{
		if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
			return nullptr;
		} else {
			auto* tryOverlayTintMasks = REL::RelocateMemberIfNewer<BSTArray<TintMask*>*>(SKSE::RUNTIME_SSE_1_6_629, this, 0xB28, 0xB30);
			return tryOverlayTintMasks ? tryOverlayTintMasks : &REL::RelocateMemberIfNewer<BSTArray<TintMask*>>(SKSE::RUNTIME_SSE_1_6_629, this, 0xB10, 0xB18);
		}
	}

	TintMask* PlayerCharacter::GetTintMask(std::uint32_t a_tintType, std::uint32_t a_index)
	{
		if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
			return nullptr;
		} else {
			using func_t = decltype(&PlayerCharacter::GetTintMask);
			REL::Relocation<func_t> func{ Offset::PlayerCharacter::GetTintMask };
			return func(this, a_tintType, a_index);
		}
	}

	bool PlayerCharacter::HasActorDoingCommand() const
	{
        if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
            return static_cast<bool>(REL::RelocateMember<ActorHandle>(this, 0, 0xE8C));
        }
        else {
            return static_cast<bool>(REL::RelocateMemberIfNewer<ActorHandle>(SKSE::RUNTIME_SSE_1_6_629, this, 0x894, 0x89C));
        }
	}

	bool PlayerCharacter::IsGrabbing() const
	{
		if SKYRIM_REL_CONSTEXPR (Module::IsVR()) {
			return false;
		} else {
			return static_cast<bool>(REL::RelocateMemberIfNewer<ObjectRefHandle>(SKSE::RUNTIME_SSE_1_6_629, this, 0x8C8, 0x8D0));
		}
	}

	void PlayerCharacter::PlayPickupEvent(TESForm* a_item, TESForm* a_containerOwner, TESObjectREFR* a_containerRef, EventType a_eventType)
	{
		using func_t = decltype(&PlayerCharacter::PlayPickupEvent);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::PlayPickupEvent };
		return func(this, a_item, a_containerOwner, a_containerRef, a_eventType);
	}

	void PlayerCharacter::SetAIDriven(bool a_enable)
	{
		using func_t = decltype(&PlayerCharacter::SetAIDriven);
		REL::Relocation<func_t> func{ RELOCATION_ID(39507, 40586) };
		return func(this, a_enable);
	}

	void PlayerCharacter::SetEscaping(bool a_flag, bool a_escaped)
	{
		using func_t = decltype(&PlayerCharacter::SetEscaping);
		REL::Relocation<func_t> func{ RELOCATION_ID(39574, 40660) };
		return func(this, a_flag, a_escaped);
	}

	void PlayerCharacter::StartGrabObject()
	{
		using func_t = decltype(&PlayerCharacter::StartGrabObject);
		REL::Relocation<func_t> func{ Offset::PlayerCharacter::StartGrabObject };
		return func(this);
	}

	void PlayerCharacter::UpdateCrosshairs()
	{
		using func_t = decltype(&PlayerCharacter::UpdateCrosshairs);
		REL::Relocation<func_t> func(RELOCATION_ID(39535, 40621));
		return func(this);
	}

	bool PlayerCharacter::CenterOnCell_Impl(const char* a_cellName, RE::TESObjectCELL* a_cell)
	{
		using func_t = decltype(&PlayerCharacter::CenterOnCell_Impl);
		REL::Relocation<func_t> func{ RELOCATION_ID(39365, 40437) };
		return func(this, a_cellName, a_cell);
	}

	void PlayerCharacter::AddSkillExperience(ActorValue a_skill, float a_experience)
	{
		using func_t = decltype(&PlayerCharacter::AddSkillExperience);
		REL::Relocation<func_t> func(RELOCATION_ID(39413, 40488));
		return func(this, a_skill, a_experience);
	}

	void PlayerCharacter::Unk_12A()
	{
		return RelocateVirtual<decltype(&PlayerCharacter::Unk_12A)>(0x12A, 0x12C, this);
	}

	std::uint32_t PlayerCharacter::GetViolentCrimeGoldValue(const TESFaction* a_faction) const
	{
		return RelocateVirtual<decltype(&PlayerCharacter::GetViolentCrimeGoldValue)>(0x12B, 0x12D, this, a_faction);
	}

	std::uint32_t PlayerCharacter::GetNonViolentCrimeGoldValue(const TESFaction* a_faction) const
	{
		return RelocateVirtual<decltype(&PlayerCharacter::GetNonViolentCrimeGoldValue)>(0x12C, 0x12E, this, a_faction);
	}

	void PlayerCharacter::ClearAllCrimeGold(TESFaction* a_faction)
	{
		RelocateVirtual<decltype(&PlayerCharacter::ClearAllCrimeGold)>(0x12D, 0x12F, this, a_faction);
	}

	void PlayerCharacter::Unk_12E()
	{
		return RelocateVirtual<decltype(&PlayerCharacter::Unk_12E)>(0x12E, 0x130, this);
	}
}
