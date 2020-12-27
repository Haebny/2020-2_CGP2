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

	/// FBX ���ø����̼��� �ϳ��� �� �ʿ�.
	/// ���� ���� ���� FBX ������ �ε��ϰ� ���ÿ� ����� ��쿡�� �� ���Ͽ� ���� �� ����.
	m_Scene = FbxScene::Create(m_Manager, "Scene");
	if (!m_Scene)
	{
		return false;
	}

	/// Importer�� �ʱ�ȭ �Ǹ� ���� �ε��ϱ� ���� �� �����̳ʸ� ������ ��.
	/// FBX SKD�� Scene�� FbxSceneŬ������ ���� �߻�ȭ��.
	// fbx ���� ������ Scene���� �����´�.
	m_Importer->Import(m_Scene);

	/// ���� ������ ������ ä�� �Ŀ��� �޸� ����� ���̱� ���� �Ҹ�
	// Destory the Importer object.
	m_Importer->Destroy();

	/// �Ű������� �����ϴ� �� ��ü�� �ڽ��� ���� SDK �����ڸ� �˰� ����.
	/// �ش� SDK �����ڴ� �ν��Ͻ�ȭ ���� ���, �޽� �Ǵ� ��Ÿ FBX ��ü�� ���� �޸� �Ҵ�.
	/// Note. ��ü�� ���Ϸ� ������ �� �� ��ü�� ������ ��� ��ü�� ��������
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

	// Scene�� ��Ʈ ��带 �����´�.
	m_RootNode = m_Scene->GetRootNode();

	LoadNode(m_RootNode);

	/// Global Scene Settings
	// ��ǥ�� ������.
	FbxAxisSystem sceneAxisSystem = m_Scene->GetGlobalSettings().GetAxisSystem();

	// Scene �� ��ǥ�� ����
	FbxAxisSystem::MayaYUp.ConvertScene(m_Scene);

	// Scene �� �ﰢ�� ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��.
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
		///	Mesh�� �ٰ������� ������� ������Ʈ��. FBX SDK���� �޽� Ŭ������ �ʿ信 ���� n���� �ٰ������� ������ ����.
		/// ����ڴ� �ﰢ��, �簢�� ���� ��Ÿ �ٰ����� �����Ӱ� ȥ�� ����.

		// ����� �Ӽ� == Mesh
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FbxMesh�� ĳ���õ� ��� �Ӽ��� �����͸� ������.
			m_Mesh = node->GetMesh();

			D3DXVECTOR3* positions = nullptr;
			ProcessControlPoints(m_Mesh);	// ���������κ��� ��ġ ����Ʈ�� ä���. 
			unsigned int triCount = m_Mesh->GetPolygonCount(); // �޽��� �ﰢ�� ������ �����´�.
			unsigned int vertexCount = 0;

			for (unsigned int i = 0; i < triCount; ++i)
			{
				for (unsigned int j = 0; j < 3; ++j)
				{
					int controlPointIndex = m_Mesh->GetPolygonVertex(i, j);	// ������ �ε����� �����´�.
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
	// �������� ������ �����´�.
	unsigned int count = mesh->GetControlPointsCount();
	D3DXVECTOR3* positions = new D3DXVECTOR3[count];

	for (unsigned int i = 0; i < count; ++i)
	{
		D3DXVECTOR3 position;

		// �������� ���������� GetControlPointAt(int index) ��� �Լ��� �̿�.
		position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]); // x��ǥ
		position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]); // y��ǥ
		position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]); // z��ǥ
	}
}