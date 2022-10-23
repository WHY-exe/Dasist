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
	
	class Layout
	{
	public:
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
		template <Layout::ElementType Type>
		auto& Attri() noexcept(!IS_DEBUG)
		{
			auto pAttri = m_pBuffer + m_layout.Resolve(Type).GetOffset();
			return *reinterpret_cast<Layout::Map<Type>::SysType*>(pAttri);
		}
	private:
		template <typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
		{
			const auto& element = m_layout.ResolveByIndex(i);
			auto pAttribute = m_pBuffer + m_layout.ResolveByIndex(i).GetOffset();
			switch (element.GetType())
			{
			case Layout::Position3D:
				SetAttribute<Layout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Position2D:
				SetAttribute<Layout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Tex2D:
				SetAttribute<Layout::Tex2D>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Normal:
				SetAttribute<Layout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Bitangent:
				SetAttribute<Layout::Bitangent>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Tangent:
				SetAttribute<Layout::Tangent>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Float4Color:
				SetAttribute<Layout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Float3Color:
				SetAttribute<Layout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Byte4Color:
				SetAttribute<Layout::Byte4Color>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad Element Type" && false);
				break;
			}
		}
		// 多参数传递（设置顶点的多个信息）
		template <typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		template <Layout::ElementType DestLayouType, typename Src>
		void SetAttribute(char* pAttribute, Src&& val) noexcept(!IS_DEBUG)
		{
			using Dest = Layout::Map<DestLayouType>::SysType;
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
		template <Layout::ElementType Type>
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
		template<typename ...Params>
		void EmplaceBack(Params&& ...params)
		{
			m_Buffer.resize(m_Buffer.size() + m_layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		Data Back();
		Data Front();
		Data operator[](size_t i);
		ConstData Back() const noexcept(!IS_DEBUG);
		ConstData Front() const noexcept(!IS_DEBUG);
		ConstData operator[](size_t i) const noexcept(!IS_DEBUG);
	private:
		std::vector<char> m_Buffer;
		Layout m_layout;
	};
}