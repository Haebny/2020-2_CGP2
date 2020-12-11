// Globals
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

	// can change the zero to any number to indicate which set of coordinates you are working with as multiple texture coordinates are allowed.
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex Shader
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	// 복사한 텍스쳐 좌표를 픽셀 셰이더에게 전달
	output.tex = input.tex;

	return output;
}