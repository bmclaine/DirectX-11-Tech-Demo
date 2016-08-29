struct vInput
{
	float4 posL : POSITION;
};

struct vOutput
{
	float4 svPos : SV_POSITION;
};

vOutput main(vInput input)
{
	vOutput output = (vOutput)0;

	output.svPos = input.posL;

	return output;
}