
cbuffer matrixBuffer
{
	matrix viewProjectionMatrix;
	float3 camposition;
	float pad;
};

struct GSInput
{
	float3 centerW : POSITION;
	float2 sizeW : SIZE;
};

struct PixelInput
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 textureCoord : TEXTCOORD;
	uint primID : SV_PrimitiveID;
};

[maxvertexcount(4)]
void main(point GSInput input[1], uint primID : SV_PrimitiveID, inout TriangleStream<PixelInput> triangleStream)
{

	float2 textureCoord[4] =
	{
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f),
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f)
	};

	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = camposition - input[0].centerW;
	look.y = 0.0f;
	look = normalize(look);
	float3 right = cross(up, look);

	float halfWidth = 0.5f * input[0].sizeW.x;
	float halfHeight = 0.5f * input[0].sizeW.y;
	
	float4 v[4];
	v[0] = float4(input[0].centerW - halfWidth*right + halfHeight*up, 1.0f);
	v[1] = float4(input[0].centerW + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(input[0].centerW - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(input[0].centerW + halfWidth*right - halfHeight*up, 1.0f);


	//v[0] = float4(input[0].centerW + halfWidth*right - halfHeight*up, 1.0f);
	//v[1] = float4(input[0].centerW + halfWidth*right + halfHeight*up, 1.0f);
	//v[2] = float4(input[0].centerW - halfWidth*right - halfHeight*up, 1.0f);
	//v[3] = float4(input[0].centerW - halfWidth*right + halfHeight*up, 1.0f);

	float4 vertices[3];
	vertices[0] = float4(-1.0f, -1.0f, 0.0f,1.0f);  // Bottom left.
	//vertices[0] = float4(0.0f, 0.0f, -1.0f,1.0f);

	vertices[1] = float4(0.0f, 1.0f, 0.0f,1.0f);  // Top middle.
	//vertices[1] = float4(0.0f, 0.0f, -1.0f,1.0f);

	vertices[2] = float4(1.0f, -1.0f, 0.0f,1.0f);  // Bottom right.
	//vertices[2] = float4(0.0f, 0.0f, -1.0f,1.0f);








	PixelInput pout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		pout.posH = mul(v[i], viewProjectionMatrix);
		pout.posW = v[i].xyz;
		pout.normalW = look;
		pout.textureCoord = textureCoord[i];
		pout.primID = primID;
		triangleStream.Append(pout);
	}
}
