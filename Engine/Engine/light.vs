// 셰이더: 실제 모델의 렌더링을 수행하는, HLSL로 작성된 프로그램
// texture.vs: 텍스쳐 씌우기 추가

#define NUM_LIGHTS 3

// Globals
// [중요]셰이더의 효율적인 실행 + 그래픽카드가 어떻게 이 버퍼들을 저장하는지와 관련
// 매 프레임마다 같이 업데이트-> 같은 버퍼 내에 삽입
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

cbuffer LightPositionBuffer
{
	float4 lightPosition[NUM_LIGHTS];
};

// 픽셀 셰이더
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 lightPos2 : TEXCOORD2;
	float3 lightPos3 : TEXCOORD3;
	float3 lightPos4 : TEXCOORD4;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
	// 올바른 행렬 연산을 위해 position벡터를 w까지 있는 4성분이 있는 것으로 사용
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	// 정점의 위치를 월드, 뷰, 사영의 순으로 계산
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	// 픽셀 셰이더에서 사용하기 위해 텍스쳐 좌표를 저장
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
	
	output.lightPos2.xyz = lightPosition[0].xyz - worldPosition.xyz;
	output.lightPos3.xyz = lightPosition[1].xyz - worldPosition.xyz;
	output.lightPos4.xyz = lightPosition[2].xyz - worldPosition.xyz;
	
	output.lightPos2 = normalize(output.lightPos2);
	output.lightPos3 = normalize(output.lightPos3);
	output.lightPos4 = normalize(output.lightPos4);

	return output;
}