// Globals
// [중요]셰이더의 효율적인 실행과 그래픽카드가 어떻게 이 버퍼를 저장하는지와 관련
// vs와 ps를 합쳐 fx(한 파일)로 처리 가능
cbuffer MatrixBuffer
{
	matrix worldMatrix;			// 월드
	matrix viewMatrix;			// 카메라
	matrix projectionMatrix;	// 투영
};

// Type definitions
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// Vertex Shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	// x, y, z좌표만 사용
	// w = 0일 경우 방향벡터로 사용, w = 1일 경우 위치벡터로 사용
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	// 3D 세계에서의 정점을 카메라 시야에 맞는 2D화면의 위치로 지정
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the input color for the pixel shader to use.
	output.color = input.color;

	return output;
}