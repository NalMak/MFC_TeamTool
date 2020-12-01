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
	lstrcpy(fontInfo.FaceName, L"���� ���");

	if (FAILED(D3DXCreateFontIndirect(m_device, &fontInfo, &m_font)))
	{
		ERR_MSG(L"font ���� ����");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateLine(m_device, &m_line)))
	{
		ERR_MSG(L"line ���� ����");
		return E_FAIL;
	}

	return S_OK;
}


void DeviceManager::Check_SDKVersion()
{
	// 1. ���� ���� ��ġ(�׷��� ī��)�� ����� üũ.
	// ���� �����ϰ� �ִ� ����� �����ϴ� ���������� SDK ����
	// Direct3D9 ��ü�� ���ʷ� �Ҵ� �Ǿ���ϴ� ��ü�̸�, ���� �������� �Ҵ������Ѵ�.
	// �ش� ��ü�� ���� �������̽� IDirect3D9 ���� �ϵ����� ���õǾ� �ش� �ϵ������ ������ �����Ѵ�.
	m_SDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 Caps;								// Capability �ɷ�, ���� 
												// IDirect3D9 ��ü�� ���� �ϵ������ ������ �����Ѵ�.
	ZeroMemory(&Caps, sizeof(D3DCAPS9));

	// ���ڷ� ���޵Ǵ� ������ġ(�׷��� ī��)�� ���� ������ �˻��Ѵ�. 
	// �ټ��� �ϵ��� ����Ǿ� ���� �ÿ��� IDirect3D9::GetAdapterCount �� ���� ����� �ϵ��� �˻��Ѵ�.
	// D3DDEVTYPE_HAL HAL(Hardware Abstraction Layer) �ϵ���� �߻� ����
	// �پ��� ������ �ϵ���� ���ۻ簡 OS �� �ϰ������� ������ �� �ֵ��� ����ȭ �س��� �������̽�.
	if (E_FAIL == (m_SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps)))
	{
		AfxMessageBox(L"Create Fail");
	}



	if (Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)				// �ϵ����� ��ĺ�ȯ�� �������� ������ �� �ִ°� �˻��Ѵ�.
		m_behaviourFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// ������ ��� �ϵ����� ���������� �����Ѵ�.
	else
		m_behaviourFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// �Ұ����� ��� ����Ʈ����� ���������� �����Ѵ�.
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
	_desc->BackBufferCount = 1;									// ������ BackBuffer�� ��.					
	_desc->BackBufferWidth = WINCX;								// ������ BackBuffer�� ����.
	_desc->BackBufferHeight = WINCY;							// ������ BackBuffer�� ����(���� ���� ��). D3DFMT_
	_desc->BackBufferFormat = D3DFMT_A8R8G8B8;					// ������ BackBuffer�� ��.

	_desc->MultiSampleType = D3DMULTISAMPLE_NONE;				// MSAA �� ���� ��Ƽ���ø� Ÿ��. D3DMULTISAMPLE_
	_desc->MultiSampleQuality = m_msQualityLev - 1;				// MS ���� ���� ����  0 ~ IDirect3D9::CheckMultiSampleType�� ����� - 1 �� ����Ѵ�.

	_desc->SwapEffect = D3DSWAPEFFECT_DISCARD;					// Present ȣ�� ��Ŀ� ���� ������ �ο��Ѵ�.
	_desc->hDeviceWindow = g_hWnd;								// ��� Ŭ���̾�Ʈ ������ �ڵ�.
	_desc->Windowed = m_displayMode;							// ������ â��� ���� ���� true �� �� â���.
	_desc->EnableAutoDepthStencil = true;						// ����, ���ٽ� ���� ��뿩��. 2D ������ �ʿ������, 3D ������ �ʼ���.
	_desc->AutoDepthStencilFormat = D3DFMT_D24S8;				// ���� ���ٽ� ���� ����.	D3DFMT_
	_desc->Flags = false;										// Present ���� ������ ��� ����.

	_desc->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ��üȭ�� ����� �� ȭ�� �ֻ���.
	_desc->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present ȣ�� �� �߻��� ���� ����.
																 //_desc->PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// Present ȣ�� �� �߻��� ���� ����.
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
