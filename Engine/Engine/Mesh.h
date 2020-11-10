#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>

using namespace std;
using namespace Microsoft::WRL;

struct Vertex
{
public:
	Vertex(float position[3], float uv[2], float normal[3]) :
		position{ position[0], position[1], position[2] },
		uv{ uv[0], uv[1] },
		normal{ normal[0], normal[1], normal[2] }
	{

	}

	float position[3];
	float uv[2];
	float normal[3];
};

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	bool Initialize(const vector<Vertex>& vertices, const vector<unsigned int>& indices);
	void Destroy();

	ComPtr<ID3D11Buffer> GetVertexBuffer() const;
	unsigned int GetVertexCount() const;

	ComPtr<ID3D11Buffer> GetIndexBuffer() const;
	unsigned int GetIndexCount() const;

private:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	unsigned int m_vertexCount;

	ComPtr<ID3D11Buffer> m_indexBuffer;
	unsigned int m_indexCount;
};

