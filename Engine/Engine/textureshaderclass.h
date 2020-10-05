#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

using namespace std;

// (GPU�� �����ϴ� 3D �𵨵��� �׸��� �� ����ϴ�)HLSL ���̴��� ȣ���ϴ� Ŭ����
class TextureShaderClass
{
private:
	// ���� ���̴��� ���� cBuffer ����ü
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	}; // ���� ���̴��� ���� ����ü�� �����ؾ� ��

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
		ID3D11ShaderResourceView*);	//���̴��� ���Ǵ� �������� + ���̴��� �غ�� ���� �������� �׷����ϴ�.

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
