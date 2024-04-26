/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "utils/ICMath.h"

namespace Utils {

	void QuaternionToMatrix(const RE::NiQuaternion* src, RE::NiMatrix3* dst)
	{
		float xx = src->x * src->x;
		float xy = src->x * src->y;
		float xz = src->x * src->z;
		float xw = src->x * src->w;
		float yy = src->y * src->y;
		float yz = src->y * src->z;
		float yw = src->y * src->w;
		float zz = src->z * src->z;
		float zw = src->z * src->w;

		dst->entry[0][0] = 1.0f - (zz + yy) * 2.0f;
		dst->entry[0][1] = (xy - zw) * 2.0f;
		dst->entry[0][2] = (yw + xz) * 2.0f;
		dst->entry[1][0] = (xy + zw) * 2.0f;
		dst->entry[1][1] = 1.0f - (zz + xx) * 2.0f;
		dst->entry[1][2] = (yz - xw) * 2.0f;
		dst->entry[2][0] = (xz - yw) * 2.0f;
		dst->entry[2][1] = (yz + xw) * 2.0f;
		dst->entry[2][2] = 1.0f - (xx + yy) * 2.0f;
	}

	void RotMatrixToQuaternion(const RE::NiMatrix3* src, RE::NiQuaternion* dst)
	{
		float diag = src->entry[0][0] + src->entry[1][1] + src->entry[2][2];
		float s, d;

		if (diag > 0.0f)
		{
			s = sqrt(diag + 1.0f);
			d = 0.5f / s;

			dst->w = 0.5f * s;
			dst->x = (src->entry[2][1] - src->entry[1][2]) * d;
			dst->y = (src->entry[0][2] - src->entry[2][0]) * d;
			dst->z = (src->entry[1][0] - src->entry[0][1]) * d;
		}
		else if ((src->entry[0][0] >= src->entry[1][1]) && (src->entry[0][0] >= src->entry[2][2]))
		{
			s = sqrt(((1.0f + src->entry[0][0]) - src->entry[1][1]) - src->entry[2][2]);
			d = 0.5f / s;

			dst->x = 0.5f * s;
			dst->y = (src->entry[1][0] + src->entry[0][1]) * d;
			dst->z = (src->entry[2][0] + src->entry[0][2]) * d;
			dst->w = (src->entry[2][1] - src->entry[1][2]) * d;
		}
		else if (src->entry[1][1] > src->entry[2][2])
		{
			s = sqrt(((1.0f + src->entry[1][1]) - src->entry[0][0]) - src->entry[2][2]);
			d = 0.5f / s;

			dst->x = (src->entry[0][1] + src->entry[1][0]) * d;
			dst->y = 0.5f * s;
			dst->z = (src->entry[1][2] + src->entry[2][1]) * d;
			dst->w = (src->entry[0][2] - src->entry[2][0]) * d;
		}
		else
		{
			s = sqrt(((1.0f + src->entry[2][2]) - src->entry[0][0]) - src->entry[1][1]);
			d = 0.5f / s;

			dst->x = (src->entry[0][2] + src->entry[2][0]) * d;
			dst->y = (src->entry[1][2] + src->entry[2][1]) * d;
			dst->z = 0.5f * s;
			dst->w = (src->entry[1][0] - src->entry[0][1]) * d;
		}
	}

	void SlerpQuat(RE::NiQuaternion* dst, const RE::NiQuaternion* quat1, const RE::NiQuaternion* quat2, float amount)
	{
		float sum = (((quat1->x * quat2->x) + (quat1->y * quat2->y)) + (quat1->z * quat2->z)) + (quat1->w * quat2->w);
		bool negative = false;
		float f1, f2;

		if (sum < 0.0f)
		{
			negative = true;
			sum = -sum;
		}
		if (sum > 0.999999f)
		{
			f2 = 1.0f - amount;
			f1 = negative ? -amount : amount;
		}
		else
		{
			float f3 = acos(sum);
			float f4 = 1.0f / sin(f3);

			f2 = sin(((1.0f - amount) * f3)) * f4;
			f1 = negative ? ((-sin((amount * f3))) * f4) : ((sin((amount * f3))) * f4);
		}

		dst->x = (f2 * quat1->x) + (f1 * quat2->x);
		dst->y = (f2 * quat1->y) + (f1 * quat2->y);
		dst->z = (f2 * quat1->z) + (f1 * quat2->z);
		dst->w = (f2 * quat1->w) + (f1 * quat2->w);
	}

	void EulerToMatrix(RE::NiMatrix3* dst, float x, float y, float z)
	{
		dst->entry[0][0] = cos(y) * cos(z);
		dst->entry[0][1] = -cos(y) * sin(z);
		dst->entry[0][2] = sin(y);
		dst->entry[1][0] = cos(z) * sin(x) * sin(y) + cos(x) * sin(z);
		dst->entry[1][1] = cos(x) * cos(z) - sin(x) * sin(y) * sin(z);
		dst->entry[1][2] = -cos(y) * sin(x);
		dst->entry[2][0] = -cos(x) * cos(z) * sin(y) + sin(x) * sin(z);
		dst->entry[2][1] = cos(z) * sin(x) + cos(x) * sin(y) * sin(z);
		dst->entry[2][2] = cos(x) * cos(y);
	}

	void MatrixToEuler(const RE::NiMatrix3* src, float* x, float* y, float* z)
	{
		if (src->entry[0][2] < +1.0f)
		{
			if (src->entry[0][2] > -1.0f)
			{
				*y = asin(src->entry[0][2]);
				*x = atan2(-src->entry[1][2], src->entry[2][2]);
				*z = atan2(-src->entry[0][1], src->entry[0][0]);
			}
			else
			{
				*y = -M_PI * 0.5f;
				*x = -atan2(src->entry[1][0], src->entry[1][1]);
				*z = 0.0f;
			}
		}
		else
		{
			*y = +M_PI * 0.5f;
			*x = atan2(src->entry[1][0], src->entry[1][1]);
			*z = 0.0f;
		}
	}

	void MatrixInverse(const RE::NiMatrix3* src, RE::NiMatrix3* dst)
	{
		float f1 = (src->entry[2][2] * src->entry[1][1]) - (src->entry[2][1] * src->entry[1][2]);
		float f2 = src->entry[2][2] * src->entry[1][0];
		float f3 = src->entry[1][2] * src->entry[2][0];
		float f4 = (src->entry[1][0] * src->entry[2][1]) - (src->entry[1][1] * src->entry[2][0]);
		float det = f4 * src->entry[0][2] + f1 * src->entry[0][0] - (f2 - f3) * src->entry[0][1];

		dst->entry[0][0] = f1 / det;
		dst->entry[0][1] = (src->entry[2][1] * src->entry[0][2] - src->entry[2][2] * src->entry[0][1]) / det;
		dst->entry[0][2] = (src->entry[1][2] * src->entry[0][1] - src->entry[1][1] * src->entry[0][2]) / det;
		dst->entry[1][0] = (f3 - f2) / det;
		dst->entry[1][1] = (src->entry[2][2] * src->entry[0][0] - src->entry[2][0] * src->entry[0][2]) / det;
		dst->entry[1][2] = (src->entry[0][2] * src->entry[1][0] - src->entry[1][2] * src->entry[0][0]) / det;
		dst->entry[2][0] = f4 / det;
		dst->entry[2][1] = (src->entry[2][0] * src->entry[0][1] - src->entry[2][1] * src->entry[0][0]) / det;
		dst->entry[2][2] = (src->entry[0][0] * src->entry[1][1] - src->entry[0][1] * src->entry[1][0]) / det;
	}

	void MatrixVectorMultiply(RE::NiPoint3* dst, const RE::NiMatrix3* src, const RE::NiPoint3* vec)
	{
		dst->x = (src->entry[0][0] * vec->x) + (src->entry[0][1] * vec->y) + (src->entry[0][2] * vec->z);
		dst->y = (src->entry[1][0] * vec->x) + (src->entry[1][1] * vec->y) + (src->entry[1][2] * vec->z);
		dst->z = (src->entry[2][0] * vec->x) + (src->entry[2][1] * vec->y) + (src->entry[2][2] * vec->z);
	}
}
