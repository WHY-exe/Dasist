#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics& gfx, const Vertex::DataBuffer& vbuf)
	:
	m_uStride((UINT)vbuf.GetLayout().Size())
{
	IMPORT_INFOMAN(gfx);
	// buffer description
	D3D11_BUFFER_DESC vbbd = {};
	vbbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbbd.Usage = D3D11_USAGE_DEFAULT;
	vbbd.CPUAccessFlags = 0u;
	vbbd.MiscFlags = 0u;
	vbbd.ByteWidth = (UINT)vbuf.ByteWidth();
	vbbd.StructureByteStride = m_uStride;
	D3D11_SUBRESOURCE_DATA sdVerts = {};
	sdVerts.pSysMem = vbuf.GetData();
	GFX_THROW_INFO_ONLY(GetDevice(gfx)->CreateBuffer(&vbbd, &sdVerts, &m_pBuffer));
}

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
    // bind the vertex buffer to the pipeline
    const UINT vboffset = 0u;
    GetContext(gfx)->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_uStride, &vboffset);
}
