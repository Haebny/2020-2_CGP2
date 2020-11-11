#pragma comment (lib, "libfbxsdk-md.lib")

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

    if (fbxNodeAttribute != nullptr && fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
    {
        FbxMesh* fbxMesh = fbxNode->GetMesh();
        fbxMesh->ApplyPivot();
        fbxMesh->GenerateTangentsData(0);

        int polygonCount = fbxMesh->GetPolygonCount();
        int vertexId = 0;

        FbxMatrix matrix = GetWorldMatrix(fbxNode);
        for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
        {
            float facePositions[3][3];
            float faceUVs[3][2];
            float faceNormals[3][3];

            for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
            {
                int controlPointIndex = fbxMesh->GetPolygonVertex(polygonIndex, vertexIndex);
                FbxVector4 controlPoint = fbxMesh->GetControlPointAt(controlPointIndex);

                int uvIndex = fbxMesh->GetTextureUVIndex(polygonIndex, vertexIndex);
                FbxVector2 uv = GetMeshUV(fbxMesh, controlPointIndex, fbxMesh->GetTextureUVIndex(polygonIndex, vertexIndex));

                FbxVector4 normal = GetMeshNormal(fbxMesh, controlPointIndex, vertexId);
                //FbxVector4 tangent = GetMeshTangent(fbxMesh, controlPointIndex, vertexId);
                // float uvWinding = static_cast<float> (tangent.mData[3]);

                //controlPoint = matrix.MultT(controlPoint);

                facePositions[vertexIndex][0] = static_cast<float> (controlPoint.mData[0]);
                facePositions[vertexIndex][1] = static_cast<float> (controlPoint.mData[1]);
                facePositions[vertexIndex][2] = static_cast<float> (controlPoint.mData[2]);

                faceUVs[vertexIndex][0] = static_cast<float> (uv.mData[0]);
                faceUVs[vertexIndex][1] = static_cast<float> (1.0f - uv.mData[1]);

                faceNormals[vertexIndex][0] = static_cast<float> (normal.mData[0]);
                faceNormals[vertexIndex][1] = static_cast<float> (normal.mData[1]);
                faceNormals[vertexIndex][2] = static_cast<float> (normal.mData[2]);

                vertexId += 1;
            }

            vertices.push_back({ facePositions[0], faceUVs[0], faceNormals[0] });
            vertices.push_back({ facePositions[1], faceUVs[1], faceNormals[1] });
            vertices.push_back({ facePositions[2], faceUVs[2], faceNormals[2] });
;        }
    }

    for (int childIndex = 0; childIndex < fbxNode->GetChildCount(); childIndex++)
    {
        TraverseNode(fbxNode->GetChild(childIndex), vertices);
    }
}

FbxVector2 FBXImporter::GetMeshUV(FbxMesh* fbxMesh, int controlPointIndex, int uvIndex)
{
    FbxVector2 uv;

    const FbxGeometryElementUV* fbxUV = fbxMesh->GetElementUV(0);
    const FbxLayerElement::EMappingMode uvMapping = fbxUV->GetMappingMode();
    const FbxLayerElement::EReferenceMode uvReference = fbxUV->GetReferenceMode();

    if (uvMapping == FbxGeometryElement::eByControlPoint)
    {
        if (uvReference == FbxGeometryElement::eDirect)
        {
            uv = fbxUV->GetDirectArray().GetAt(controlPointIndex);
        }
        else if (uvReference == FbxGeometryElement::eIndexToDirect)
        {
            int index = fbxUV->GetIndexArray().GetAt(controlPointIndex);
            uv = fbxUV->GetDirectArray().GetAt(index);
        }
    }
    else if (uvMapping == FbxGeometryElement::eByPolygonVertex)
    {
        uv = fbxUV->GetDirectArray().GetAt(uvIndex);
        uv = fbxUV->GetDirectArray().GetAt(uvIndex);
    }

    return uv;
}

FbxVector4 FBXImporter::GetMeshNormal(FbxMesh* fbxMesh, int controlPointIndex, int vertexId)
{
    FbxVector4 normal;

    const FbxGeometryElementNormal* fbxNormal = fbxMesh->GetElementNormal(0);
    const FbxLayerElement::EMappingMode mapping = fbxNormal->GetMappingMode();
    const FbxLayerElement::EReferenceMode Reference = fbxNormal->GetReferenceMode();

    if (mapping == FbxGeometryElement::eByControlPoint)
    {
        if (Reference == FbxGeometryElement::eDirect)
        {
            normal = fbxNormal->GetDirectArray().GetAt(controlPointIndex);
        }
        else if (Reference == FbxGeometryElement::eIndexToDirect)
        {
            int index = fbxNormal->GetIndexArray().GetAt(controlPointIndex);
            normal = fbxNormal->GetDirectArray().GetAt(index);
        }
    }
    else if (mapping == FbxGeometryElement::eByPolygonVertex)
    {
        if (Reference == FbxGeometryElement::eDirect)
        {
            normal = fbxNormal->GetDirectArray().GetAt(vertexId);
        }
        else if (Reference == FbxGeometryElement::eIndexToDirect)
        {
            int index = fbxNormal->GetIndexArray().GetAt(vertexId);
            normal = fbxNormal->GetDirectArray().GetAt(index);
        }

        normal = fbxNormal->GetDirectArray().GetAt(vertexId);
    }

    return normal;
}

FbxAMatrix FBXImporter::GetWorldMatrix(FbxNode* fbxNode)
{
    FbxAMatrix worldMatrix;
    worldMatrix.SetIdentity();

    FbxNode* parentNode = fbxNode;

    while (parentNode != nullptr)
    {
        worldMatrix = parentNode->EvaluateLocalTransform() * worldMatrix;
        parentNode = parentNode->GetParent();
    }

    FbxAMatrix geometryMatrix;
    geometryMatrix.SetT(fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot));
    geometryMatrix.SetR(fbxNode->GetGeometricRotation(FbxNode::eSourcePivot));
    geometryMatrix.SetS(fbxNode->GetGeometricScaling(FbxNode::eSourcePivot));

    return worldMatrix;
}
