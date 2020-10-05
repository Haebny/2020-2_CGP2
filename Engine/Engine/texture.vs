// 셰이더: 실제 모델의 렌더링을 수행하는, HLSL로 작성된 프로그램
// texture.vs: 텍스쳐 씌우기 추가

// Globals
// [중요]셰이더의 효율적인 실행 + 그래픽카드가 어떻게 이 버퍼들을 저장하는지와 관련
// 매 프레임마다 같이 업데이트-> 같은 버퍼 내에 삽입
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// Type definitions
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;		// 텍스트용 data
};

// 픽셀 셰이더
struct PixelInputType
{
	float4 position : SV_POSITION;		// 픽셀 셰이더에서 동작
	float2 tex : TEXCOORD0;
};

// Vertex Shader
// 정점 셰이더: 정점 버퍼의 데이터 처리를 위해 GPU가 호출
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

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

	return output;
}