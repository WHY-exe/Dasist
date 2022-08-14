#pragma once
#include "GfxThrowMacro.h"
#include "Bindable.h"
class VertexBuffer :public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
        :
        m_uStride(sizeof(V))
	{
        IMPORT_INFOMAN(gfx);
        // buffer description
        D3D11_BUFFER_DESC vbbd = {};
        vbbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbbd.Usage = D3D11_USAGE_DEFAULT;
        vbbd.CPUAccessFlags = 0u;
        vbbd.MiscFlags = 0u;
        vbbd.ByteWidth = (UINT)vertices.size() * m_uStride;
        vbbd.StructureByteStride = m_uStride;
        D3D11_SUBRESOURCE_DATA sdVerts = {};
        sdVerts.pSysMem = vertices.data();
        GFX_THROW_INFO_ONLY(GetDevice(gfx)->CreateBuffer(&vbbd, &sdVerts, &m_pBuffer));
	}
    void Bind(Graphics& gfx) noexcept override;
private:
    UINT m_uStride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
};

