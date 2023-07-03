/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "systems/Window.h"
#include "systems/UI.h"

#include <d3d11.h>
#include <wrl/client.h>

// Forward declare UIMenu
namespace Menu {
	class UIMenu;
}


namespace Systems {

	typedef HRESULT(STDMETHODCALLTYPE* PresentHook)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	class Graphics {

	public:
		Graphics(std::int32_t newThread);
		~Graphics() = default;

	public:
		const bool IsInitalized() const { return m_Initialized; };
		void ResizeBuffer(const glm::uvec2 size);
		Window* Window() const { return m_Window.get(); };

		bool IsOverlayHooked() const { return m_OverlayHooked; };

	private:
		bool m_Initialized = false;
		bool m_Loader = false;
		bool m_OverlayHooked = false;

		HWND m_OutputWindow = nullptr;
		glm::uvec2 m_WindowSize;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

		PresentHook m_Present = nullptr;
		PresentHook m_PresentTarget = nullptr;

		std::unique_ptr<Systems::Window> m_Window = nullptr;
		std::unique_ptr<UI> m_UI = nullptr;

	private:
		bool CheckLoader(const std::string& fileName);
		bool CheckOverlay(const std::string& fileName);
		bool GetSystemD3D11();
		bool HookOverlay();
		void CreateRenderTargetView();
		void CleanupRenderTarget();
		void CreateDummyDevice();

	private:
		static HRESULT STDMETHODCALLTYPE Hook_Present(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
		static DWORD WINAPI CreateDevice(LPVOID lpParam);

		friend class Window;
		friend class Menu::UIMenu;
	};

}
