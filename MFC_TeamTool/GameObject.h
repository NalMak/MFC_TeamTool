#pragma once

struct ObjectData
{
	D3DXVECTOR3 position;
	float angle;
	D3DXVECTOR3 scale;

	wstring name;
	wstring texName;

	D3DXVECTOR2 colliderOffset;
	D3DXVECTOR2 colliderScale;
};

class GameObject
{
public:

public:
	GameObject();
	~GameObject();
public:
	void Render();
public:
	ObjectData data;
};

