#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<UINT>& indicies)
	:
	m_uBufferSize(indicies.size())
{
    IMPORT_INFOMAN(gfx);
    D3D11_BUFFER_DESC ibbd = {};
    ibbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibbd.Usage = D3D11_USAGE_DEFAULT;
    ibbd.CPUAccessFlags = 0u;
    ibbd.ByteWidth = indicies.size() * sizeof(UINT);
    ibbd.StructureByteStride = sizeof(UINT);
    D3D11_SUBRESOURCE_DATA sdIdics = {};
    sdIdics.pSysMem = indicies.data();

    GFX_THROW_INFO_ONLY(GetDevice(gfx)->CreateBuffer(&ibbd, &sdIdics, &m_IndexBuffer));

}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

UINT IndexBuffer::GetSize() const
{
	return m_uBufferSize;
}
