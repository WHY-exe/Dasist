#pragma once
#include <cmath>
#include <DirectXMath.h>
#define PI (float)3.14159265f
#define PI_D (double)3.1415926535897932

namespace math_tool {
	template<typename T>
	T wrap_angle(T theta) noexcept
	{
		constexpr T twoPi = (T)2.0 * (T)PI_D;
		const T mod = (T)fmod(theta, twoPi);
		if (mod > (T)PI_D)
		{
			return mod - twoPi;
		}
		else if (mod < -(T)PI_D)
		{
			return mod + twoPi;
		}
		return mod;
	};
	template <typename T>
	T square(T x) noexcept
	{
		return x * x;
	};
	template <typename T>
	T gauss(T x, T sigma) noexcept
	{
		// 这里假设μ为0
		return ((T)1.0 / sqrt( (T)2.0 * (T)PI_D * square(sigma) )) * exp( -square(x) / ((T)2.0 * square(sigma)) );
	};
	DirectX::XMFLOAT3 ExtraEulerAngle(const DirectX::XMFLOAT4X4 matrix);
	DirectX::XMFLOAT3 ExtraTranslation(const DirectX::XMFLOAT4X4 matrix);
	DirectX::XMFLOAT3 ExtraScaling(const DirectX::XMFLOAT4X4 matrix);


}