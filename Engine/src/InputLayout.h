#pragma once
#include "Bindable.h"
#include "Vertex.h"
#include <memory>
class InputLayout: public Bindable
{
public:
	InputLayout(
		Graphics& gfx,
		const Vertex::Layout& layout,
		ID3DBlob* pVertexShaderByteCode
	);
	void Bind(Graphics& gfx) noexcept override; 
    static std::shared_ptr<Bindable> Resolve(
        Graphics& gfx,
        const Vertex::Layout& layout, 
        ID3DBlob* pVertexShaderByteCode) noexcept;
    template <typename...Ignore>
    static std::wstring GenUID(const Vertex::Layout& layout, Ignore&&...ingnore) noexcept
    {
        return GenUID_(layout);
    }
    std::wstring GetUID() const noexcept override;
private:
    static std::wstring GenUID_(const Vertex::Layout& layout) noexcept;
    const Vertex::Layout& m_layout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};

