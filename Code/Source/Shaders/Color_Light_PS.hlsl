cbuffer light : register(b1)
{
	float4 dLightDir;
	float4 dLightCol;
	float4 dLightAmb;

	float4 pLightPos;
	float4 pLightRad;
	float4 pLightCol;
	float4 pLightAmb;

	float4 sLightPos;
	float4 sLightDir;
	float4 sLightInnerRat;
	float4 sLightOuterRat;
	float4 sLightCol;
	float4 sLightAmb;

	float4 camWorldPos;
}

float4 main(float4 uvw : UV, float4 nrm : NORMAL, float4 col : COLOR, float4 pos : SV_POSITION) : SV_TARGET
{
	nrm.xyz = normalize(nrm.xyz);

		float4 specColor = { 0.5f, 0.5f, 0.5f, 1.0f };
		float3 toCamera = normalize(camWorldPos.xyz - pos.xyz);

		// directional light
		float3 reflection = normalize(reflect(dLightDir.xyz, nrm.xyz));
		float dotResult = saturate(dot(reflection.xyz, toCamera.xyz));
	float specIntensity = saturate(pow(dotResult, 64));
	float4 finalSpec = saturate(specIntensity * specColor);

		float dLightRatio = clamp(dot(-dLightDir.xyz, nrm.xyz), dLightAmb.x, 1.0f);
	float4 dLight = dLightRatio * dLightCol * col;

		dLight = saturate(dLight + finalSpec);

	// point light
	float pLightAtten = 1.0f - clamp(length(pLightPos.xyz - pos.xyz) / pLightRad.x, 0.0f, 1.0f);
	float3 pLightDir = normalize(pLightPos.xyz - pos.xyz);

		reflection = normalize(reflect(-pLightDir.xyz, nrm.xyz));
	dotResult = saturate(dot(reflection, toCamera));
	specIntensity = saturate(pow(dotResult, 64));
	finalSpec = saturate(specIntensity * specColor);

	float pLightRatio = clamp(dot(pLightDir.xyz, nrm.xyz), pLightAmb.x, 1.0f);
	float4 pLight = pLightRatio * pLightCol * col;

		pLight = saturate(pLight + finalSpec) * pLightAtten;

	// spot light
	float sLightDistAtten = 1.0f - clamp(length(sLightPos.xyz - pos.xyz) / 40.0f, 0.0f, 1.0f);
	float3 lightDir = normalize(sLightPos.xyz - pos.xyz);

		reflection = normalize(reflect(-sLightDir.xyz, nrm.xyz));
	dotResult = saturate(dot(reflection.xyz, toCamera.xyz));
	specIntensity = saturate(pow(dotResult, 64));
	finalSpec = saturate(specIntensity * specColor);

	float surfaceRatio = clamp(dot(-lightDir.xyz, sLightDir.xyz), 0.0f, 1.0f);
	float sLightConeAtten = 1.0f - clamp((sLightInnerRat.x - surfaceRatio) / (sLightInnerRat.x - sLightOuterRat.x), 0.0f, 1.0f);
	float spotFactor = (surfaceRatio > sLightOuterRat.x) ? 1 : 0;
	float sLightRatio = clamp(dot(lightDir.xyz, nrm.xyz), sLightAmb.x, 1.0f);
	float4 sLight = spotFactor * sLightRatio * sLightCol * col;

		sLight = saturate(sLight + finalSpec) * sLightDistAtten * sLightConeAtten;

	return saturate(dLight + pLight + sLight);
}