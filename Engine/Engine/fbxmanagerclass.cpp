#include "fbxmanagerclass.h"
FbxManagerClass::FbxManagerClass()
{
	// Create FBX SDK manager object.
	m_Manager = FbxManager::Create();
}

FbxManagerClass::FbxManagerClass(FbxManagerClass& other)
{
}

FbxManagerClass::~FbxManagerClass()
{
}

bool FbxManagerClass::Initialize(HWND hwnd)
{
	// Create and set the IOSettings object.
	m_IOSeetings = FbxIOSettings::Create(m_Manager, IOSROOT);
	if (!m_IOSeetings)
	{
		return false;
	}

	m_Manager->SetIOSettings(m_IOSeetings);

	// Create the FbxImporter object.
	m_Importer = FbxImporter::Create(m_Manager, "");
	if (!m_Importer)
	{
		return false;
	}

	// Initialize the FbxImporter object.
	bool status = m_Importer->Initialize("../Engine/data/models/tricycle.fbx", -1, m_Manager->GetIOSettings());
	if (!status)
	{
		MessageBox(hwnd, L"Could not initialize the FBX Importer object. \nPlease check the file name.", L"Error", MB_OK);
		return false;
	}

	/// FBX 애플리케이션은 하나의 씬 필요.
	/// 만약 여러 개의 FBX 파일을 로드하고 동시에 사용할 경우에는 각 파일에 대한 씬 생성.
	m_Scene = FbxScene::Create(m_Manager, "Scene");
	if (!m_Scene)
	{
		return false;
	}

	/// Importer가 초기화 되면 씬을 로드하기 위해 씬 컨테이너를 만들어야 함.
	/// FBX SKD의 Scene은 FbxScene클래스에 의해 추상화됨.
	// fbx 파일 내용을 Scene으로 가져온다.
	m_Importer->Import(m_Scene);

	/// 파일 내용을 씬으로 채운 후에는 메모리 사용을 줄이기 위해 소멸
	// Destory the Importer object.
	m_Importer->Destroy();

	/// 매개변수로 전달하는 씬 객체는 자신이 속한 SDK 관리자를 알고 있음.
	/// 해당 SDK 관리자는 인스턴스화 중인 노드, 메쉬 또는 기타 FBX 객체에 대한 메모리 할당.
	/// Note. 객체를 파일로 내보낼 때 씬 객체로 생성한 모든 객체가 내보내짐
	// Create a node object.
	m_Node = FbxNode::Create(m_Scene, "");
	if (!m_Node)
	{
		return false;
	}

	// Create a mesh object.
	m_Mesh = FbxMesh::Create(m_Scene, "");
	if (!m_Mesh)
	{
		return false;
	}

	// Scene의 루트 노드를 가져온다.
	m_RootNode = m_Scene->GetRootNode();

	LoadNode(m_RootNode);

	/// Global Scene Settings
	// 좌표축 가져옴.
	FbxAxisSystem sceneAxisSystem = m_Scene->GetGlobalSettings().GetAxisSystem();

	// Scene 내 좌표축 변경
	FbxAxisSystem::MayaYUp.ConvertScene(m_Scene);

	// Scene 내 삼각형 화 할 수 있는 모든 노드를 삼각형화 함.
	FbxGeometryConverter geometryConverter(m_Manager);
	geometryConverter.Triangulate(m_Scene, true);

	return true;
}

bool FbxManagerClass::Shutdown()
{
	// Destroy these objects.
	if (m_Mesh)	m_Mesh->Destroy();		// Destroy the mesh.
	if (m_Node)	m_Node->Destroy();		// Destroy the node.
	if (m_Scene)	m_Scene->Destroy();		// Destroy the scene and its objects.

	if(m_Manager)	m_Manager->Destroy();	// Destroy SDK Manager and its objects.

	return true;
}

void FbxManagerClass::LoadNode(FbxNode* node)
{
	nodeAttribute = m_Node->GetNodeAttribute();

	if (nodeAttribute)
	{
		///	Mesh란 다각형으로 만들어진 지오메트리. FBX SDK에서 메쉬 클래스는 필요에 따라 n면의 다각형으로 형상을 정의.
		/// 사용자는 삼각형, 사각형 등의 기타 다각형을 자유롭게 혼합 가능.

		// 노드의 속성 == Mesh
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FbxMesh로 캐스팅된 노드 속성의 포인터를 가져옴.
			m_Mesh = node->GetMesh();

			D3DXVECTOR3* positions = nullptr;
			ProcessControlPoints(m_Mesh);	// 제어점으로부터 위치 리스트를 채운다. 
			unsigned int triCount = m_Mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다.
			unsigned int vertexCount = 0;

			for (unsigned int i = 0; i < triCount; ++i)
			{
				for (unsigned int j = 0; j < 3; ++j)
				{
					int controlPointIndex = m_Mesh->GetPolygonVertex(i, j);	// 제어점 인덱스를 가져온다.
					D3DXVECTOR3& position = positions[controlPointIndex];

					vertexCount++;
				}
			}
		}
	}

	const int childCount = node->GetChildCount();
	for (unsigned int i = 0; i < (unsigned int)childCount; ++i)
		LoadNode(node->GetChild(i));
}

void FbxManagerClass::ProcessControlPoints(FbxMesh* mesh)
{
	// 제어점의 개수를 가져온다.
	unsigned int count = mesh->GetControlPointsCount();
	D3DXVECTOR3* positions = new D3DXVECTOR3[count];

	for (unsigned int i = 0; i < count; ++i)
	{
		D3DXVECTOR3 position;

		// 제어점을 가져오려면 GetControlPointAt(int index) 멤버 함수를 이용.
		position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]); // x좌표
		position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]); // y좌표
		position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]); // z좌표
	}
}