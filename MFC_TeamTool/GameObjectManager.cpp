#include "stdafx.h"
#include "GameObjectManager.h"
#include "GameObject.h"

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
	for (auto& obj : m_objectList)
	{
		obj->Render();
	}
}
