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

	// npc model
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

	// player model
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

#pragma region tree model

	// tree model
	treetop.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop.tex_path= L"../Engine/data/textures/tree/treetop.dds";
	treetop.name = "treetop";
	treetop.tag = "tree";

	treebottom.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom.name = "treebottom";
	treebottom.tag = "tree";

	treetop2.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop2.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop2.name = "treetop2";
	treetop2.tag = "tree";

	treebottom2.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom2.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom2.name = "treebottom2";
	treebottom2.tag = "tree";

	treetop3.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop3.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop3.name = "treetop3";
	treetop3.tag = "tree";

	treebottom3.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom3.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom3.name = "treebottom3";
	treebottom3.tag = "tree";

	treetop4.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop4.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop4.name = "treetop4";
	treetop4.tag = "tree";

	treebottom4.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom4.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom4.name = "treebottom4";
	treebottom4.tag = "tree";

	treetop5.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop5.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop5.name = "treetop5";
	treetop5.tag = "tree";

	treebottom5.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom5.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom5.name = "treebottom5";
	treebottom5.tag = "tree";

	treetop6.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop6.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop6.name = "treetop6";
	treetop6.tag = "tree";

	treebottom6.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom6.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom6.name = "treebottom6";
	treebottom6.tag = "tree";

	treetop7.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop7.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop7.name = "treetop7";
	treetop7.tag = "tree";

	treebottom7.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom7.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom7.name = "treebottom7";
	treebottom7.tag = "tree";

	treetop8.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop8.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop8.name = "treetop8";
	treetop8.tag = "tree";

	treebottom8.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom8.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom8.name = "treebottom8";
	treebottom8.tag = "tree";

	treetop9.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop9.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop9.name = "treetop9";
	treetop9.tag = "tree";

	treebottom9.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom9.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom9.name = "treebottom9";
	treebottom9.tag = "tree";

	treetop10.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop10.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop10.name = "treetop10";
	treetop10.tag = "tree";

	treebottom10.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom10.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom10.name = "treebottom10";
	treebottom10.tag = "tree";

	treetop11.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop11.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop11.name = "treetop11";
	treetop11.tag = "tree";

	treebottom11.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom11.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom11.name = "treebottom11";
	treebottom11.tag = "tree";

	treetop12.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop12.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop12.name = "treetop12";
	treetop12.tag = "tree";

	treebottom12.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom12.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom12.name = "treebottom12";
	treebottom12.tag = "tree";

	treetop13.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop13.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop13.name = "treetop13";
	treetop13.tag = "tree";

	treebottom13.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom13.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom13.name = "treebottom13";
	treebottom13.tag = "tree";

	treetop14.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop14.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop14.name = "treetop14";
	treetop14.tag = "tree";

	treebottom14.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom14.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom14.name = "treebottom14";
	treebottom14.tag = "tree";

	treetop15.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop15.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop15.name = "treetop15";
	treetop15.tag = "tree";

	treebottom15.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom15.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom15.name = "treebottom15";
	treebottom15.tag = "tree";

	treetop16.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop16.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop16.name = "treetop16";
	treetop16.tag = "tree";

	treebottom16.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom16.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom16.name = "treebottom16";
	treebottom16.tag = "tree";

	treetop17.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop17.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop17.name = "treetop17";
	treetop17.tag = "tree";

	treebottom17.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom17.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom17.name = "treebottom17";
	treebottom17.tag = "tree";

	treetop18.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop18.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop18.name = "treetop18";
	treetop18.tag = "tree";

	treebottom18.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom18.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom18.name = "treebottom18";
	treebottom18.tag = "tree";
	
	treetop19.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop19.tex_path= L"../Engine/data/textures/tree/treetop.dds";
	treetop19.name = "treetop19";
	treetop19.tag = "tree";

	treebottom19.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom19.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom19.name = "treebottom19";
	treebottom19.tag = "tree";

	treetop20.obj_path = "../Engine/data/models/tree/treetop.obj";
	treetop20.tex_path = L"../Engine/data/textures/tree/treetop.dds";
	treetop20.name = "treetop20";
	treetop20.tag = "tree";

	treebottom20.obj_path = "../Engine/data/models/tree/treebottom.obj";
	treebottom20.tex_path = L"../Engine/data/textures/tree/treebottom.dds";
	treebottom20.name = "treebottom20";
	treebottom20.tag = "tree";
#pragma endregion

#pragma region ghost model

	// ghost models
	ghost.obj_path = "../Engine/data/models/ghost.obj";
	ghost.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost.name = "ghost1";
	ghost.tag = "ghost";

	ghost2.obj_path = "../Engine/data/models/ghost.obj";
	ghost2.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost2.name = "ghost2";
	ghost2.tag = "ghost";

	ghost3.obj_path = "../Engine/data/models/ghost.obj";
	ghost3.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost3.name = "ghost3";
	ghost3.tag = "ghost";

	ghost4.obj_path = "../Engine/data/models/ghost.obj";
	ghost4.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost4.name = "ghost4";
	ghost4.tag = "ghost";

	ghost5.obj_path = "../Engine/data/models/ghost.obj";
	ghost5.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost5.name = "ghost5";
	ghost5.tag = "ghost";

	ghost6.obj_path = "../Engine/data/models/ghost.obj";
	ghost6.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost6.name = "ghost6";
	ghost6.tag = "ghost";

	ghost7.obj_path = "../Engine/data/models/ghost.obj";
	ghost7.tex_path = L"../Engine/data/textures/ghost.dds";
	ghost7.name = "ghost7";
	ghost7.tag = "ghost";

#pragma endregion

	// house models
	house.obj_path = "../Engine/data/models/house.obj";
	house.tex_path = L"../Engine/data/textures/house.dds";
	house.name = "house1";
	house.tag = "house";

	house2.obj_path = "../Engine/data/models/house.obj";
	house2.tex_path = L"../Engine/data/textures/house.dds";
	house2.name = "house2";
	house2.tag = "house";

	house3.obj_path = "../Engine/data/models/house.obj";
	house3.tex_path = L"../Engine/data/textures/house.dds";
	house3.name = "house3";
	house3.tag = "house";

	house4.obj_path = "../Engine/data/models/house.obj";
	house4.tex_path = L"../Engine/data/textures/house.dds";
	house4.name = "house4";
	house4.tag = "house";

	camSpeed = 0.05f;
	ghostSpeed = 0.02;

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

#pragma region push objects to vector container

	// Push back models to vector container.
	//floor
	m_Models.push_back(floor);
	//player
	m_Models.push_back(player);
	m_Models.push_back(horns2);
	m_Models.push_back(eyelips);
	m_Models.push_back(nose);
	//npc
	m_Models.push_back(reindeer);
	m_Models.push_back(horns);
	m_Models.push_back(face);
	//house
	m_Models.push_back(house);
	m_Models.push_back(house2);
	m_Models.push_back(house3);
	m_Models.push_back(house4);
	// ghost
	m_Models.push_back(ghost);
	m_Models.push_back(ghost2);
	m_Models.push_back(ghost3);
	m_Models.push_back(ghost4);
	m_Models.push_back(ghost5);
	m_Models.push_back(ghost6);
	m_Models.push_back(ghost7);
	// tree
	m_Models.push_back(treetop);
	m_Models.push_back(treetop2);
	m_Models.push_back(treetop3);
	m_Models.push_back(treetop4);
	m_Models.push_back(treetop5);
	m_Models.push_back(treetop6);
	m_Models.push_back(treetop7);
	m_Models.push_back(treetop8);
	m_Models.push_back(treetop9);
	m_Models.push_back(treetop10);
	m_Models.push_back(treetop11);
	m_Models.push_back(treetop12);
	m_Models.push_back(treetop13);
	m_Models.push_back(treetop14);
	m_Models.push_back(treetop15);
	m_Models.push_back(treetop16);
	m_Models.push_back(treetop17);
	m_Models.push_back(treetop18);
	m_Models.push_back(treetop19);
	m_Models.push_back(treetop20);
	m_Models.push_back(treebottom);
	m_Models.push_back(treebottom2);
	m_Models.push_back(treebottom3);
	m_Models.push_back(treebottom4);
	m_Models.push_back(treebottom5);
	m_Models.push_back(treebottom6);
	m_Models.push_back(treebottom7);
	m_Models.push_back(treebottom8);
	m_Models.push_back(treebottom9);
	m_Models.push_back(treebottom10);
	m_Models.push_back(treebottom11);
	m_Models.push_back(treebottom12);
	m_Models.push_back(treebottom13);
	m_Models.push_back(treebottom14);
	m_Models.push_back(treebottom15);
	m_Models.push_back(treebottom16);
	m_Models.push_back(treebottom17);
	m_Models.push_back(treebottom18);
	m_Models.push_back(treebottom19);
	m_Models.push_back(treebottom20);

#pragma endregion

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
			// Set the position of this object.
			m_Models.at(i).pos = D3DXVECTOR3(-379.799f, 0.0f, 378.019f);
			playerPos = m_Models.at(i).pos;

			// Set the collider of this object.
			m_Models.at(i).min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
			m_Models.at(i).max = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}

		if (m_Models.at(i).tag == "npc")
		{
			// Set the collider of this object.
			m_Models.at(i).min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
			m_Models.at(i).max = D3DXVECTOR3(1.0f, 0.0f, 1.0f);

			// Set the position of this object
			m_Models.at(i).pos = D3DXVECTOR3(-399.799f, -1.0f, 398.019f);
		}

		if (m_Models.at(i).tag == "ghost")
		{
			// Set the collider of this object.
			m_Models.at(i).min = D3DXVECTOR3(-7.0f, 0.0f, -7.0f);
			m_Models.at(i).max = D3DXVECTOR3(7.0f, 0.0f, 7.0f);

			// Set the position of this object.
			switch (i % 7)
			{
			case 0:
				m_Models.at(i).pos = D3DXVECTOR3(-86.358f, 5.0f, 312.452f);
				break;
			case 1:
				m_Models.at(i).pos = D3DXVECTOR3(225.587f, 5.0f, 432.953f);
				break;
			case 2:
				m_Models.at(i).pos = D3DXVECTOR3(203.598f, 5.0f, 106.189f);
				break;
			case 3:
				m_Models.at(i).pos = D3DXVECTOR3(-418.468f, 5.0f, 20.78f);
				break;
			case 4:
				m_Models.at(i).pos = D3DXVECTOR3(-49.124f, 5.0f, 20.78f);
				break;
			case 5:
				m_Models.at(i).pos = D3DXVECTOR3(-243.247f, 5.0f, -434.883f);
				break;
			case 6:
				m_Models.at(i).pos = D3DXVECTOR3(438.038f, 5.0f, -204.766f);
				break;
			default:
				break;
			}
		}

		if (m_Models.at(i).tag == "house")
		{
			// Set the collider of this object.
			m_Models.at(i).min = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
			m_Models.at(i).max = D3DXVECTOR3(10.0f, 0.0f, 10.0f);

			// Set the position of this object.
			switch (i % 4)
			{
			case 0:
				m_Models.at(i).pos = D3DXVECTOR3(-437.611f, 0.0f, 473.122f);
				break;
			case 1:
				m_Models.at(i).pos = D3DXVECTOR3(387.798, 0.0f, 236.138);
				break;
			case 2:
				m_Models.at(i).pos = D3DXVECTOR3(-389.729, 0.0f, -330.507);
				break;
			case 3:
				m_Models.at(i).pos = D3DXVECTOR3(287.665, 0.0f, -394.659);
				break;
			default:
				break;
			}
		}

		if (m_Models.at(i).tag == "tree")
		{
			m_Models.at(i).min = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
			m_Models.at(i).max = D3DXVECTOR3(10.0f, 0.0f, 10.0f);

			// Set the position of this object.
			switch (i % 20)
			{
			case 0:
				m_Models.at(i).pos = D3DXVECTOR3(-436.578f, 0.0, 334.143f);
				break;
			case 1:
				m_Models.at(i).pos = D3DXVECTOR3(-240.204f, 0.0, 407.44f);
				break;
			case 2:
				m_Models.at(i).pos = D3DXVECTOR3(-234.355f, 0.0, 191.119f);
				break;
			case 3:
				m_Models.at(i).pos = D3DXVECTOR3(-54.913f, 0.0, 260.42f);
				break;
			case 4:
				m_Models.at(i).pos = D3DXVECTOR3(-327.393f, 0.0, 74.176f);
				break;
			case 5:
				m_Models.at(i).pos = D3DXVECTOR3(-51.116f, 0.0, 35.982f);
				break;
			case 6:
				m_Models.at(i).pos = D3DXVECTOR3(-438.644f, 0.0, -88.999f);
				break;
			case 7:
				m_Models.at(i).pos = D3DXVECTOR3(-260.592f, 0.0, -181.816f);
				break;
			case 8:
				m_Models.at(i).pos = D3DXVECTOR3(-106.868, 0.0, -37.13f);
				break;
			case 9:
				m_Models.at(i).pos = D3DXVECTOR3(-3.059f, 0.0, 67.441f);
				break;
			case 10:
				m_Models.at(i).pos = D3DXVECTOR3(-130.813f, 0.0, 199.395f);
				break;
			case 11:
				m_Models.at(i).pos = D3DXVECTOR3(-263.334f, 0.0, 297.856f);
				break;
			case 12:
				m_Models.at(i).pos = D3DXVECTOR3(203.553f, 0.0, 14.846f);
				break;
			case 13:
				m_Models.at(i).pos = D3DXVECTOR3(56.014f, 0.0, -118.705f);
				break;
			case 14:
				m_Models.at(i).pos = D3DXVECTOR3(-190.649f, 0.0, -304.069f);
				break;
			case 15:
				m_Models.at(i).pos = D3DXVECTOR3(343.574f, 0.0, 146.298f);
				break;
			case 16:
				m_Models.at(i).pos = D3DXVECTOR3(76.132f, 0.0, -304.069f);
				break;
			case 17:
				m_Models.at(i).pos = D3DXVECTOR3(338.032f, 0.0, -304.567f);
				break;
			case 18:
				m_Models.at(i).pos = D3DXVECTOR3(378.253f, 0.0, -125.459f);
				break;
			case 19:
				m_Models.at(i).pos = D3DXVECTOR3(338.032f, 0.0, -304.567f);
				break;
			default:
				break;
			}
		}

		// Remember the start position of models.
		m_Models.at(i).startPos = m_Models.at(i).pos;
		m_Models.at(i).state = Idle;
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
	m_Light->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(0.35f, 0.35f, 0.35f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}
	m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);	// red
	m_Light2->SetPosition(0.0f, 0.0f, 0.0f);

	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}
	m_Light3->SetDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);	// yellow
	m_Light3->SetPosition(0.0f, 0.0f, 0.0f);

	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}
	m_Light4->SetDiffuseColor(0.2f, 1.0f, 0.2f, 1.0f);	// green
	m_Light4->SetPosition(0.0f, 0.0f, 0.0f);

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
	//m_TitleScene = new BitmapClass;
	//if (!m_TitleScene)
	//{
	//	return false;
	//}

	//// Initialize the bitmap object.
	//result = m_TitleScene->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
	//	L"../Engine/data/textures/floor.dds", screenWidth, screenHeight);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
	//	return false;
	//}

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

	// Check the player collided with boundary of the map.
	if (playerPos.x < -495.0f)
	{
		playerPos.x = -495.0f;
		m_CamPos.x = -495.0f;
	}
	else if (playerPos.x > 495.0f)
	{
		playerPos.x = 495.0f;
		m_CamPos.x = 495.0f;
	}
	if (playerPos.z < -495.0f)
	{
		playerPos.z = -495.0f;
		m_CamPos.z = -495.0f;
	}
	else if (playerPos.z > 495.0f)
	{
		playerPos.z = 495.0f;
		m_CamPos.z = 495.0f;
	}

	if (isCollided)
	{
		playerPos = prePos;
		m_CamPos = prePos;
		m_CamPos.y = 7.0f;
	}

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

	result = m_Text->SetLives(m_playerLife, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < m_Models.size(); i++)
	{
		if (m_Models.at(i).tag == "player" && !isImmortal)
		{
			for (int j = 10; j < m_Models.size(); j++)
			{
				isCollided = CheckAABB(m_Models.at(i), m_Models.at(j));

				if (isCollided && m_Models.at(j).tag == "ghost")
				{
					m_Result = Failure;
					continue;
				}

				else if (isCollided && m_Models.at(j).tag == "tree")
				{
					m_playerLife--;
					isImmortal = true;
					m_startTime = frameTime;
					continue;
				}
			}
		}

		if (m_Models.at(i).name == "house")
		{
			m_Light2->SetPosition(m_Models.at(i).pos.x, 10.0f, m_Models.at(i).pos.z);
			continue;
		}
		else if (m_Models.at(i).name == "house2")
		{
			m_Light3->SetPosition(m_Models.at(i).pos.x, 10.0f, m_Models.at(i).pos.z);
			continue;
		}
		else if (m_Models.at(i).name == "house3")
		{
			m_Light4->SetPosition(m_Models.at(i).pos.x, 10.0f, m_Models.at(i).pos.z);
			continue;
		}

		// Set point lights' position.
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

	result = m_Text->SetState(isImmortal, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
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

		if (m_Models.at(i).tag == "npc")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 5.0f, 5.0f, 5.0f);
			worldMatrix *= scale;
		}

		if (m_Models.at(i).tag == "tree")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 7.0f, 7.0f, 7.0f);
			worldMatrix *= scale;
		}

		if (m_Models.at(i).tag == "ghost")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.01f, 0.01f, 0.01f);
			worldMatrix *= scale;
		}

		if (m_Models.at(i).name == "house")
		{
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 0.5f, 0.5f, 0.5f);
			worldMatrix *= scale;
		}

		if (m_Models.at(i).tag == "player")
		{
			m_Models.at(i).pos = playerPos;
			// Resizing the model object.
			D3DXMatrixScaling(&scale, 3.0f, 3.0f, 3.0f);
			worldMatrix *= scale;

			D3DXMatrixRotationYawPitchRoll(&rotate, m_CamRot.y*0.0174532925f + D3DXToRadian(180), 0.0f, 0.0f);
			worldMatrix *= rotate;

			D3DXMatrixTranslation(&translation, playerPos.x, playerPos.y, playerPos.z);
			worldMatrix *= translation;
		}
		else if (m_Models.at(i).name == "floor")
		{
			// Transtlate object
			D3DXMatrixTranslation(&translation, 0.0f, -2.0f, 0.0f);
			worldMatrix *= translation;
		}
		else
		{
			D3DXMatrixTranslation(&translation, m_Models.at(i).pos.x, m_Models.at(i).pos.y, m_Models.at(i).pos.z);
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
	prePos = playerPos;

	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

void GraphicsClass::GoLeft(float frameTime)
{
	prePos = playerPos;

	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

void GraphicsClass::GoBack(float frameTime)
{
	prePos = playerPos;

	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;

	m_CamPos.x = playerPos.x;
	m_CamPos.y = 7.0f;
	m_CamPos.z = playerPos.z;
}

void GraphicsClass::GoRight(float frameTime)
{
	prePos = playerPos;

	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	playerPos += Direction * camSpeed * frameTime;
	playerPos.y = 0.0f;

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

void GraphicsClass::GhostFSM(float frameTime, Model model)
{
	float GtoPdistance = sqrt(pow(playerPos.x-model.pos.x, 2) + pow(playerPos.z - model.pos.z, 2));
	float GtoSdistance = sqrt(pow(model.startPos.x - model.pos.x, 2) + pow(model.startPos.z - model.pos.z, 2));

	// Ghost <-> Player
	if (GtoPdistance <= 20.0f)
	{
		model.state = Trace;
	}
	// Current ghost position <-> Start ghost position
	if (GtoSdistance > 40.0f)
	{
		model.state = Idle;
	}

	if (model.state == Idle)
	{
		if (model.pos.x <= model.startPos.x)
		{
			model.pos.x += ghostSpeed * frameTime;
		}
		else
		{
			model.pos.x -= ghostSpeed * frameTime;
		}

		if (model.pos.z <= model.startPos.z)
		{
			model.pos.z += ghostSpeed * frameTime;
		}
		else
		{
			model.pos.z -= ghostSpeed * frameTime;
		}
	}
	else // (model.state == Trace)
	{
		if (model.pos.x <= playerPos.x)
		{
			D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
			model.pos.x += Direction.x * ghostSpeed * frameTime;
		}
		else
		{
			D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
			model.pos.x -= Direction.x * ghostSpeed * frameTime;
		}

		if (model.pos.z <= playerPos.z)
		{
			D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
			model.pos.z += Direction.z * ghostSpeed * frameTime;
		}
		else
		{
			D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
			model.pos.z -= Direction.z * ghostSpeed * frameTime;
		}
	}
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
	m_Score = 0;
	m_playerLife = 3;
	isImmortal = false;
	isTimeOver = false;
	isCollided = false;

	for (int i = 0; i < m_Models.size(); i++)
	{
		m_Models.at(i).pos = m_Models.at(i).startPos;
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

	if (timeDifference >= 1.5f)
	{
		isImmortal = false;
	}
}