#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
    // bind the vertex buffer to the pipeline
    const UINT vboffset = 0u;
    GetContext(gfx)->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_uStride, &vboffset);
}
