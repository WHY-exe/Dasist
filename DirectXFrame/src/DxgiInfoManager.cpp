#include "DxgiInfoManager.h"
#include <dxgidebug.h>
#include "Windows.h"
#include "Graphics.h"
#pragma comment(lib, "dxguid.lib")
DxgiInfoManager::DxgiInfoManager()
{
	// define the function signature of DXGIGetDebugInterface
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void **);
	// load the dll library to get the function
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32_NO_FORWARDER);
	if (!hModDxgiDebug)
	{
		throw WND_LAST_EXCEPT();
	}
	// Get the address of the function
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
	);
	if (!DxgiGetDebugInterface)
	{
		throw WND_LAST_EXCEPT();
	}
	HRESULT hr;
	// call the function, fill the info queue
	GFX_CALL(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&m_pDxgiInfoQueue)));

}

DxgiInfoManager::~DxgiInfoManager()
{
	if (m_pDxgiInfoQueue)
	{
		m_pDxgiInfoQueue->Release();
	}
}

void DxgiInfoManager::Set() noexcept
{
	// call set before get message, so you only get the error message
	// that generated after the set is called
	next = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> v_szMsg;
	const auto end = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		SIZE_T ulMsgLen = 0;
		HRESULT hr;
		// get the meassage of index i
		GFX_CALL(m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &ulMsgLen));
		// allocate the memory for the message
		auto bytes = std::make_unique<std::byte[]>(ulMsgLen);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// fill the memory with message in the queue at index i
		GFX_CALL(m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &ulMsgLen));
		v_szMsg.emplace_back(pMessage->pDescription);
	}
	return v_szMsg;
}
