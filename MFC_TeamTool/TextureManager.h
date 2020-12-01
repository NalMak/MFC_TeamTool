#pragma once

#include "Singleton.h"
class Texture;
class TextureManager : public Singleton<TextureManager>
{
public:
	TextureManager();
	~TextureManager();

public:
	void LoadTexture(const TCHAR* _ext);
	Texture* GetTexture(wstring _texName);
	map<wstring, Texture*>& GetTexList() { return m_textureList; }
	void Release();
private:
	map<wstring, Texture*> m_textureList;
	const TCHAR* m_directoryPath;
};

