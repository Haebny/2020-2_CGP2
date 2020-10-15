#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
	m_Texture = 0;
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, const char* modelFilename, const WCHAR* textureFilename)
{
	bool result;
	
	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	// 정점 버퍼와 인덱스 버퍼의 초기화 함수 호출
	/// device는 GPU 메모리를 건드릴 때 사용
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	// 해당 모델의 텍스쳐 로드
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

// 정점 버퍼와 인덱스 버퍼 해제
void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

// [Called by GraphicsClass::Render]
// 그래픽 카드에 모델들의 기하 정보(정점버퍼,인덱스버퍼)를 그래픽 파이프라인에 넣고 컬러 셰이더로 그릴 준비를 함
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// 정점 버퍼와 인덱스 버퍼를 그래픽스 파이프라인에 넣어 화면에 그릴 준비
	RenderBuffers(deviceContext);

	return;
}

//  해당 모델의 인덱스의 개수 반환(컬러 셰이더에서 필요)
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

// 모델 텍스쳐 리소스 반환
// (텍스쳐 셰이더가 모델을 그리기 위해 텍스쳐에 접근 필요)
ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

// 정점 버퍼와 인덱스 버퍼를 생성하는 작업 제어
// (데이터 파일의 모델 정보-> 버퍼 생성)
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	/// 배열 생성
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	int i;

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		indices[i] = i;
	}

	/// 각 배열의 description 작성
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;	// 버퍼 크기
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// 버퍼 타입
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	// 정점 데이터를 가리키는 보조 리소스 구조체 작성
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	// 정점 버퍼 생성
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);	// desc, subresource ptr, new buffer ptr
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// Give the subresource structure a pointer to the index data.
	// 인덱스 데이터를 가리키는 보조 리소스 구조체 작성
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// Create the index buffer.
	// 인덱스 버퍼 생성
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);	// desc, subresource ptr, new buffer ptr
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

// InitializeBuffers 함수에서 만든 정점 버퍼와 인덱스 버퍼 해제
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// [Called by ModelClass::Render]
// 정점 버퍼와 인덱스 버퍼를 GPU의 어셈블러의 버퍼로써 활성화
// (GPU가 활성화된 정점 버퍼를 가지면 셰이더를 이용해 버퍼의 내용을 그릴 수 있음)
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	// 정점 버퍼 단위와 오프셋 설정
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	// input assembler에 정점 버퍼를 활성화(-> 그려질 수 있게 함)
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	
	// Set the index buffer to active in the input assembler so it can be rendered.
	// input assembler에 인덱스 버퍼를 활성화(-> 그려질 수 있게 함)
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	// 점 버퍼로 그릴 기본형을 설정(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST: 삼각형으로 그림)
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return;
}

// 텍스쳐 객체 생성/초기화
bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;
	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}
	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}
	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}

// Parsing(.txt파일)
bool ModelClass::LoadModel(const char* filename)
{
	HRESULT result;

	// Initialize the counts.
	m_vertexCount = 0;
	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;

	result = ReadFileCounts(filename, m_vertexCount, m_textureCount, m_normalCount, m_faceCount);
	if (FAILED(result))
	{
		return false;
	}

	result = LoadDataStructures(filename, m_vertexCount, m_textureCount, m_normalCount, m_faceCount);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::ReadFileCounts(const char* filename, int& vertexCount, int& textureCount, int& normalCount, int&faceCount)
{
	ifstream fin;
	char input;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);

	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}

bool ModelClass::LoadDataStructures(const char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	D3DXVECTOR3 *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new D3DXVECTOR3[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new D3DXVECTOR3[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new D3DXVECTOR3[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
					faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	m_vertexCount = faceCount * 3;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		m_model[i * 3].x = vertices[vIndex].x;
		m_model[i * 3].y = vertices[vIndex].y;
		m_model[i * 3].z = vertices[vIndex].z;

		m_model[i * 3].tu = texcoords[tIndex].x;
		m_model[i * 3].tv = texcoords[tIndex].y;

		m_model[i * 3].nx = normals[nIndex].x;
		m_model[i * 3].ny = normals[nIndex].y;
		m_model[i * 3].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		m_model[i * 3 + 1].x = vertices[vIndex].x;
		m_model[i * 3 + 1].y = vertices[vIndex].y;
		m_model[i * 3 + 1].z = vertices[vIndex].z;

		m_model[i * 3 + 1].tu = texcoords[tIndex].x;
		m_model[i * 3 + 1].tv = texcoords[tIndex].y;

		m_model[i * 3 + 1].nx = normals[nIndex].x;
		m_model[i * 3 + 1].ny = normals[nIndex].y;
		m_model[i * 3 + 1].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		m_model[i * 3 + 2].x = vertices[vIndex].x;
		m_model[i * 3 + 2].y = vertices[vIndex].y;
		m_model[i * 3 + 2].z = vertices[vIndex].z;

		m_model[i * 3 + 2].tu = texcoords[tIndex].x;
		m_model[i * 3 + 2].tv = texcoords[tIndex].y;

		m_model[i * 3 + 2].nx = normals[nIndex].x;
		m_model[i * 3 + 2].ny = normals[nIndex].y;
		m_model[i * 3 + 2].nz = normals[nIndex].z;
	}

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}