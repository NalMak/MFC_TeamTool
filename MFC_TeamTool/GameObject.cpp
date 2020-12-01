#include "stdafx.h"
#include "GameObject.h"
#include "DeviceManager.h"
#include "TextureManager.h"
#include "Texture.h"
GameObject::GameObject()
{

	data.texName = L"Cube";
	data.position.x = 0;
	data.position.y = 0;
	data.position.z = 0;

	data.angle = 0;

	data.scale.x = 1;
	data.scale.y = 1;
	data.scale.z = 1;

}


GameObject::~GameObject()
{
}

void GameObject::Render()
{
	Texture* tex = TextureManager::GetInstance()->GetTexture(data.texName);

	D3DXMATRIX matRot, matScale, matTrans, matWorld;
	
	D3DXMatrixScaling(&matScale, data.scale.x , data.scale.y, data.scale.z);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(data.angle));
	D3DXMatrixTranslation(&matTrans, data.position.x, data.position.y, data.position.z);
	matWorld = matScale * matRot * matTrans;


	DeviceManager::GetInstance()->GetSprite()->SetTransform(&matWorld);
	DeviceManager::GetInstance()->GetSprite()->Draw(
		tex->m_tex.texture,
		nullptr,
		&D3DXVECTOR3((float)(tex->m_tex.info.Width >> 1), (float)(tex->m_tex.info.Height >> 1), 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}
