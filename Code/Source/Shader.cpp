#include "Shader.h"
#include "DRX.h"

#include "Shaders\Skybox_VS.csh"
#include "Shaders\Matrix_VS.csh"
#include "Shaders\Instanced_VS.csh"
#include "Shaders\Point_to_Quad_VS.csh"
#include "Shaders\PostProcess_VS.csh"

#include "Shaders\Skybox_PS.csh"
#include "Shaders\Color_PS.csh"
#include "Shaders\Color_Light_PS.csh"
#include "Shaders\Texture_PS.csh"
#include "Shaders\Texture_Light_PS.csh"
#include "Shaders\PostProcess_PS.csh"

#include "Shaders\Point_To_Quad_GS.csh"

Shader::Shader()
{
	pVertexShader = NULL;
	pPixelShader = NULL;
	pGeometryShader = NULL;
	pInputLayout = NULL;
	pConstantBuffers[0] = NULL;
	pConstantBuffers[1] = NULL;
}

Shader::Shader(const Shader&)
{

}

Shader::~Shader()
{

}

bool Shader::Initialize(ID3D11Device* device, VertexShaderType vsType, PixelShaderType psType)
{
	if (!InitializeShaders(device, vsType, psType)) return false;

	return true;
}

void Shader::Shutdown()
{
	ShutdownShaders();

	return;
}

void Shader::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, Light* dLight, Light* pLight, Light* sLight, XMFLOAT4 camWorldPos, XMFLOAT4 timeStep)
{
	MatrixToShader matrixToShader;
	matrixToShader.worldMatrix = worldMatrix;
	matrixToShader.viewMatrix = viewMatrix;
	matrixToShader.projectionMatrix = projectionMatrix;

	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(resource));

	deviceContext->Map(pConstantBuffers[0], 0, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
	memcpy_s(resource.pData, sizeof(MatrixToShader), &matrixToShader, sizeof(matrixToShader));
	deviceContext->Unmap(pConstantBuffers[0], 0);

	switch (m_vsType)
	{
	case VSSkyboxShader:
	{
		deviceContext->VSSetConstantBuffers(0, 1, pConstantBuffers);
	}
	break;
	case VSMatrixShader:
	{
		deviceContext->VSSetConstantBuffers(0, 1, pConstantBuffers);
	}
	break;
	case VSInstanceShader:
	{
		deviceContext->VSSetConstantBuffers(0, 1, pConstantBuffers);
	}
	break;
	case VSPointToQuadShader:
	{
		deviceContext->GSSetShader(pGeometryShader, NULL, NULL);
		deviceContext->GSSetConstantBuffers(0, 1, pConstantBuffers);
	}
	break;
	case VSPostProcessShader:
	{
		//deviceContext->VSSetConstantBuffers(0, 1, pConstantBuffers);
	}
	break;
	default:
	{

	}
	break;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (m_psType)
	{
	case PSColorShader:
	{

	}
	break;
	case PSColorLightShader:
	{
		LightToShader lightToShader;
		lightToShader.dLightDirection = dLight->GetDirection();
		lightToShader.dLightColor = dLight->GetColor();
		lightToShader.dLightAmbience = dLight->GetAmbience();

		lightToShader.pLightPosition = pLight->GetPosition();
		lightToShader.pLightRadius = pLight->GetRadius();
		lightToShader.pLightColor = pLight->GetColor();
		lightToShader.pLightAmbience = pLight->GetAmbience();

		lightToShader.sLightPosition = sLight->GetPosition();
		lightToShader.sLightDirection = sLight->GetDirection();
		lightToShader.sLightInnerConeRatio = sLight->GetInnerConeRatio();
		lightToShader.sLightOuterConeRatio = sLight->GetOuterConeRatio();
		lightToShader.sLightColor = sLight->GetColor();
		lightToShader.sLightAmbience = sLight->GetAmbience();

		lightToShader.camWorldPos = camWorldPos;

		deviceContext->Map(pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
		memcpy_s(resource.pData, sizeof(LightToShader), &lightToShader, sizeof(lightToShader));
		deviceContext->Unmap(pConstantBuffers[1], 0);

		deviceContext->PSSetConstantBuffers(0, 2, pConstantBuffers);
	}
	break;
	case PSTextureShader:
	{

	}
	break;
	case PSTextureLightShader:
	{
		LightToShader lightToShader;
		lightToShader.dLightDirection = dLight->GetDirection();
		lightToShader.dLightColor = dLight->GetColor();
		lightToShader.dLightAmbience = dLight->GetAmbience();

		lightToShader.pLightPosition = pLight->GetPosition();
		lightToShader.pLightRadius = pLight->GetRadius();
		lightToShader.pLightColor = pLight->GetColor();
		lightToShader.pLightAmbience = pLight->GetAmbience();

		lightToShader.sLightPosition = sLight->GetPosition();
		lightToShader.sLightDirection = sLight->GetDirection();
		lightToShader.sLightInnerConeRatio = sLight->GetInnerConeRatio();
		lightToShader.sLightOuterConeRatio = sLight->GetOuterConeRatio();
		lightToShader.sLightColor = sLight->GetColor();
		lightToShader.sLightAmbience = sLight->GetAmbience();

		lightToShader.camWorldPos = camWorldPos;

		deviceContext->Map(pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
		memcpy_s(resource.pData, sizeof(LightToShader), &lightToShader, sizeof(lightToShader));
		deviceContext->Unmap(pConstantBuffers[1], 0);

		deviceContext->PSSetConstantBuffers(0, 2, pConstantBuffers);
	}
	break;
	case PSPostProcessShader:
	{
		TimeToShader timeToShader;
		timeToShader.timeStep = timeStep;

		deviceContext->Map(pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
		memcpy_s(resource.pData, sizeof(TimeToShader), &timeToShader, sizeof(timeToShader));
		deviceContext->Unmap(pConstantBuffers[1], 0);

		deviceContext->PSSetConstantBuffers(0, 2, pConstantBuffers);
	}
	break;
	default:
	{

	}
	break;
	}

	deviceContext->VSSetShader(pVertexShader, NULL, NULL);

	deviceContext->PSSetShader(pPixelShader, NULL, NULL);

	deviceContext->IASetInputLayout(pInputLayout);

	return;
}

bool Shader::InitializeShaders(ID3D11Device* device, VertexShaderType vsType, PixelShaderType psType)
{
	HRESULT hResult;
	D3D11_BUFFER_DESC constantBufferDesc;

	m_vsType = vsType;
	m_psType = psType;

	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	switch (vsType)
	{
	case VSSkyboxShader:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hResult = device->CreateVertexShader(&Skybox_VS, sizeof(Skybox_VS), NULL, &pVertexShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(MatrixToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[0]);
		if (FAILED(hResult)) return false;

		hResult = device->CreateInputLayout(inputElementDesc, 4, Skybox_VS, sizeof(Skybox_VS), &pInputLayout);
		if (FAILED(hResult)) return false;
	}
	break;
	case VSMatrixShader:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hResult = device->CreateVertexShader(&Matrix_VS, sizeof(Matrix_VS), NULL, &pVertexShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(MatrixToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[0]);
		if (FAILED(hResult)) return false;

		hResult = device->CreateInputLayout(inputElementDesc, 4, Matrix_VS, sizeof(Matrix_VS), &pInputLayout);
		if (FAILED(hResult)) return false;
	}
	break;
	case VSInstanceShader:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "XAXIS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "YAXIS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ZAXIS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WAXIS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};

		hResult = device->CreateVertexShader(&Instanced_VS, sizeof(Instanced_VS), NULL, &pVertexShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(MatrixToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[0]);
		if (FAILED(hResult)) return false;

		hResult = device->CreateInputLayout(inputElementDesc, 8, Instanced_VS, sizeof(Instanced_VS), &pInputLayout);
		if (FAILED(hResult)) return false;
	}
	break;
	case VSPointToQuadShader:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hResult = device->CreateVertexShader(&Point_To_Quad_VS, sizeof(Point_To_Quad_VS), NULL, &pVertexShader);
		if (FAILED(hResult)) return false;

		hResult = device->CreateGeometryShader(&Point_To_Quad_GS, sizeof(Point_To_Quad_GS), NULL, &pGeometryShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(MatrixToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[0]);
		if (FAILED(hResult)) return false;

		hResult = device->CreateInputLayout(inputElementDesc, 1, Point_To_Quad_VS, sizeof(Point_To_Quad_VS), &pInputLayout);
		if (FAILED(hResult)) return false;
	}
	break;
	case VSPostProcessShader:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hResult = device->CreateVertexShader(&PostProcess_VS, sizeof(PostProcess_VS), NULL, &pVertexShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(MatrixToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[0]);
		if (FAILED(hResult)) return false;

		hResult = device->CreateInputLayout(inputElementDesc, 2, PostProcess_VS, sizeof(PostProcess_VS), &pInputLayout);
		if (FAILED(hResult)) return false;
	}
	break;
	default:
	{

	}
	break;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (psType)
	{
	case PSSkyboxShader:
	{
		hResult = device->CreatePixelShader(&Skybox_PS, sizeof(Skybox_PS), NULL, &pPixelShader);
		if (FAILED(hResult)) return false;
	}
	break;
	case PSColorShader:
	{
		hResult = device->CreatePixelShader(&Color_PS, sizeof(Color_PS), NULL, &pPixelShader);
		if (FAILED(hResult)) return false;
	}
	break;
	case PSColorLightShader:
	{
		hResult = device->CreatePixelShader(&Color_Light_PS, sizeof(Color_Light_PS), NULL, &pPixelShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(LightToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[1]);
		if (FAILED(hResult)) return false;
	}
	break;
	case PSTextureShader:
	{
		hResult = device->CreatePixelShader(&Texture_PS, sizeof(Texture_PS), NULL, &pPixelShader);
		if (FAILED(hResult)) return false;
	}
	break;
	case PSTextureLightShader:
	{
		hResult = device->CreatePixelShader(&Texture_Light_PS, sizeof(Texture_Light_PS), NULL, &pPixelShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(LightToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[1]);
		if (FAILED(hResult)) return false;
	}
	break;
	case PSPostProcessShader:
	{
		hResult = device->CreatePixelShader(&PostProcess_PS, sizeof(PostProcess_PS), NULL, &pPixelShader);
		if (FAILED(hResult)) return false;

		constantBufferDesc.ByteWidth = sizeof(TimeToShader);
		hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffers[1]);
		if (FAILED(hResult)) return false;
	}
	break;
	default:
	{

	}
	break;
	}

	return true;
}

void Shader::ShutdownShaders()
{
	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pGeometryShader);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pConstantBuffers[0]);
	SAFE_RELEASE(pConstantBuffers[1]);
}
