#pragma once
#include "InitWin.h"
#include "Exception.h"
#include "DxgiInfoManager.h"
#include <string>
#include <wrl.h>
#include <d3d11.h>
class Graphics
{
public:
	class GfxExcepion :public Exception
	{
	public:
		GfxExcepion(int nLine, const char* szFile, HRESULT hr, std::vector<std::string> v_szMsg);
		GfxExcepion(int nLine, const char* szFile, HRESULT hr);
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		static std::string TransferErrorCode(HRESULT hr);
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT m_hr;
		std::string m_info;
	};
private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_pView;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics& gfx) = delete;
	Graphics& operator=(const Graphics& gfx) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float r, float g, float b, float a);

};

// put HRESULT hr; in front of the GFX_CALL();
#define GFX_CALL(hrcall)\
	if(FAILED(hr = hrcall))\
		throw Graphics::GfxExcepion( __LINE__, __FILE__, hr)
