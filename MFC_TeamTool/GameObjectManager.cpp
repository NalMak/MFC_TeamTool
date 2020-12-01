#include "stdafx.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "MFC_TeamToolView.h"
GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
	Release();
}

void GameObjectManager::Release()
{
	for (auto& obj : m_objectList)
	{
		SAFE_DELETE(obj);
	}

	m_objectList.clear();
}

void GameObjectManager::AddObject(GameObject * _obj)
{
	m_objectList.emplace_back(_obj);
}

void GameObjectManager::DeleteObject(GameObject * _obj)
{
	for (auto iter = m_objectList.begin(); iter != m_objectList.end(); ++iter)
	{
		if ((*iter) == _obj)
		{
			m_objectList.erase(iter);
			return;
		}
	}
}

void GameObjectManager::Render()
{

	RECT thisRect = {};
	m_deviceView->GetWindowRect(&thisRect);
	SetRect(&thisRect, 0, 0, thisRect.right - thisRect.left, thisRect.bottom - thisRect.top);
	float ratioX = float(WINCX) / thisRect.right;
	float ratioY = float(WINCY) / thisRect.bottom;

	for (auto& obj : m_objectList)
	{
		obj->Render(ratioX, ratioY);
	}
}
