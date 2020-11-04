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


bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// Load the texture in. 
	//텍스쳐 로드. filename은 파일 위치 및 이름. dds파일로부터 m_texture로 저장
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
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


// 다른 객체가 접근할 때 사용 (ex. 텍스쳐 렌더링)
ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}