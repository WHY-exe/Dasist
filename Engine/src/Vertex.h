#pragma once
#include "InitWin.h"
#include <type_traits>
#include <vector>
#include <map>
#include <string>
#include <typeinfo>
#include <d3d11.h>
#include <DirectXMath.h>
namespace Vertex {
	struct BGRAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	enum ElementType
	{
		Position2D,
		Position3D,
		Tex2D,
		Normal,
		Float3Color,
		Float4Color,
		Byte4Color,
		Tangent,
		Bitangent
	};
	template <ElementType type> struct Map;

	template <> struct Map<Position2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Position2D";
		static constexpr const char* code = "P2";
	};
	template <> struct Map<Position3D>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Position3D";
		static constexpr const char* code = "P3";
	};
	template <> struct Map<Tex2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Tex2D";
		static constexpr const char* code = "T2";
	};
	template <> struct Map<Normal>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Normal";
		static constexpr const char* code = "N";
	};
	template<> struct Map<Tangent>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Tangent";
		static constexpr const char* code = "Nt";
	};
	template<> struct Map<Bitangent>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Bitangent";
		static constexpr const char* code = "Nb";
	};
	template <> struct Map<Float3Color>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Float3Color";
		static constexpr const char* code = "C3";
	};

	template <> struct Map<Float4Color>
	{
		using SysType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* semantic = "Float4Color";
		static constexpr const char* code = "C4";
	};

	template <> struct Map<Byte4Color>
	{
		using SysType = BGRAColor;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32_FLOAT;
		static constexpr const char* semantic = "Byte4Color";
		static constexpr const char* code = "CB";
	};


	class Layout
	{
	public:
		class Element
		{
		public:
			Element(ElementType type, size_t offset);
			ElementType GetType() const noexcept;
			size_t GetOffset() const noexcept;
			size_t GetOffsetAfter() const noexcept(!IS_DEBUG);
			size_t Size() const noexcept(!IS_DEBUG);
			static constexpr size_t SizeOf(ElementType type) noexcept(!IS_DEBUG);
			D3D11_INPUT_ELEMENT_DESC GetLayoutDesc() const noexcept(!IS_DEBUG);
			const char* GetCode() const noexcept;
		private:
			template <ElementType type>
			static D3D11_INPUT_ELEMENT_DESC GenDesc(size_t offset) noexcept(!IS_DEBUG)
			{
				return {Map<type>::semantic, 0u, Map<type>::dxgiFormat, 0u, (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA, 0u};
			}
			ElementType m_type;
			size_t m_offset;
		};
	public:
		const Element& Resolve(ElementType Type) const noexcept(!IS_DEBUG);
		const Element& ResolveByIndex(size_t i) const noexcept(!IS_DEBUG);
		Layout& Append(ElementType Type) noexcept(!IS_DEBUG);
		size_t Size() const noexcept(!IS_DEBUG);
		size_t Count() const noexcept;
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetD3DLayout() const;
		std::string GetCode() const noexcept;
	private:
		std::vector<Element> m_Elements;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_d3d_desc;
	};

	class Data
	{
		friend class DataBuffer;
	public:
		template <ElementType Type>
		auto& Attri() noexcept(!IS_DEBUG)
		{
			auto pAttri = m_pBuffer + m_layout.Resolve(Type).GetOffset();
			return *reinterpret_cast<Map<Type>::SysType*>(pAttri);
		}
	private:
		template <typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
		{
			const auto& element = m_layout.ResolveByIndex(i);
			auto pAttribute = m_pBuffer;
			switch (element.GetType())
			{
			case Position3D:
				SetAttribute<Position3D>(pAttribute, std::forward<T>(val));
				break;
			case Position2D:
				SetAttribute<Position2D>(pAttribute, std::forward<T>(val));
				break;
			case Tex2D:
				SetAttribute<Tex2D>(pAttribute, std::forward<T>(val));
				break;
			case Normal:
				SetAttribute<Normal>(pAttribute, std::forward<T>(val));
				break;
			case Bitangent:
				SetAttribute<Bitangent>(pAttribute, std::forward<T>(val));
				break;
			case Tangent:
				SetAttribute<Tangent>(pAttribute, std::forward<T>(val));
				break;
			case Float4Color:
				SetAttribute<Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case Float3Color:
				SetAttribute<Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case Byte4Color:
				SetAttribute<Byte4Color>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad Element Type" && false);
				break;
			}
		}
		// 多参数传递（设置顶点的多个信息）

		template <ElementType DestLayoutType, typename Src>
		void SetAttribute(char* pAttribute, Src&& val) noexcept(!IS_DEBUG)
		{
			using Dest = Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, Src>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
				assert("Parameter attribute type mismatch" && false);
		}
	protected:
		Data(char* pBuffer, const Layout& layout) noexcept(!IS_DEBUG);
	private:
		char* m_pBuffer = nullptr;
		const Layout& m_layout;
	};


	class ConstData
	{
	public:
		ConstData(const Data& data) noexcept(!IS_DEBUG);
		template <ElementType Type>
		const auto& Attri() const noexcept(!IS_DEBUG)
		{
			return const_cast<Data&>(m_data).Attri<Type>();
		}
	private:
		Data m_data;
	};

	class DataBuffer
	{
	public:
		DataBuffer(const Layout& layout);
		const Layout& GetLayout() const noexcept;
		const char* GetData() const noexcept;
		size_t ByteWidth() const noexcept;
		size_t Size() const noexcept;
		template<typename First, typename ...Rest>
		void EmplaceBack(First first, Rest&& ...params)
		{
			EmplaceBack(first);
			EmplaceBack(params...);
		}
		template<typename T>
		void EmplaceBack(T&& t)
		{
			if (m_CurAttrIndex == m_layout.Count() || m_Buffer.empty())
			{
				m_CurAttrIndex = 0;
				m_Buffer.resize(m_Buffer.size() + m_layout.Size());
			}
			Back().SetAttributeByIndex(m_CurAttrIndex++, std::forward<T>(t));
		}
		Data Back();
		Data Front();
		Data operator[](size_t i);
		ConstData Back() const noexcept(!IS_DEBUG);
		ConstData Front() const noexcept(!IS_DEBUG);
		ConstData operator[](size_t i) const noexcept(!IS_DEBUG);
	private:
		size_t m_CurAttrIndex;
		std::vector<char> m_Buffer;
		Layout m_layout;
	};
}