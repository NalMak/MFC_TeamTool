#pragma once
#include "Singleton.h"
class GameObject;
class CMFC_TeamToolView;
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
public:
	void SetDeviceView(CMFC_TeamToolView* _target) { m_deviceView = _target; }
private:
	list<GameObject*> m_objectList;
	CMFC_TeamToolView* m_deviceView;
	
};

