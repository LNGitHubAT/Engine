cbuffer PerVertexBuffer
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInput
{
	float3 position : POSITION;
};

struct  PixelInput
{
	float4 position : SV_POSITION;
	float3 positionLocal : POSITION;
};

PixelInput main( VertexInput  input )
{
	PixelInput output;

	output.positionLocal = input.position;
	output.position = mul(float4(input.position, 1.0f), viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	
	return output;
}