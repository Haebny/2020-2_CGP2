#include "graphicsclass.h"
#include "Mesh.h"
#include "FBXImporter.h"

extern Mesh g_mesh;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	//m_Bitmap = 0;
	m_Text = 0;
	m_Skybox = 0;

	floor.obj_path = "../Engine/data/models/floor.obj";
	floor.tex_path = L"../Engine/data/textures/floor.dds";
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

	//m_switch = 0;

	// Initializing position and rotation of camera.
	PreX = 0.0f;
	PreY = 0.0f;
	
	CamPos.x = 0.0f;
	CamPos.y = 0.0f;
	CamPos.z = 0.0f;

	CamRot.x = 0.0f;
	CamRot.y = 0.0f;
	CamRot.z = 0.0f;

	speed = 0.5f;
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
	D3DXMATRIX baseViewMatrix;

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

	// Mesh 초기화
	std::unique_ptr<char[]> fileData;
	std::ifstream file("../Engine/data/models/tricycle.FBX", std::ifstream::binary);
	size_t fileSize;

	if (file)
	{
		file.seekg(0, std::ios::end);
		fileSize = static_cast<size_t> (file.tellg());
		file.seekg(0, std::ios::beg);

		fileData = std::make_unique<char[]>(fileSize);
		file.read(fileData.get(), fileSize);
	}
	else
	{
		return false;
	}

	if (FBXImporter::Start() == false)
	{
		return false;
	}

	if (FBXImporter::Import(g_mesh, fileData.get(), fileSize) == false)
	{
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(CamPos.x, CamPos.y, CamPos.z);
	m_Camera->SetRotation(CamRot.x, CamRot.y, CamRot.z);

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the skybox object.
	m_Skybox = new SkyboxClass;
	result = m_Skybox->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not Initialize Skybox.", L"Error", MB_OK);
		return false;
	}

	m_Models.push_back(floor);
	m_Models.push_back(wood);
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

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	//// Create the bitmap object.
	//m_Bitmap = new BitmapClass;
	//if (!m_Bitmap)
	//{
	//	return false;
	//}

	//// Initialize the bitmap object.
	//result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
	//	L"../Engine/data/textures/sky.dds", screenWidth, screenHeight);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
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
	m_Light->SetDirection(0.0f, -0.5f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}
	m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(-7.0f, 3.0f, 0.0f);
	m_Light2->StoreDiffuseColor();

	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}
	m_Light3->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light3->SetPosition(0.0f, 3.0f, 0.0f);
	m_Light3->StoreDiffuseColor();

	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}
	m_Light4->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(7.0f, 3.0f, 0.0f);
	m_Light4->StoreDiffuseColor();

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	// Set the position and rotation of camera.
	CamPos.x = 0.0f;
	CamPos.y = 3.0f;
	CamPos.z = -20.0f;

	CamRot.x = 0.0f;
	CamRot.y = 0.0f;
	CamRot.z = 0.0f;

	return true;
}

//모든 그래픽 객체의 해제
void GraphicsClass::Shutdown()
{
	// Release the skybox object.
	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		delete m_Skybox;
		m_Skybox = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
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

	//// Release the bitmap object.
	//if (m_Bitmap)
	//{
	//	m_Bitmap->Shutdown();
	//	delete m_Bitmap;
	//	m_Bitmap = 0;
	//}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

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

	FBXImporter::Shutdown();

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

bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime)
{
	bool result;

	// Set the camera movement.
	CamRot.y -= (PreX - mouseX) * 0.1f;
	CamRot.x -= (PreY - mouseY) * 0.1f;

	PreX = (float)mouseX;
	PreY = (float)mouseY;

	m_Camera->SetPosition(CamPos.x, CamPos.y, CamPos.z);
	m_Camera->SetRotation(CamRot.x, CamRot.y, CamRot.z);

	// Set the skybox.
	m_Skybox->Frame(CamPos);

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	D3DXMATRIX translation, scale, baseWorldMat;
	bool result;
	D3DXVECTOR4 diffuseColors[3];
	D3DXVECTOR4 lightPosition[3];

	// Create the diffuse color array from the four light colors.
	diffuseColors[0] = m_Light2->GetDiffuseColor();
	diffuseColors[1] = m_Light3->GetDiffuseColor();
	diffuseColors[2] = m_Light4->GetDiffuseColor();

	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light2->GetPosition();
	lightPosition[1] = m_Light3->GetPosition();
	lightPosition[2] = m_Light4->GetPosition();

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
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	// Render the skybox.
	m_Skybox->Render(m_D3D->GetDeviceContext(), m_D3D->GetRenderTarget(), m_D3D->GetDepthStencil(), viewMatrix, projectionMatrix);


	//// Turn off the Z buffer to begin all 2D rendering.
	//m_D3D->TurnZBufferOff();

	//// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	//if (!result)
	//{
	//	return false;
	//}

	//// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(),
	//	worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	//// Turn the Z buffer back on now that all 2D rendering has completed.
	//m_D3D->TurnZBufferOn();

	baseWorldMat = worldMatrix;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// 그래픽 파이프라인에 모델을 그림
	for (int i = 0; i < m_Models.size(); i++)
	{
		worldMatrix = baseWorldMat;
		if (m_Models.at(i).name == "floor")
		{
			// Transtlate object
			D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 0.0f);
			worldMatrix *= translation;
		}
		if (m_Models.at(i).name == "wood")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 2.0f, 2.0f, 2.0f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, -7.0f, 1.0f, 0.0f);
			worldMatrix *= translation;
		}
		if (m_Models.at(i).name == "cat")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.1f, 0.1f, 0.1f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 7.0f, 0.0f, 0.0f);
			worldMatrix *= translation;
		}
		if (m_Models.at(i).name == "dog")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 0.0f);
			worldMatrix *= translation;
		}

		m_Models.at(i).model->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models.at(i).model->GetIndexCount(), worldMatrix,
			viewMatrix, projectionMatrix, m_Models.at(i).model->GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), diffuseColors, lightPosition);
		if (!result)
		{
			return false;
		}		
	}
#pragma endregion

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), baseWorldMat, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene to the screen.
	// 버퍼에 그려진 씬 화면에 표시
	m_D3D->EndScene();

	// 이 이후는 외부 API

	return true;
}

/// Camera Movements
void GraphicsClass::GoForward()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}

void GraphicsClass::GoLeft()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}

void GraphicsClass::GoBack()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}

void GraphicsClass::GoRight()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}