 // Hull shader
cbuffer PerPatchBuffer
{
	// Tesselation depends on distant between point and camera
	float3 cameraPosition;
	float minDist;
	float maxDist;
	float minTess;
	float maxTess;
	float pad;
};
struct VertexOut
{
	float3 position : POSITION;
	float3 positionW : POSITION1;
	float2 textureCoordinate : TEXCOORD;
	float2 yBoundary : TEXCOORD1;
};

struct PatchTesselationFactor
{
	float EdgeTess[4] :  SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

float calculateTessFactor(float3 position)
{
	//float result;
	
	//float distance = length(position - cameraPosition);

	//float lerpValue = saturate( (distance - minDist) / (maxDist - minDist) );
	//lerpValue = 1.0f;
	// hard code max tesselation = 64 = 2 ^ 6. min = 1 = 2 ^ 0
	//result  = pow(2.0f , (lerp( maxTess , minTess , lerpValue)) );
	//result  = pow(2.0f, 6.0f);

	//result = 3;
	//result  = 64.0f;
	//return result;




	float d = distance(position, cameraPosition);
	
	float s = saturate( (d - minDist) / (maxDist - minDist) );

	float number = (lerp(maxTess , minTess , s));

	if(number > 0)
	{
		return pow(2, number);
	}
	else return 1;


}

PatchTesselationFactor ConstantHS( InputPatch<VertexOut , 4> patch , uint patchID : SV_PrimitiveID )
{
	PatchTesselationFactor factor;

	// TO DO : frustum culling

	// Compute mid point of every edge
	float3 edgesMidPointPosition[4];

	float3 centerPoint = 0.25f *(patch[0].positionW + patch[1].positionW + patch[2].positionW + patch[3].positionW);
	
	edgesMidPointPosition[0] = 0.5f*(patch[0].positionW + patch[2].positionW);
	edgesMidPointPosition[1] = 0.5f*(patch[0].positionW + patch[1].positionW);
	edgesMidPointPosition[2] = 0.5f*(patch[1].positionW + patch[3].positionW);
	edgesMidPointPosition[3] = 0.5f*(patch[2].positionW + patch[3].positionW);

	for(int i = 0; i < 4; i++)
	{
		factor.EdgeTess[i] = calculateTessFactor(edgesMidPointPosition[i]);
	}

	factor.InsideTess[0] = calculateTessFactor(centerPoint);
	factor.InsideTess[1] = factor.InsideTess[0];

	return factor;
}



struct HullOut
{
	float3 position : POSITION;
	float3 positionW     : POSITION1;
	float2 textureCoordinate : TEXCOORD0;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]

// process per output control point regardless tess factor
HullOut main ( InputPatch<VertexOut, 4> patch, 
				uint controlOutPutID : SV_OutputControlPointID,
				uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	hout.position = patch[controlOutPutID].position;
	hout.positionW = patch[controlOutPutID].positionW;
	hout.textureCoordinate = patch[controlOutPutID].textureCoordinate;

	return hout;
}
