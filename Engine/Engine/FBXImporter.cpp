#pragma comment(lib, "libfbxsdk-md.lib")

#include <vector>
#include <array>

#include "FBXImporter.h"
#include "FBXMemoryReadStream.h"

static FbxManager* g_fbxManager;
static FbxIOSettings* g_fbxIOSettings;

bool FBXImporter::Start()
{
    g_fbxManager = FbxManager::Create();

    if (g_fbxManager == nullptr)
    {
        return false;
    }

    g_fbxIOSettings = FbxIOSettings::Create(g_fbxManager, IOSROOT);
    g_fbxManager->SetIOSettings(g_fbxIOSettings);
    
    return true;
}

void FBXImporter::Shutdown()
{
    if (g_fbxIOSettings != nullptr)
    {
        g_fbxIOSettings->Destroy();
        g_fbxIOSettings = nullptr;
    }
    
    if (g_fbxManager != nullptr)
    {
        g_fbxManager->Destroy();
        g_fbxManager = nullptr;
    }
}

bool FBXImporter::Import(Mesh& mesh, char* data, const int dataSize)
{
    FBXMemoryReadStream stream(dataSize, g_fbxManager);
    FbxImporter* fbxImporter = FbxImporter::Create(g_fbxManager, "Importer");

    if (fbxImporter->Initialize(&stream, data, -1, g_fbxManager->GetIOSettings()) == false)
    {
        fbxImporter->Destroy();

        return false;
    }

    // Scene 생성
    FbxScene* fbxScene = FbxScene::Create(g_fbxManager, "Scene");

    if (fbxImporter->Import(fbxScene) == false)
    {
        fbxImporter->Destroy();
        fbxScene->Destroy();

        return false;
    }

    fbxImporter->Destroy();

    FbxNode* fbxRootNode = fbxScene->GetRootNode();
    std::vector<Vertex> vertices;

    if (fbxRootNode)
    {
        // Triangle로 이루어진 모델이 아니면 제대로 import가 안될 수 있음
        // 때문에 Triangulate를 시행하고 잘 안될경우 모델이 이상한 것으로 판단
        FbxGeometryConverter fbxGeometryConverter(g_fbxManager);
        if (fbxGeometryConverter.Triangulate(fbxScene, true) == false)
        {
            fbxScene->Destroy();

            return false;
        }

        TraverseNode(fbxRootNode, vertices);
    }

    return true;
}

void FBXImporter::TraverseNode(FbxNode* fbxNode, vector<Vertex>& vertices)
{
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute != nullptr && fbxNodeAttribute)

    for (int childIndex = 0; childIndex < fbxNode->GetChildCount(); childIndex++)
    {
        TraverseNode(fbxNode->GetChild(childIndex), vertices);
    }
}

FbxVector4 FBXImporter::GetMeshUV(FbxMesh* fbxMesh, int controlPointIndex, int uvIndex)
{
    return FbxVector4();
}

FbxVector4 FBXImporter::GetMeshNormal(FbxMesh* fbxMesh, int controlPointIndex, int uvIndex)
{
    return FbxVector4();
}

FbxMatrix FBXImporter::GetWorldMatrix(FbxNode* fbxNode)
{
    return FbxMatrix();
}
