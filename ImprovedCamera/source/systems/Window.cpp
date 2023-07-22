/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "systems/Window.h"

#include "plugin.h"
#include "utils/Log.h"

#include <dwmapi.h>

namespace Systems {

	LRESULT CALLBACK Window::ApplicationMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

		// Make sure the UI is valid
		if (!pluginGraphics->m_UI.get())
			return CallWindowProc(pluginGraphics->Window()->m_Properties->Wndproc, hWnd, msg, wParam, lParam);

		// External Menu
		if (!pluginGraphics->m_UI.get()->IsUIDisplayed() && pluginGraphics->Window()->m_MenuMode == Window::UIDisplay::kOverlay)
		{
			switch (msg)
			{
				case WM_WINDOWPOSCHANGED:
				case WM_MOVE:
				case WM_SIZE:
				{
					RECT rectClient{}, rectWindow{};
					glm::ivec2 size = { 0, 0 }, position = { 0, 0 };

					if (GetClientRect(hWnd, &rectClient))
					{
						size = { rectClient.right - rectClient.left, rectClient.bottom - rectClient.top };
					}
					if (GetWindowRect(hWnd, &rectWindow))
					{
						position = { ((rectWindow.right - rectWindow.left) - size.x) / 2, (rectWindow.bottom - rectWindow.top) - size.y };
						// Bottom and Top does not take into account the horizontal borders which are the same thickness as the vertical borders. (above calculation)
						position.y -= position.x;
						if (position.y < 0)
							position.y = 0;

						SetWindowPos(pluginGraphics->Window()->m_MenuHwnd, nullptr, rectWindow.left + position.x, rectWindow.top + position.y, size.x, size.y, SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
					}
					break;
				}
			}
		}
		return CallWindowProc(pluginGraphics->Window()->m_Properties->Wndproc, hWnd, msg, wParam, lParam);
	}

	Window::Window()
	{
		m_Initialized = true;
		m_Properties = &properties;

		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		m_Properties->name = pluginConfig->ModuleData().sWindowName;
		LOG_INFO("Searching for {} window...", m_Properties->name.c_str());

		// Retrieve the desired MenuMode
		m_MenuMode = pluginConfig->ModuleData().iMenuMode;

		if (m_MenuMode > UIDisplay::kNone)
		{
			// Retrieve MenuTimeout
			std::int32_t maxTimeout = pluginConfig->ModuleData().iMenuTimeout - 1;
			std::int32_t timeout = 0;

			// Don't want negative numbers.
			if (maxTimeout < 0)
				maxTimeout = 0;
			// 3mins should be plenty!!!
			if (maxTimeout > 180)
				maxTimeout = 180;

			// Begin searching for the window
			while (!m_Properties->hWnd)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));

				timeout++;
				if (timeout > maxTimeout)
					break;

				m_Properties->hWnd = FindWindow(nullptr, m_Properties->name.c_str());
			}

			if (m_Properties->hWnd)
			{
				LOG_TRACE("  Found:\t\t\t\t{} in {} secs", m_Properties->name.c_str(), timeout);
				LOG_TRACE("  Handle:\t\t\t\t0x{:016X}", (std::uint64_t)m_Properties->hWnd);

				RECT rectClient{}, rectWindow{};
				if (GetClientRect(m_Properties->hWnd, &rectClient))
				{
					m_Properties->size = { rectClient.right - rectClient.left, rectClient.bottom - rectClient.top };
				}
				if (GetWindowRect(m_Properties->hWnd, &rectWindow))
				{
					m_Properties->position = { ((rectWindow.right - rectWindow.left) - m_Properties->size.x) / 2,
						(rectWindow.bottom - rectWindow.top) - m_Properties->size.y };

					// Bottom and Top does not take into account the horizontal borders which are the same thickness as the vertical borders.
					m_Properties->position.y -= m_Properties->position.x;

					if (m_Properties->position.y < 0)
						m_Properties->position.y = 0;
				}

				LOG_TRACE("  Width:\t\t\t\t{}", m_Properties->size.x);
				LOG_TRACE("  Height:\t\t\t\t{}", m_Properties->size.y);
				LOG_TRACE("  PositionX:\t\t\t{}", m_Properties->position.x);
				LOG_TRACE("  PositionY:\t\t\t{}", m_Properties->position.y);

				if (!m_Properties->size.x || !m_Properties->size.y)
				{
					m_Initialized = false;
					LOG_ERROR("Invalid window height/width, neither should be 0");
				}
				else
				{
					// Workaround for Window not responding which causes the mouse to reappear.
					DisableProcessWindowsGhosting();
					// Setup WNDPROC, overlay also needs this for window position/size.
					m_Properties->Wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_Properties->hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ApplicationMessageHandler)));
					// Check if a module has already hooked the WNDPROC.
					HMODULE hModule = nullptr;
					char moduleName[MAX_PATH]{};
					GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCSTR>(m_Properties->Wndproc), &hModule);
					GetModuleBaseName(GetCurrentProcess(), hModule, moduleName, MAX_PATH);
					LOG_TRACE("  Wndproc:\t\t\t\t0x{:016X} ({})", (std::uint64_t)m_Properties->Wndproc, moduleName);

					if (m_MenuMode == UIDisplay::kOverlay)
					{
						if (!CreateOverlay())
						{
							m_Initialized = false;
							m_MenuMode = UIDisplay::kNone;
							LOG_ERROR("Failed to create Overlay window");
						}
					}
					else if (m_MenuMode == UIDisplay::kInternal)
						m_MenuHwnd = m_Properties->hWnd;
				}
			}
			else
			{
				m_Initialized = false;
				LOG_ERROR("Failed finding {} within {} secs", m_Properties->name.c_str(), maxTimeout);
			}
		}
		else
		{
			m_Initialized = false;
			LOG_INFO("Aborting Menu, not required");
		}
	}

	Window::~Window()
	{
		m_WindowSystemRunning = false;

		if (m_Initialized && m_MenuMode == UIDisplay::kOverlay)
			DestroyWindow(m_MenuHwnd);
	}

	// Clips the mouse cursor to the HTCLIENT area of the specified window.
	void Window::ClipMouseCursor(HWND window)
	{
		RECT rectClient{}, rectWindow{}, rectClip{};
		std::int32_t height = 0, width = 0, posX = 0, posY = 0;

		if (GetClientRect(window, &rectClient))
		{
			width = rectClient.right - rectClient.left;
			height = rectClient.bottom - rectClient.top;
		}
		if (GetWindowRect(window, &rectWindow))
		{
			posX = ((rectWindow.right - rectWindow.left) - width) / 2;      // Border size for both Horizontal and Vertical, top and bottom borders are usually forgotten.
			posY = ((rectWindow.bottom - rectWindow.top) - height) - posX;  // Offset from HTCAPTION and/or HTMENU including above to get the topmost HTCLIENT position.
			rectClip.left = rectWindow.left + posX;
			rectClip.top = rectWindow.top + posY;
			rectClip.right = rectWindow.right - posX;
			rectClip.bottom = rectWindow.bottom - posX;
			ClipCursor(&rectClip);
		}
	}

	bool Window::CreateOverlay()
	{
		auto plugin = DLLMain::Plugin::Get();

		WNDCLASSEX wcex{};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MessageHandler;
		wcex.hInstance = GetModuleHandleA(nullptr);
		wcex.lpszClassName = TEXT(plugin->Name().c_str());
		RegisterClassEx(&wcex);

		m_MenuHwnd = CreateWindowEx(
			WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,  // Makes the Window appear above everything else, also removes the taskbar icon.
			wcex.lpszClassName,
			TEXT(plugin->Description().c_str()),
			WS_POPUP,
			m_Properties->position.x,
			m_Properties->position.y,
			m_Properties->size.x,
			m_Properties->size.y,
			nullptr,
			nullptr,
			wcex.hInstance,
			this);

		if (!m_MenuHwnd)
		{
			UnregisterClass(wcex.lpszClassName, wcex.hInstance);
			return false;
		}

		// Set background transparency when displayed.
		SetLayeredWindowAttributes(m_MenuHwnd, RGB(0, 0, 0), BYTE(0), LWA_ALPHA | LWA_COLORKEY);
		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(m_MenuHwnd, &margins);

		// Setup Window class accessibility inside of MessageHandler.
		SetWindowLongPtr(m_MenuHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		// Display the Window, or not in our case
		ShowWindow(m_MenuHwnd, SW_HIDE);

		return true;
	}

	void Window::RunOverlay()
	{
		if (!m_Initialized)
			return;

		auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

		// Allow capturing of window messages now since all systems should be good to go.
		m_CaptureMsgs = true;

		while (m_WindowSystemRunning)
		{
			if (!pluginGraphics->m_UI.get())
				continue;

			if (pluginGraphics->m_UI.get()->IsUIDisplayed())
			{
				MSG msg{};
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if (msg.message == WM_QUIT)
					break;

				if (!IsWindowVisible(m_MenuHwnd))
				{
					SetForegroundWindow(m_MenuHwnd);
					ShowWindow(m_MenuHwnd, SW_SHOW);
					ShowCursor(true);
				}
			}
			else
			{
				Sleep(10);

				if (IsWindowVisible(m_MenuHwnd))
				{
					SetForegroundWindow(m_Properties->hWnd);
					ShowWindow(m_MenuHwnd, SW_HIDE);
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
				}
			}

			if (!pluginGraphics->IsOverlayHooked())
			{
				pluginGraphics->m_UI.get()->BeginFrame();
				pluginGraphics->m_UI.get()->OnUpdate();
				pluginGraphics->m_UI.get()->EndFrame();
			}
		}
	}

	LRESULT CALLBACK Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// Retrieve Window ptr
		Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (!pWindow)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		if (pWindow->m_CaptureMsgs)
		{
			auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

			// Pass to Menu WndProc
			if (pluginGraphics->m_UI.get() && pluginGraphics->m_UI.get()->WndprocHandler(hWnd, msg, wParam, lParam))
				return 1;

			switch (msg)
			{
				case WM_SIZE:
				{
					if (wParam != SIZE_MINIMIZED)
					{
						const glm::uvec2 size = { (UINT)LOWORD(lParam), (UINT)HIWORD(lParam) };
						pluginGraphics->ResizeBuffer(size);
						return 0;
					}
					break;
				}
				case WM_SYSCOMMAND:
				{
					// Disable ALT application menu
					if ((wParam & 0xFFF0) == SC_KEYMENU)
					{
						return 0;
					}
					// Disable whole menu closing (ALT+F4)
					if ((wParam & 0xFFF0) == SC_CLOSE)
					{
						return 0;
					}
					break;
				}
				case WM_DESTROY:
				{
					PostQuitMessage(0);
					return 0;
				}
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}
