#pragma once
class Texture
{
public:
	Texture();
	~Texture();
public:
	HRESULT Initialize(const TCHAR* _fp);
	void Release();
public:
	TexInfo m_tex;
};

