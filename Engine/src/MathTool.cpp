#include "MathTool.h"
#include <cmath>
namespace math_tool
{
	DirectX::XMFLOAT3 ExtraEulerAngle(const DirectX::XMFLOAT4X4 matrix)
	{
		DirectX::XMFLOAT3 result;
		result.x = asin(-matrix._32);
		if (cos(result.x) > 0.0001f)
		{
			result.y = atan2f(matrix._31, matrix._33);
			result.z = atan2f(matrix._12, matrix._22);
		}
		else
		{
			result.y = 0.0f;
			result.z = atan2f(-matrix._21, matrix._11);
		}
		return result;
	}
	DirectX::XMFLOAT3 ExtraTranslation(const DirectX::XMFLOAT4X4 matrix)
	{
		return { matrix._41, matrix._42, matrix._43 };
	}
	DirectX::XMFLOAT3 ExtraScaling(const DirectX::XMFLOAT4X4 matrix)
	{
		return {
			math_tool::sqrt(matrix._11 * matrix._11 + matrix._21 * matrix._21 + matrix._31 * matrix._31),
			math_tool::sqrt(matrix._12 * matrix._12 + matrix._22 * matrix._22 + matrix._32 * matrix._32),
			math_tool::sqrt(matrix._13 * matrix._13 + matrix._23 * matrix._23 + matrix._33 * matrix._33),
		};
	}
}