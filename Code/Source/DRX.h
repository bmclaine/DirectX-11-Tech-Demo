#ifndef _DRX_H_
#define _DRX_H_

#include <d3d11.h>
#pragma comment(lib, "D3D11.Lib")

#include <DirectXMath.h>
using namespace DirectX;

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f
#define VERTICAL_FOV 65.0f
#define BACKBUFFER_WIDTH 1024.0f
#define BACKBUFFER_HEIGHT 768.0f

#define SAFE_RELEASE(ptr) if((ptr)){ (ptr)->Release(); (ptr) = NULL; }

class DRX
{
private:
	IDXGISwapChain* pSwapChain;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11Texture2D* pDepthStencilBuffer;
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11RasterizerState* pRasterizerStateBackCull;
	ID3D11RasterizerState* pRasterizerStateFrontCull;
	ID3D11BlendState* pBlendState;
	D3D11_VIEWPORT viewPort;

	XMFLOAT4X4 m_ProjectionMatrix;

public:
	DRX();
	DRX(const DRX&);
	~DRX();

	bool Initialize(HWND hwnd);
	void Shutdown();

	void ResetScene(float red, float green, float blue, float alpha);
	void ClearDepthStencilView();
	void PresentScene();
	void ResizeWindow();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11RasterizerState* GetRasterStateBackCull();
	ID3D11RasterizerState* GetRasterStateFrontCull();
	ID3D11DepthStencilView* GetDepthStencilView();
	D3D11_TEXTURE2D_DESC GetWindowDesc();

	void ResetRenderTargetView();

	XMFLOAT4X4 GetProjectionMatrix();

	void SetViewPort();
	void SetBlendState();
};

#endif