#pragma once
#include "InitWin.h"
#include "WinException.h"
#include "DxgiInfoManager.h"
#include <d3d11.h>
#include <DirectXMath.h>

class Graphics
{
	friend class Bindable;
public:
	class GfxExcepion :public WinException
	{
	public:
		GfxExcepion(int nLine, const char* szFile, HRESULT hr, std::vector<std::string> v_szMsg);
		GfxExcepion(int nLine, const char* szFile, HRESULT hr);
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string m_info;
	};
	class GfxInfoOnlyException : public Exception
	{
	public:
		GfxInfoOnlyException(int nLine, const char* szFile, std::vector<std::string>& v_szInfo);
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string m_szInfo;
	};
public:
	Graphics(HWND hWnd, int nWinWidth, int nWinHeight);
	Graphics(const Graphics& gfx) = delete;
	Graphics& operator=(const Graphics& gfx) = delete;
	~Graphics();
	void DrawIndexed(UINT index_count);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	
	DirectX::XMMATRIX GetCamera() const noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void BeginFrame();
	void EndFrame();
public:
	void CreateRenderTarget();
	void CreateAndSetViewPort(int nWinWidth = 0, int nWinHeight = 0);
	void CreateAndSetStencilDepthView(int nWinWidth = 0, int nWinHeight = 0);
	void ResizeFrameBuffer(UINT bufferWidth, UINT bufferHeight);
	void CleanUpRenderTarget();
private:
	DirectX::XMMATRIX m_projection;
	DirectX::XMMATRIX m_camTransform;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDSV;
};

