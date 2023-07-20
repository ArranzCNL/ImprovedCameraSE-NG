/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "systems/Graphics.h"

#include "plugin.h"
#include "utils/Utils.h"
#include "utils/Log.h"
#include "Utils/PatternScan.h"

#include <MinHook.h>


namespace Systems {

	typedef HRESULT(WINAPI* pD3D11CreateDeviceAndSwapChain)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, CONST D3D_FEATURE_LEVEL*, UINT, UINT, CONST DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
	static pD3D11CreateDeviceAndSwapChain D3D11SystemCreateDeviceAndSwapChain = nullptr;

	HRESULT STDMETHODCALLTYPE Graphics::Hook_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

		if (!pluginGraphics->m_Initialized)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&pluginGraphics->m_Device))))
			{
				// Get the real swap chain (Thanks to PureDark - Skyrim Upscaler)
				pSwapChain->QueryInterface(IID_PPV_ARGS(&pluginGraphics->m_SwapChain));

				DXGI_SWAP_CHAIN_DESC sd{};
				pSwapChain->GetDesc(&sd);
				LOG_TRACE("  DirectX WindowHandle:\t\t0x{:016X} (0x{:016X})", (std::uint64_t)sd.OutputWindow, (std::uint64_t)pluginGraphics->m_OutputWindow);

				pluginGraphics->m_Device->GetImmediateContext(&pluginGraphics->m_DeviceContext);
				pluginGraphics->m_UI = UI::CreateMenu(pluginGraphics->m_OutputWindow);

				if (pluginGraphics->m_UI->Initialize())
					pluginGraphics->m_Initialized = true;
			}
			else
			{
				return pluginGraphics->m_Present(pSwapChain, SyncInterval, Flags);
			}
		}
		pluginGraphics->m_UI->BeginFrame();
		pluginGraphics->m_UI->OnUpdate();
		pluginGraphics->m_UI->EndFrame();
		
		return pluginGraphics->m_Present(pSwapChain, SyncInterval, Flags);
	}

	struct DXGIPresentHook
	{
		static void thunk(std::uint32_t a_timer)
		{
			func(a_timer);

			static auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

			if (!pluginGraphics->m_Initialized)
			{
				const auto renderer = RE::BSRenderManager::GetSingleton();

				if (SUCCEEDED(renderer->swapChain->GetDevice(IID_PPV_ARGS(&pluginGraphics->m_Device))))
				{
					pluginGraphics->m_Device->GetImmediateContext(&pluginGraphics->m_DeviceContext);
					pluginGraphics->m_UI = UI::CreateMenu(pluginGraphics->m_OutputWindow);

					if (pluginGraphics->m_UI->Initialize())
						pluginGraphics->m_Initialized = true;
				}
				else
				{
					return;
				}
			}
			pluginGraphics->m_UI->BeginFrame();
			pluginGraphics->m_UI->OnUpdate();
			pluginGraphics->m_UI->EndFrame();
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static inline constexpr std::size_t size{ 5 };
	};

	Graphics::Graphics(std::int32_t newThread)
	{
		LOG_INFO("Checking for graphics loaders...");
		bool d3d11 = CheckLoader("d3d11.dll");
		bool dxgi = CheckLoader("dxgi.dll");
		LOG_INFO("Finished checking for graphics loaders.");

		LOG_INFO("Checking for overlay hooks...");
		CheckOverlay("GameOverlayRenderer64.dll");
		CheckOverlay("DiscordHook64.dll");
		LOG_INFO("Finished checking for overlay hooks.");

		m_Loader = d3d11 ? true : dxgi;
		bool getD3D11 = GetSystemD3D11();

		if (getD3D11)
		{
			if (newThread == 1)
			{
				CreateThread(nullptr, 0, CreateDevice, nullptr, 0, nullptr);
			}
			else
			{
				// Create Window
				m_Window = std::make_unique<Systems::Window>();

				// Check if the window is initalized
				if (m_Window->m_Initialized)
				{
					// Setup Output window
					m_OutputWindow = m_Window->m_MenuHwnd;

					m_DXGIPresentAddress = REL::RelocationID(75461, 77246).address() + 0x9;
					stl::write_thunk_call<DXGIPresentHook>(m_DXGIPresentAddress);
				}
			}
		}
		else
			LOG_ERROR("Failed to find system d3d11.dll.");
	}

	void Graphics::ResizeBuffer(const glm::uvec2 size)
	{
		if (!m_Initialized) return;

		CleanupRenderTarget();
		m_SwapChain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTargetView();
	}

	bool Graphics::CheckLoader(const std::string& fileName)
	{
		auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();

		std::string fullFilePath = pluginSkyrimSE->Path() + fileName;
		std::string productName = Utils::GetFileInfo(fullFilePath, "ProductName");

		if (!productName.empty())
		{
			std::string fileVersion = Utils::GetFileInfo(fullFilePath, "ProductVersion");
			LOG_TRACE("  Found:\t\t\t\t{} v{} ({})", productName.c_str(), fileVersion.c_str(), fileName.c_str());
			return true;
		}
		return false;
	}

	bool Graphics::CheckOverlay(const std::string& fileName)
	{
		HMODULE gameOverlay = GetModuleHandleA(fileName.c_str());

		if (gameOverlay)
		{
			char modulePath[MAX_PATH]{};
			GetModuleFileNameA(gameOverlay, modulePath, sizeof(modulePath));

			std::string fullFilePath = modulePath;
			std::string productName = Utils::GetFileInfo(fullFilePath, "ProductName");

			if (!productName.empty())
			{
				std::string fileVersion = Utils::GetFileInfo(fullFilePath, "FileVersion");
				LOG_TRACE("  Found:\t\t\t\t{} v{} ({})", productName.c_str(), fileVersion.c_str(), fileName.c_str());
				return true;
			}
			else
			{
				LOG_TRACE("  Found:\t\t\t\t{} (NO DLL INFORMATION)", fileName.c_str());
				return true;
			}
		}
		return false;
	}

	bool Graphics::GetSystemD3D11()
	{
		// Get the pointer to D3D11CreateDeviceAndSwapChain via System directory for d3d11.dll
		if (!D3D11SystemCreateDeviceAndSwapChain)
		{
			char dllFile[MAX_PATH]{};
			if (!GetSystemDirectoryA(dllFile, MAX_PATH))
				return false;

			strcat_s(dllFile, MAX_PATH * sizeof(char), "\\d3d11.dll");

			if (HMODULE hD3D11 = LoadLibraryA(dllFile))
				D3D11SystemCreateDeviceAndSwapChain = (pD3D11CreateDeviceAndSwapChain)GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain");

			if (!D3D11SystemCreateDeviceAndSwapChain)
				return false;

			return true;
		}
		return false;
	}

	bool Graphics::HookOverlay()
	{
		// Just hook Steam Overlay if it is present.
		auto gameOverlay = Utils::FindPattern("GameOverlayRenderer64.dll", "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8");

		if (gameOverlay)
		{
			m_PresentTarget = reinterpret_cast<PresentHook>(gameOverlay);

			MH_STATUS status = MH_Initialize();
			if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to Initialize.");
				return false;
			}
			status = MH_CreateHook(reinterpret_cast<LPVOID>(m_PresentTarget), reinterpret_cast<LPVOID>(Hook_Present), reinterpret_cast<LPVOID*>(&m_Present));
			if (status != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to CreateHook for [IDXGISwapChain] Present. Error Code: {}", (std::int32_t)status);
				return false;
			}
			if (MH_EnableHook(reinterpret_cast<LPVOID>(m_PresentTarget)) != MH_OK)
			{
				LOG_ERROR("  MinHook:\t\t\t\tFailed to EnableHook for [IDXGISwapChain] Present.");
				return false;
			}
			LOG_TRACE("  MinHook:\t\t\t\tHooked [IDXGISwapChain] Present.");
			m_OverlayHooked = true;
			return true;
		}
		return false;
	}

	void Graphics::CreateRenderTargetView()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
		m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

		if (pBackBuffer)
			m_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf());
	}

	void Graphics::CleanupRenderTarget()
	{
		if (m_RenderTargetView)
			m_RenderTargetView.Reset();
	}

	DWORD WINAPI Graphics::CreateDevice(LPVOID)
	{
		auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();
		// Create Window
		pluginGraphics->m_Window = std::make_unique<Systems::Window>();

		// Check if the window is initalized
		if (!pluginGraphics->m_Window->m_Initialized)
			return 0;

		// Setup Output window along with it's height and width.
		pluginGraphics->m_OutputWindow = pluginGraphics->m_Window->m_MenuHwnd;
		pluginGraphics->m_WindowSize = pluginGraphics->m_Window->Properties().size;

		if (!pluginGraphics->HookOverlay())
		{
			// Setup swap chain
			DXGI_SWAP_CHAIN_DESC scd{};
			scd.BufferCount = 2;
			scd.BufferDesc.Width = pluginGraphics->m_WindowSize.x;
			scd.BufferDesc.Height = pluginGraphics->m_WindowSize.y;
			scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			scd.BufferDesc.RefreshRate.Numerator = 60;
			scd.BufferDesc.RefreshRate.Denominator = 1;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.OutputWindow = pluginGraphics->m_OutputWindow;
			scd.SampleDesc.Count = 1;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.Windowed = TRUE;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevel;
			const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

			HRESULT hr = S_OK;

			hr = D3D11SystemCreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				createDeviceFlags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				&scd,
				pluginGraphics->m_SwapChain.GetAddressOf(),
				pluginGraphics->m_Device.GetAddressOf(),
				&featureLevel,
				pluginGraphics->m_DeviceContext.GetAddressOf()
			);

			if (FAILED(hr))
			{
				LOG_ERROR("  Failed to Create Device and SwapChain");
				return 0;
			}

			// Disable ALT+ENTER (Makes no sense for an external overlay)
			Microsoft::WRL::ComPtr<IDXGIFactory1> pFactory;
			if (SUCCEEDED(pluginGraphics->m_SwapChain->GetParent(IID_PPV_ARGS(&pFactory))))
			{
				pFactory->MakeWindowAssociation(pluginGraphics->m_OutputWindow, DXGI_MWA_NO_ALT_ENTER);
				pFactory.Reset();
			}

			pluginGraphics->CreateRenderTargetView();
			LOG_TRACE("  DirectX WindowHandle:\t\t0x{:016X}", (std::uint64_t)scd.OutputWindow);
			LOG_TRACE("  DeviceAndSwapChain:\t\tCreated");

			// Create UI
			pluginGraphics->m_UI = UI::CreateMenu(pluginGraphics->m_OutputWindow);

			if (pluginGraphics->m_UI.get()->Initialize())
				pluginGraphics->m_Initialized = true;
		}

		pluginGraphics->m_Window->RunOverlay();

		return 0;
	}

}
