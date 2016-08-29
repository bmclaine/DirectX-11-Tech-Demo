#pragma pack_matrix(row_major)

struct vInput
{
	float4 posL : POSITION;
	float4 uvwL : UV;
	float4 nrmL : NORMAL;
	float4 colL : COLOR;

	float4 xAxis : XAXIS;
	float4 yAxis : YAXIS;
	float4 zAxis : ZAXIS;
	float4 wAxis : WAXIS;
};

struct vOutput
{
	float4 uvwH : UV;
	float4 nrmH : NORMAL;
	float4 colH : COLOR;
	float4 pPos : POSITION;
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

		float4x4 worldInstance = float4x4(input.xAxis, input.yAxis, input.zAxis, input.wAxis);

		localH = mul(localH, worldInstance);
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projectionMatrix);

	output.posH = localH;
	output.pPos = mul(input.posL, worldInstance);
	output.uvwH = input.uvwL;
	output.nrmH = mul(input.nrmL, worldInstance);
	output.colH = input.colL;

	return output;
}