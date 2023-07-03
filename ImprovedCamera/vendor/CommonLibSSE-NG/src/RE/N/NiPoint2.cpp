#include "RE/N/NiPoint2.h"

namespace RE
{
	float& NiPoint2::operator[](std::size_t a_idx)
	{
		assert(a_idx < 2);
		return std::addressof(x)[a_idx];
	}

	const float& NiPoint2::operator[](std::size_t a_idx) const
	{
		assert(a_idx < 2);
		return std::addressof(x)[a_idx];
	}

	bool NiPoint2::operator==(const NiPoint2& a_rhs) const
	{
		return (x == a_rhs.x && y == a_rhs.y);
	}

	bool NiPoint2::operator!=(const NiPoint2& a_rhs) const
	{
		return !operator==(a_rhs);
	}

	NiPoint2 NiPoint2::operator+(const NiPoint2& a_rhs) const
	{
		return NiPoint2(x + a_rhs.x, y + a_rhs.y);
	}

	NiPoint2 NiPoint2::operator-(const NiPoint2& a_rhs) const
	{
		return NiPoint2(x - a_rhs.x, y - a_rhs.y);
	}

	float NiPoint2::operator*(const NiPoint2& a_rhs) const
	{
		return x * a_rhs.x + y * a_rhs.y;
	}

	NiPoint2 NiPoint2::operator*(float a_scalar) const
	{
		return NiPoint2(x * a_scalar, y * a_scalar);
	}

	NiPoint2 NiPoint2::operator/(float a_scalar) const
	{
		return operator*(static_cast<float>(1.0) / a_scalar);
	}

	NiPoint2 NiPoint2::operator-() const
	{
		return NiPoint2(-x, -y);
	}

	NiPoint2& NiPoint2::operator+=(const NiPoint2& a_rhs)
	{
		x += a_rhs.x;
		y += a_rhs.y;
		return *this;
	}

	NiPoint2& NiPoint2::operator-=(const NiPoint2& a_rhs)
	{
		x -= a_rhs.x;
		y -= a_rhs.y;
		return *this;
	}

	NiPoint2& NiPoint2::operator*=(float a_scalar)
	{
		x *= a_scalar;
		y *= a_scalar;
		return *this;
	}

	NiPoint2& NiPoint2::operator/=(float a_scalar)
	{
		return operator*=(static_cast<float>(1.0) / a_scalar);
	}

	float NiPoint2::Cross(const NiPoint2& a_pt) const
	{
		return x * a_pt.y - y * a_pt.x;
	}

	float NiPoint2::Dot(const NiPoint2& a_pt) const
	{
		return x * a_pt.x + y * a_pt.y;
	}

	float NiPoint2::GetDistance(const NiPoint2& a_pt) const noexcept
	{
		return std::sqrtf(GetSquaredDistance(a_pt));
	}

	float NiPoint2::GetSquaredDistance(const NiPoint2& a_pt) const noexcept
	{
		const float dx = a_pt.x - x;
		const float dy = a_pt.y - y;
		return dx * dx + dy * dy;
	}

	float NiPoint2::Length() const
	{
		return std::sqrtf(x * x + y * y);
	}

	float NiPoint2::SqrLength() const
	{
		return x * x + y * y;
	}

	float NiPoint2::Unitize()
	{
		auto length = Length();
		if (length == 1.f) {
			return length;
		} else if (length > FLT_EPSILON) {
			operator/=(length);
		} else {
			x = 0.0;
			y = 0.0;
			length = 0.0;
		}
		return length;
	}
}
