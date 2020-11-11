#pragma once

#include <fbxsdk.h>

#include "Mesh.h"

class FBXImporter
{
public:
	static bool Start();
	static void Shutdown();

	static bool Import(Mesh& mesh, char* data, const int dataSize);

private:
	static void TraverseNode(FbxNode* fbxNode, vector<Vertex>& vertices);
	static FbxVector2 GetMeshUV(FbxMesh* fbxMesh, int controlPointIndex, int uvIndex);
	static FbxVector4 GetMeshNormal(FbxMesh* fbxMesh, int controlPointIndex, int uvIndex);
	//static FbxVector4 GetMeshTangent(FbxMesh* fbxMesh, int controlPointIndex, int uvIndex);

	static FbxAMatrix GetWorldMatrix(FbxNode* fbxNode);
};

