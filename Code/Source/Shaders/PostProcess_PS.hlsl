texture2D baseTexture : register(t0);

SamplerState filter : register(s0);

cbuffer time : register(b1)
{
	float4 timeStep;
}

float4 main(float4 uvw : UV) : SV_TARGET
{
	uvw.y += (sin(timeStep.x + uvw.x) * 0.5f);

	float4 textureColor = baseTexture.Sample(filter, uvw.xy);

	return textureColor;
}