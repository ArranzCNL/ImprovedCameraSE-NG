#pragma once

namespace RE
{
	namespace BSGraphics
	{
		enum class Format
		{
			kNone = static_cast<std::underlying_type_t<Format>>(-1),
			kUnknown = 0,
			kR32G32B32A32_TYPELESS = 1,
			kR32G32B32A32_FLOAT = 2,
			kR32G32B32A32_UINT = 3,
			kR32G32B32A32_SINT = 4,
			kR32G32B32_TYPELESS = 5,
			kR32G32B32_FLOAT = 6,
			kR32G32B32_UINT = 7,
			kR32G32B32_SINT = 8,
			kR16G16B16A16_TYPELESS = 9,
			kR16G16B16A16_FLOAT = 10,
			kR16G16B16A16_UNORM = 11,
			kR16G16B16A16_UINT = 12,
			kR16G16B16A16_SNORM = 13,
			kR16G16B16A16_SINT = 14,
			kR32G32_TYPELESS = 15,
			kR32G32_FLOAT = 16,
			kR32G32_UINT = 17,
			kR32G32_SINT = 18,
			kR32G8X24_TYPELESS = 19,
			kD32_FLOAT_S8X24_UINT = 20,
			kR32_FLOAT_X8X24_TYPELESS = 21,
			kX32_TYPELESS_G8X24_UINT = 22,
			kR10G10B10A2_TYPELESS = 23,
			kR10G10B10A2_UNORM = 24,
			kR10G10B10A2_UINT = 25,
			kR11G11B10_FLOAT = 26,
			kR8G8B8A8_TYPELESS = 27,
			kR8G8B8A8_UNORM = 28,
			kR8G8B8A8_UNORM_SRGB = 29,
			kR8G8B8A8_UINT = 30,
			kR8G8B8A8_SNORM = 31,
			kR8G8B8A8_SINT = 32,
			kR16G16_TYPELESS = 33,
			kR16G16_FLOAT = 34,
			kR16G16_UNORM = 35,
			kR16G16_UINT = 36,
			kR16G16_SNORM = 37,
			kR16G16_SINT = 38,
			kR32_TYPELESS = 39,
			kD32_FLOAT = 40,
			kR32_FLOAT = 41,
			kR32_UINT = 42,
			kR32_SINT = 43,
			kR24G8_TYPELESS = 44,
			kD24_UNORM_S8_UINT = 45,
			kR24_UNORM_X8_TYPELESS = 46,
			kX24_TYPELESS_G8_UINT = 47,
			kR8G8_TYPELESS = 48,
			kR8G8_UNORM = 49,
			kR8G8_UINT = 50,
			kR8G8_SNORM = 51,
			kR8G8_SINT = 52,
			kR16_TYPELESS = 53,
			kR16_FLOAT = 54,
			kD16_UNORM = 55,
			kR16_UNORM = 56,
			kR16_UINT = 57,
			kR16_SNORM = 58,
			kR16_SINT = 59,
			kR8_TYPELESS = 60,
			kR8_UNORM = 61,
			kR8_UINT = 62,
			kR8_SNORM = 63,
			kR8_SINT = 64,
			kA8_UNORM = 65,
			kR1_UNORM = 66,
			kR9G9B9E5_SHAREDEXP = 67,
			kR8G8_B8G8_UNORM = 68,
			kG8R8_G8B8_UNORM = 69,
			kBC1_TYPELESS = 70,
			kBC1_UNORM = 71,
			kBC1_UNORM_SRGB = 72,
			kBC2_TYPELESS = 73,
			kBC2_UNORM = 74,
			kBC2_UNORM_SRGB = 75,
			kBC3_TYPELESS = 76,
			kBC3_UNORM = 77,
			kBC3_UNORM_SRGB = 78,
			kBC4_TYPELESS = 79,
			kBC4_UNORM = 80,
			kBC4_SNORM = 81,
			kBC5_TYPELESS = 82,
			kBC5_UNORM = 83,
			kBC5_SNORM = 84,
			kB5G6R5_UNORM = 85,
			kB5G5R5A1_UNORM = 86,
			kB8G8R8A8_UNORM = 87,
			kB8G8R8X8_UNORM = 88,
			kR10G10B10_XR_BIAS_A2_UNORM = 89,
			kB8G8R8A8_TYPELESS = 90,
			kB8G8R8A8_UNORM_SRGB = 91,
			kB8G8R8X8_TYPELESS = 92,
			kB8G8R8X8_UNORM_SRGB = 93,
			kBC6H_TYPELESS = 94,
			kBC6H_UF16 = 95,
			kBC6H_SF16 = 96,
			kBC7_TYPELESS = 97,
			kBC7_UNORM = 98,
			kBC7_UNORM_SRGB = 99,
			kAYUV = 100,
			kY410 = 101,
			kY416 = 102,
			kNV12 = 103,
			kP010 = 104,
			kP016 = 105,
			k420_OPAQUE = 106,
			kYUY2 = 107,
			kY210 = 108,
			kY216 = 109,
			kNV11 = 110,
			kAI44 = 111,
			kIA44 = 112,
			kP8 = 113,
			kA8P8 = 114,
			kB4G4R4A4_UNORM = 115,
			kP208 = 130,
			kV208 = 131,
			kV408 = 132,
		};

		struct RenderTargetProperties
		{
			std::uint32_t                           width;                   // 00
			std::uint32_t                           height;                  // 04
			stl::enumeration<Format, std::uint32_t> format;                  // 08
			bool                                    copyable;                // 0C
			bool                                    supportUnorderedAccess;  // 0D
			bool                                    allowMipGeneration;      // 0E
			bool                                    unk0F;                   // 0F
			std::int32_t                            mipLevel;                // 10
			std::uint32_t                           textureTarget;           // 14
			std::uint32_t                           unk18;                   // 18
		};
		static_assert(sizeof(RenderTargetProperties) == 0x1C);

		struct DepthStencilTargetProperties
		{
			std::uint32_t width;           // 00
			std::uint32_t height;          // 04
			std::uint32_t arraySize;       // 08
			bool          unk0C;           // 0C
			bool          stencil;         // 0D
			bool          use16BitsDepth;  // 0E
		};
		static_assert(sizeof(DepthStencilTargetProperties) == 0x10);

		struct CubeMapRenderTargetProperties
		{
			std::uint32_t                           width;   // 00
			std::uint32_t                           height;  // 04
			stl::enumeration<Format, std::uint32_t> format;  // 08
		};
		static_assert(sizeof(CubeMapRenderTargetProperties) == 0xC);
	}
}
