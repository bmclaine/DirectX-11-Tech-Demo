textureCUBE baseTexture : register(t0);
textureCUBE altTexture : register(t1);

SamplerState filter : register(s0);

float4 main(float4 uvw : UV, float4 nrm : NORMAL, float4 col : COLOR) : SV_TARGET
{
	float4 textureColor = baseTexture.Sample(filter, uvw.xyz);
	float4 altTextureColor = altTexture.Sample(filter, uvw.xyz);

	float4 finalColor = float4(lerp(textureColor.xyz, altTextureColor.xyz, 0.5f), 1.0f);

	return finalColor;
}