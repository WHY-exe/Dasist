#pragma once
#include "GfxThrowMacro.h"
#include "Bindable.h"
template <typename C>
class ConstantBuffer : public Bindable
{
public:
    void Update(Graphics& gfx, const C& consts)
    {
        IMPORT_INFOMAN(gfx);
        // the pointer point to the memory of data that will be updated
        D3D11_MAPPED_SUBRESOURCE msr;
        // lock the memory and give the msr the pointer
        GFX_THROW_INFO(
            GetContext(gfx)->Map(
                m_pBuffer.Get(), 0u,
                D3D11_MAP_WRITE_DISCARD, 0u,
                &msr
            )
        );
        memcpy(msr.pData, &consts, sizeof(consts));
        GetContext(gfx)->Unmap(m_pBuffer.Get(), 0u);
    }
    ConstantBuffer(Graphics& gfx, const C& consts)
	{
        IMPORT_INFOMAN(gfx);
        D3D11_BUFFER_DESC cbbd = {};
        cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbbd.Usage = D3D11_USAGE_DYNAMIC;
        cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbbd.MiscFlags = 0u;
        cbbd.ByteWidth = sizeof(C);
        cbbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA sdcb = {};
        sdcb.pSysMem = &consts;
        GFX_THROW_INFO_ONLY(GetDevice(gfx)->CreateBuffer(&cbbd, &sdcb, &m_pBuffer));
	}
    ConstantBuffer(Graphics& gfx)
    {
        IMPORT_INFOMAN(gfx);
        D3D11_BUFFER_DESC cbbd = {};
        cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbbd.Usage = D3D11_USAGE_DYNAMIC;
        cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbbd.MiscFlags = 0u;
        cbbd.ByteWidth = sizeof(C);
        cbbd.StructureByteStride = 0u;
        GFX_THROW_INFO_ONLY(GetDevice(gfx)->CreateBuffer(&cbbd, nullptr, &m_pBuffer));
    }
protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
};

template <typename C>
class VertexConstantBuffer :public ConstantBuffer<C>
{
    using ConstantBuffer<C>::m_pBuffer;
    using Bindable::GetContext;
public:
    using ConstantBuffer<C>::ConstantBuffer;

    void Bind(Graphics& gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers(0u, 1u, m_pBuffer.GetAddressOf());
    }
};
template <typename C>
class PixelConstantBuffer :public ConstantBuffer<C>
{
    using ConstantBuffer<C>::m_pBuffer;
    using Bindable::GetContext;
public:
    using ConstantBuffer<C>::ConstantBuffer;

    void Bind(Graphics& gfx) noexcept override
    {
        GetContext(gfx)->PSSetConstantBuffers(0u, 1u, m_pBuffer.GetAddressOf());
    }
};