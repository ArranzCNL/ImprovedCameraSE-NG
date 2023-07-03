#pragma once

#include "RE/B/BSVRInterface.h"

namespace RE
{
	class BSOpenVR : public BSVRInterface
	{
	public:
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
		constexpr static auto RTTI = RTTI_BSOpenVR;
#endif
		~BSOpenVR() override;  // 00

		// override BSVRInterface
		void* Shutdown(void) override;  // 01
		void  Unk_02(void) override;    // 02
		void  Unk_03(void) override;    // 03
		void  Unk_04(void) override;    // 04
		void  Unk_05(void) override;    // 05
		void  Unk_06(void) override;    // 06
		void  Unk_07(void) override;    // 07
		void  Unk_08(void) override;    // 08
		void  Unk_09(void) override;    // 09
		void  Unk_0A(void) override;    // 0A
		void  Unk_0B(void) override;    // 0B
		void  Unk_0C(void) override;    // 0C
		void  Unk_0D(void) override;    // 0D
		void  Unk_0E(void) override;    // 0E
		void  Unk_0F(void) override;    // 0F
		void  Unk_10(void) override;    // 10
		void  Unk_12(void) override;    // 12
		void  Unk_13(void) override;    // 13
		void  Unk_14(void) override;    // 14
		void  Unk_16(void) override;    // 16
		void  Unk_17(void) override;    // 17
	};
}
