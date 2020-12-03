#include "graphicsclass.h"
#include <ctime>

GraphicsClass::GraphicsClass()
{
	m_Camera = 0;
	m_D3D = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	//m_Bitmap = 0;
	m_Text = 0;
	m_Skybox = 0;

	// floor model
	floor.obj_path = "../Engine/data/models/floor.obj";
	floor.tex_path = L"../Engine/data/textures/floor.dds";
	floor.name = "floor";
	floor.tag = "floor";

	// reindeer model
	reindeer.obj_path = "../Engine/data/models/reindeer/body.obj";
	reindeer.tex_path = L"../Engine/data/textures/reindeer/fur.dds";
	reindeer.name = "reindeer";
	reindeer.tag = "npc";

	horns.obj_path = "../Engine/data/models/reindeer/horns.obj";
	horns.tex_path = L"../Engine/data/textures/reindeer/horn.dds";
	horns.name = "horns";
	horns.tag = "npc";

	face.obj_path = "../Engine/data/models/reindeer/face.obj";
	face.tex_path = L"../Engine/data/textures/reindeer/eyes.dds";
	face.name = "face";
	face.tag = "npc";

	player.obj_path = "../Engine/data/models/reindeer/body.obj";
	player.tex_path = L"../Engine/data/textures/reindeer/fur.dds";
	player.name = "player";
	player.tag = "player";

	horns2.obj_path = "../Engine/data/models/reindeer/horns.obj";
	horns2.tex_path = L"../Engine/data/textures/reindeer/horn.dds";
	horns2.name = "player";
	horns2.tag = "player";

	eyelips.obj_path = "../Engine/data/models/reindeer/eyelips.obj";
	eyelips.tex_path = L"../Engine/data/textures/reindeer/eyes.dds";
	eyelips.name = "player";
	eyelips.tag = "player";

	nose.obj_path = "../Engine/data/models/reindeer/nose.obj";
	nose.tex_path = L"../Engine/data/textures/reindeer/red.dds";
	nose.name = "player";
	nose.tag = "player";

	// tree model
	treetop.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop.tex_path= L"../Engine/data/textures/tree/treetop.dds";
	treetop.name = "treetop";
	treetop.tag = "tree";

	treebottom.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom.name = "treebottom";
	treebottom.tag = "tree";

	// ghost model
	ghost.obj_path = "../Engine/data/models/ghost.obj";
	ghost.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost.name = "ghost";
	ghost.tag = "ghost";

	// house model
	house.obj_path = "../Engine/data/models/house.obj";
	house.tex_path = L"../Engine/data/textures/house.dds";
	house.name = "house";
	house.tag = "house";

	camSpeed = 0.05f;

	m_Score = 0;
	m_playerLife = 3;
	isImmortal = false;
	isTimeOver = false;
	isCollided = false;
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

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
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
	m_Camera->SetPosition(0.0f, 0.0f, 0.0f);
	//m_Camera->SetRotation(20.0f, 0.0f, 0.0f);

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

	// Push back models to vector container.
	m_Models.push_back(floor);
	m_Models.push_back(player);
	m_Models.push_back(horns2);
	m_Models.push_back(eyelips);
	m_Models.push_back(nose);
	m_Models.push_back(reindeer);
	m_Models.push_back(horns);
	m_Models.push_back(face);
	m_Models.push_back(house);
	m_Models.push_back(treebottom);
	m_Models.push_back(treetop);
	m_Models.push_back(ghost);

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

		if (m_Models.at(i).tag == "player")
		{
			m_Models.at(i).min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
			m_Models.at(i).max = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
			m_Models.at(i).pos = D3DXVECTOR3(0.0f, 0.0f, -20.0f);
			playerPos = m_Models.at(i).pos;
		}

		if (m_Models.at(i).tag == "npc")
		{
			m_Models.at(i).min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
			m_Models.at(i).max = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
			m_Models.at(i).pos = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
		}

		if (m_Models.at(i).tag == "tree")
		{
			m_Models.at(i).min = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
			m_Models.at(i).max = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
			m_Models.at(i).pos = D3DXVECTOR3(-20.0f, 0.0f, 0.0f);
		}

		if (m_Models.at(i).tag == "ghost")
		{
			m_Models.at(i).min = D3DXVECTOR3(-7.0f, 0.0f, -7.0f);
			m_Models.at(i).max = D3DXVECTOR3(7.0f, 0.0f, 7.0f);
			m_Models.at(i).pos = D3DXVECTOR3(20.0f, 5.0f, 0.0f);
		}
	}

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
	m_Light->SetDiffuseColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}
	m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(playerPos.x, 3.0f, playerPos.z);

	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}
	m_Light3->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light3->SetPosition(m_Models.at(5).pos.x, 5.0f, m_Models.at(5).pos.z);

	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}
	m_Light4->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(m_Models.at(11).pos.x, 5.0f, m_Models.at(11).pos.z);

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

	// Create the bitmap object.
	m_TitleScene = new BitmapClass;
	if (!m_TitleScene)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_TitleScene->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
		L"../Engine/data/textures/floor.dds", screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	//// Create the bitmap object.
	//m_ResultScene = new BitmapClass;
	//if (!m_ResultScene)
	//{
	//	return false;
	//}

	//// Initialize the bitmap object.
	//result = m_ResultScene->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
	//	L"../Engine/data/textures/floor.dds", screenWidth, screenHeight);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
	//	return false;
	//}

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

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;

	m_CamRot.x = 0.0f;
	m_CamRot.y = 0.0f;
	m_CamRot.z = 0.0f;

	return true;
}


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
	m_CamRot.y -= (PreX - mouseX) * 0.1f;
	m_CamRot.x -= (PreY - mouseY) * 0.1f;

	PreX = (float)mouseX;
	PreY = (float)mouseY;

	m_Camera->SetPosition(m_CamPos.x, m_CamPos.y, m_CamPos.z);
	m_Camera->SetRotation(m_CamRot.x, m_CamRot.y, m_CamRot.z);

	// Set point lights' position.
	m_Light2->SetPosition(playerPos.x, 3.0f, playerPos.z);
	m_Light3->SetPosition(m_Models.at(5).pos.x, 5.0f, m_Models.at(5).pos.z);
	m_Light4->SetPosition(m_Models.at(11).pos.x, 5.0f, m_Models.at(11).pos.z);

	// Set the skybox.
	m_Skybox->Frame(m_CamPos);

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

	result = m_Text->SetLives(m_playerLife, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < m_Models.size(); i++)
	{
		if (m_Models.at(i).tag == "player")
		{
			for (int j = 10; j < m_Models.size(); j++)
			{
				isCollided = CheckAABB(m_Models.at(i), m_Models.at(j));

				if (isCollided && m_Models.at(j).tag == "ghost" && !isImmortal)
				{
					m_Result = Failure;
					break;
				}

				else if (isCollided && m_Models.at(j).tag == "tree" && !isImmortal)
				{
					m_playerLife--;
					isImmortal = true;
					m_startTime = frameTime;
					break;
				}
			}
		}
	}

	result = m_Text->SetCollision(isCollided, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// 부딪혔을 때 3초간 무적
	if (isImmortal)
	{
		CountSeconds(frameTime);
	}

	// 결과 화면 출력
	if (m_playerLife == 0 || m_Result == Failure)
	{
		ShowGameResult();
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
	D3DXMATRIX translation, scale, baseWorldMat, rotate;
	bool result = true;
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
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Render the skybox.
	m_Skybox->Render(m_D3D->GetDeviceContext(), m_D3D->GetRenderTarget(), m_D3D->GetDepthStencil(), viewMatrix, projectionMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

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

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	baseWorldMat = worldMatrix;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// 그래픽 파이프라인에 모델을 그림
	for (int i = 0; i < m_Models.size(); i++)
	{
		worldMatrix = baseWorldMat;

		if (m_Models.at(i).name == "floor")
		{
			// Transtlate object
			D3DXMatrixTranslation(&translation, 0.0f, -2.0f, 0.0f);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).tag == "npc")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 4.0f, 4.0f, 4.0f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, m_Models.at(i).pos.x, m_Models.at(i).pos.y, m_Models.at(i).pos.z);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).tag == "player")
		{
			m_Models.at(i).pos = playerPos;
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 3.0f, 3.0f, 3.0f);
			worldMatrix *= scale;

			D3DXMatrixRotationYawPitchRoll(&rotate, playerRot.y*0.0174532925f + D3DXToRadian(180), 0.0f, 0.0f);
			worldMatrix *= rotate;

			D3DXMatrixTranslation(&translation, playerPos.x, playerPos.y, playerPos.z);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).tag == "tree")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 7.0f, 7.0f, 7.0f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, m_Models.at(i).pos.x, m_Models.at(i).pos.y, m_Models.at(i).pos.z);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).tag == "ghost")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.01f, 0.01f, 0.01f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, m_Models.at(i).pos.x, m_Models.at(i).pos.y, m_Models.at(i).pos.z);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "house")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 50.0f);
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
	m_D3D->EndScene();

	return true;
}

/// Player Movements
void GraphicsClass::GoForward(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;
	playerRot.y = m_CamRot.y * 0.0174532925f + D3DXToRadian(180.0f);

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

void GraphicsClass::GoLeft(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;
	playerRot.y = m_CamRot.y * 0.0174532925f + D3DXToRadian(180.0f);

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

void GraphicsClass::GoBack(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;
	playerRot.y = m_CamRot.y * 0.0174532925f + D3DXToRadian(180.0f);

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

void GraphicsClass::GoRight(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;
	playerRot.y = m_CamRot.y * 0.0174532925f + D3DXToRadian(180.0f);

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

/// Camera Movements Only
void GraphicsClass::NevigateForward(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed * frameTime;
}

void GraphicsClass::NevigateLeft(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed * frameTime;
}

void GraphicsClass::NevigateBackward(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed * frameTime;
}

void GraphicsClass::NevigateRight(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed * frameTime;
}

void GraphicsClass::EnemyFSM(float frameTime)
{
	
}

int GraphicsClass::GetScore()
{
	return m_Score;
}

void GraphicsClass::IncreaseScore()
{
	m_Score += 1;
}

void GraphicsClass::ShowGameResult()
{
	if (m_Score == 3 && !isTimeOver)
	{
		m_Text->SetResult(0, m_D3D->GetDeviceContext());
		m_Result = Success;
	}

	if (isTimeOver || m_playerLife == 0)
	{
		m_Text->SetResult(1, m_D3D->GetDeviceContext());
		m_Result = Failure;
	}

	return;
}

void GraphicsClass::RestartGame()
{
	m_Result = Playing;
	m_playerLife = 3;
	isTimeOver = false;

	for (int i = 0; i < m_Models.size(); i++)
	{
		if (m_Models.at(i).tag == "player")
		{
			m_Models.at(i).min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
			m_Models.at(i).max = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
			m_Models.at(i).pos = D3DXVECTOR3(0.0f, 0.0f, -20.0f);
			playerPos = m_Models.at(i).pos;
		}

		if (m_Models.at(i).tag == "npc")
		{
			m_Models.at(i).min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
			m_Models.at(i).max = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
			m_Models.at(i).pos = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
		}

		if (m_Models.at(i).tag == "tree")
		{
			m_Models.at(i).min = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
			m_Models.at(i).max = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
			m_Models.at(i).pos = D3DXVECTOR3(-20.0f, 0.0f, 0.0f);
		}

		if (m_Models.at(i).tag == "ghost")
		{
			m_Models.at(i).min = D3DXVECTOR3(-7.0f, 0.0f, -7.0f);
			m_Models.at(i).max = D3DXVECTOR3(7.0f, 0.0f, 7.0f);
			m_Models.at(i).pos = D3DXVECTOR3(20.0f, 5.0f, 0.0f);
		}
	}
}

void GraphicsClass::StartGame()
{
	m_Result = Playing;
}

int GraphicsClass::GetResult()
{
	return m_Result;
}

bool GraphicsClass::CheckAABB(Model a, Model b)
{
	return (a.pos.x + a.min.x <= b.pos.x + b.max.x &&
		a.pos.x + a.max.x >= b.pos.x + b.min.x &&
		a.pos.z + a.min.z <= b.pos.z + b.max.z &&
		a.pos.z + a.max.z >= b.pos.z + b.min.z);
}

void GraphicsClass::CountSeconds(float frameTime)
{
	float timeDifference;

	currentTime = frameTime;

	timeDifference = (float)(currentTime - m_startTime);

	if (timeDifference >= 3.0f)
	{
		isImmortal = false;
	}
}