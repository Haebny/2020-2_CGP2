#include "graphicsclass.h"


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

	floor.obj_path = "../Engine/data/models/floor.obj";
	floor.tex_path = L"../Engine/data/textures/floor.dds";
	floor.name = "floor";

	woods.obj_path = "../Engine/data/models/woods.obj";
	woods.tex_path = L"../Engine/data/textures/wood.dds";
	woods.name = "woods";

	cushions.obj_path = "../Engine/data/models/cushions.obj";
	cushions.tex_path = L"../Engine/data/textures/cushion.dds";
	cushions.name = "cushions";

	table.obj_path = "../Engine/data/models/table.obj";
	table.tex_path = L"../Engine/data/textures/table.dds";
	table.name = "table";

	cat.obj_path = "../Engine/data/models/cat.obj";
	cat.tex_path = L"../Engine/data/textures/cat.dds";
	cat.name = "cat";

	dog.obj_path = "../Engine/data/models/dog.obj";
	dog.tex_path = L"../Engine/data/textures/dog.dds";
	dog.name = "dog";

	c_cup.obj_path = "../Engine/data/models/c_cup.obj";
	c_cup.tex_path = L"../Engine/data/textures/beermug.dds";
	c_cup.name = "c_cup";

	d_cup.obj_path = "../Engine/data/models/d_cup.obj";
	d_cup.tex_path = L"../Engine/data/textures/beermug.dds";
	d_cup.name = "d_cup";

	pepsi.obj_path = "../Engine/data/models/pepsi.obj";
	pepsi.tex_path = L"../Engine/data/textures/pepsi.dds";
	pepsi.name = "pepsi";

	camSpeed = 0.005f;
	playerSpeed = 0.0025f;
	enemySpeed = 0.0009f;
	pepsiSpeed = 0.001f;

	m_PlaPos.x = 5.0f;
	m_PlaPos.z = 0.0f;
	m_EnePos.x = -5.0f;
	m_EnePos.z = 0.0f;
	
	D3DXVECTOR3 initVec(0.0f, 0.0f, 0.0f);
	lastColPos = initVec;

	start = false;
	result = false;
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
	m_Models.push_back(woods);
	m_Models.push_back(cushions);
	m_Models.push_back(table);
	m_Models.push_back(cat);
	m_Models.push_back(dog);
	m_Models.push_back(c_cup);
	m_Models.push_back(d_cup);
	m_Models.push_back(pepsi);

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
	m_Light->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}
	m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(-10.0f, 5.0f, 0.0f);

	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}
	m_Light3->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light3->SetPosition(0.0f, 5.0f, 0.0f);

	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}
	m_Light4->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(10.0f, 5.0f, 0.0f);

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

	//// Create the bitmap object.
	//m_Bitmap = new BitmapClass;
	//if (!m_Bitmap)
	//{
	//	return false;
	//}

	//// Initialize the bitmap object.
	//result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
	//	L"../Engine/data/textures/seafloor.dds", screenWidth, screenHeight);
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

	m_CamPos.x = 10.0f;
	m_CamPos.y = 10.0f;
	m_CamPos.z = 0.0f;

	m_CamRot.x = 0.0f;
	m_CamRot.y = 0.0f;
	m_CamRot.z = 0.0f;

	m_PepRot.y = 20.0f;

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

	// Set the location of the mouse.
	result = m_Text->SetScore(GetPlayerScore(), GetEnemyScore(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// If player win or lose, show the Result of the game.
	if ((p_score >= 3 || e_score >= 3) && result)
	{
		ShowGameResult();
	}

	PepsiMoves();
	EnemyMoves();

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

		if (m_Models.at(i).name == "woods")
		{
						// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, 0.0f);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "cushions")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, 0.0f);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "table")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, 0.0f);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "cat")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, 0.0f);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "dog")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, 0.0f);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "c_cup")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, m_EnePos.z);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "d_cup")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, 0.0f, -3.0f, m_PlaPos.z);
			worldMatrix *= translation;
		}

		if (m_Models.at(i).name == "pepsi")
		{
			D3DXMatrixRotationY(&worldMatrix, rotation);
			
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;

			D3DXMatrixTranslation(&translation, m_PepPos.x, -3.0f, m_PepPos.z);
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

/// Camera Movements
void GraphicsClass::GoForward()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed;
}

void GraphicsClass::GoLeft()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed;
}

void GraphicsClass::GoBack()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed;
}

void GraphicsClass::GoRight()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * camSpeed;
}

/// Player Movements
void GraphicsClass::MoveLeft()
{
	m_PlaPos.z -= playerSpeed;

	if (m_PlaPos.z < -3.0f)
		m_PlaPos.z = -3.0f;
}

void GraphicsClass::MoveRight()
{
	m_PlaPos.z += playerSpeed;

	if (m_PlaPos.z > 3.0f)
		m_PlaPos.z = 3.0f;
}

void GraphicsClass::EnemyMoves()
{
	//// If the pepsi object is going to player, enemy object doesn't need to move.
	//if (turn)
	//	return;

	if (m_EnePos.z <= m_PepPos.z)
		m_EnePos.z += enemySpeed;
	else
		m_EnePos.z -= enemySpeed;

	//if (m_EnePos.z < -3.0f)
	//	m_EnePos.z = -3.0f;

	//if (m_EnePos.z > 3.0f)
	//	m_EnePos.z = 3.0f;
}

void GraphicsClass::PepsiMoves()
{
	if (!start)
		return;

	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_PepRot.y * 0.0174532925f, m_PepRot.x * 0.0174532925f, m_PepRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	// Pepsi object moves to player.
	if (turn)
	{
		m_PepPos += Direction * pepsiSpeed;
	}
	// Pepsi object moves to enemy.
	else
	{
		m_PepPos -= Direction * pepsiSpeed;
	}

	// Collision
	if (turn && m_PepPos.x >= m_PlaPos.x - 1.0f)
	{
		// Player 기준
		if (m_PepPos.z <= m_PlaPos.z + 2.0f || m_PepPos.z >= m_PlaPos.z - 2.0f)
		{
			turn = !turn;

			D3DXVECTOR3 bounceDir(-(lastColPos.x - m_PepRot.x), 0.0f, -(lastColPos.z - m_PepRot.z));
			m_PepRot = bounceDir;
			lastColPos = m_PepPos;
		}
	}
	else if (!turn && m_PepPos.x >= m_EnePos.x - 1.0f)
	{
		// Enemy 기준
		if (m_PepPos.z >= m_EnePos.z - 2.0f || m_PepPos.z <= m_EnePos.z + 2.0f)
		{
			turn = !turn;

			D3DXVECTOR3 bounceDir(-(lastColPos.x - m_PepRot.x), 0.0f, -(lastColPos.z - m_PepRot.z));
			m_PepRot = bounceDir;
			lastColPos = m_PepPos;
		}
	}

	// Restricts Pepsi from going out of range.
	if (m_PepPos.z < -3.0f)
	{
		D3DXVECTOR3 bounceDir(-(lastColPos.x - m_PepRot.x), 0.0f, -(lastColPos.z - m_PepRot.z));
		m_PepRot = bounceDir;
		lastColPos = m_PepPos;
	}
	else if (m_PepPos.z > 3.0f)
	{
		D3DXVECTOR3 bounceDir(-(lastColPos.x - m_PepRot.x), 0.0f, -(lastColPos.z - m_PepRot.z));
		m_PepRot = bounceDir;
		lastColPos = m_PepPos;
	}

	// Scoring
	if (m_PepPos.x >= 7.0f)
	{
		start = false;

		m_PlaPos.z = 0.0f;
		m_EnePos.z = 0.0f;
		m_PepPos.x = 0.0f;
		m_PepPos.z = 0.0f;

		IncreaseEnemyScore();
		turn = true;
	}
	else if (m_PepPos.x <= -7.0f)
	{
		start = false;

		m_PlaPos.z = 0.0f;
		m_EnePos.z = 0.0f;
		m_PepPos.x = 0.0f;
		m_PepPos.z = 0.0f;

		IncreasePlayerScore();
		turn = false;
	}
}

int GraphicsClass::GetPlayerScore()
{
	return p_score;
}

int GraphicsClass::GetEnemyScore()
{
	return e_score;
}

void GraphicsClass::IncreasePlayerScore()
{
	p_score += 1;
}

void GraphicsClass::IncreaseEnemyScore()
{
	e_score += 1;
}

void GraphicsClass::ShowGameResult()
{
	result = true;

	if (p_score >= 3)
	{
		m_Text->SetResult(0, m_D3D->GetDeviceContext());
	}
	else if (e_score >= 3)
	{
		m_Text->SetResult(1, m_D3D->GetDeviceContext());
	}
}

void GraphicsClass::RestartGame()
{
	result = false;
	m_CamPos.x = 10.0f;
	m_CamPos.y = 10.0f;
	m_CamPos.z = 0.0f;

	m_PlaPos.z = 0.0f;
	m_EnePos.z = 0.0f;
	m_PepPos.x = 0.0f;
	m_PepPos.z = 0.0f;

	p_score = 0;
	e_score = 0;

	D3DXVECTOR3 initVec(0.0f, 0.0f, 0.0f);
	lastColPos = initVec;
}

void GraphicsClass::StartGame()
{
	start = true;
	if(p_score == 0 && e_score == 0)
		turn = true;
}

bool GraphicsClass::GetResult()
{
	return result;
}
