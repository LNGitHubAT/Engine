
struct VertexInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 textCoord : TEXCOORD;
};

struct  PixelInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 textCoord : TEXCOORD;
};

PixelInput main( VertexInput  input )
{
	PixelInput output;

	output.position = input.position;

	output.normal = input.normal ;

	output.textCoord = input.textCoord;

	return output;
}