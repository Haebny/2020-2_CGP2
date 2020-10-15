#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"
#include <fstream>

using namespace std;

// 3D �𵨵��� ������ �����е��� ĸ��ȭ
class ModelClass
{
private:
	// ModelClass�� ���� ���ۿ� ����� ������ ����ü
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	}; // ColorShaderClass���� ����� �Ͱ� ������ ���ƾ� ��

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	// Model�� �̷�� �������� ����ü
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	}; // Vertex�� �� �ϳ��� ���� ��

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, const char*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(const char*);
	void ReleaseModel();

	bool ReadFileCounts(const char*, int&, int&, int&, int&);
	bool LoadDataStructures(const char*, int, int, int, int);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;	// ���� ���ۿ� �ε��� ����(���� �� ���� description���� ����)
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount;
	TextureClass* m_Texture;						// ���� �ؽ��� �ε�/��ȯ/����
	ModelType* m_model;
};

#endif