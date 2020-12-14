#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"
#include <fstream>
#include <list>
using namespace std;

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	//�� ������ ǥ���ϱ� ���� ����ü
	struct ModelType
	{
		float x, y, z;		// �� ��ġ
		float tu, tv;		// �� �ؽ���
		float nx, ny, nz;	// �� �븻	
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	bool ReadFileCounts(char*);
	bool LoadDataStructures(char*, int, int, int, int);


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_indexCount;
	int m_vertexCount, m_textureCount, m_normalCount, m_faceCount;

	TextureClass* m_Texture;
	ModelType* m_model; // �� �����͸� �а� ���� ���ۿ� �������� ������ ������ ����
};
#endif