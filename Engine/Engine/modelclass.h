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

	//모델 포맷을 표현하기 위한 구조체
	struct ModelType
	{
		float x, y, z;		// 모델 위치
		float tu, tv;		// 모델 텍스쳐
		float nx, ny, nz;	// 모델 노말	
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
	ModelType* m_model; // 모델 데이터를 읽고 정점 버퍼에 놓여지기 전까지 데이터 저장
};
#endif