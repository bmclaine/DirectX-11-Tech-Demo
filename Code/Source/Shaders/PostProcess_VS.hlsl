struct vInput
{
	float4 posL : POSITION;
	float4 uvwL : UV;
};

struct vOutput
{
	float4 uvwH : UV;
	float4 posH : SV_POSITION;
};

vOutput main(vInput input)
{
	vOutput output = (vOutput)0;

	output.posH = input.posL;
	output.uvwH = input.uvwL;

	return output;
}