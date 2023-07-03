#pragma once

#include "RE/B/BSEffectShaderData.h"
#include "RE/B/BSShaderMaterial.h"
#include "RE/B/BSTSmartPointer.h"
#include "RE/N/NiShadeProperty.h"

namespace RE
{
	class BSRenderPass;
	class BSShaderAccumulator;
	class BSShaderMaterial;
	class BSShaderPropertyLightData;
	class NiSourceTexture;

	class BSShaderProperty : public NiShadeProperty
	{
	private:
		static constexpr auto BIT64 = static_cast<std::uint64_t>(1);

	public:
		inline static constexpr auto RTTI = RTTI_BSShaderProperty;
		inline static auto           Ni_RTTI = NiRTTI_BSShaderProperty;

		class ForEachVisitor
		{
		public:
			inline static constexpr auto RTTI = RTTI_BSShaderProperty__ForEachVisitor;

			virtual ~ForEachVisitor();  // 00

			// add
			virtual std::uint32_t Accept(NiSourceTexture* a_texture) = 0;  // 01
		};

		struct RenderPassArray
		{
			BSRenderPass* head;  // 0
		};
		static_assert(sizeof(RenderPassArray) == 0x8);

		enum class EShaderPropertyFlag : std::uint64_t
		{
			kSpecular = BIT64 << 0,
			kSkinned = BIT64 << 1,
			kTempRefraction = BIT64 << 2,
			kVertexAlpha = BIT64 << 3,
			kGrayscaleToPaletteColor = BIT64 << 4,
			kGrayscaleToPaletteAlpha = BIT64 << 5,
			kFalloff = BIT64 << 6,
			kEnvMap = BIT64 << 7,
			kReceiveShadows = BIT64 << 8,
			kCastShadows = BIT64 << 9,
			kFace = BIT64 << 10,
			kParallax = BIT64 << 11,
			kModelSpaceNormals = BIT64 << 12,
			kNonProjectiveShadows = BIT64 << 13,
			kMultiTextureLandscape = BIT64 << 14,
			kRefraction = BIT64 << 15,
			kRefractionFalloff = BIT64 << 16,
			kEyeReflect = BIT64 << 17,
			kHairTint = BIT64 << 18,
			kScreendoorAlphaFade = BIT64 << 19,
			kLocalMapClear = BIT64 << 20,
			kFaceGenRGBTint = BIT64 << 21,
			kOwnEmit = BIT64 << 22,
			kProjectedUV = BIT64 << 23,
			kMultipleTextures = BIT64 << 24,
			kRemappableTextures = BIT64 << 25,
			kDecal = BIT64 << 26,
			kDynamicDecal = BIT64 << 27,
			kParallaxOcclusion = BIT64 << 28,
			kExternalEmittance = BIT64 << 29,
			kSoftEffect = BIT64 << 30,
			kZBufferTest = BIT64 << 31,
			kZBufferWrite = BIT64 << 32,
			kLODLandscape = BIT64 << 33,
			kLODObjects = BIT64 << 34,
			kNoFade = BIT64 << 35,
			kTwoSided = BIT64 << 36,
			kVertexColors = BIT64 << 37,
			kGlowMap = BIT64 << 38,
			kAssumeShadowmask = BIT64 << 39,
			kCharacterLighting = BIT64 << 40,
			kMultiIndexSnow = BIT64 << 41,
			kVertexLighting = BIT64 << 42,
			kUniformScale = BIT64 << 43,
			kFitSlope = BIT64 << 44,
			kBillboard = BIT64 << 45,
			kNoLODLandBlend = BIT64 << 46,
			kEnvmapLightFade = BIT64 << 47,
			kWireframe = BIT64 << 48,
			kWeaponBlood = BIT64 << 49,
			kHideOnLocalMap = BIT64 << 50,
			kPremultAlpha = BIT64 << 51,
			kCloudLOD = BIT64 << 52,
			kAnisotropicLighting = BIT64 << 53,
			kNoTransparencyMultiSample = BIT64 << 54,
			kMenuScreen = BIT64 << 55,
			kMultiLayerParallax = BIT64 << 56,
			kSoftLighting = BIT64 << 57,
			kRimLighting = BIT64 << 58,
			kBackLighting = BIT64 << 59,
			kSnow = BIT64 << 60,
			kTreeAnim = BIT64 << 61,
			kEffectLighting = BIT64 << 62,
			kHDLODObjects = BIT64 << 63
		};

		enum class EShaderPropertyFlag8 : std::uint8_t
		{
			kSpecular = 0,
			kSkinned = 1,
			kTempRefraction = 2,
			kVertexAlpha = 3,
			kGrayscaleToPaletteColor = 4,
			kGrayscaleToPaletteAlpha = 5,
			kFalloff = 6,
			kEnvMap = 7,
			kReceiveShadows = 8,
			kCastShadows = 9,
			kFace = 10,
			kParallax = 11,
			kModelSpaceNormals = 12,
			kNonProjectiveShadows = 13,
			kMultiTextureLandscape = 14,
			kRefraction = 15,
			kRefractionFalloff = 16,
			kEyeReflect = 17,
			kHairTint = 18,
			kScreendoorAlphaFade = 19,
			kLocalMapClear = 20,
			kFaceGenRGBTint = 21,
			kOwnEmit = 22,
			kProjectedUV = 23,
			kMultipleTextures = 24,
			kRemappableTextures = 25,
			kDecal = 26,
			kDynamicDecal = 27,
			kParallaxOcclusion = 28,
			kExternalEmittance = 29,
			kSoftEffect = 30,
			kZBufferTest = 31,
			kZBufferWrite = 32,
			kLODLandscape = 33,
			kLODObjects = 34,
			kNoFade = 35,
			kTwoSided = 36,
			kVertexColors = 37,
			kGlowMap = 38,
			kAssumeShadowmask = 39,
			kCharacterLighting = 40,
			kMultiIndexSnow = 41,
			kVertexLighting = 42,
			kUniformScale = 43,
			kFitSlope = 44,
			kBillboard = 45,
			kNoLODLandBlend = 46,
			kEnvmapLightFade = 47,
			kWireframe = 48,
			kWeaponBlood = 49,
			kHideOnLocalMap = 50,
			kPremultAlpha = 51,
			kCloudLOD = 52,
			kAnisotropicLighting = 53,
			kNoTransparencyMultiSample = 54,
			kMenuScreen = 55,
			kMultiLayerParallax = 56,
			kSoftLighting = 57,
			kRimLighting = 58,
			kBackLighting = 59,
			kSnow = 60,
			kTreeAnim = 61,
			kEffectLighting = 62,
			kHDLODObjects = 63
		};

		~BSShaderProperty() override;  // 00

		// override (NiShadeProperty)
		const NiRTTI* GetRTTI() const override;                           // 02
		NiObject*     CreateClone(NiCloningProcess& a_cloning) override;  // 17 - { return 0; }
		void          LoadBinary(NiStream& a_stream) override;            // 18
		void          LinkObject(NiStream& a_stream) override;            // 19 - { NiShadeProperty::LinkObject(a_stream); }
		bool          RegisterStreamables(NiStream& a_stream) override;   // 1A - { return NiShadeProperty::RegisterStreamables(a_stream); }
		void          SaveBinary(NiStream& a_stream) override;            // 1B
		bool          IsEqual(NiObject* a_object) override;               // 1C - { return false; }
		void          PostLinkObject(NiStream& a_stream) override;        // 1E - { NiObjectNET::PostLinkObject(a_stream); }
		bool          SetupGeometry(BSGeometry* a_geometry) override;     // 27 - { return 1; }
		void          Unk_29(void) override;                              // 29

		// add
		virtual RenderPassArray*               GetRenderPasses(BSGeometry* a_geometry, std::uint32_t a_arg2, BSShaderAccumulator* a_accumulator) = 0;  // 2A
		virtual void                           Unk_2B(void);                                                                                           // 2B - { return 0; }
		virtual void                           Unk_2C(void);                                                                                           // 2C - { return 0; }
		virtual void                           Unk_2D(void);                                                                                           // 2D - { return 0; }
		virtual void                           Unk_2E(void);                                                                                           // 2E - { return 1; }
		virtual void                           Unk_2F(void);                                                                                           // 2F - { return 0; }
		virtual bool                           CanMerge(const BSShaderProperty* a_other);                                                              // 30
		virtual void                           SetMaterialAlpha(float a_alpha);                                                                        // 31 - { return; }
		[[nodiscard]] virtual float            QMaterialAlpha();                                                                                       // 32 - { return 1.0; }
		virtual std::int32_t                   ForEachTexture(ForEachVisitor& a_visitor);                                                              // 33 - { return 1; }
		virtual void                           DoClearRenderPasses();                                                                                  // 34
		virtual std::int32_t                   QShader();                                                                                              // 35 - { return 0; }
		virtual void                           Unk_36(void);                                                                                           // 36 - { return 0; }
		[[nodiscard]] virtual NiSourceTexture* GetBaseTexture();                                                                                       // 37 - { return 0; }
		virtual void                           Unk_38(void);                                                                                           // 38 - { return 0; }
		[[nodiscard]] virtual bool             AcceptsEffectData() const;                                                                              // 39 - { return false; }
		virtual void                           Unk_3A(void);                                                                                           // 3A - { return; }
		virtual void                           Unk_3B(void);                                                                                           // 3B - { return; }
		virtual void                           Unk_3C(void);                                                                                           // 3C - { return 0; }
		virtual std::uint32_t                  DetermineUtilityShaderDecl();                                                                           // 3D - { return 0; }
		virtual BSShaderMaterial::Type         GetMaterialType();                                                                                      // 3E - { return 0; }

		void SetEffectShaderData(const BSTSmartPointer<BSEffectShaderData>& a_data);
		void SetMaterial(BSShaderMaterial* a_material, bool a_unk1);
		void SetFlags(EShaderPropertyFlag8 a_flag, bool a_set);

		// members
		float                                                alpha;                // 30
		std::int32_t                                         lastRenderPassState;  // 34
		stl::enumeration<EShaderPropertyFlag, std::uint64_t> flags;                // 38
		RenderPassArray                                      renderPassList;       // 40
		std::uint64_t                                        unk48;                // 48
		RenderPassArray                                      debugRenderPassList;  // 50
		std::uint64_t                                        unk58;                // 58
		BSFadeNode*                                          fadeNode;             // 60
		BSTSmartPointer<BSEffectShaderData>                  effectData;           // 68
		BSShaderPropertyLightData*                           lightData;            // 70
		BSShaderMaterial*                                    material;             // 78
		std::uint64_t                                        unk80;                // 80
	};
	static_assert(sizeof(BSShaderProperty) == 0x88);
}
