
cbuffer PerVertexBuffer
{
	float4x4 wolrdMatrix;
};

struct VertexIn
{
	float3 position : POSITION;
	float2 textureCoordinate : TEXCOORD;
	float2 yBoundary : TEXCOORD1;
};



struct VertexOut
{
	float3 position : POSITION;
	float3 positionW : POSITION1;
	float2 textureCoordinate : TEXCOORD;
	float2 yBoundary : TEXCOORD1;
};

Texture2D heightMap : register( t2 );
SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};



VertexOut main ( VertexIn vin )
{
	VertexOut vout;

	vout.position = vin.position;

	// height map is gray scale
	vout.position.y = heightMap.SampleLevel(samHeightmap, vin.textureCoordinate, 0 ).r;

	//vout.position.y = -2.5f;

	vout.positionW = mul(float4(vout.position, 1.0f), wolrdMatrix).xyz;

	vout.textureCoordinate = vin.textureCoordinate;

	vout.yBoundary = vin.yBoundary;

	return vout;
}
