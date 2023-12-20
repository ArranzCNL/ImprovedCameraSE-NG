/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/UIMenu.h"

#include "menu/Menus.h"
#include "menu/UIMenuHelper.h"
#include "menu/IconsFontAwesome6.h"
#include "menu/Logo.h"

#include "skyrimse/ImprovedCameraSE.h"
#include "utils/Log.h"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#pragma warning(push)
#pragma warning(disable: 4244)
#include "stb/stb_image.h"
#pragma warning(pop)

namespace Menu {

	UIMenu::UIMenu(HWND windowMenu) :
		m_UIMenuWindow(windowMenu)
	{
		// Get the Plugin so we can access Config, Graphics and Input
		auto plugin = DLLMain::Plugin::Get();
		m_pluginConfig = plugin->Config();
		m_pluginGraphics = plugin->Graphics();
		m_pluginSkyrimSE = plugin->SkyrimSE();

		m_ProfileName = m_pluginConfig->ModuleData().sProfileName.substr(0, m_pluginConfig->ModuleData().sProfileName.size() - 4);
		m_TitlebarName = plugin->Description() + " Editor";
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
				std::string fontAwesome = m_pluginConfig->FontPath() + "FontAwesome\\" + FONT_ICON_FILE_NAME_FAS;
				float fontSize = m_pluginConfig->ModuleData().fMenuFontSize;

				if (std::filesystem::exists(customFont))
				{
					// TODO: Workout maximum size
					if (fontSize < 10)
					{
						fontSize = 10.0f;
					}
					LOG_TRACE("  MenuFont:\t\t\t\t{}", customFont.c_str());
					LOG_TRACE("  MenuFontSize:\t\t\t{}", fontSize);  // %.1f
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
							LOG_TRACE("  MenuFontSize:\t\t\t{}", fontSize);  // %.1f
							io.FontDefault = io.Fonts->AddFontFromFileTTF(customFont.c_str(), fontSize);
						}
					}
				}
				ImFontConfig config;
				config.MergeMode = true;
				config.GlyphMinAdvanceX = 14.0f;  // Use if you want to make the icon monospaced
				static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
				io.Fonts->AddFontFromFileTTF(fontAwesome.c_str(), 14.0f, &config, icon_ranges);
			}
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			m_ApplicationWindow = m_pluginGraphics->Window()->Properties().hWnd;
			m_MenuKey = m_pluginConfig->ModuleData().iMenuKey;

			LOG_TRACE("  MenuKey:\t\t\t\t0x{:0X}", m_MenuKey);  // 0x%I64X
			LOG_INFO("UIMenu Loaded.");

			// Load Improved Camera's logo
			std::int32_t logoWidth = 0;
			std::int32_t logoHeight = 0;
			LoadTextureFromMemory(logoBlob, &m_logoTexture, &logoWidth, &logoHeight);

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

	LRESULT UIMenu::WndprocHandler(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
	{
		if (!m_DisplayUI)
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
		if (ForegroundWindow && ForegroundWindow != m_UIMenuWindow && m_DisplayUI)
		{
			OnUpdateMenuClose();
		}
		// Display the Window - Configured key pressed.
		if (ForegroundWindow == m_ApplicationWindow && !m_DisplayUI && (GetAsyncKeyState(m_MenuKey) & 0x8000) != 0 && (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0)
		{
			m_DisplayUI = true;

			if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::UIDisplay::kInternal)
			{
				ShowCursor(true);
			}
		}
		// Close the Window - Escape key pressed.
		if (m_ApplicationWindow && ForegroundWindow == m_UIMenuWindow && m_DisplayUI && (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0)
		{
			OnUpdateMenuClose();
			SetForegroundWindow(m_ApplicationWindow);
		}
		// Save Config - CTRL+S (0x53).
		if (ForegroundWindow == m_UIMenuWindow && m_DisplayUI && !m_ViewSaveWindow && (GetAsyncKeyState(0x53) & 0x8000) != 0 && (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			OnUpdateMenuSave();
		}
		// Update Menu(s)
		if (m_UIMenuWindow && IsWindowVisible(m_UIMenuWindow) && m_DisplayUI)
		{
			OnUpdateMenu();
		}
	}

	void UIMenu::EndFrame()
	{
		ImGui::EndFrame();

		ImGui::Render();

		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::UIDisplay::kOverlay && !m_pluginGraphics->IsOverlayHooked())
		{
			m_pluginGraphics->m_DeviceContext->OMSetRenderTargets(1, m_pluginGraphics->m_RenderTargetView.GetAddressOf(), nullptr);
			m_pluginGraphics->m_DeviceContext->ClearRenderTargetView(m_pluginGraphics->m_RenderTargetView.Get(), m_ClearColour);
		}

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::UIDisplay::kOverlay && !m_pluginGraphics->IsOverlayHooked())
			m_pluginGraphics->m_SwapChain->Present(1, 0);  // VSync
	}

	void UIMenu::ResizeBuffer(const glm::uvec2 size)
	{
		m_pluginGraphics->ResizeBuffer(size);
	}

	void UIMenu::AddKeyEvent(const std::uint32_t key, const bool down)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ImGui::VirtualKeyToImGuiKey(key), down);
	}

	void UIMenu::AddKeyModEvent(const std::uint32_t key, const bool down)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (key == VK_LSHIFT || key == VK_RSHIFT)
			io.AddKeyEvent(ImGuiMod_Shift, down);

		else if (key == VK_LCONTROL || key == VK_RCONTROL)
			io.AddKeyEvent(ImGuiMod_Ctrl, down);

		else if (key == VK_LMENU || key == VK_RMENU)
			io.AddKeyEvent(ImGuiMod_Alt, down);
	}

	void UIMenu::AddCharacterEvent(const std::uint32_t character)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(character);
	}
	// Not used
	void UIMenu::AddMousePosEvent(const float, const float) {}

	void UIMenu::AddMouseButtonEvent(const std::uint32_t button, const bool down)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(button, down);
	}

	void UIMenu::AddMouseWheelEvent(const float x_wheel, const float y_wheel)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(x_wheel, y_wheel);
	}

	void UIMenu::OnUpdateMenu()
	{
		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::UIDisplay::kInternal)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDrawCursor = true;
		}

		ShowMenuBarWindow();
		if (m_ViewSaveWindow)
			ShowSaveWindow();
		// In order of precedence from the MenuBar
		if (m_ViewModuleDataWindow)
			ShowModuleDataWindow();

		for (Interface::IMenu* menu : m_Menu)
			menu->OnUpdate();

		// ImGui Tools
		if (m_ViewStackToolWindow)
			ImGui::ShowStackToolWindow(&m_ViewStackToolWindow);

		if (m_ViewMetricsWindow)
			ImGui::ShowMetricsWindow(&m_ViewMetricsWindow);

		if (m_ViewStyleEditorWindow)
		{
			ImGui::Begin("Style Editor", &m_ViewStyleEditorWindow);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
		if (m_ViewDemoWindow)
			ImGui::ShowDemoWindow(&m_ViewDemoWindow);

		if (m_ViewAboutImGuiWindow)
			ImGui::ShowAboutWindow(&m_ViewAboutImGuiWindow);
	}

	void UIMenu::OnUpdateMenuClose()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = false;

		m_DisplayUI = false;
		for (Interface::IMenu* menu : m_Menu)
			menu->OnClose();

		// Close all non-essential Menus
		m_ViewSaveWindow = false;

		m_ViewStackToolWindow = false;
		m_ViewMetricsWindow = false;
		m_ViewStyleEditorWindow = false;
		m_ViewDemoWindow = false;
		m_ViewAboutImGuiWindow = false;

		// Clear ImGui input
		io.ClearEventsQueue();
		io.ClearInputKeys();

		if (m_pluginConfig->ModuleData().iMenuMode == Systems::Window::UIDisplay::kInternal)
		{
			ShowCursor(false);
			// Fix held down left shift
			INPUT ipKeyboard{};
			ipKeyboard.type = INPUT_KEYBOARD;
			ipKeyboard.ki.wScan = 0x2A;  // DIK_LSHIFT
			ipKeyboard.ki.dwFlags = KEYEVENTF_SCANCODE;
			SendInput(1, &ipKeyboard, sizeof(INPUT));
			Sleep(10);
			ipKeyboard.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			SendInput(1, &ipKeyboard, sizeof(INPUT));
			// Close Journal Menu
			const auto msgQueue = RE::UIMessageQueue::GetSingleton();
			msgQueue->AddMessage("Journal Menu", RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}
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
			ImGui::Image((void*)m_logoTexture, ImVec2(25, 25));
			if (ImGui::BeginMenu("File"))
			{
				ImGui::TextColored(ImVec4(0.573f, 0.894f, 0.573f, 1.0f), ICON_FA_USERS);
				ImGui::SameLine();
				if (ImGui::BeginMenu("Profiles"))
				{
					ShowProfileWindow();
					ImGui::EndMenu();
				}

				ImGui::TextColored(ImVec4(0.314f, 0.6f, 0.894f, 1.0f), ICON_FA_FLOPPY_DISK);
				ImGui::SameLine();
				if (ImGui::MenuItem("Save Profile", "Ctrl+S") && !m_ViewSaveWindow)
					OnUpdateMenuSave();

				ImGui::Separator();
				ImGui::TextColored(ImVec4(0.961f, 0.341f, 0.384f, 1.0f), ICON_FA_TRASH_CAN);
				ImGui::SameLine();
				if (m_ProfileName.compare("Default") == 0)
					ImGui::MenuItem("Delete Profile", NULL, false, false);

				else if (ImGui::MenuItem("Delete Profile"))
					OnUpdateDeleteProfile();

				ImGui::Separator();
				ImGui::Text(ICON_FA_RIGHT_FROM_BRACKET);
				ImGui::SameLine();
				if (ImGui::MenuItem("Close", "Esc"))
					OnUpdateMenuClose();

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
				ImGui::TextColored(ImVec4(0.961f, 0.341f, 0.384f, 1.0f), ICON_FA_BUG);
				ImGui::SameLine();
				if (ImGui::MenuItem("Stack Tool"))
					m_ViewStackToolWindow = true;

				ImGui::TextColored(ImVec4(0.314f, 0.6f, 0.894f, 1.0f), ICON_FA_SCREWDRIVER_WRENCH);
				ImGui::SameLine();
				if (ImGui::MenuItem("Metrics/Debugger"))
					m_ViewMetricsWindow = true;

#ifdef IMGUI_ENABLE_DEMO_WINDOWS
				ImGui::Separator();
				if (ImGui::MenuItem("Style Editor"))
					m_ViewStyleEditorWindow = true;

				if (ImGui::MenuItem("ImGui Demo"))
					m_ViewDemoWindow = true;
#endif
				ImGui::EndMenu();
			}
#ifdef _DEBUG
			if (ImGui::BeginMenu("Logging"))
			{
				ImGui::Checkbox("Animations", &m_pluginConfig->m_Logging.bAnimations);
				ImGui::Checkbox("Menus", &m_pluginConfig->m_Logging.bMenus);
				ImGui::Checkbox("Camera Delta", &m_pluginConfig->m_Logging.bCameraDelta);

				ImGui::EndMenu();
			}
#endif
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.89f, 0.62f, 1.0f), ICON_FA_ROTATE);
				ImGui::SameLine();
				if (ImGui::MenuItem("Reset State"))
					m_pluginSkyrimSE->Camera()->ResetState(true);

				ImGui::TextColored(ImVec4(0.314f, 0.6f, 0.894f, 1.0f), ICON_FA_CIRCLE_INFO);
				ImGui::SameLine();
				if (ImGui::MenuItem("About Dear ImGui"))
					m_ViewAboutImGuiWindow = true;

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
		ImGui::Begin(m_TitlebarName.c_str(), &m_ViewSaveWindow, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize("Save Settings").x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text("Save Settings");

		if (m_ProfileName.compare("Default") == 0)
		{
			struct TextFilters {

				static int FilterAllowedCharacters(ImGuiInputTextCallbackData* data)
				{
					if (data->EventChar < 256 && strchr("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", (char)data->EventChar))
						return 0;
					return 1;
				}
			};

			static std::string textInput{};
			ImGui::Text("Profile Name:");
			ImGui::SameLine();
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
		ImGui::Begin("[MODULE DATA]", &m_ViewModuleDataWindow, ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginTable("GeneralTable", 2, ImGuiTableFlags_SizingFixedFit))
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

		if (ImGui::Button("Close"))
			m_ViewModuleDataWindow = false;

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

	bool UIMenu::LoadTextureFromMemory(const unsigned char* blob, ID3D11ShaderResourceView** outputResource, std::int32_t* outputWidth, std::int32_t* outputHeight)
	{
		std::int32_t textureWidth = 0;
		std::int32_t textureHeight = 0;
		std::int32_t textureLength = 32 * 32 * 4;
		unsigned char* textureData = stbi_load_from_memory(blob, textureLength, &textureWidth, &textureHeight, nullptr, 4);
		if (textureData == nullptr)
			return false;

		// Create texture
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = textureWidth;
		desc.Height = textureHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = nullptr;
		D3D11_SUBRESOURCE_DATA subResource{};
		subResource.pSysMem = textureData;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		m_pluginGraphics->m_Device.Get()->CreateTexture2D(&desc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_pluginGraphics->m_Device.Get()->CreateShaderResourceView(pTexture, &srvDesc, outputResource);
		pTexture->Release();

		*outputWidth = textureWidth;
		*outputHeight = textureHeight;
		stbi_image_free(textureData);

		return true;
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
