#include "textclass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;
	memset(m_sentences, 0, MAX_SENTENCES);
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int
	screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, "../Engine/data/fontdata.txt", L"../Engine/data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize sentences.
	for (int i = 0; i < MAX_SENTENCES; i++)
	{
		result = InitializeSentence(&m_sentences[i], 32, device);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

void TextClass::Shutdown()
{
	for (int i = 0; i < MAX_SENTENCES; i++)
	{
		// Release the first sentence.
		if(m_sentences[i] != NULL)
			ReleaseSentence(&m_sentences[i]);
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX
	orthoMatrix)
{
	bool result;

	for (int i = 0; i < MAX_SENTENCES; i++)
	{
		// Draw the first sentence.
		if (m_sentences[i] != NULL)
		{
			result = RenderSentence(deviceContext, m_sentences[i], worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float
	green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX
	worldMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix,
		orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;

	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[0], fpsString, 20, 20, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[1], cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;

	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[2], mouseString, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[3], mouseString, 20, 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetScore(int score, ID3D11DeviceContext *deviceContext)
{
	char tempString[16];
	char scoreString[16];
	char playerString[16];
	char enemyString[16];
	char startString[32];
	bool result;

	// Setup the player score string.
	strcpy_s(scoreString, "Score");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[4], scoreString, 20, m_screenHeight-60, 0.5f, 0.5f, 0.5f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert the player's score integer to string format.
	_itoa_s(score, tempString, 10);

	// Setup the player's score string.
	strcpy_s(playerString, "Dog(Player): ");
	strcat_s(playerString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[5], playerString, m_screenWidth/3, 80, 0.0f, 0.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	strcpy_s(startString, "Press Spacebar to Start Game!");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[7], startString, (m_screenWidth/7) * 3, 100, 0.5f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

//bool TextClass::SetResult(int num, ID3D11DeviceContext *deviceContext)
//{
//	char resultString[32];
//	bool result;
//
//	switch (num)
//	{
//	case 0:
//		// Setup the player win string.
//		strcpy_s(resultString, "CLEAR !");
//
//		result = UpdateSentence(m_sentences[8], resultString, m_screenWidth/2, 20, 0.7f, 0.0f, 0.7f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//		break;
//	
//	case 1:
//		// Setup the player win string.
//		strcpy_s(resultString, "OOPS !..");
//
//		result = UpdateSentence(m_sentences[8], resultString, m_screenWidth/2, 20, 0.5f, 0.0f, 0.5f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//		break;
//	
//	default:
//		// Erase the result string.
//		strcpy_s(resultString, "");
//
//		result = UpdateSentence(m_sentences[8], resultString, m_screenWidth / 2, 20, 0.5f, 0.0f, 0.5f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//		break;
//	}
//
//	if (num == -1)
//	{
//		strcpy_s(resultString, "");
//	}
//	else
//	{
//		strcpy_s(resultString, "Press Enter to Restart!");
//	}
//
//	result = UpdateSentence(m_sentences[9], resultString, m_screenWidth / 2, 40, 1.0f, 0.0f, 0.0f, deviceContext);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}

bool TextClass::SetGameUI(int lives, int gifts, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char livesString[16];
	char gfitsString[32];
	bool result;

	// Convert the number of lives integer to string format.
	_itoa_s(lives, tempString, 10);

	// Setup the lives string.
	strcpy_s(livesString, "Lives: ");
	strcat_s(livesString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[10], livesString, 20, m_screenHeight - 80, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert the number of gifts (left to deliver) integer to string format.
	_itoa_s(gifts, tempString, 10);

	// Setup the lives string.
	strcpy_s(gfitsString, "Delivered: ");
	strcat_s(gfitsString, tempString);
	strcat_s(gfitsString, " / 3");

	// Update the sentence vertex buffer with the new string information.
	if(gifts <= 1)
		result = UpdateSentence(m_sentences[15], gfitsString, 20, m_screenHeight - 100, 1.0f, 0.0f, 0.0f, deviceContext);
	else if(gifts == 2)
		result = UpdateSentence(m_sentences[15], gfitsString, 20, m_screenHeight - 100, 0.5f, 1.0f, 0.0f, deviceContext);
	else if (gifts == 3)
		result = UpdateSentence(m_sentences[15], gfitsString, 20, m_screenHeight - 100, 0.0f, 0.5f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCollision(bool collision, ID3D11DeviceContext* deviceContext)
{
	char collisionString[32];
	bool result;

	// Setup the cpu string.
	strcpy_s(collisionString, "Collision: ");
	if (collision)
	{
		strcat_s(collisionString, "True");
		result = UpdateSentence(m_sentences[11], collisionString, 20, 100, 0.0f, 1.0f, 0.0f, deviceContext);
	}
	else
	{
		strcat_s(collisionString, "False");
		result = UpdateSentence(m_sentences[11], collisionString, 20, 100, 1.0f, 0.0f, 0.5f, deviceContext);
	}
	
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetState(bool immortal, ID3D11DeviceContext *deviceContext)
{
	char printString[32];
	bool result;

	// Setup the time string.
	strcpy_s(printString, "Immortal: ");

	// Update the sentence vertex buffer with the new string information.
	if (immortal)
	{
		strcat_s(printString, "True");
		result = UpdateSentence(m_sentences[14], printString, 20, 160, 0.0f, 1.0f, 0.0f, deviceContext);
	}
	else
	{
		strcat_s(printString, "False");
		result = UpdateSentence(m_sentences[14], printString, 20, 160, 1.0f, 0.0f, 0.5f, deviceContext);
	}

	if (!result)
	{
		return false;
	}

	return true;
}

//bool TextClass::SetTitle(int num, ID3D11DeviceContext *deviceContext)
//{
//	char resultString[32];
//	bool result;
//	
//	if (num == -1)
//	{
//		strcpy_s(resultString, "");
//		result = UpdateSentence(m_sentences[16], resultString, m_screenWidth / 3, m_screenHeight / 2 - 80, 1.0f, 1.0f, 1.0f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//
//		strcpy_s(resultString, "");
//		result = UpdateSentence(m_sentences[17], resultString, m_screenWidth / 3, m_screenHeight / 2 - 80, 1.0f, 1.0f, 1.0f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//
//		strcpy_s(resultString, "");
//		result = UpdateSentence(m_sentences[18], resultString, m_screenWidth / 3, m_screenHeight / 2 - 80, 1.0f, 1.0f, 1.0f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//	}
//	else
//	{
//		strcpy_s(resultString, "<< How To Play >>");
//		result = UpdateSentence(m_sentences[16], resultString, m_screenWidth / 3+ 50, m_screenHeight / 2 - 80, 1.0f, 1.0f, 1.0f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//
//		strcpy_s(resultString, "Move : WASD  |  Holding : Space");
//		result = UpdateSentence(m_sentences[17], resultString, m_screenWidth / 3+30, m_screenHeight / 2 - 60, 1.0f, 1.0f, 1.0f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//
//		strcpy_s(resultString, "Press Enter to Start!");
//		result = UpdateSentence(m_sentences[18], resultString, m_screenWidth / 3 + 50, m_screenHeight / 2, 1.0f, 1.0f, 0.0f, deviceContext);
//		if (!result)
//		{
//			return false;
//		}
//	}
//
//	return false;
//}


//#ifdef DEBUG
bool TextClass::SetPos(float plaX, float plaZ, ID3D11DeviceContext *deviceContext)
{
	char tempString[16];
	char printString[16];
	bool result;

	// Convert the mouseX integer to string format.
	_itoa_s((int)plaX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(printString, "Player X: ");
	strcat_s(printString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[12], printString, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert the mouseX integer to string format.
	_itoa_s((int)plaZ, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(printString, "Player Z: ");
	strcat_s(printString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentences[13], printString, 20, 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}
//#endif