/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "api/skse_api.h"
#include "skyrimse/Hooks.h"

namespace ImprovedCamera {

	class ImprovedCameraSE;
}

namespace Patch {

	class SkyrimSE {

	public:
		struct BuildInfo {
			enum
			{
				kInvalid = 0,
				k15970 = 1,
				k16353 = 2,
				k16640 = 3,
				k16659 = 4,  // GOG

				kTotal
			};
		};

	public:
		SkyrimSE();
		virtual ~SkyrimSE() = default;

	public:
		const std::string& FullName() const { return m_FullName; };
		const std::string& Name() const { return m_Name; };
		const std::string& Path() const { return m_Path; };
		const std::uint8_t Build() const { return m_Build; };
		const std::uint32_t VersionMajor() const { return m_VersionMajor; };
		const std::uint32_t VersionMinor() const { return m_VersionMinor; };
		const std::uint32_t VersionRevision() const { return m_VersionRevision; };
		const std::uint32_t VersionBuild() const { return m_VersionBuild; };
		const std::uintptr_t BaseAddress() const { return m_BaseAddress; };
		const std::uintptr_t ImageSize() const { return m_ImageSize; };

		Patch::Hooks* Hooks() const { return m_Hooks.get(); };
		ImprovedCamera::ImprovedCameraSE* Camera() const { return m_Camera.get(); };

	private:
		std::string m_FullName{};
		std::string m_Name{};
		std::string m_Path{};
		std::uint8_t m_Build = BuildInfo::kInvalid;
		std::uint32_t m_VersionMajor = 0;
		std::uint32_t m_VersionMinor = 0;
		std::uint32_t m_VersionRevision = 0;
		std::uint32_t m_VersionBuild = 0;
		std::uintptr_t m_BaseAddress = NULL;
		std::uintptr_t m_ImageSize = NULL;

		std::unique_ptr<Patch::Hooks> m_Hooks = nullptr;
		std::unique_ptr<ImprovedCamera::ImprovedCameraSE> m_Camera = nullptr;

	private:
		void OnLoad();
		void LoadGame();
		void ExecutableInfo();
		void BuildInfo();

	private:
		bool m_OnLoaded = false;
		bool m_InstalledHooks = false;

	private:
		friend void SKSE::SKSEMessageHandler(SKSE::MessagingInterface::Message* message);
		friend bool SKSE::SKSEPlugin_Load(const SKSE::LoadInterface* skse);
	};

}
