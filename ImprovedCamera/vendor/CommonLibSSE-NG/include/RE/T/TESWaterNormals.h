#pragma once

#include "RE/N/NiRefObject.h"
#include "RE/N/NiSmartPointer.h"

namespace RE
{
	class BSWaterShaderMaterial;
	class NiSourceTexture;
	class TESWaterForm;

	class TESWaterNormals : public NiRefObject
	{
	public:
		inline static constexpr auto RTTI = RTTI_TESWaterNormals;

		virtual ~TESWaterNormals();  // 00

		// members
		std::uint8_t               flags;          // 10
		TESWaterForm*              waterType;      // 18
		std::uint32_t              unk20;          // 20
		BSWaterShaderMaterial*     waterMaterial;  // 28
		NiPointer<NiSourceTexture> noiseLayer0;    // 30
		NiPointer<NiSourceTexture> noiseLayer1;    // 38
		NiPointer<NiSourceTexture> noiseLayer2;    // 40
		NiPointer<NiSourceTexture> noiseLayer3;    // 48
	};
	static_assert(sizeof(TESWaterNormals) == 0x50);
}
