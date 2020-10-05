#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

using namespace std;

// (GPU상에 존재하는 3D 모델들을 그리는 데 사용하는)HLSL 셰이더를 호출하는 클래스
class TextureShaderClass
{
private:
	// 정점 셰이더에 사용될 cBuffer 구조체
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	}; // 정점 셰이더에 사용된 구조체와 동일해야 함

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
		ID3D11ShaderResourceView*);	//셰이더에 사용되는 변수설정 + 셰이더로 준비된 모델의 정점들을 그려냅니다.

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);				// 1
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,	// 2
		ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);										// 3

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif
