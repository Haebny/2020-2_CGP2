#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"
#include <fstream>

using namespace std;

// 3D 모델들의 복잡한 기하학들을 캡슐화
class ModelClass
{
private:
	// ModelClass의 정점 버퍼에 사용할 정점의 구조체
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	}; // ColorShaderClass에서 사용할 것과 구조가 같아야 함

	// Model을 이루는 정점들의 구조체
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	}; // Vertex가 모여 하나의 모델이 됨

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

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;	// 정점 버퍼와 인덱스 버퍼(생성 시 버퍼 description으로 구분)
	int m_vertexCount, m_indexCount;				// 버퍼의 크기 정보
	TextureClass* m_Texture;						// 모델의 텍스쳐 로드/반환/접근
	ModelType* m_model;
};

#endif