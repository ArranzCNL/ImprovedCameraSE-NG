/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "menu/IMenu.h"

// ImGui
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include <imgui.h>
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Menu {

	class UIMenu final : public Systems::UI {

	public:
		UIMenu(HWND windowMenu);
		virtual ~UIMenu();

		virtual bool Initialize() override;
		virtual LRESULT WndprocHandler(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) override;
		virtual void BeginFrame() override;
		virtual void OnUpdate() override;
		virtual void EndFrame() override;
		virtual void ResizeBuffer(const glm::uvec2 size) override;
		// Keyboard input
		virtual void AddKeyEvent(const std::uint32_t key, const bool down) override;
		virtual void AddKeyModEvent(const std::uint32_t key, const bool down) override;
		virtual void AddCharacterEvent(const std::uint32_t character) override;
		// Mouse input
		virtual void AddMousePosEvent(const float x_pos, const float y_pos) override;
		virtual void AddMouseButtonEvent(const std::uint32_t button, const bool down) override;
		virtual void AddMouseWheelEvent(const float x_wheel, const float y_wheel) override;

	private:
		const float m_ClearColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		HWND m_UIMenuWindow = nullptr;
		glm::uvec2 m_Size;

		Interface::Menu m_Menu{};
		Interface::IMenu* m_IMenu = nullptr;
		Systems::Config* m_pluginConfig = nullptr;
		Systems::Graphics* m_pluginGraphics = nullptr;
		Patch::SkyrimSE* m_pluginSkyrimSE = nullptr;

		bool m_Initialized = false;
		HWND m_ApplicationWindow = nullptr;
		std::string m_ImGuiConfig{};
		std::int32_t m_MenuKey = 0;

		ID3D11ShaderResourceView* m_logoTexture = nullptr;

	private:
		bool LoadTextureFromMemory(const unsigned char* blob, ID3D11ShaderResourceView** outputResource, std::int32_t* outputWidth, std::int32_t* outputHeight);

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
