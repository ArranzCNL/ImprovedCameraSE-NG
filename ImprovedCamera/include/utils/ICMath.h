/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

constexpr auto M_PI = 3.141593f;


namespace Utils {

	extern void QuaternionToMatrix(const RE::NiQuaternion* src, RE::NiMatrix3* dst);
	extern void RotMatrixToQuaternion(const RE::NiMatrix3* src, RE::NiQuaternion* dst);
	extern void SlerpQuat(RE::NiQuaternion* dst, const RE::NiQuaternion* quat1, const RE::NiQuaternion* quat2, float amount);
	extern void EulerToMatrix(RE::NiMatrix3* src, float x, float y, float z);
	extern void MatrixToEuler(const RE::NiMatrix3* src, float* x, float* y, float* z);
	extern void MatrixInverse(const RE::NiMatrix3* src, RE::NiMatrix3* dst);
	extern void MatrixVectorMultiply(RE::NiPoint3* dst, const RE::NiMatrix3* src, const RE::NiPoint3* vec);

}
