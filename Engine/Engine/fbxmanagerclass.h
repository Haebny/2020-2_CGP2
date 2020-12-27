#pragma once

#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <Windows.h>
#include <D3DX10math.h>

class FbxManagerClass
{
public:
	FbxManagerClass();
	FbxManagerClass(FbxManagerClass& other);
	virtual ~FbxManagerClass();

	bool Initialize(HWND hwnd);
	bool Shutdown();

	void LoadNode(FbxNode* node);
	void ProcessControlPoints(FbxMesh* mesh);

private:
	FbxManager* m_Manager;
	FbxIOSettings* m_IOSeetings;
	FbxImporter* m_Importer;

	FbxScene* m_Scene;
	FbxNode* m_Node;
	FbxNode* m_RootNode;
	FbxMesh* m_Mesh;
	FbxNodeAttribute* nodeAttribute;
};

