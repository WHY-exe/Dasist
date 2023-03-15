#include "BlurPack.h"
#include "MathTool.h"
BlurPack::BlurPack(Graphics& gfx, int radius, float sigma) noexcept
	:
	m_ps(gfx, L"res\\cso\\BlurPS.cso"),
	m_Kernel(gfx, 0u),
	m_Control(gfx, 1u),
	m_radius(radius),
	m_sigma(sigma)
{
	SetKernel(gfx, radius, sigma);
}

void BlurPack::Bind(Graphics& gfx) noexcept
{
	m_ps.Bind(gfx);
	m_Kernel.Bind(gfx);
	m_Control.Bind(gfx);
}

void BlurPack::SetVertical(Graphics& gfx) noexcept
{
	m_Control.Update(gfx, { FALSE });
}

void BlurPack::SetHorizontal(Graphics& gfx) noexcept
{
	m_Control.Update(gfx, { TRUE });
}

void BlurPack::SetKernel(Graphics& gfx, int radius, float sigma) noexcept
{
	Kernel k;
	k.nTabs = 2 * radius + 1;
	float sum = 0.0f;
	for (int i = 0; i < k.nTabs; i++)
	{
		const float x = float(i - (radius + 1));
		const float g = math_tool::gauss(x, sigma);

		sum += g;
		k.cofficients[i].x = g;
	}

	for (int i = 0; i < k.nTabs; i++)
	{
		// normalize the cofficients
		k.cofficients[i].x /= sum;
	}

	m_Kernel.Update(gfx, k);
}
