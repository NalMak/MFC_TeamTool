#include "stdafx.h"
#include "Texture.h"
#include "DeviceManager.h"

Texture::Texture()
{
}


Texture::~Texture()
{
	Release();
}

HRESULT Texture::Initialize(const TCHAR * _fp)
{
	if (FAILED(D3DXGetImageInfoFromFile(_fp, &m_tex.info)))
	{
		ERR_MSG(L"Loading Image Info Failed");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileEx(DeviceManager::GetInstance()->GetDevice(),
		_fp,
		m_tex.info.Width,
		m_tex.info.Height,
		m_tex.info.MipLevels,
		0,
		m_tex.info.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_tex.texture)))
	{
		wstring wstrErr = L"Loading Failed";
		ERR_MSG(wstrErr.c_str());
		return E_FAIL;
	}
	return S_OK;
}

void Texture::Release()
{
	SAFE_RELEASE(m_tex.texture);
}
