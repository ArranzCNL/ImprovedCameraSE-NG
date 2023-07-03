#pragma once

#include "RE/B/BSReloadShaderI.h"
#include "RE/N/NiBoneMatrixSetterI.h"
#include "RE/N/NiRefObject.h"

namespace RE
{
	class BSRenderPass;
	class BSShaderMaterial;

	struct ID3D11Buffer;
	struct ID3D11PixelShader;
	struct ID3D11VertexShader;

	namespace BSGraphics
	{
		class ConstantGroup
		{
		public:
			// members
			ID3D11Buffer* buffer;  // 00
			void*         data;    // 08
		};
		static_assert(sizeof(ConstantGroup) == 0x10);

		class PixelShader
		{
		public:
			// members
			std::uint32_t      id;                  // 00
			ID3D11PixelShader* shader;              // 08
			ConstantGroup      constantBuffers[3];  // 10
			std::int8_t        constantTable[64];   // 58
		};
		static_assert(sizeof(PixelShader) == 0x80);

		class VertexShader
		{
		public:
			// members
			std::uint32_t       id;                  // 00
			ID3D11VertexShader* shader;              // 08
			std::uint32_t       byteCodeSize;        // 10
			ConstantGroup       constantBuffers[3];  // 18
			std::uint64_t       shaderDesc;          // 48
			std::int8_t         constantTable[20];   // 50
			std::uint32_t       pad64;               // 64
			std::uint8_t        rawBytecode[0];      // 68
		};
		static_assert(sizeof(VertexShader) == 0x68);
	}

	namespace BSShaderTechniqueIDMap
	{
		template <class T>
		struct HashPolicy
		{
			[[nodiscard]] std::uint32_t operator()(const T a_shader) const noexcept
			{
				assert(a_shader != nullptr);
				return a_shader->id;
			}
		};

		template <class T>
		struct ComparePolicy
		{
			[[nodiscard]] bool operator()(const T a_lhs, const T a_rhs) const noexcept
			{
				assert(a_lhs != nullptr);
				assert(a_rhs != nullptr);
				return a_lhs->id == a_rhs->id;
			}
		};

		template <class T>
		using MapType = BSTSet<T, HashPolicy<T>, ComparePolicy<T>>;
	}

	class BSShader :
		public NiRefObject,          // 00
		public NiBoneMatrixSetterI,  // 10
		public BSReloadShaderI       // 18
	{
	public:
		inline static constexpr auto RTTI = RTTI_BSShader;
		inline static constexpr auto VTABLE = VTABLE_BSShader;

		~BSShader() override;  // 00

		// add
		virtual bool SetupTechnique(std::uint32_t a_technique) = 0;                                              // 02
		virtual void RestoreTechnique(std::uint32_t a_technique) = 0;                                            // 03
		virtual void SetupMaterial(const BSShaderMaterial* a_material);                                          // 04
		virtual void RestoreMaterial(const BSShaderMaterial* a_material);                                        // 05
		virtual void SetupGeometry(BSRenderPass* a_currentPass, std::uint32_t a_flags) = 0;                      // 06
		virtual void RestoreGeometry(BSRenderPass* a_currentPass, std::uint32_t a_renderFlags) = 0;              // 07
		virtual void GetTechniqueName(std::uint32_t a_techniqueID, char* a_buffer, std::uint32_t a_bufferSize);  // 08
		virtual void ReloadShaders(bool a_clear);                                                                // 09

		// members
		std::int32_t                                               shaderType;     // 20
		BSShaderTechniqueIDMap::MapType<BSGraphics::VertexShader*> vertexShaders;  // 28
		BSShaderTechniqueIDMap::MapType<BSGraphics::PixelShader*>  pixelShaders;   // 58
		const char*                                                fxpFilename;    // 88
	};
	static_assert(sizeof(BSShader) == 0x90);
}
