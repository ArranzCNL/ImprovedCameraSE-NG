/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "api/SmoothCamAPI.h"
#include "api/TrueDirectionalMovementAPI.h"
#include "cameras/ICamera.h"

namespace Events {
	class Observer;
}

namespace ImprovedCamera {

	class ImprovedCameraSE {

	public:
		ImprovedCameraSE();
		~ImprovedCameraSE() = default;

	public:
		bool ProcessInput(const RE::InputEvent* const* a_event);

		void UpdateSwitchPOV();
		void UpdateCamera(std::uint8_t currentID, std::uint8_t previousID, float deltaTime);
		void UpdateFirstPerson();
		bool SmoothAnimationTransitions();
		bool UpdateHeadTracking() const;
		bool ShaderReferenceEffectFix1(void* pThis, RE::Actor* actor);
		void ShaderReferenceEffectFix2(void* pThis, RE::Actor* actor);
		void ResetPlayerNodes();
		void ForceFirstPerson();
		void ForceThirdPerson();
		void TogglePOV();
		void ResetState(bool forced = false);
		void Ragdoll(RE::Actor* actor);
		bool Ragdoll_IsTaskPoolRequired(RE::Actor* actor) const;
		void Ragdoll_UpdateObjectUpwards(RE::Actor* actor);

		bool IsFirstPerson() const { return m_IsFirstPerson; }
		float UpdateNearDistance();

		void RequestAPIs();
		void DetectMods();

	private:
		class NodeOverride {

		public:
			NodeOverride(RE::NiNode* node, float scale) :
				node(node)
			{
				old_scale = node->local.scale;
				node->local.scale = scale;
			}
			~NodeOverride()
			{
				node->local.scale = old_scale;
			}

		private:
			RE::NiNode* node;
			float old_scale;
		};

		bool UseThirdPersonArms();
		bool UseThirdPersonLeftArm();
		bool UseThirdPersonRightArm();

		void UpdateSkeleton(bool show);
		void FixWeaponPosition(RE::Actor* actor, RE::NiNode* firstpersonNode, RE::NiNode* thirdpersonNode);
		void DisplayShadows(bool show);

		void UpdateFOV(RE::PlayerCamera* camera);

		void TranslateCamera();
		void TranslateFirstPersonModel();
		void TranslateThirdPersonModel();
		void AdjustModelPosition(RE::NiPoint3& position, bool headbob);

		void UpdateLootAtPosition();
		bool GetHeadRotation(float* rotation);
		bool HeadRotation();
		void ScalePoint(RE::NiPoint3* point, float scale);

		void ReleaseAPIs();
		void SetupCameraData();

	private:
		Systems::Config* m_pluginConfig = nullptr;

		Interface::Camera m_Camera{};
		Interface::ICamera* m_ICamera = nullptr;

		std::uint8_t m_CameraEventID = CameraEvent::kFirstPerson;
		float m_DeltaTime = 0.0f;
		uint8_t m_PreviousCameraID = 255;
		uint8_t m_CurrentCameraID = 255;

		bool m_IsThirdPersonForced = false;
		bool m_IsFirstPerson = false;
		bool m_IsFakeCamera = false;
		bool m_TweenShown = false;
		uint8_t m_LastStateID = 0;
		uint8_t m_iRagdollFrame = 0;
		RE::NiPoint3 m_thirdpersonLocalTranslate{};

		SKSE::PluginHandle m_ICHandle = 0;

		static inline SmoothCamAPI::IVSmoothCam3* m_SmoothCamAPI = nullptr;
		static inline TDM_API::IVTDM2* m_TDMAPI = nullptr;
		bool m_SmoothCamSnapshot = false;
		bool m_TDMSnapshot = false;

		// Need to shut these off when needed with TDM to stop horse issues.
		//    Since we are doing this might as well do the same for Werewolf/VampireLord etc.
		std::uint32_t* m_directionalMovementSheathed = nullptr;
		std::uint32_t* m_directionalMovementDrawn = nullptr;
		std::uint32_t m_directionalMovementSheathedOriginal = TDM_API::DirectionalMovementMode::kDirectional;
		std::uint32_t m_directionalMovementDrawnOriginal = TDM_API::DirectionalMovementMode::kDirectional;

		friend class Events::Observer;
	};

}
