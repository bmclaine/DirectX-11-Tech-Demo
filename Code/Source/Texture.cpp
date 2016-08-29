#include "Texture.h"

#include "DRX.h"
#include "../Utilities/DDSTextureLoader.h"

Texture::Texture()
{
	pTexture2D = NULL;
	pShaderResourceView = NULL;
	pAltShaderResourceView = NULL;
	pSamplerState = NULL;
}

Texture::Texture(const Texture&)
{

}

Texture::~Texture()
{

}

bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT hResult;
	D3D11_SAMPLER_DESC samplerStateDesc;

	ZeroMemory(&samplerStateDesc, sizeof(samplerStateDesc));
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.MinLOD = 0.0f;
	samplerStateDesc.MaxLOD = 1.0;
	samplerStateDesc.MipLODBias = 0.0f;
	samplerStateDesc.MaxAnisotropy = 1;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStateDesc.BorderColor[0] = 1.0f;
	samplerStateDesc.BorderColor[1] = 1.0f;
	samplerStateDesc.BorderColor[2] = 1.0f;
	samplerStateDesc.BorderColor[3] = 1.0f;

	hResult = device->CreateSamplerState(&samplerStateDesc, &pSamplerState);
	if (FAILED(hResult)) return false;

	hResult = CreateDDSTextureFromFile(device, filename, &pTexture2D, &pShaderResourceView, 0);
	if (FAILED(hResult)) return false;
	
	return true;
}

bool Texture::InitializeMulti(ID3D11Device* device, WCHAR* filename, WCHAR* altFilename)
{
	HRESULT hResult;
	D3D11_SAMPLER_DESC samplerStateDesc;

	ZeroMemory(&samplerStateDesc, sizeof(samplerStateDesc));
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.MinLOD = 0.0f;
	samplerStateDesc.MaxLOD = 1.0;
	samplerStateDesc.MipLODBias = 0.0f;
	samplerStateDesc.MaxAnisotropy = 1;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStateDesc.BorderColor[0] = 1.0f;
	samplerStateDesc.BorderColor[1] = 1.0f;
	samplerStateDesc.BorderColor[2] = 1.0f;
	samplerStateDesc.BorderColor[3] = 1.0f;

	hResult = device->CreateSamplerState(&samplerStateDesc, &pSamplerState);
	if (FAILED(hResult)) return false;

	hResult = CreateDDSTextureFromFile(device, filename, &pTexture2D, &pShaderResourceView, 0);
	if (FAILED(hResult)) return false;
	SAFE_RELEASE(pTexture2D);

	hResult = CreateDDSTextureFromFile(device, altFilename, &pTexture2D, &pAltShaderResourceView, 0);
	if (FAILED(hResult)) return false;

	return true;
}

void Texture::Shutdown()
{
	SAFE_RELEASE(pTexture2D);
	SAFE_RELEASE(pShaderResourceView);
	SAFE_RELEASE(pAltShaderResourceView);
	SAFE_RELEASE(pSamplerState);
}

void Texture::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);

	deviceContext->PSSetSamplers(0, 1, &pSamplerState);
}

void Texture::RenderMulti(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);

	deviceContext->PSSetShaderResources(1, 1, &pAltShaderResourceView);

	deviceContext->PSSetSamplers(0, 1, &pSamplerState);
}
