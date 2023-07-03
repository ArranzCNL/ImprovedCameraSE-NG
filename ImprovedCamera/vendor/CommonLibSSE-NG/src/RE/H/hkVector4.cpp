#include "RE/H/hkVector4.h"

namespace RE
{
	hkVector4& hkVector4::operator=(const hkVector4& a_rhs)
	{
		quad = a_rhs.quad;
		return *this;
	}

	hkVector4 hkVector4::operator+(const hkVector4& a_rhs) const
	{
		return _mm_add_ps(quad, a_rhs.quad);
	}

	hkVector4 hkVector4::operator-(const hkVector4& a_rhs) const
	{
		return _mm_sub_ps(quad, a_rhs.quad);
	}

	hkVector4 hkVector4::operator*(const hkVector4& a_rhs) const
	{
		return _mm_mul_ps(quad, a_rhs.quad);
	}

	hkVector4 hkVector4::operator/(const hkVector4& a_rhs) const
	{
		return _mm_div_ps(quad, a_rhs.quad);
	}

	bool hkVector4::IsEqual(const hkVector4& a_pt, float a_epsilon /*= 1e-3f*/) const
	{
		auto eps = _mm_set_ps1(a_epsilon);
		auto absoluteDiff = _mm_andnot_ps(_mm_set_ps1(-0.f), _mm_sub_ps(quad, a_pt.quad));

		return _mm_movemask_ps(_mm_cmple_ps(absoluteDiff, eps)) == 0xF;
	}

	hkVector4 hkVector4::Cross(const hkVector4& a_pt) const
	{
		hkQuadReal cross0 = _mm_mul_ps(_mm_shuffle_ps(quad, quad, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(a_pt.quad, a_pt.quad, _MM_SHUFFLE(3, 1, 0, 2)));
		hkQuadReal cross1 = _mm_mul_ps(_mm_shuffle_ps(quad, quad, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(a_pt.quad, a_pt.quad, _MM_SHUFFLE(3, 0, 2, 1)));
		return _mm_sub_ps(cross0, cross1);
	}

	float hkVector4::Dot3(const hkVector4& a_pt) const
	{
		hkQuadReal x2 = _mm_mul_ps(quad, a_pt.quad);
		hkQuadReal sum = _mm_add_ss(_mm_shuffle_ps(x2, x2, _MM_SHUFFLE(1, 1, 1, 1)), x2);

		float ret;
		_mm_store_ss(&ret, _mm_add_ss(_mm_shuffle_ps(x2, x2, _MM_SHUFFLE(2, 2, 2, 2)), sum));
		return ret;
	}

	float hkVector4::Dot4(const hkVector4& a_pt) const
	{
		hkQuadReal x2 = _mm_mul_ps(quad, a_pt.quad);
		hkQuadReal sum0 = _mm_add_ps(_mm_shuffle_ps(x2, x2, _MM_SHUFFLE(1, 0, 3, 2)), x2);
		hkQuadReal sum1 = _mm_shuffle_ps(sum0, sum0, _MM_SHUFFLE(2, 3, 0, 1));

		float ret;
		_mm_store_ss(&ret, _mm_add_ss(sum0, sum1));
		return ret;
	}

	float hkVector4::GetDistance3(const hkVector4& a_pt) const noexcept
	{
		hkVector4 distance = _mm_sub_ps(quad, a_pt.quad);
		return distance.Length3();
	}

	float hkVector4::GetSquaredDistance3(const hkVector4& a_pt) const noexcept
	{
		hkVector4 distance = _mm_sub_ps(quad, a_pt.quad);
		return distance.SqrLength3();
	}

	float hkVector4::Length3() const
	{
		return sqrt(Dot3(*this));
	}

	float hkVector4::SqrLength3() const
	{
		return Dot3(*this);
	}

	float hkVector4::Length4() const
	{
		return sqrt(Dot4(*this));
	}

	float hkVector4::SqrLength4() const
	{
		return Dot4(*this);
	}

}
