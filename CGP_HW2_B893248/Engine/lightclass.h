#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dx10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetSpecularColor();
	float GetSpecularPower();

	void SetPosition(float, float, float);
	D3DXVECTOR4 GetPosition();

private:
	ID3D11ShaderResourceView* m_texture;

	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR4 m_specularColor;
	float m_specularPower;

	D3DXVECTOR4 m_position;
};

#endif