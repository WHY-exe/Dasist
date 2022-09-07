#pragma once
#include "InitWin.h"
#include <type_traits>
#include <vector>
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
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			Byte4Color
		};
		template <ElementType type> struct Map;

		template <> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position2D";
		};
		template <> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position3D";
		};
		template <> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texture2D";
		};
		template <> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};

		template <> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Float3Color";
		};

		template <> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Float4Color";
		};

		template <> struct Map<Byte4Color>
		{
			using SysType = BGRAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32_FLOAT;
			static constexpr const char* semantic = "Byte4Color";
		};


		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				:
				m_type(type),
				m_offset(offset)
			{}
			ElementType GetType() const noexcept
			{
				return m_type;
			}
			size_t GetOffset() const noexcept
			{
				return m_offset;
			}
			size_t GetOffsetAfter() const noexcept(!IS_DEBUG)
			{
				return m_offset + Size();
			}
			size_t Size() const noexcept(!IS_DEBUG)
			{
				return SizeOf(m_type);
			}
			static constexpr size_t SizeOf(ElementType type) noexcept(!IS_DEBUG)
			{
				switch (type)
				{
				case ElementType::Position2D:
					return sizeof(Map<Position2D>::SysType);
					break;
				case ElementType::Position3D:
					return sizeof(Map<Position3D>::SysType);
					break;
				case ElementType::Texture2D:
					return sizeof(Map<Texture2D>::SysType);
					break;
				case ElementType::Normal:
					return sizeof(Map<Normal>::SysType);
					break;
				case ElementType::Float3Color:
					return sizeof(Map<Float3Color>::SysType);
					break;
				case ElementType::Float4Color:
					return sizeof(Map<Float4Color>::SysType);
					break;
				case ElementType::Byte4Color:
					return sizeof(Map<Byte4Color>::SysType);
					break;
				}
				assert("Invalid Element Type" && false);
				return 0u;
			}
			D3D11_INPUT_ELEMENT_DESC GetLayoutDesc() const noexcept(!IS_DEBUG)
			{
				switch (m_type)
				{
				case Position2D:
					return GenDesc<Position2D>(m_offset);
					break;
				case Position3D:
					return GenDesc<Position3D>(m_offset);
					break;
				case Texture2D:
					return GenDesc<Texture2D>(m_offset);
					break;
				case Normal:
					return GenDesc<Normal>(m_offset);
					break;
				case Float3Color:
					return GenDesc<Float3Color>(m_offset);
					break;
				case Float4Color:
					return GenDesc<Float4Color>(m_offset);
					break;
				case Byte4Color:
					return GenDesc<Byte4Color>(m_offset);
					break;
				}
				assert("Invalid element type" && false);
				return {"INVALID", 0u, DXGI_FORMAT_UNKNOWN, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u};
			}
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
		const Element& Resolve(ElementType Type) const noexcept(!IS_DEBUG)
		{
			for (const auto& e : m_Elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			assert("Can't resolve element type" && false);
			return m_Elements.front();
		}
		
		const Element& ResolveByIndex(size_t i) const noexcept(!IS_DEBUG)
		{
			return m_Elements[i];
		}

		Layout& Append(ElementType Type) noexcept(!IS_DEBUG)
		{
			Element e(Type, Size());
			m_d3d_desc.emplace_back(e.GetLayoutDesc());
			m_Elements.emplace_back(std::move(e));
			return *this;
		}

		size_t Size() const noexcept(!IS_DEBUG)
		{
			return m_Elements.empty() ? 0u : m_Elements.back().GetOffsetAfter();
		}
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetD3DLayout() const 
		{
			return m_d3d_desc;
		}
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
			const auto& element = m_layout.Resolve(Type);
			auto pAttri = m_pBuffer + element.GetOffset();
			if constexpr(Type == Layout::ElementType::Position3D)
			{
				return *reinterpret_cast<DirectX::XMFLOAT3*>(pAttri);
			}
			else if constexpr(Type == Layout::ElementType::Position2D)
			{
				return *reinterpret_cast<DirectX::XMFLOAT2*>(pAttri);
			}
			else if constexpr (Type == Layout::ElementType::Texture2D)
			{
				return *reinterpret_cast<DirectX::XMFLOAT2*>(pAttri);
			}
			else if constexpr (Type == Layout::ElementType::Normal)
			{
				return *reinterpret_cast<DirectX::XMFLOAT3*>(pAttri);
			}
			else if constexpr (Type == Layout::ElementType::Float3Color)
			{
				return *reinterpret_cast<DirectX::XMFLOAT3*>(pAttri);
			}
			else if constexpr (Type == Layout::ElementType::Float4Color)
			{
				return *reinterpret_cast<DirectX::XMFLOAT4*>(pAttri);
			}
			else if constexpr (Type == Layout::ElementType::Byte4Color)
			{
				return *reinterpret_cast<BGRAColor*>(pAttri);
			}
			else
			{
				assert("Bad Element Type" && false);
				return *reinterpret_cast<char*>(pAttri);
			}
		}
	private:
		template <typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
		{
			const auto& element = m_layout.ResolveByIndex(i);
			auto pAttribute = m_pBuffer + element.GetOffset();
			switch (element.GetType())
			{
			case Layout::ElementType::Position3D:
				SetAttribute<DirectX::XMFLOAT3>(pAttribute, std::forward<T>(val));
				break;
			case Layout::ElementType::Position2D:
				SetAttribute<DirectX::XMFLOAT2>(pAttribute, std::forward<T>(val));
				break;
			case Layout::ElementType::Texture2D:
				SetAttribute<DirectX::XMFLOAT2>(pAttribute, std::forward<T>(val));
				break;
			case Layout::ElementType::Normal:
				SetAttribute<DirectX::XMFLOAT3>(pAttribute, std::forward<T>(val));
				break;
			case Layout::ElementType::Float4Color:
				SetAttribute<DirectX::XMFLOAT4>(pAttribute, std::forward<T>(val));
				break;
			case Layout::ElementType::Float3Color:
				SetAttribute<DirectX::XMFLOAT3>(pAttribute, std::forward<T>(val));
				break;
			case Layout::ElementType::Byte4Color:
				SetAttribute<BGRAColor>(pAttribute, std::forward<T>(val));
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
		template <typename Dest, typename Src>
		void SetAttribute(char* pAttribute, Src&& val) noexcept(!IS_DEBUG)
		{
			if constexpr (std::is_assignable<Dest, Src>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
				assert("Parameter attribute type mismatch" && false);
		}
	protected:
		Data(char* pBuffer, const Layout& layout) noexcept(!IS_DEBUG)
			:
			m_pBuffer(pBuffer),
			m_layout(layout)
		{
			assert(m_pBuffer != nullptr);
		}
	private:
		char* m_pBuffer = nullptr;
		const Layout& m_layout;
	};

	class ConstData
	{
	public:
		ConstData(const Data& data) noexcept(!IS_DEBUG)
			:
			m_data(data)
		{
		}
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
		DataBuffer(const Layout& layout)
			:
			m_layout(layout)
		{}
		const Layout& GetLayout() const noexcept
		{
			return m_layout;
		}
		const char* GetData() const noexcept
		{
			return m_Buffer.data();
		}
		size_t ByteWidth() const noexcept
		{
			return m_Buffer.size();
		}
		size_t Size() const noexcept
		{
			return m_Buffer.size() / m_layout.Size();
		}
		template<typename ...Params>
		void EmplaceBack(Params&& ...params)
		{
			m_Buffer.resize(m_Buffer.size() + m_layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Data Back()
		{
			assert(m_Buffer.size() != 0u);
			return Data(m_Buffer.data() + m_Buffer.size() - m_layout.Size(), m_layout);
		}
		Data Front()
		{
			assert(m_Buffer.size() != 0u);
			return Data(m_Buffer.data(), m_layout);
		}
		Data operator[](size_t i)
		{
			assert(i < Size());
			return Data(m_Buffer.data() + m_layout.Size() * i, m_layout);
		}
		ConstData Back() const noexcept(!IS_DEBUG)
		{
			return const_cast<DataBuffer*>(this)->Back();
		}
		ConstData Front() const noexcept(!IS_DEBUG)
		{
			return const_cast<DataBuffer*>(this)->Front();
		}
		ConstData operator[](size_t i) const noexcept(!IS_DEBUG)
		{
			return const_cast<DataBuffer&>(*this)[i];
		}
	private:
		std::vector<char> m_Buffer;
		Layout m_layout;
	};
}