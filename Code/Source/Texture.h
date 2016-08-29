#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>

class Texture
{
private:
	ID3D11Resource* pTexture2D;
	ID3D11ShaderResourceView* pShaderResourceView;
	ID3D11ShaderResourceView* pAltShaderResourceView;
	ID3D11SamplerState* pSamplerState;

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device* device, WCHAR* filename);
	bool InitializeMulti(ID3D11Device* device, WCHAR* fileName, WCHAR* altFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	void RenderMulti(ID3D11DeviceContext* deviceContext);
};

#endif