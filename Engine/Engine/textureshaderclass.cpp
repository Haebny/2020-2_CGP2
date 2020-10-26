#include "textureshaderclass.h"

TextureShaderClass::TextureShaderClass() 
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{
}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the vertex and pixel shaders.
	// 셰이더 초기화 함수 호출(셰이더 파일의 이름을 넘김)
	result = InitializeShader(device, hwnd, L"../Engine/texture.vs", L"../Engine/texture.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

// 셰이더를 정리하는 함수를 호출
void TextureShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	// 셰이더에서 사용될 인자 설정
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}
	
	// Now render the prepared buffers with the shader.
	// 이더를 이용하여 준비된 버퍼를 그림
	RenderShader(deviceContext, indexCount);
	
	return true;
}

// 셰이더 파일을 불러오고 DirectX와 GPU에서 사용 가능하도록 함
bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR*
	psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;			// 필터링 (Point, Bilinear, Anisotropic)

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	// 셰이더 프로그램을 버퍼로 컴파일
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage,
		NULL);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		// 셰이더가 컴파일에 실패함
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		// 셰이더 파일을 찾지 못함
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		// 셰이더 컴파일 실패
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		// 셰이더 파일을 찾지 못함
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	/// 셰이더 객체 생성 (여기서 나온 포인터를 정점 셰이더와 픽셀 셰이더의 인터페이스로서 사용)
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";							// 레이아웃에서 어떻게 사용되는지
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// 위치 벡터의 Format
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;								// 버퍼에 데이터가 배열되는 방식
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";							// 텍스쳐 좌표
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// 버퍼에 데이터가 배열되는 방식
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	// 레이아웃의 요소 개수
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	// 정점 입력 레이아웃 생성
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}
	
	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	// 더 이상 사용되지 않는 정점 셰이더 퍼버와 픽셀 셰이더 버퍼를 해제
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	/// 상수 버퍼:  매 프레임마다 상수 버퍼를 업데이트(버퍼의 사용은 동적)
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	// 정점 세이더의 행렬 상수 버퍼의 description
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 이 버퍼를 상수버퍼로 사용
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// 용도에 맞춰 설정
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	// 상수 버퍼 포인터 생성-> 이 클래스에서 정점 셰이더 상수 버퍼에 접근 가능
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// POINT | LINEAR | ANISOTROPIC
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// Wrap: 0.0f~1.0f의 범위를 벗어나면 범위 내 값으로 재조정
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// Clamp: 잘 안쓴다!
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);	// 변경 시 호출!
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureShaderClass::ShutdownShader() 
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	return;
}

// 정점/픽셀 셰이더의 컴파일 도중 오류로 만들어진 에러 메세지를 출력
void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR*
	shaderFilename) 
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename,
		MB_OK);

	return;
}

// 셰이더의 전역 변수 제어
// (함수에 사용된 행렬들은 GraphicsClass에서 만들어진 것)
bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) 
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	// 셰이더에서 사용할 수 있도록 행렬을 전치
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	// 상수 버퍼의 내용을 쓸 수 있게 잠금
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	// 상수 버퍼의 데이터에 대한 포인터 가져옴
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	// 상수 버퍼에 행렬 복사
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	// 상수 버퍼의 잠금 해제
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	// 정점 셰이더에서의 상수 버퍼의 위치 설정
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	// 정점 셰이더의 상수 버퍼를 바뀐 값으로 교환
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	// 픽셀 셰이더에 텍스쳐를 설정 (텍스쳐 데이터를 셰이더에게)
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) 
{
	// Set the vertex input layout.
	// 정점 입력 레이아웃 설정(어셈블러와 연결-> GPU 정점 버퍼의 자료구조 파악)
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	// 그릴 정점 셰이더와 픽셀 셰이더 설정
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	
	// Set the sampler state in the pixel shader.
	// 픽셀 셰이더의 샘플러 상태 설정
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	// 그리기
	deviceContext->DrawIndexed(indexCount, 0, 0);
	
	return;
}