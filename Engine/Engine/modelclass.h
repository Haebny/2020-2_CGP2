#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"

// 3D �𵨵��� ������ �����е��� ĸ��ȭ
class ModelClass
{
private:
	// ModelClass�� ���� ���ۿ� ����� ������ ����ü
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	}; // ColorShaderClass���� ����� �Ͱ� ������ ���ƾ� ��

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);		// �׷��� ī�忡 �𵨵��� ���� ������ �ְ� �÷� ���̴��� �׸� �غ� ��

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;	// ���� ���ۿ� �ε��� ����(���� �� ���� description���� ����)
	int m_vertexCount, m_indexCount;				// ������ ũ�� ����
	TextureClass* m_Texture;						// ���� �ؽ��� �ε�/��ȯ/����
};

#endif