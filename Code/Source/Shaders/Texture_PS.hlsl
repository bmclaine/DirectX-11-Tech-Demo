texture2D baseTexture : register(t0);

SamplerState filter : register(s0);

float4 main(float4 uvw : UV, float4 nrm : NORMAL, float4 col : COLOR) : SV_TARGET
{
	float4 textureColor = baseTexture.Sample(filter, uvw.xy);

	return textureColor;
}