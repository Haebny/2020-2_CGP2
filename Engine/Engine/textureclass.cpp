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
	// 텍스쳐 파일 로드 (dds파일로부터 셰이더가 쓸 수 있게 변환해주는 유틸리티 라이브러리)
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		// dds 파일의 규격이 안 맞거나 dds 파일이 아니면 fail
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
// 다른 객체가 텍스쳐 셰이더 자원에 접근 필요 시 호출(ex. 텍스쳐 렌더링)
ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}