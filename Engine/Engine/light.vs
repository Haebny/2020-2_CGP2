// ���̴�: ���� ���� �������� �����ϴ�, HLSL�� �ۼ��� ���α׷�
// texture.vs: �ؽ��� ����� �߰�

#define NUM_LIGHTS 4

// Globals
// [�߿�]���̴��� ȿ������ ���� + �׷���ī�尡 ��� �� ���۵��� �����ϴ����� ����
// �� �����Ӹ��� ���� ������Ʈ-> ���� ���� ���� ����
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightPositionBuffer
{
	float4 lightPositions[NUM_LIGHTS];
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

// Type definitions
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;		// �ؽ�Ʈ�� data
	float3 normal : NORMAL;
};

// �ȼ� ���̴�
struct PixelInputType
{
	float4 position : SV_POSITION;		// �ȼ� ���̴����� ����
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 lightPos1 : TEXCOORD2;
	float3 lightPos2 : TEXCOORD3;
	float3 lightPos3 : TEXCOORD4;
	float3 lightPos4 : TEXCOORD5;
};

// Vertex Shader
// ���� ���̴�: ���� ������ ������ ó���� ���� GPU�� ȣ��
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
	// �ùٸ� ��� ������ ���� position���͸� w���� �ִ� 4������ �ִ� ������ ���
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	// ������ ��ġ�� ����, ��, �翵�� ������ ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	// �ȼ� ���̴����� ����ϱ� ���� �ؽ��� ��ǥ�� ����
	output.tex = input.tex; 

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	// Determine the light positions based on the position of the lights and the position of the vertex in the world.
	output.lightPos1.xyz = lightPositions[0].xyz - worldPosition.xyz;
	output.lightPos2.xyz = lightPositions[1].xyz - worldPosition.xyz;
	output.lightPos3.xyz = lightPositions[2].xyz - worldPosition.xyz;
	output.lightPos4.xyz = lightPositions[3].xyz - worldPosition.xyz;
	
	// Normalize the light position vectors.
	output.lightPos1 = normalize(output.lightPos1);
	output.lightPos2 = normalize(output.lightPos2);
	output.lightPos3 = normalize(output.lightPos3);
	output.lightPos4 = normalize(output.lightPos4);

	return output;
}