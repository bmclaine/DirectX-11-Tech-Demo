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
	float4 posH : SV_POSITION;
};

cbuffer Matrix : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

vOutput main(vInput input)
{
	vOutput output = (vOutput)0;

	float4 localH = input.posL;

		localH = mul(localH, worldMatrix);

	float4x4 tempView = viewMatrix;
//	tempView[0][3] = 0.0f;
//	tempView[1][3] = 0.0f;
//	tempView[2][3] = 0.0f;

	localH = mul(localH, tempView);
	localH = mul(localH, projectionMatrix);

	output.posH = localH;
	output.uvwH = input.posL;
	output.nrmH = input.nrmL;
	output.colH = input.colL;

	return output;
}