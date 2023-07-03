#pragma once

namespace RE
{
	class BSGeometry;
	class BSLight;
	class BSShader;
	class BSShaderProperty;

	class BSRenderPass
	{
	public:
		struct LODMode
		{
			std::uint8_t index: 7;
			bool         singleLevel: 1;
		};
		static_assert(sizeof(LODMode) == 0x1);

		// members
		BSShader*         shader;            // 00
		BSShaderProperty* shaderProperty;    // 08
		BSGeometry*       geometry;          // 10
		std::uint32_t     passEnum;          // 18
		std::uint8_t      accumulationHint;  // 1C
		std::uint8_t      extraParam;        // 1D
		LODMode           LODMode;           // 1E
		std::uint8_t      numLights;         // 1F
		std::uint16_t     unk20;             // 20
		BSRenderPass*     next;              // 28
		BSRenderPass*     passGroupNext;     // 30
		BSLight**         sceneLights;       // 38
		std::uint32_t     cachePoolId;       // 40
		std::uint32_t     pad44;             // 44
	};
	static_assert(sizeof(BSRenderPass) == 0x48);
}
