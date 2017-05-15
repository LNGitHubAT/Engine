struct VertexInput
{
	float3 posW : POSITION;
	float2 sizeW : SIZE;
};

struct GSInput
{
	float3 centerW : POSITION;
	float2 sizeW : SIZE;
};
GSInput main(VertexInput input)
{
	GSInput output;
	output.centerW = input.posW;
	output.sizeW = input.sizeW;
	return output;
}