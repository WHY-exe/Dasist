#pragma once
#include <cmath>
#include <DirectXMath.h>
#define PI (float)3.14159265f
#define PI_D (double)3.1415926535897932

namespace math_tool {
	template<typename T>
	T wrap_angle(T theta)
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
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

	}
	DirectX::XMFLOAT3 ExtraEulerAngle(const DirectX::XMFLOAT4X4 matrix);
	DirectX::XMFLOAT3 ExtraTranslation(const DirectX::XMFLOAT4X4 matrix);
	DirectX::XMFLOAT3 ExtraScaling(const DirectX::XMFLOAT4X4 matrix);
}