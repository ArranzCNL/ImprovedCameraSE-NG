/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "menu/IMenu.h"

// ImGui
#include <imgui.h>
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Menu {

	class UIMenu : public Systems::UI {

	public:
		UIMenu(HWND windowMenu);
		virtual ~UIMenu();

		virtual bool Initialize() override;
		virtual LRESULT WndprocHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		virtual void BeginFrame() override;
		virtual void OnUpdate() override;
		virtual void EndFrame() override;
		virtual void ResizeBuffer(const glm::uvec2 size) override;

	private:
		const float m_ClearColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		HWND m_UIMenuWindow = nullptr;
		glm::uvec2 m_Size;

		Interface::Menu m_Menu{};
		Interface::IMenu* m_IMenu = nullptr;
		Systems::Config* m_pluginConfig = nullptr;
		Systems::Graphics* m_pluginGraphics = nullptr;
		Patch::SkyrimSE* m_pluginSkyrimSE = nullptr;
		Systems::Input* m_pluginInput = nullptr;

		bool m_Initialized = false;
		HWND m_ApplicationWindow = nullptr;
		std::string m_ImGuiConfig{};
		std::int32_t m_MenuKey = 0;

	private:
		void Shutdown();
		void OnUpdateMenu();
		void OnUpdateMenuClose();
		void OnUpdateMenuSave();
		void OnUpdateDeleteProfile();

		void RegisterMenus();

		void ShowMenuBarWindow();
		std::string m_ProfileName{};
		std::string m_TitlebarName{};

		void ShowProfileWindow();
		bool m_ViewProfileWindow = false;

		void ShowSaveWindow();
		bool m_ViewSaveWindow = false;

		void ShowModuleDataWindow();
		bool m_ViewModuleDataWindow = false;

		// ImGui Tools
		bool m_ViewStackToolWindow = false;
		bool m_ViewMetricsWindow = false;
		bool m_ViewStyleEditorWindow = false;
		bool m_ViewDemoWindow = false;
		bool m_ViewAboutImGuiWindow = false;
	};

}
