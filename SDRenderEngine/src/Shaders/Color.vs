// GLOBALS
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewmatrix;
	matrix projectionMatrix;
};

// TypeDefs
struct VertexInputType
{
	float4 position:POSITION;
	float4 color: COLOR;
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

// Vertex Shader
PixelInputType colorVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix );
	output.position = mul( output.position, projectionmatrix );

	output.color = input.color;
	return output;
};