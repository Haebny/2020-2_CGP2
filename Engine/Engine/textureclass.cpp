#include "textureclass.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, const WCHAR* filename)
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

void TextureClass::Shutdown()
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
ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}