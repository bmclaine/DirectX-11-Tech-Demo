#ifndef _SHADER_H_
#define _SHADER_H_

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "Light.h"

enum VertexShaderType
{
	VSSkyboxShader = 0,
	VSMatrixShader = 1,
	VSInstanceShader = 2,
	VSPointToQuadShader = 3,
	VSPostProcessShader = 4,
};

enum PixelShaderType
{
	PSSkyboxShader = 0,
	PSColorShader = 1,
	PSColorLightShader = 2,
	PSTextureShader = 3,
	PSTextureLightShader = 4,
	PSPostProcessShader = 5,
};

class Shader
{
private:
	struct MatrixToShader
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
	};

	struct LightToShader
	{
		XMFLOAT4 dLightDirection;
		XMFLOAT4 dLightColor;
		XMFLOAT4 dLightAmbience;

		XMFLOAT4 pLightPosition;
		XMFLOAT4 pLightRadius;
		XMFLOAT4 pLightColor;
		XMFLOAT4 pLightAmbience;

		XMFLOAT4 sLightPosition;
		XMFLOAT4 sLightDirection;
		XMFLOAT4 sLightInnerConeRatio;
		XMFLOAT4 sLightOuterConeRatio;
		XMFLOAT4 sLightColor;
		XMFLOAT4 sLightAmbience;

		XMFLOAT4 camWorldPos;
	};

	struct TimeToShader
	{
		XMFLOAT4 timeStep;
	};

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11GeometryShader* pGeometryShader;
	ID3D11InputLayout* pInputLayout;
	ID3D11Buffer* pConstantBuffers[2];

	VertexShaderType m_vsType;
	PixelShaderType m_psType;

public:


public:
	Shader();
	Shader(const Shader&);
	~Shader();

	bool Initialize(ID3D11Device* device, VertexShaderType vsType, PixelShaderType psType);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, Light* dLight, Light* pLight, Light* sLight, XMFLOAT4 camWorldPos, XMFLOAT4 timeStep);

private:
	bool InitializeShaders(ID3D11Device* device, VertexShaderType vsType, PixelShaderType psType);
	void ShutdownShaders();
};

#endif