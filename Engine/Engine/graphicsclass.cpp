#include "graphicsclass.h"
GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	//m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;

	floor.obj_path = "../Engine/data/models/floor.obj";
	floor.tex_path = L"../Engine/data/textures/metal.dds";
	floor.name = "floor";

	wood.obj_path = "../Engine/data/models/wood.obj";
	wood.tex_path = L"../Engine/data/textures/wood.dds";
	wood.name = "wood";

	cat.obj_path = "../Engine/data/models/cat.obj";
	cat.tex_path = L"../Engine/data/textures/cat.dds";
	cat.name = "cat";

	dog.obj_path = "../Engine/data/models/dog.obj";
	dog.tex_path = L"../Engine/data/textures/dog.dds";
	dog.name = "dog";
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// D3DClass == Direct3D 기능들을 다루는 클래스
	// Create the Direct3D object.
	// D3D 객체 생성
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	// D3D 객체 초기화
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN,
		SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 7.0f, -20.0f);
	m_Camera->SetRotation(20.0f, 0.0f, 0.0f);

	m_Models.push_back(floor);
	//m_Models.push_back(wood);
	m_Models.push_back(cat);
	m_Models.push_back(dog);

	// Initialize the model object.
	// 모델 텍스쳐도 같이 지정
	for (int i = 0; i < m_Models.size(); i++) 
	{
		// Create the model object.
		m_Models.at(i).model = new ModelClass;
		if (!m_Models.at(i).model)
		{
			MessageBox(hwnd, L"Could not create the model object.", L"Error", MB_OK);
			return false;
		}

		result = m_Models.at(i).model->Initialize(m_D3D->GetDevice(), m_Models.at(i).obj_path, m_Models.at(i).tex_path);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	//// Create the texture shader object.
	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader)
	//{
	//	return false;
	//}

	//// Initialize the color shader object.
	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}
	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the first light object.
	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}

	// Initialize the first light object.
	m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->SetPosition(-3.0f, 1.0f, 3.0f);
	
	// Create the second light object.
	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(3.0f, 1.0f, 3.0f);
	
	// Create the third light object.
	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light3->SetPosition(-3.0f, 1.0f, -3.0f);
	
	// Create the fourth light object.
	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}

	// Initialize the fourth light object.
	m_Light4->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(3.0f, 1.0f, -3.0f);

	return true;
}

//모든 그래픽 객체의 해제
void GraphicsClass::Shutdown()
{
	// Release the light objects.
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}
	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}
	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}
	if (m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	//// Release the texture shader object.
	//if(m_TextureShader)
	//{
	//	m_TextureShader->Shutdown();
	//	delete m_TextureShader;
	//	m_TextureShader = 0;
	//}

	// Release the model object.
	for (int i = 0; i < m_Models.size(); i++)
	{
		if (m_Models.at(i).model)
		{
			m_Models.at(i).model->Shutdown();
			delete m_Models.at(i).model;
			m_Models.at(i).model = 0;
		}

	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame(int key, bool state)
{
	// Update the light states
	m_Light->TurnOnLight(key, state);

	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	// 그래픽 렌더링 수행
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	D3DXMATRIX translation, scale;
	bool result;
	D3DXVECTOR4 diffuseColors[4];
	D3DXVECTOR4 lightPositions[4];

	// Create the diffuse color array from the four light colors.
	diffuseColors[0] = m_Light1->GetDiffuseColor();
	diffuseColors[1] = m_Light2->GetDiffuseColor();
	diffuseColors[2] = m_Light3->GetDiffuseColor();
	diffuseColors[3] = m_Light4->GetDiffuseColor();
	
	// Create the light position array from the four light positions.
	lightPositions[0] = m_Light1->GetPosition();
	lightPositions[1] = m_Light2->GetPosition();
	lightPositions[2] = m_Light3->GetPosition();
	lightPositions[3] = m_Light4->GetPosition();
	

	// Clear the buffers to begin the scene.
	// 씬 그리기를 위해 버퍼의 내용을 지움(화면을 특정 색으로 초기화)
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

#pragma region 출력
	// Generate the view matrix based on the camera's position.
	// Initialize 함수에서 지정한 카메라의 위치로 뷰 행렬을 만들기 위해 호출
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	// D3DClass 객체로부터 월드 행렬과 투영 행렬을 복사
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// 그래픽 파이프라인에 모델을 그림
	for (int i = 0; i < m_Models.size(); i++)
	{
		if (m_Models.at(i).name == "floor")
		{
			// Transtlate object
			D3DXMatrixTranslation(&translation, 0.0f, -2.0f, 0.0f);

			worldMatrix *= translation;
		}
		//if (m_Models.at(i).name == "wood")
		//{
		//	// Rotate object
		//	D3DXMatrixRotationX(&worldMatrix, rotation);

		//	D3DXMatrixTranslation(&translation, -7.0f, 0.0f, 0.0f);

		//	worldMatrix *= translation;
		//}
		if (m_Models.at(i).name == "cat")
		{
			// Rotate object
			D3DXMatrixRotationZ(&worldMatrix, rotation);

			// 크기 조정
			D3DXMatrixScaling(&scale, 0.1f, 0.1f, 0.1f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 7.0f, 1.0f, 0.0f);
			worldMatrix *= translation;
		}
		if (m_Models.at(i).name == "dog")
		{
			// Rotate object
			D3DXMatrixRotationY(&worldMatrix, rotation);

			// 크기 조정
			D3DXMatrixScaling(&scale, 0.15f, 0.15f, 0.15f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 0.0f);
			worldMatrix *= translation;
		}
		
		m_Models.at(i).model->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models.at(i).model->GetIndexCount(), worldMatrix,
			viewMatrix, projectionMatrix, m_Models.at(i).model->GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), diffuseColors, lightPositions);
		if (!result)
		{
			return false;
		}
	}
#pragma endregion

	// Present the rendered scene to the screen.
	// 버퍼에 그려진 씬 화면에 표시
	m_D3D->EndScene();

	// 이 이후는 외부 API

	return true;
}