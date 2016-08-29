#pragma pack_matrix(row_major)

struct vInput
{
	float4 posL : POSITION;
	float4 uvwL : UV;
	float4 nrmL : NORMAL;
	float4 colL : COLOR;
};

struct vOutput
{
	float4 uvwH : UV;
	float4 nrmH : NORMAL;
	float4 colH : COLOR;
	float4 posH : POSITION;
	float4 svPos : SV_POSITION;
};

cbuffer Matrix : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

vOutput main( vInput input)
{
	vOutput output = (vOutput)0;

	float4 localH = input.posL;

	localH = mul(localH, worldMatrix);
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projectionMatrix);

	output.svPos = localH;
	output.posH = mul(input.posL, worldMatrix);
	output.uvwH = input.uvwL;
	output.nrmH = mul(input.nrmL, worldMatrix);
	output.colH = input.colL;

	return output;
}