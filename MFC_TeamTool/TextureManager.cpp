#include "stdafx.h"
#include "TextureManager.h"
#include "Texture.h"
#include "FileIO.h"
TextureManager::TextureManager()
{
	m_directoryPath = L"../Texture";
}


TextureManager::~TextureManager()
{
	Release();
}

void TextureManager::LoadTexture(const TCHAR * _ext)
{
	list<wstring> filePaths;
	FileIO::SearchingDir(&filePaths, m_directoryPath, _ext);

	for (auto& file : filePaths)
	{
		wstring filePath = file;
		wstring fileName = filePath.substr(filePath.find_last_of(L"/") + 1);
		int targetNum = fileName.find_last_of(L".");
		fileName = fileName.substr(0, targetNum);

		Texture* tex = new Texture;
		m_textureList[fileName] = tex;
		tex->Initialize(filePath.c_str());
	}
}

Texture * TextureManager::GetTexture(wstring _texName)
{
	if (m_textureList.end() == m_textureList.find(_texName))
		return nullptr;

	return m_textureList[_texName];
}

void TextureManager::Release()
{
	for (auto& tex : m_textureList)
	{
		SAFE_DELETE(tex.second);
	}
}
