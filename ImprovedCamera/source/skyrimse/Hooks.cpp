/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "skyrimse/Hooks.h"

#include "plugin.h"
#include "skyrimse/Addresses.h"
#include "skyrimse/ImprovedCameraSE.h"
#include <MinHook.h>
#include "utils/Log.h"

#include <chrono>

namespace Patch {

	static ImprovedCamera::ImprovedCameraSE* ic = nullptr;

	// Credits to Ershin. Added force closing of journal menu which caused an issue.
	struct ProcessInput {

		static void thunk(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent* const* a_event)
		{
			auto plugin = DLLMain::Plugin::Get();
			// Failsafe incase iMenuMode got downgraded.
			if (plugin->IsGraphicsInitialized() && plugin->Config()->ModuleData().iMenuMode == Systems::Window::UIDisplay::kInternal)
			{
				if (ic->ProcessInput(a_event))
				{
					// Close Journal Menu
					auto ui = RE::UI::GetSingleton();
					if (ui->IsMenuOpen("Journal Menu"))
					{
						const auto msgQueue = RE::UIMessageQueue::GetSingleton();
						msgQueue->AddMessage("Journal Menu", RE::UI_MESSAGE_TYPE::kHide, nullptr);
					}

					constexpr RE::InputEvent* const dummy[]{ nullptr };
					func(a_dispatcher, dummy);
					return;
				}
			}
			func(a_dispatcher, a_event);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct UpdateSwitchPOV {

		static void thunk(RE::PlayerCharacter* player, void* arg2)
		{
			func(player, arg2);
#ifdef _DEBUG
			LOG_INFO("UpdateSwitchPOV: called.");
#endif
			ic->UpdateSwitchPOV();
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct UpdateCamera {

		static void thunk(RE::TESCamera* tesCamera)
		{
			static std::chrono::steady_clock::time_point currentTime;
			static std::chrono::steady_clock::time_point previousTime;
			static float deltaTime;
			currentTime = std::chrono::steady_clock::now();

			// Call original function
			func(tesCamera);

			static bool init;
			static std::uint8_t prevID;
			static std::uint8_t currID;

			if (!init)
			{
				init = true;
				currID = 255;
			}
			if (currID != tesCamera->currentState.get()->id)
			{
				prevID = currID;
				currID = (std::uint8_t)tesCamera->currentState.get()->id;
#ifdef _DEBUG
				LOG_INFO("UpdateCamera: CurrentID: {} - PreviousID: {}", currID, prevID);
#endif
			}
			ic->UpdateCamera(currID, prevID, deltaTime);

			deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count() / 1000000.0f;
			previousTime = currentTime;
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct UpdateFirstPerson {

		static void thunk(RE::NiAVObject* firstpersonObject, RE::NiUpdateData* updateData)
		{
			func(firstpersonObject, updateData);
			ic->UpdateFirstPerson();
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct TESObjectCell {

		static RE::NiNode* thunk(RE::TESObjectREFR* objectREFR)
		{
			// Replace func with Get3D as a node
			return Address::Function::Get3D(objectREFR);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 6 };
	};

	struct SmoothAnimationTransitions {

		static bool thunk(RE::Actor* actor)
		{
			if (actor)
			{
				return ic->SmoothAnimationTransitions();
			}
			return false;
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 6 };
	};

	struct HeadTracking {

		static void thunk(void* arg1, void* arg2, void* arg3)
		{
			if (ic->UpdateHeadTracking())
				func(arg1, arg2, arg3);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t index{ 23 };
	};

	struct ShaderReferenceEffect_Update {

		struct Patch1 {

			static void Install()
			{
				struct Patch : Xbyak::CodeGenerator {

					Patch(std::uintptr_t func)
					{
						Xbyak::Label f;

						mov(rdx, rbx);  // RE::Actor*
						jmp(ptr[rip + f]);

						L(f);
						dq(func);
					}
				};

				Patch patch{ reinterpret_cast<std::uintptr_t>(Effect) };
				patch.ready();

				auto& trampoline = SKSE::GetTrampoline();
				SKSE::AllocTrampoline(32);

				_Effect = trampoline.write_call<5>(Address::Hook::ShaderReferenceEffect1, trampoline.allocate(patch));
			}

		private:
			static bool Effect(void* arg1, RE::Actor* actor)
			{
				return ic->ShaderReferenceEffectFix1(arg1, actor);
			}
			static inline REL::Relocation<bool()> _Effect;
		};

		struct Patch2 {

			static void Install()
			{
				struct Patch : Xbyak::CodeGenerator {

					Patch(std::uintptr_t func)
					{
						Xbyak::Label f;

						mov(rdx, rbx);  // RE::Actor*
						jmp(ptr[rip + f]);

						L(f);
						dq(func);
					}
				};

				Patch patch{ reinterpret_cast<std::uintptr_t>(Effect) };
				patch.ready();

				auto& trampoline = SKSE::GetTrampoline();
				SKSE::AllocTrampoline(32);

				_Effect = trampoline.write_call<6>(Address::Hook::ShaderReferenceEffect2, trampoline.allocate(patch));
			}

		private:
			static void Effect(void* arg1, RE::Actor* actor)
			{
				ic->ShaderReferenceEffectFix2(arg1, actor);
			}
			static inline REL::Relocation<void()> _Effect;
		};
	};

	struct GetEffectNode_IsThirdPerson {

		static bool thunk(void*)
		{
			auto pluginConfig = DLLMain::Plugin::Get()->Config();
			auto player = RE::PlayerCharacter::GetSingleton();
			auto camera = RE::PlayerCamera::GetSingleton();
			bool rtnVal = true;

			if (camera->IsInFirstPerson())
			{
				rtnVal = player->AsActorState()->IsWeaponDrawn() ? pluginConfig->General().bEnableThirdPersonArms : 0;
			}
			return rtnVal;
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 6 };
	};

	struct ForceFirstPerson {

		static void func()
		{
			auto camera = RE::PlayerCamera::GetSingleton();
			camera->ForceFirstPerson();

			ic->ForceFirstPerson();
			return;
		}
		static inline constexpr std::size_t size{ 0xC };
	};

	struct ForceThirdPerson {

		static void func()
		{
			auto camera = RE::PlayerCamera::GetSingleton();
			camera->ForceThirdPerson();

			ic->ForceThirdPerson();
			return;
		}
		static inline constexpr std::size_t size{ 0xC };
	};

	struct Ragdoll {

		static void* thunk(void* BSTaskPool, RE::Actor* actor, float arg3, float arg4, float arg5, float arg6)
		{
			ic->Ragdoll(actor);
			return func(BSTaskPool, actor, arg3, arg4, arg5, arg6);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct Ragdoll_IsTaskPoolRequired {

		static void Install()
		{
			struct Patch : Xbyak::CodeGenerator {

				Patch(std::uintptr_t func)
				{
					Xbyak::Label f;

					mov(rcx, rbx);  // RE::Actor*
					jmp(ptr[rip + f]);

					L(f);
					dq(func);
				}
			};

			Patch patch{ reinterpret_cast<std::uintptr_t>(IsTaskPoolRequired) };
			patch.ready();

			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(32);

			_IsTaskPoolRequired = trampoline.write_call<5>(Address::Hook::Ragdoll_IsTaskPoolRequired, trampoline.allocate(patch));
		}

	private:
		static bool IsTaskPoolRequired(RE::Actor* actor)
		{
			if (!Address::Function::Ragdoll_IsTaskPoolRequired())
				return ic->Ragdoll_IsTaskPoolRequired(actor);

			return _IsTaskPoolRequired();
		}
		static inline REL::Relocation<bool()> _IsTaskPoolRequired;
	};

	struct Ragdoll_UpdateObjectUpwards {

		static void thunk(RE::NiAVObject* object, RE::NiUpdateData* updateData)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			auto playerObject = player->Get3D();

			if (playerObject == object)
				ic->Ragdoll_UpdateObjectUpwards(player);

			func(object, updateData);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct RagdollDeath {

		static void* thunk(void* BSTaskPool, RE::Actor* actor, float arg3, void* arg4, std::uint8_t arg5, std::uint8_t arg6)
		{
			ic->Ragdoll(actor);
			return func(BSTaskPool, actor, arg3, arg4, arg5, arg6);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct KillActor {

		static void* thunk(RE::Actor* actor, void* arg2, float arg3, std::uint8_t arg4, std::uint8_t arg5)
		{
			ic->Ragdoll_UpdateObjectUpwards(actor);
			return func(actor, arg2, arg3, arg4, arg5);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };
	};

	struct TogglePOV {

		static void thunk(RE::TogglePOVHandler* povHandler, RE::ButtonEvent* buttonEvent, RE::PlayerControlsData* controlsData)
		{
			// buttonEvent->value should be either bool or an int.
			if (povHandler->pressRegistered && buttonEvent->value == 0.0f && buttonEvent->heldDownSecs < 0.3f)
			{
#ifdef _DEBUG
				LOG_INFO("TogglePOV: Value: {} - Held: {}", buttonEvent->value, buttonEvent->heldDownSecs);
#endif
				ic->TogglePOV();
			}
			func(povHandler, buttonEvent, controlsData);
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t index{ 4 };
	};

	typedef void(STDMETHODCALLTYPE* NiCamera_Update_Hook)(RE::NiCamera*, float, float, float, std::uint32_t, std::uint32_t, std::uint8_t, std::uint8_t, float);
	static NiCamera_Update_Hook NiCameraUpdate;
	static NiCamera_Update_Hook NiCameraUpdateTarget;

	struct NiCamera_Update
	{
		static void Hook_NiCamera_Update(RE::NiCamera* camera, float fFov, float fNear, float fFar, std::uint32_t iScreenWidth, std::uint32_t iScreenHeight, std::uint8_t unk7, std::uint8_t unk8, float fFov2)
		{
			fNear = ic->UpdateNearDistance(fNear);
			NiCameraUpdate(camera, fFov, fNear, fFar, iScreenWidth, iScreenHeight, unk7, unk8, fFov2);
		}

		static void Install()
		{
			NiCameraUpdateTarget = reinterpret_cast<NiCamera_Update_Hook>(Address::Hook::NiCameraUpdate);

			auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();
			std::uintptr_t baseAddress = pluginSkyrimSE->BaseAddress();

			std::uint8_t nearDistancePayload[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			REL::safe_write(Address::Hook::NearDistanceIndoorsFix, nearDistancePayload, sizeof(nearDistancePayload));

			MH_STATUS status = MH_Initialize();
			if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to Initialize");
				return;
			}
			status = MH_CreateHook(reinterpret_cast<LPVOID>(NiCameraUpdateTarget), reinterpret_cast<LPVOID>(Hook_NiCamera_Update), reinterpret_cast<LPVOID*>(&NiCameraUpdate));
			if (status != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to CreateHook for NiCamera::sub_14{:07X}. Error Code: {}", Address::Hook::NiCameraUpdate - baseAddress, (std::int32_t)status);
				return;
			}
			if (MH_EnableHook(reinterpret_cast<LPVOID>(NiCameraUpdateTarget)) != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to EnableHook for NiCamera::sub_14{:07X}", Address::Hook::NiCameraUpdate - baseAddress);
				return;
			}
			LOG_TRACE("  MinHook:\t\t\t\tHooked NiCamera::sub_14{:07X}", Address::Hook::NiCameraUpdate - baseAddress);
		}
	};

	typedef void(STDMETHODCALLTYPE* FadeOutGame_Hook)(bool, bool, float, bool, float);
	static FadeOutGame_Hook FadeOutGame;
	static FadeOutGame_Hook FadeOutGameTarget;

	struct FadeOutGame_Update
	{
		static void Hook_FadeOutGame(bool a_fadingOut, bool a_blackFade, float a_fadeDuration, bool a_unk, float a_secsBeforeFade)
		{
			// LOG_INFO("FadeOutGame:\n\tFadingOut: {}\n\tBlackFade: {}\n\tFadeDuration: {}\n\tUnk: {}\n\tSecsBeforeFade: {}", a_fadingOut, a_blackFade, a_fadeDuration, a_unk, a_secsBeforeFade);

			if (!a_fadingOut && a_unk)
				a_secsBeforeFade *= 1.5f;

			FadeOutGame(a_fadingOut, a_blackFade, a_fadeDuration, a_unk, a_secsBeforeFade);
		}

		static void Install()
		{
			FadeOutGameTarget = reinterpret_cast<FadeOutGame_Hook>(Address::Hook::FadeOutGame);

			auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();
			std::uintptr_t baseAddress = pluginSkyrimSE->BaseAddress();

			MH_STATUS status = MH_Initialize();
			if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to Initialize");
				return;
			}
			status = MH_CreateHook(reinterpret_cast<LPVOID>(FadeOutGameTarget), reinterpret_cast<LPVOID>(Hook_FadeOutGame), reinterpret_cast<LPVOID*>(&FadeOutGame));
			if (status != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to CreateHook for FadeOutGame (sub_14{:07X}). Error Code: {}", Address::Hook::FadeOutGame - baseAddress, (std::int32_t)status);
				return;
			}
			if (MH_EnableHook(reinterpret_cast<LPVOID>(FadeOutGameTarget)) != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to EnableHook for FadeOutGame (sub_14{:07X})", Address::Hook::FadeOutGame - baseAddress);
				return;
			}
			LOG_TRACE("  MinHook:\t\t\t\tHooked FadeOutGame (sub_14{:07X})", Address::Hook::FadeOutGame - baseAddress);
		}
	};

	typedef void(STDMETHODCALLTYPE* PlayerCameraUnk0_Hook)(std::uint64_t, float*, char);
	static PlayerCameraUnk0_Hook PlayerCameraUnk0;
	static PlayerCameraUnk0_Hook PlayerCameraUnk0Target;
	static const char* fadePayload[8];

	struct PlayerCameraUnk0_Update {

		static void Hook_PlayerCameraUnk0(std::uint64_t unk1, float* unk2, char unk3)
		{
			// Patch Thirdperson fading
			static bool displayTweaks;
			static std::uint8_t currentPayload[8]{};
			static std::uint8_t payload[8]{};

			if (!displayTweaks)
			{
				if (currentPayload[0] == 0)
				{
					std::memcpy(reinterpret_cast<void*>(currentPayload), reinterpret_cast<const void*>(Address::Hook::ThirdpersonFade + REL::VariantOffset(0x431, 0x4DD, 0).offset()), sizeof(currentPayload));
					if (currentPayload[2] == 0x90)
					{
						displayTweaks = true;
						PlayerCameraUnk0(unk1, unk2, unk3);
						return;
					}
				}

				bool fadePlayer = ic->IsFirstPerson();
				if (fadePlayer && payload[0] != 0xEB)
				{
					std::memcpy(reinterpret_cast<void*>(payload), reinterpret_cast<const void*>(fadePayload), sizeof(payload));
					// Create a short jump.
					payload[0] = 0xEB;
					payload[1] = 0x58;
					REL::safe_write(Address::Hook::ThirdpersonFade + REL::VariantOffset(0x431, 0x4DD, 0).offset(), payload, sizeof(payload));
				}
				else if (!fadePlayer && payload[0] != 0xF3)
				{
					// Copy and restore the original fade state.
					std::memcpy(reinterpret_cast<void*>(payload), reinterpret_cast<const void*>(fadePayload), sizeof(payload));
					REL::safe_write(Address::Hook::ThirdpersonFade + REL::VariantOffset(0x431, 0x4DD, 0).offset(), payload, sizeof(payload));
				}
			}
			PlayerCameraUnk0(unk1, unk2, unk3);
		}

		static void Install()
		{
			PlayerCameraUnk0Target = reinterpret_cast<PlayerCameraUnk0_Hook>(Address::Hook::ThirdpersonFade);

			auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();
			std::uintptr_t baseAddress = pluginSkyrimSE->BaseAddress();

			MH_STATUS status = MH_Initialize();
			if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to Initialize");
				return;
			}
			status = MH_CreateHook(reinterpret_cast<LPVOID>(PlayerCameraUnk0Target), reinterpret_cast<LPVOID>(Hook_PlayerCameraUnk0), reinterpret_cast<LPVOID*>(&PlayerCameraUnk0));
			if (status != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to CreateHook for PlayerCamera::sub_14{:07X}. Error Code: {}", Address::Hook::ThirdpersonFade - baseAddress, (std::int32_t)status);
				return;
			}
			if (MH_EnableHook(reinterpret_cast<LPVOID>(PlayerCameraUnk0Target)) != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to EnableHook for PlayerCamera::sub_14{:07X}", Address::Hook::ThirdpersonFade - baseAddress);
				return;
			}
			std::uintptr_t patch = Address::Hook::ThirdpersonFade + REL::VariantOffset(0x431, 0x4DD, 0).offset();
			std::memcpy(reinterpret_cast<void*>(fadePayload), reinterpret_cast<const void*>(patch), 8);
			LOG_TRACE("  MinHook:\t\t\t\tHooked PlayerCamera::sub_14{:07X}", Address::Hook::ThirdpersonFade - baseAddress);
		}
	};

	Hooks::~Hooks()
	{
		ic = nullptr;
	}

	void Hooks::Install()
	{
		Setup();
		auto plugin = DLLMain::Plugin::Get();
		auto pluginSkyrimSE = plugin->SkyrimSE();
		ic = pluginSkyrimSE->Camera();

		stl::write_thunk_call<UpdateSwitchPOV>(Address::Hook::UpdateSwitchPOV);
		stl::write_thunk_call<UpdateCamera>(Address::Hook::UpdateCamera);
		stl::write_thunk_call<UpdateFirstPerson>(Address::Hook::UpdateFirstPerson);
		stl::write_thunk_call<TESObjectCell>(Address::Hook::TESObjectCell_Get3D);  // Fixes body being hidden indoors
		stl::write_thunk_call<SmoothAnimationTransitions>(Address::Hook::SmoothAnimationTransitions);
		stl::write_vfunc<RE::BSLookAtModifier, HeadTracking>();
		stl::write_vfunc<RE::TogglePOVHandler, TogglePOV>();
		stl::write_thunk_call<GetEffectNode_IsThirdPerson>(Address::Hook::GetEffectNode_IsThirdPerson);  // Fixes firstperson magic casting for thirdperson arms
		stl::asm_replace<ForceFirstPerson>(Address::Hook::ForceFirstPerson);
		stl::asm_replace<ForceThirdPerson>(Address::Hook::ForceThirdPerson);
		// Ragdoll fixes
		stl::write_thunk_call<Ragdoll>(Address::Hook::Ragdoll);
		Ragdoll_IsTaskPoolRequired::Install();
		stl::write_thunk_call<Ragdoll_UpdateObjectUpwards>(Address::Hook::Ragdoll_UpdateObjectUpwards);
		stl::write_thunk_call<RagdollDeath>(Address::Hook::RagdollDeath);
		stl::write_thunk_call<KillActor>(Address::Hook::KillActor);
		// Shader fixes
		ShaderReferenceEffect_Update::Patch1::Install();
		ShaderReferenceEffect_Update::Patch2::Install();
		// Update zNear, this one fixes water going opaque.
		NiCamera_Update::Install();
		// Fix third person player fading.
		if (!GetModuleHandle("OStim.dll"))
			PlayerCameraUnk0_Update::Install();
		// Fix CaptureWarmer fading.
		if (GetModuleHandle("CaptureWarmer.dll"))
			FadeOutGame_Update::Install();

		// Patch Horse looking downwards
		std::uint8_t horsePayload[] = { 0x66, 0x90 };
		if (pluginSkyrimSE->Build() > SkyrimSE::BuildInfo::k15970)
		{
			REL::safe_write(Address::Hook::HorseLookingDownFix1 + 0x16, horsePayload, sizeof(horsePayload));
			REL::safe_write(Address::Hook::HorseLookingDownFix2 + 0x5F, horsePayload, sizeof(horsePayload));
			REL::safe_write(Address::Hook::HorseLookingDownFix3 + 0x66, horsePayload, sizeof(horsePayload));
		}
		else if (pluginSkyrimSE->Build() == SkyrimSE::BuildInfo::k15970)
			REL::safe_write(Address::Hook::HorseLookingDownFix1, horsePayload, sizeof(horsePayload));
	}

	void Hooks::Input()
	{
		auto plugin = DLLMain::Plugin::Get();
		// No point activating this hook unless Menu is internal.
		if (plugin->Config()->ModuleData().iMenuMode == Systems::Window::UIDisplay::kInternal)
		{
			stl::write_thunk_call<ProcessInput>(Address::Hook::ProcessInput);
		}
	}

	void Hooks::Setup()
	{
		Address::Hook::ProcessInput = REL::RelocationID(67315, 68617).address() + 0x7B;
		Address::Hook::UpdateSwitchPOV = REL::RelocationID(39401, 40476).address() + REL::VariantOffset(0x2AF, 0x294, 0).offset();
		Address::Hook::UpdateCamera = REL::RelocationID(49852, 50784).address() + 0x1A6;
		Address::Hook::UpdateFirstPerson = REL::RelocationID(39446, 40522).address() + 0xD7;
		Address::Hook::TESObjectCell_Get3D = REL::RelocationID(18683, 19165).address() + REL::VariantOffset(0x7C, 0x7B, 0).offset();
		Address::Hook::SmoothAnimationTransitions = REL::RelocationID(40937, 41996).address() + REL::VariantOffset(0x2EA, 0x2F4, 0).offset();
		Address::Hook::ShaderReferenceEffect1 = REL::RelocationID(34111, 34913).address() + REL::VariantOffset(0xE1, 0xE1, 0).offset();
		Address::Hook::ShaderReferenceEffect2 = REL::RelocationID(34111, 34913).address() + REL::VariantOffset(0x18A, 0x1F5, 0).offset();
		Address::Hook::GetEffectNode_IsThirdPerson = REL::RelocationID(33361, 34142).address() + REL::VariantOffset(0x51, 0x51, 0).offset();
		Address::Hook::ForceFirstPerson = REL::RelocationID(54947, 55564).address();
		Address::Hook::ForceThirdPerson = REL::RelocationID(54948, 55565).address();
		Address::Hook::Ragdoll = REL::RelocationID(38858, 39895).address() + REL::VariantOffset(0x71, 0x5B, 0).offset();
		Address::Hook::Ragdoll_IsTaskPoolRequired = REL::RelocationID(38858, 39895).address() + REL::VariantOffset(0x41, 0x2E, 0).offset();
		Address::Hook::Ragdoll_UpdateObjectUpwards = REL::RelocationID(38858, 39895).address() + REL::VariantOffset(0x37D, 0x2C5, 0).offset();
		Address::Hook::RagdollDeath = REL::RelocationID(36326, 37316).address() + REL::VariantOffset(0x85, 0x76, 0).offset();
		Address::Hook::KillActor = REL::RelocationID(39646, 40733).address() + REL::VariantOffset(0xEC, 0xEC, 0).offset();
		Address::Hook::ThirdpersonFade = REL::RelocationID(49899, 50832).address();
		Address::Hook::HorseLookingDownFix1 = REL::RelocationID(36602, 37356).address() + REL::VariantOffset(0x77, 0xB4E, 0).offset();
		Address::Hook::HorseLookingDownFix2 = REL::RelocationID(36602, 37610).address() + REL::VariantOffset(0x77, 0x19, 0).offset();
		Address::Hook::HorseLookingDownFix3 = REL::RelocationID(36602, 37611).address() + REL::VariantOffset(0x77, 0x19, 0).offset();
		Address::Hook::NearDistanceIndoorsFix = REL::RelocationID(100407, 107125).address() + 0x14;
		Address::Hook::NiCameraUpdate = REL::RelocationID(69273, 70643).address();
		Address::Hook::FadeOutGame = REL::RelocationID(51909, 52847).address();

		Address::Variable::NiNodeGlobalTime = (float*)REL::RelocationID(514188, 400333).address();
		Address::Variable::fDefaultWorldFOV = (float*)REL::RelocationID(512129, 388785).address();
		Address::Variable::fMinCurrentZoom = (float*)REL::RelocationID(509882, 382633).address();
		Address::Variable::fSittingMaxLookingDown = (float*)REL::RelocationID(503108, 371032).address();
		Address::Variable::fMountedMaxLookingUp = (float*)REL::RelocationID(509846, 382579).address();
		Address::Variable::fMountedMaxLookingDown = (float*)REL::RelocationID(503104, 371026).address();
		Address::Variable::fFlyingMaxLookingUp = (float*)REL::RelocationID(508527, 380228).address();
		Address::Variable::fFlyingMaxLookingDown = (float*)REL::RelocationID(503106, 371029).address();

#ifdef _DEBUG
		AddressDump();
	}

	void Hooks::AddressDump()
	{
		auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();
		std::uintptr_t baseAddress = pluginSkyrimSE->BaseAddress();

		LOG_INFO("Dumping addresses for {} v{}.{}.{}.{}...", pluginSkyrimSE->Name().c_str(),
			pluginSkyrimSE->VersionMajor(), pluginSkyrimSE->VersionMinor(), pluginSkyrimSE->VersionRevision(), pluginSkyrimSE->VersionBuild());

		LOG_DEBUG("Hook::ProcessInput:\t\t\t\t0x{:08X}", Address::Hook::ProcessInput - baseAddress);

		LOG_DEBUG("Hook::UpdateSwitchPOV:\t\t\t0x{:08X}", Address::Hook::UpdateSwitchPOV - baseAddress);
		LOG_DEBUG("Hook::UpdateCamera:\t\t\t\t0x{:08X}", Address::Hook::UpdateCamera - baseAddress);
		LOG_DEBUG("Hook::UpdateFirstPerson:\t\t\t0x{:08X}", Address::Hook::UpdateFirstPerson - baseAddress);
		LOG_DEBUG("Hook::TESObjectCell_Get3D:\t\t\t0x{:08X}", Address::Hook::TESObjectCell_Get3D - baseAddress);
		LOG_DEBUG("Hook::NiCameraUpdate:\t\t\t0x{:08X}", Address::Hook::NiCameraUpdate - baseAddress);
		LOG_DEBUG("Hook::FadeOutGame:\t\t\t\t0x{:08X}", Address::Hook::FadeOutGame - baseAddress);
		LOG_DEBUG("Hook::SmoothAnimationTransitions:\t\t0x{:08X}", Address::Hook::SmoothAnimationTransitions - baseAddress);
		LOG_DEBUG("Hook::ShaderReferenceEffect1:\t\t0x{:08X}", Address::Hook::ShaderReferenceEffect1 - baseAddress);
		LOG_DEBUG("Hook::ShaderReferenceEffect2:\t\t0x{:08X}", Address::Hook::ShaderReferenceEffect2 - baseAddress);
		LOG_DEBUG("Hook::GetEffectNode_IsThirdPerson:\t\t0x{:08X}", Address::Hook::GetEffectNode_IsThirdPerson - baseAddress);
		LOG_DEBUG("Hook::ForceFirstPerson:\t\t\t0x{:08X}", Address::Hook::ForceFirstPerson - baseAddress);
		LOG_DEBUG("Hook::ForceThirdPerson:\t\t\t0x{:08X}", Address::Hook::ForceThirdPerson - baseAddress);
		LOG_DEBUG("Hook::Ragdoll:\t\t\t\t0x{:08X}", Address::Hook::Ragdoll - baseAddress);
		LOG_DEBUG("Hook::Ragdoll_IsTaskPoolRequired:\t\t0x{:08X}", Address::Hook::Ragdoll_IsTaskPoolRequired - baseAddress);
		LOG_DEBUG("Hook::Ragdoll_UpdateObjectUpwards:\t\t0x{:08X}", Address::Hook::Ragdoll_UpdateObjectUpwards - baseAddress);
		LOG_DEBUG("Hook::RagdollDeath:\t\t\t\t0x{:08X}", Address::Hook::RagdollDeath - baseAddress);
		LOG_DEBUG("Hook::KillActor:\t\t\t\t0x{:08X}", Address::Hook::KillActor - baseAddress);
		LOG_DEBUG("Hook::ThirdpersonFade:\t\t\t0x{:08X}", Address::Hook::ThirdpersonFade - baseAddress);
		LOG_DEBUG("Hook::HorseLookingDownFix1:\t\t\t0x{:08X}", Address::Hook::HorseLookingDownFix1 - baseAddress);
		LOG_DEBUG("Hook::HorseLookingDownFix2:\t\t\t0x{:08X}", Address::Hook::HorseLookingDownFix2 - baseAddress);
		LOG_DEBUG("Hook::HorseLookingDownFix3:\t\t\t0x{:08X}", Address::Hook::HorseLookingDownFix3 - baseAddress);
		LOG_DEBUG("Hook::NearDistanceIndoorsFix:\t\t0x{:08X}", Address::Hook::NearDistanceIndoorsFix - baseAddress);

		LOG_DEBUG("Variable::NiNodeGlobalTime:\t\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::NiNodeGlobalTime) - baseAddress);
		LOG_DEBUG("Variable::fDefaultWorldFOV:\t\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fDefaultWorldFOV) - baseAddress);
		LOG_DEBUG("Variable::fMinCurrentZoom:\t\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fMinCurrentZoom) - baseAddress);
		LOG_DEBUG("Variable::fSittingMaxLookingDown:\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fSittingMaxLookingDown) - baseAddress);
		LOG_DEBUG("Variable::fMountedMaxLookingUp:\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fMountedMaxLookingUp) - baseAddress);
		LOG_DEBUG("Variable::fMountedMaxLookingDown:\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fMountedMaxLookingDown) - baseAddress);
		LOG_DEBUG("Variable::fFlyingMaxLookingUp:\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fFlyingMaxLookingUp) - baseAddress);
		LOG_DEBUG("Variable::fFlyingMaxLookingDown:\t\t0x{:08X}", (std::uintptr_t)std::addressof(*Address::Variable::fFlyingMaxLookingDown) - baseAddress);

		LOG_INFO("Finished dumping addresses.");
#endif
	}

}
