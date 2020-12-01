#pragma once

struct ObjectData
{
	ObjectData();
	D3DXVECTOR3 position;
	float angle;
	D3DXVECTOR3 scale;
		
#ifdef _AFX
	CString name;
	CString texName;
#else // _AFX
	wstring name;
	wstring texName;
#endif

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
	void Render(float _x, float _y);
public:
	ObjectData data;
};

