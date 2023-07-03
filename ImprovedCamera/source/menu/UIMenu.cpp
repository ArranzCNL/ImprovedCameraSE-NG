/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/UIMenu.h"

#include "menu/Menus.h"
#include "menu/UIMenuHelper.h"
#include "skyrimse/ImprovedCameraSE.h"
#include "utils/Log.h"

#include <filesystem>


namespace Menu {

	UIMenu::UIMenu(HWND windowMenu)
		: m_UIMenuWindow(windowMenu)
	{
		// Get the Plugin so we can access Config, Graphics and Input
		auto plugin = DLLMain::Plugin::Get();
		m_pluginConfig = plugin->Config();
		m_pluginGraphics = plugin->Graphics();
		m_pluginSkyrimSE = plugin->SkyrimSE();
		m_pluginInput = plugin->Input();

		m_ProfileName = m_pluginConfig->ModuleData().sProfileName.substr(0, m_pluginConfig->ModuleData().sProfileName.size() - 4);
		m_TitlebarName = plugin->Description().c_str();
		m_TitlebarName += " Editor";
	}

	UIMenu::~UIMenu()
	{
		Shutdown();
	}

	bool UIMenu::Initialize()
	{
		if (!m_Initialized)
		{
			m_Initialized = true;
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			// Set imgui.ini to the proper location also change it's name to editorconfig.ini
			m_ImGuiConfig = m_pluginConfig->Path() + "editorconfig.ini";
			io.IniFilename = m_ImGuiConfig.c_str();

			LOG_TRACE("  ImGuiConfig:\t\t\t{}", m_ImGuiConfig.c_str());

			// Setup Font
			if (!m_pluginConfig->ModuleData().sMenuFont.empty())
			{
				std::string customFont = m_pluginConfig->FontPath() + m_pluginConfig->ModuleData().sMenuFont;
				float fontSize = m_pluginConfig->ModuleData().fMenuFontSize;

				if (std::filesystem::exists(customFont))
				{
					// TODO: Workout maximum size
					if (fontSize < 10)
					{
						fontSize = 10.0f;
					}
					LOG_TRACE("  MenuFont:\t\t\t\t{}", customFont.c_str());
					LOG_TRACE("  MenuFontSize:\t\t\t{}", fontSize); // %.1f
					io.FontDefault = io.Fonts->AddFontFromFileTTF(customFont.c_str(), fontSize);
				}
				else
				{
					char winDir[MAX_PATH];
					if (GetWindowsDirectory(winDir, MAX_PATH))
					{
						sprintf_s(winDir, "%s\\Fonts\\", winDir);
						customFont = winDir + m_pluginConfig->ModuleData().sMenuFont;

						if (std::filesystem::exists(customFont))
						{
							// TODO: Workout maximum size
							if (fontSize < 10)
							{
								fontSize = 10.0f;
							}
							LOG_TRACE("  MenuFont:\t\t\t\t{}", customFont.c_str());
							LOG_TRACE("  MenuFontSize:\t\t\t{}", fontSize); // %.1f
							io.FontDefault = io.Fonts->AddFontFromFileTTF(customFont.c_str(), fontSize);
						}
					}
				}
			}
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			m_ApplicationWindow = m_pluginGraphics->Window()->Properties().hWnd;
			m_MenuKey = m_pluginConfig->ModuleData().iMenuKey;

			LOG_TRACE("  MenuKey:\t\t\t\t0x{:0X}", m_MenuKey); // 0x%I64X
			LOG_INFO("UIMenu Loaded.");

			// Setup Platform/Renderer backends
			ImGui_ImplWin32_Init(m_UIMenuWindow);
			ImGui_ImplDX11_Init(m_pluginGraphics->m_Device.Get(), m_pluginGraphics->m_DeviceContext.Get());

			ImGui::StyleColorsDefault();

			SetForegroundWindow(m_ApplicationWindow);
			m_pluginGraphics->Window()->ClipMouseCursor(m_ApplicationWindow);

			RegisterMenus();

			return true;
		}
		return false;
	}

	LRESULT UIMenu::WndprocHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (!m_DisplayMenu)
			return false;

		// ImGui callback
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		{
			SetForegroundWindow(hWnd);
			m_pluginGraphics->Window()->ClipMouseCursor(hWnd);
			return true;
		}
		return false;
	}

	void UIMenu::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void UIMenu::OnUpdate()
	{
		static HWND ForegroundWindow;
		ForegroundWindow = GetForegroundWindow();

		// Focus loss detection - Force close.
		if (ForegroundWindow && ForegroundWindow != m_UIMenuWindow && m_DisplayMenu)
		{
			OnUpdateMenuClose();
		}
		// Display the Window - Configured key pressed.
		if (ForegroundWindow == m_ApplicationWindow && !m_DisplayMenu && (GetAsyncKeyState(m_MenuKey) & 0x8000) != 0 && (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0)
		{
			m_DisplayMenu = true;

			if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::MenuDisplay::kInternal)
			{
				ShowCursor(true);
				m_pluginInput->MenuDisplayed(m_DisplayMenu);
			}
		}
		// Close the Window - Escape key pressed.
		if (m_ApplicationWindow && ForegroundWindow == m_UIMenuWindow && m_DisplayMenu && (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0)
		{
			OnUpdateMenuClose();
			SetForegroundWindow(m_ApplicationWindow);
		}
		// Save Config - CTRL+S (0x53).
		if (ForegroundWindow == m_UIMenuWindow && m_DisplayMenu && !m_ViewSaveWindow && (GetAsyncKeyState(0x53) & 0x8000) != 0 && (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			OnUpdateMenuSave();
		}
		// Update Menu(s)
		if (m_UIMenuWindow && IsWindowVisible(m_UIMenuWindow) && m_DisplayMenu)
		{
			OnUpdateMenu();
		}
	}

	void UIMenu::EndFrame()
	{
		ImGui::EndFrame();

		ImGui::Render();

		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::MenuDisplay::kOverlay && !m_pluginGraphics->IsOverlayHooked())
		{
			m_pluginGraphics->m_DeviceContext->OMSetRenderTargets(1, m_pluginGraphics->m_RenderTargetView.GetAddressOf(), nullptr);
			m_pluginGraphics->m_DeviceContext->ClearRenderTargetView(m_pluginGraphics->m_RenderTargetView.Get(), m_ClearColour);
		}

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::MenuDisplay::kOverlay && !m_pluginGraphics->IsOverlayHooked())
			m_pluginGraphics->m_SwapChain->Present(1, 0); // VSync
	}

	void UIMenu::ResizeBuffer(const glm::uvec2 size)
	{
		m_pluginGraphics->ResizeBuffer(size);
	}

	void UIMenu::OnUpdateMenu()
	{
		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::MenuDisplay::kInternal)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDrawCursor = true;
		}

		ShowMenuBarWindow();
		if (m_ViewSaveWindow) ShowSaveWindow();
		// In order of precedence from the MenuBar
		if (m_ViewModuleDataWindow) ShowModuleDataWindow();
		for (Interface::IMenu* menu : m_Menu)
			menu->OnUpdate();

		// ImGui Tools
		if (m_ViewStackToolWindow) ImGui::ShowStackToolWindow(&m_ViewStackToolWindow);
		if (m_ViewMetricsWindow) ImGui::ShowMetricsWindow(&m_ViewMetricsWindow);
		if (m_ViewStyleEditorWindow)
		{
			ImGui::Begin("Style Editor", &m_ViewStyleEditorWindow);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
		if (m_ViewDemoWindow) ImGui::ShowDemoWindow(&m_ViewDemoWindow);
		if (m_ViewAboutImGuiWindow) ImGui::ShowAboutWindow(&m_ViewAboutImGuiWindow);
	}

	void UIMenu::OnUpdateMenuClose()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = false;

		m_DisplayMenu = false;
		for (Interface::IMenu* menu : m_Menu)
			menu->OnClose();

		// Close all non-essential Menus
		m_ViewSaveWindow = false;

		m_ViewStackToolWindow = false;
		m_ViewMetricsWindow = false;
		m_ViewStyleEditorWindow = false;
		m_ViewDemoWindow = false;
		m_ViewAboutImGuiWindow = false;

		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::MenuDisplay::kInternal)
		{
			ShowCursor(false);
			m_pluginInput->MenuDisplayed(m_DisplayMenu);
		}
		// Clear ImGui input
		io.ClearInputCharacters();
		io.ClearInputKeys();
	}

	void UIMenu::OnUpdateMenuSave()
	{
		m_ViewSaveWindow = true;
	}

	void UIMenu::OnUpdateDeleteProfile()
	{
		std::string defaultFile = "Default.ini";
		std::string deleteFile = m_pluginConfig->m_ModuleData.sProfileName;
		// Reset to default
		m_pluginConfig->m_ModuleData.sProfileName = defaultFile;
		// Update main file to point to new profile
		m_pluginConfig->WriteIni(m_pluginConfig->m_FileName, true);
		// Update settings
		m_pluginConfig->ReadIni(defaultFile);
		// Update ShowMenuBarWindow profile name
		m_ProfileName = defaultFile.substr(0, defaultFile.size() - 4);

		// Delete the file
		std::string path = m_pluginConfig->ProfilePath();
		std::string profileName = path + deleteFile;
		std::remove(profileName.c_str());
	}

	void UIMenu::ShowMenuBarWindow()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Profiles"))
				{
					ShowProfileWindow();
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save Profile", "Ctrl+S") && !m_ViewSaveWindow) OnUpdateMenuSave();
				ImGui::Separator();
				if (m_ProfileName.compare("Default") == 0) ImGui::MenuItem("Delete Profile", NULL, false, false);
				else if(ImGui::MenuItem("Delete Profile")) OnUpdateDeleteProfile();
				ImGui::Separator();
				if (ImGui::MenuItem("Close", "Esc")) OnUpdateMenuClose();

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ImGui::Checkbox("Module Data", &m_ViewModuleDataWindow);
				ImGui::Separator();

				for (Interface::IMenu* menu : m_Menu)
					menu->OnOpen();

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Stack Tool")) m_ViewStackToolWindow = true;
				if (ImGui::MenuItem("Metrics/Debugger")) m_ViewMetricsWindow = true;
#ifdef IMGUI_ENABLE_DEMO_WINDOWS
				ImGui::Separator();
				if (ImGui::MenuItem("Style Editor")) m_ViewStyleEditorWindow = true;
				if (ImGui::MenuItem("ImGui Demo")) m_ViewDemoWindow = true;
#endif
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Reset State"))
					m_pluginSkyrimSE->Camera()->ResetState(true);

				if (ImGui::MenuItem("About Dear ImGui")) m_ViewAboutImGuiWindow = true;

				ImGui::EndMenu();
			}

			// Adds profile name and titlebar name
			static auto profileNameWidth = ImGui::CalcTextSize(m_ProfileName.c_str()).x;
			ImGui::SameLine(float(((ImGui::GetWindowWidth() * 0.5) - profileNameWidth) - (m_pluginConfig->ModuleData().fMenuFontSize + 2.0)));
			ImGui::Text(m_ProfileName.c_str());
			static auto titleNameWidth = ImGui::CalcTextSize(m_TitlebarName.c_str()).x;
			ImGui::SameLine(float((ImGui::GetWindowWidth() - titleNameWidth) - (m_pluginConfig->ModuleData().fMenuFontSize + 2.0)));
			ImGui::Text(m_TitlebarName.c_str());
			ImGui::EndMainMenuBar();
		}
	}

	void UIMenu::ShowProfileWindow()
	{
		std::string path = m_pluginConfig->ProfilePath();
		std::string profileName{};

		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			profileName = file.path().filename().string();
			if (profileName.find(".ini") != std::string::npos)
			{
				// Check to see if current profile is loaded
				if (profileName.compare(m_pluginConfig->m_ModuleData.sProfileName) == 0)
				{
					ImGui::MenuItem(profileName.substr(0, profileName.size() - 4).c_str(), NULL, false, false);
				}
				else
				{
					if (ImGui::MenuItem(profileName.substr(0, profileName.size() - 4).c_str()))
					{
						m_pluginConfig->m_ModuleData.sProfileName = profileName;
						// Update main file to point to new profile
						m_pluginConfig->WriteIni(m_pluginConfig->m_FileName, true);
						// Load profile
						m_pluginConfig->ReadIni(profileName);
						// Update Profile name
						m_ProfileName = profileName.substr(0, profileName.size() - 4);
					}
				}
			}
		}
	}

	void UIMenu::ShowSaveWindow()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 150));
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::Begin(m_TitlebarName.c_str(), &m_ViewSaveWindow, ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize("Save Settings").x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text("Save Settings");
		
		if (m_ProfileName.compare("Default") == 0)
		{
			struct TextFilters
			{
				static int FilterAllowedCharacters(ImGuiInputTextCallbackData* data)
				{
					if (data->EventChar < 256 && strchr("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", (char)data->EventChar))
						return 0;
					return 1;
				}
			};

			static std::string textInput{};
			ImGui::Text("Profile Name:"); ImGui::SameLine();
			ImGui::InputText("##SaveInput", &textInput, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterAllowedCharacters);
			ImGui::NewLine();
			ImGui::SetCursorPosX((windowWidth - 90) * 0.5f);

			if (!textInput.empty())
			{
				if (ImGui::Button("OK", ImVec2(90, 30)))
				{
					m_ViewSaveWindow = false;
					std::string profileName = textInput + ".ini";

					m_pluginConfig->m_ModuleData.sProfileName = profileName;
					// Update main file to point to new profile
					m_pluginConfig->WriteIni(m_pluginConfig->m_FileName, true);
					// Save profile
					m_pluginConfig->WriteIni(profileName);
					// Update Profile name
					m_ProfileName = profileName.substr(0, profileName.size() - 4);
				}
			}
			else
			{
				ImGui::BeginDisabled();
				ImGui::Button("OK", ImVec2(90, 30));
				ImGui::EndDisabled();
			}
		}
		else
		{
			ImGui::NewLine();
			ImGui::SetCursorPosX((windowWidth - 90) * 0.5f);
			if (ImGui::Button("OK", ImVec2(90, 30)))
			{
				m_ViewSaveWindow = false;
				std::string profileName = m_ProfileName + ".ini";

				m_pluginConfig->m_ModuleData.sProfileName = profileName;
				// Update main file to point to new profile
				m_pluginConfig->WriteIni(m_pluginConfig->m_FileName, true);
				// Save profile
				m_pluginConfig->WriteIni(profileName);
				// Update Profile name
				m_ProfileName = profileName.substr(0, profileName.size() - 4);
			}
		}
		ImGui::End();
	}

	void UIMenu::ShowModuleDataWindow()
	{
		ImGui::Begin("[MODULE DATA]", &m_ViewModuleDataWindow, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginTable("GeneralTable", 2, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::BeginDisabled();

			ImGui::TableItemInputText("FileName", "##FileName", &m_pluginConfig->m_ModuleData.sFileName);
			ImGui::TableItemInputText("FileVersionMin", "##FileVersionMin", &m_pluginConfig->m_ModuleData.sFileVersionMin);
			ImGui::TableItemInputText("FileVersionMax", "##FileVersionMax", &m_pluginConfig->m_ModuleData.sFileVersionMax);
			ImGui::TableItemInputText("WindowName", "##WindowName", &m_pluginConfig->m_ModuleData.sWindowName);
			ImGui::TableItemInputInt("MenuMode", "##MenuMode", &m_pluginConfig->m_ModuleData.iMenuMode);
			ImGui::TableItemInputInt("MenuTimeout", "##MenuTimeout", &m_pluginConfig->m_ModuleData.iMenuTimeout);
			// This one will be changed in the future
			static char strMenuKey[16];
			snprintf(strMenuKey, IM_ARRAYSIZE(strMenuKey), "0x%I32X", m_pluginConfig->ModuleData().iMenuKey);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("MenuKey");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(250);
			ImGui::InputText("##MenuKey", strMenuKey, IM_ARRAYSIZE(strMenuKey));
			ImGui::PopItemWidth();

			ImGui::TableItemInputText("MenuFont", "##MenuFont", &m_pluginConfig->m_ModuleData.sMenuFont);
			ImGui::TableItemInputFloat("MenuFontSize", "##MenuFontSize", &m_pluginConfig->m_ModuleData.fMenuFontSize, 8.0f, 48.0f, "%.1f");
			ImGui::TableItemInputText("ProfileName", "##ProfileName", &m_pluginConfig->m_ModuleData.sProfileName);

			ImGui::EndDisabled();
			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_ViewModuleDataWindow = false;
		ImGui::End();
	}

	void UIMenu::RegisterMenus()
	{
		m_Menu.Register(new MenuGeneral);
		m_Menu.Register(new MenuHide);
		m_Menu.Register(new MenuFixes);
		m_Menu.Register(new MenuRestrictAngles);
		m_Menu.Register(new MenuEvents);
		m_Menu.Register(new MenuFOV);
		m_Menu.Register(new MenuNearDistance);
		m_Menu.Register(new MenuHeadbob);
		m_Menu.Register(new MenuCamera);
	}

	void UIMenu::Shutdown()
	{
		if (m_Initialized)
		{
			m_Initialized = false;
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
			LOG_INFO("  UIMenu Unloaded.");
		}
	}

}
