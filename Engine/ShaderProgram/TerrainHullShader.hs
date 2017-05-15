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
	float4 frustumPlanes[6];
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

	

	float d = distance(position, cameraPosition);
	
	float s = saturate( (d - minDist) / (maxDist - minDist) );

	float number = (lerp(maxTess , minTess , s));

	if(number > 0)
	{
		return pow(2, number);
	}
	else return 1;


}

bool AABBBehindPlane(float3 centerAABB, float3 extents, float4 plane)
{
	// plane.xyz is plane normal
	float3 n = abs ( plane.xyz );
	float r = dot(extents, n);

	// signed distance between center and plane

	float d = dot(float4(centerAABB, 1.0f), plane);

	return (r + d) < 0.0f;	
}

bool AABBoutSideFrustum(float3 centerAABB, float3 extents, float4 frustumPlanes[6])
{
	for	(int i = 0; i < 6; i ++)
	{
		if( AABBBehindPlane(centerAABB, extents,frustumPlanes[i]) )
			return true;
	}
	return false;
}

PatchTesselationFactor ConstantHS( InputPatch<VertexOut , 4> patch , uint patchID : SV_PrimitiveID )
{
	PatchTesselationFactor factor;

	// TO DO : frustum culling


	float yMax = patch[0].yBoundary.y;
	float yMin = patch[0].yBoundary.x;

	//float yMax = 50;
	//..,,float yMin = 0.0f;

	float3 vMin = float3(patch[2].positionW.x, yMin, patch[2].positionW.z);
	float3 vMax = float3(patch[1].positionW.x, yMax, patch[1].positionW.z);

	float3 centerAABB = 0.5f * ( vMin + vMax);

	float3 extents = 0.5f * ( vMax - vMin);

	if(AABBoutSideFrustum(centerAABB, extents, frustumPlanes))
	{
		factor.EdgeTess[0] = 0;
		factor.EdgeTess[1] = 0;
		factor.EdgeTess[2] = 0;
		factor.EdgeTess[3] = 0;

		factor.InsideTess[0] = 0;
		factor.InsideTess[1] = 0;
		return factor;
	}
	else
	{
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
