#include "stdafx.h"
#include "DeviceManager.h"

DeviceManager::~DeviceManager()
{
	Release();
}

HRESULT DeviceManager::Initialize()
{
	m_msQualityLev = 0;
	m_behaviourFlag = 0;
	m_displayMode = true;

	Check_SDKVersion();
	CheckMultiSample();

	D3DPRESENT_PARAMETERS desc;
	ZeroMemory(&desc, sizeof(D3DPRESENT_PARAMETERS));

	CreateSwapChain(&desc);
	CreateDevice(&desc);


	if (FAILED(D3DXCreateSprite(m_device, &m_sprite)))
	{
		ERR_MSG(L"Sprite Creating Failed");
		return E_FAIL;
	}
	D3DXFONT_DESCW fontInfo;
	ZeroMemory(&fontInfo, sizeof(D3DXFONT_DESCW));
	fontInfo.Height = 20;
	fontInfo.Width = 10;
	fontInfo.Weight = FW_HEAVY;
	fontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(fontInfo.FaceName, L"맑은 고딕");

	if (FAILED(D3DXCreateFontIndirect(m_device, &fontInfo, &m_font)))
	{
		ERR_MSG(L"font 생성 실패");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateLine(m_device, &m_line)))
	{
		ERR_MSG(L"line 생성 실패");
		return E_FAIL;
	}

	return S_OK;
}


void DeviceManager::Check_SDKVersion()
{
	// 1. 현재 물리 장치(그래픽 카드)의 사양을 체크.
	// 현재 포함하고 있는 헤더에 존재하는 설정값으로 SDK 생성
	// Direct3D9 객체는 최초로 할당 되어야하는 객체이며, 가장 마지막에 할당해제한다.
	// 해당 객체에 대한 인터페이스 IDirect3D9 으로 하드웨어와 관련되어 해당 하드웨어의 성능을 조사한다.
	m_SDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 Caps;								// Capability 능력, 성능 
												// IDirect3D9 객체를 통해 하드웨어의 성능을 조사한다.
	ZeroMemory(&Caps, sizeof(D3DCAPS9));

	// 인자로 전달되는 물리장치(그래픽 카드)의 성능 수준을 검사한다. 
	// 다수의 하드웨어가 연결되어 있을 시에는 IDirect3D9::GetAdapterCount 를 통해 연결된 하드웨어를 검사한다.
	// D3DDEVTYPE_HAL HAL(Hardware Abstraction Layer) 하드웨어 추상 계층
	// 다양한 종류의 하드웨어 제작사가 OS 와 일괄적으로 소통할 수 있도록 정규화 해놓은 인터페이스.
	if (E_FAIL == (m_SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps)))
	{
		AfxMessageBox(L"Create Fail");
	}



	if (Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)				// 하드웨어에서 행렬변환과 조명결과를 연산할 수 있는가 검사한다.
		m_behaviourFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 가능할 경우 하드웨어에서 정점연산을 일임한다.
	else
		m_behaviourFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 불가능할 경우 소프트웨어에서 정점연산을 수행한다.
}

void DeviceManager::CheckMultiSample()
{
	if (E_FAIL == (m_SDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, true, D3DMULTISAMPLE_NONE, &m_msQualityLev)))
	{
		AfxMessageBox(L"Create Fail");
	}
}

void DeviceManager::CreateSwapChain(D3DPRESENT_PARAMETERS* _desc)
{
	_desc->BackBufferCount = 1;									// 생성할 BackBuffer의 폭.					
	_desc->BackBufferWidth = WINCX;								// 생성할 BackBuffer의 높이.
	_desc->BackBufferHeight = WINCY;							// 생성할 BackBuffer의 포맷(색상 포맷 등). D3DFMT_
	_desc->BackBufferFormat = D3DFMT_A8R8G8B8;					// 생성할 BackBuffer의 수.

	_desc->MultiSampleType = D3DMULTISAMPLE_NONE;				// MSAA 에 사용될 멀티셈플링 타입. D3DMULTISAMPLE_
	_desc->MultiSampleQuality = m_msQualityLev - 1;				// MS 성능 수준 설정  0 ~ IDirect3D9::CheckMultiSampleType의 결과값 - 1 을 사용한다.

	_desc->SwapEffect = D3DSWAPEFFECT_DISCARD;					// Present 호출 방식에 대한 설정을 부여한다.
	_desc->hDeviceWindow = g_hWnd;								// 출력 클라이언트 윈도우 핸들.
	_desc->Windowed = m_displayMode;							// 윈도우 창모드 설정 여부 true 일 시 창모드.
	_desc->EnableAutoDepthStencil = true;						// 깊이, 스텐실 버퍼 사용여부. 2D 에서는 필요없지만, 3D 에서는 필수적.
	_desc->AutoDepthStencilFormat = D3DFMT_D24S8;				// 깊이 스텐실 버퍼 형식.	D3DFMT_
	_desc->Flags = false;										// Present 이후 수행할 기능 설정.

	_desc->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// 전체화면 모드일 시 화면 주사율.
	_desc->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present 호출 시 발생할 간격 설정.
																 //_desc->PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// Present 호출 시 발생할 간격 설정.
}

void DeviceManager::CreateDevice(D3DPRESENT_PARAMETERS* _desc)
{
	if (E_FAIL == (m_SDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, m_behaviourFlag, _desc, &m_device)))
	{
		AfxMessageBox(L"Create Fail");
	}
}




void DeviceManager::Release()
{
	SAFE_RELEASE(m_line);
	SAFE_RELEASE(m_font);
	SAFE_RELEASE(m_sprite);
	SAFE_RELEASE(m_device);
}

void DeviceManager::RenderBegin()
{
	m_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 255, 255), 1.f, 0);
	m_device->BeginScene();
	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void DeviceManager::RenderEnd()
{
	m_sprite->End();
	m_device->EndScene();
	m_device->Present(nullptr, nullptr, g_hWnd, nullptr);
}
