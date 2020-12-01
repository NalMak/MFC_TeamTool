#pragma once
#include "Singleton.h"
class GameObject;
class GameObjectManager : public Singleton<GameObjectManager>
{
public:
	GameObjectManager();
	~GameObjectManager();

public:
	void Release();
	void AddObject(GameObject* _obj);
	void DeleteObject(GameObject* _obj);
	void Render();

private:
	list<GameObject*> m_objectList;
};

