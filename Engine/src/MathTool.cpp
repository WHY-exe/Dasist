#include "MathTool.h"

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

}