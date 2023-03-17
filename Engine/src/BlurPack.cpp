#include "BlurPack.h"
#include "MathTool.h"
BlurPack::BlurPack(Graphics& gfx, const std::wstring& shader, int radius, float sigma) noexcept
	:
	m_ps(gfx, shader),
	m_Kernel(gfx, 0u),
	m_Control(gfx, 1u),
	m_radius(radius),
	m_sigma(sigma)
{
	SetGaussKernel(gfx, radius, sigma);
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

void BlurPack::SetGaussKernel(Graphics& gfx, int radius, float sigma) noexcept
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

void BlurPack::SetBoxKernel(Graphics& gfx, int radius) noexcept
{
	Kernel k;
	k.nTabs = 2 * radius + 1;
	float sum = 0.0f;		
	const float c = 1.0f / k.nTabs;
	for (int i = 0; i < k.nTabs; i++)
	{
		k.cofficients[i].x = c;
	}
	m_Kernel.Update(gfx, k);
}

void BlurPack::RenderWidgets(Graphics& gfx) noexcept(!IS_DEBUG)
{
	bool fliter_change = false;
	{
		const char* items[] = {"Gauss", "Box"};
		static const char* cur_item = items[0];
		if (ImGui::BeginCombo("Fliter Type", cur_item))
		{
			for (int i = 0; i < std::size(items); i++)
			{
				const bool isSelected = (cur_item == items[i]);
				if (ImGui::Selectable(items[i], isSelected))
				{
					fliter_change = true;
					cur_item = items[i];
					if (cur_item == items[0])
					{
						m_KernelType = KernelType::Gauss;
					}
					if (cur_item == items[1])
					{
						m_KernelType = KernelType::Box;
					}
				}
			}
			ImGui::EndCombo();
		}
	}
	bool radChange = ImGui::SliderInt("Radius", &m_radius, 0, 15);
	bool sigChange = false;
	if (m_KernelType == KernelType::Gauss)
	{
		sigChange = ImGui::SliderFloat("Sigma", &m_sigma, 0.1f, 10.0f);
	}
	if (radChange || sigChange || fliter_change)
	{
		if (m_KernelType == KernelType::Gauss)
		{
			SetGaussKernel(gfx, m_radius, m_sigma);
		}
		else if (m_KernelType == KernelType::Box)
		{
			SetBoxKernel(gfx, m_radius);
		}
	}
}