/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "cameras/ICamera.h"
#include "api/SmoothCamAPI.h"
#include "api/TrueDirectionalMovementAPI.h"


namespace ImprovedCamera {

	class ImprovedCameraSE {

	public:
		ImprovedCameraSE();
		~ImprovedCameraSE() = default;

	public:
		void UpdateSwitchPOV();
		void UpdateCamera(std::uint8_t currentID, std::uint8_t previousID);
		void UpdateFirstPerson();
		bool SmoothAnimationTransitions();
		bool UpdateHeadTracking();
		void ModelReferenceEffectFix1(void* pThis, RE::Actor* actor);
		bool ModelReferenceEffectFix2(void* pThis, RE::Actor* actor);
		bool ShaderReferenceEffectFix1(void* pThis, RE::Actor* actor);
		void ShaderReferenceEffectFix2(void* pThis, RE::Actor* actor);
		void ResetPlayerNodes();
		void ForceFirstPerson();
		void ForceThirdPerson();
		void TogglePOV();
		void ResetState(bool forced = false);
		void Ragdoll(RE::Actor* actor);
		bool Ragdoll_IsTaskPoolRequired(RE::Actor* actor);
		void Ragdoll_UpdateObjectUpwards(RE::Actor* actor);

		void SetElderScrollReading(bool reading);
		void SetCartRiding(bool riding);
		bool IsFirstPerson() { return m_IsFirstPerson; }

		void RequestAPIs();

	private:
		class NodeOverride {

		public:
			NodeOverride(RE::NiNode* node, float scale) : node(node) {
				old_scale = node->local.scale;
				node->local.scale = scale;
			}
			~NodeOverride() {
				node->local.scale = old_scale;
			}

		private:
			RE::NiNode* node;
			float old_scale;
		};

		bool UseThirdPersonArms();
		bool UseThirdPersonLeftArm();
		bool UseThirdPersonRightArm();

		void UpdateFirstPersonScale();
		void UpdateSkeleton(bool show);
		void DisplayShadows(bool show);

		void UpdateNearDistance(RE::PlayerCamera* camera);
		void UpdateFOV(RE::PlayerCamera* camera);

		void TranslateCamera();
		void TranslateFirstPerson();
		void TranslateThirdPerson();

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
		bool m_TDMSnapshot = false;
		bool m_ElderScrollReading = false;
		bool m_CartRiding = false;

		// Need to shut these off when needed with TDM to stop horse issues.
		//    Since we are doing this might as well do the same for Werewolf/VampireLord etc.
		std::uint32_t* m_directionalMovementSheathed = nullptr;
		std::uint32_t* m_directionalMovementDrawn = nullptr;
		std::uint32_t m_directionalMovementSheathedOriginal = TDM_API::DirectionalMovementMode::kDirectional;
		std::uint32_t m_directionalMovementDrawnOriginal = TDM_API::DirectionalMovementMode::kDirectional;
	};

}
