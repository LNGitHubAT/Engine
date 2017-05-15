cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix worldInverseTraposeMatrix;
};


struct VertexInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 textCoord : TEXCOORD;
};

struct  PixelInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 positionL : POSITION1;
	float3 normal : NORMAL;
	float2 textCoord : TEXCOORD;
};

PixelInput main( VertexInput  input )
{
	PixelInput output;

	

	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.positionL = input.position;
	
	output.positionW = mul(float4(input.position, 1.0f), worldMatrix).xyz;

	output.normal = mul(input.normal, (float3x3)worldInverseTraposeMatrix);

	output.textCoord = input.textCoord;

	return output;
}