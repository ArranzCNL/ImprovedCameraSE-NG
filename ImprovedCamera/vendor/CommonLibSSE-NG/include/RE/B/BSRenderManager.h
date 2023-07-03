#pragma once

#include "RE/N/NiTexture.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView1;
struct ID3D11ShaderResourceView1;

namespace RE
{
	class BSRenderManager
	{
	public:
		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT                          \
	ID3D11Device*              forwarder;    /* 00 */ \
	ID3D11DeviceContext*       context;      /* 08 */ \
	std::uint64_t              unk58;        /* 10 */ \
	std::uint64_t              unk60;        /* 18 */ \
	std::uint64_t              unk68;        /* 20 */ \
	IDXGISwapChain*            swapChain;    /* 28 */ \
	std::uint64_t              unk78;        /* 30 */ \
	std::uint64_t              unk80;        /* 38 */ \
	ID3D11RenderTargetView1*   renderView;   /* 40 */ \
	ID3D11ShaderResourceView1* resourceView; /* 48 */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x50);

		[[nodiscard]] NiTexture::RendererData* CreateRenderTexture(std::uint32_t width, std::uint32_t height)
		{
			using func_t = decltype(&BSRenderManager::CreateRenderTexture);
			REL::Relocation<func_t> func{ RELOCATION_ID(75507, 77299) };
			return func(this, width, height);
		}

		[[nodiscard]] static BSRenderManager* GetSingleton() noexcept
		{
			REL::Relocation<BSRenderManager*> instance{ RELOCATION_ID(524907, 411393) };
			return instance.get();
		}

		[[nodiscard]] inline RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x48, 0x50);
		}

		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x48, 0x50);
		}

		[[nodiscard]] inline SKSE::WinAPI::CRITICAL_SECTION& GetLock() noexcept
		{
			return REL::RelocateMember<SKSE::WinAPI::CRITICAL_SECTION>(this, 0x2790, 0x2F00);
		}

		[[nodiscard]] inline const SKSE::WinAPI::CRITICAL_SECTION& GetLock() const noexcept
		{
			return REL::RelocateMember<SKSE::WinAPI::CRITICAL_SECTION>(this, 0x2790, 0x2F00);
		}

#ifndef ENABLE_SKYRIM_VR
		std::uint64_t unk00[0x48 >> 3];                              // 0000
		RUNTIME_DATA_CONTENT;                                        // 0048
		std::uint64_t                  unk98[(0x2788 - 0x90) >> 3];  // 0098
		SKSE::WinAPI::CRITICAL_SECTION lock;                         // 2790
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
		std::uint64_t unk00[0x50 >> 3];                              // 0000
		RUNTIME_DATA_CONTENT;                                        // 0050
		std::uint64_t                  unkA0[(0x2F00 - 0xA0) >> 3];  // 00A0
		SKSE::WinAPI::CRITICAL_SECTION lock;                         // 2F00
#endif
	};
#ifndef ENABLE_SKYRIM_VR
	static_assert(sizeof(BSRenderManager) == 0x27B8);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
	static_assert(sizeof(BSRenderManager) == 0x2F28);
#endif
}
