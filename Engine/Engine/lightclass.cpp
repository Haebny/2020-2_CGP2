#include "lightclass.h"

LightClass::LightClass()
{
	m_texture = 0;
}

LightClass::LightClass(const LightClass& other)
{
}

LightClass::~LightClass()
{
}

bool LightClass::Initialize(ID3D11Device* device, const WCHAR* filename)
{
	HRESULT result;

	// Load the texture in.
	// �ؽ��� ���� �ε� (dds���Ϸκ��� ���̴��� �� �� �ְ� ��ȯ���ִ� ��ƿ��Ƽ ���̺귯��)
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		// dds ������ �԰��� �� �°ų� dds ������ �ƴϸ� fail
		return false;
	}

	return true;
}

void LightClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}
// �ٸ� ��ü�� �ؽ��� ���̴� �ڿ��� ���� �ʿ� �� ȣ��(ex. �ؽ��� ������)
ID3D11ShaderResourceView* LightClass::GetTexture()
{
	return m_texture;
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR4(x, y, z, 1.0f);
	return;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

D3DXVECTOR4 LightClass::GetPosition()
{
	return m_position;
}