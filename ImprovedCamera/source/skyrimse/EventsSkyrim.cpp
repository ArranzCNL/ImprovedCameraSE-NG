/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "skyrimse/EventsSkyrim.h"

#include "plugin.h"
#include "skyrimse/ImprovedCameraSE.h"
#include "utils/PatternScan.h"
#include "utils/Log.h"

namespace Events {

	static inline RE::hkbClipGenerator* ClipGeneratorNode(RE::hkbNode* a_node)
	{
		if (a_node && a_node->GetClassType())
		{
			if (strcmp(a_node->GetClassType()->name, "hkbClipGenerator") == 0)
				return skyrim_cast<RE::hkbClipGenerator*>(a_node);
		}
		return nullptr;
	}

	void Observer::Register()
	{
		RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(Observer::Get());
		// Show Player In Menus
		Observer::Get()->CheckSPIM();
	}

	EventResult Observer::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!a_event)
			return EventResult::kContinue;

		const char* menuName = a_event->menuName.c_str();

#ifdef _DEBUG
		auto pluginConf = DLLMain::Plugin::Get()->Config();
		if (pluginConf->Logging().bMenus)
			LOG_INFO("Menu: {} ({})", menuName, a_event->opening ? "Opening" : "Closing");
#endif
		// AnimationGraphEventSink registration. Only reliable way I could think of.
		if (strcmp(menuName, "Fader Menu") == 0 && !a_event->opening)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player)
			{
				player->RemoveAnimationGraphEventSink(this);
				player->AddAnimationGraphEventSink(this);
			}
		}
		// Fix RaceSex Menu
		if (strcmp(menuName, "RaceSex Menu") == 0 && !a_event->opening)
		{
			// Fix weapon missing if it was drawn
			auto playerState = RE::PlayerCharacter::GetSingleton()->AsActorState();
			if (playerState->IsWeaponDrawn())
				playerState->actorState2.weaponState = RE::WEAPON_STATE::kWantToSheathe;
			// Fix first person body being hidden
			auto camera = RE::PlayerCamera::GetSingleton();
			if (camera->IsInFirstPerson())
			{
				auto firstperson3D = RE::PlayerCharacter::GetSingleton()->Get3D(1);
				firstperson3D->GetFlags().reset(RE::NiAVObject::Flag::kHidden);
			}
		}
		// Fix displaying body in console
		if (strcmp(menuName, "Console") == 0)
		{
			auto camera = RE::PlayerCamera::GetSingleton();
			if (camera->IsInFirstPerson())
			{
				auto pluginConfig = DLLMain::Plugin::Get()->Config();
				auto bodyConsole = pluginConfig->General().bEnableBodyConsole;
				auto thirdperson3D = RE::PlayerCharacter::GetSingleton()->Get3D(0);

				if (!thirdperson3D)
					return RE::BSEventNotifyControl::kContinue;

				auto thirdpersonNode = thirdperson3D->AsNode();

				if (a_event->opening && !bodyConsole)
					thirdpersonNode->GetFlags().set(RE::NiAVObject::Flag::kHidden);
				else
					thirdpersonNode->GetFlags().reset(RE::NiAVObject::Flag::kHidden);
			}
		}
		// No need to monitor any other closing events
		if (!a_event->opening)
			return RE::BSEventNotifyControl::kContinue;
		// Show Player in Menus InventoryMenu fix
		if (strcmp(menuName, "InventoryMenu") == 0 && m_SPIMInventoryMenu && *m_SPIMInventoryMenu)
			ResetArms();
		// Show Player in Menus MagicMenu fix
		if (strcmp(menuName, "MagicMenu") == 0 && m_SPIMMagicMenu && *m_SPIMMagicMenu)
			ResetArms();

		return RE::BSEventNotifyControl::kContinue;
	}

	EventResult Observer::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>*)
	{
		if (!a_event)
			return EventResult::kContinue;

		RE::BSAnimationGraphManagerPtr graphMgr;

		if (a_event->holder->GetAnimationGraphManager(graphMgr) && graphMgr)
		{
			std::int32_t activeGraph = 0;  //graphMgr->GetRuntimeData().activeGraph; // 0 is third person, 1 is first person.

			RE::BShkbAnimationGraphPtr project = graphMgr->graphs[activeGraph];
			auto behaviourGraph = project ? graphMgr->graphs[activeGraph]->behaviorGraph : nullptr;

			if (!behaviourGraph)
				return EventResult::kContinue;

			RE::NodeList activeNodes = *behaviourGraph->activeNodes;
			std::int32_t index = 0;

			if (!activeNodes.empty())
			{
#ifdef _DEBUG
				auto pluginConfig = DLLMain::Plugin::Get()->Config();
				if (pluginConfig->Logging().bAnimations)
					LOG_DEBUG("Behavior Project: {}", project->projectName);
#endif
				for (auto nodeInfo : activeNodes)
				{
					auto nodeClone = nodeInfo.nodeClone;

					if (nodeClone && nodeClone->GetClassType())
					{
						auto clipGenerator = ClipGeneratorNode(nodeClone);

						if (clipGenerator)
						{
							std::string animationFile = clipGenerator->animationName.c_str();
#ifdef _DEBUG
							if (pluginConfig->Logging().bAnimations)
								LOG_DEBUG("Animation[{}] Name: {}\n\tFile: {}", index, nodeClone->name.c_str(), animationFile.c_str());
#endif
							if (index == 0)
							{
								auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();
								pluginCamera->CheckAnimation(animationFile);
							}
						}
					}
					index++;
				}
			}
		}
		return EventResult::kContinue;
	}

	void Observer::CheckSPIM()
	{
		auto SPIMInventoryMenu = Utils::FindPattern("ShowPlayerInMenus.dll", "62 45 6E 61 62 6C 65 49 6E 49 6E 76");
		auto SPIMMagicMenu = Utils::FindPattern("ShowPlayerInMenus.dll", "62 45 6E 61 62 6C 65 49 6E 4D 61 67");
		m_SPIMInventoryMenu = (std::uint32_t*)SPIMInventoryMenu;
		m_SPIMMagicMenu = (std::uint32_t*)SPIMMagicMenu;
	}

	void Observer::ResetArms()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto thirdpersonNode = player->Get3D(0)->AsNode();
		auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();

		pluginCamera->UpdateSkeleton(true);
		Helper::UpdateNode(thirdpersonNode);
	}

}
