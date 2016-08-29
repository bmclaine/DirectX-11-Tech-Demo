#include "DRX.h"
extern DRX* gD3d;

DRX::DRX()
{
	pSwapChain = NULL;
	pDevice = NULL;
	pDeviceContext = NULL;
	pRenderTargetView = NULL;
	pDepthStencilBuffer = NULL;
	pDepthStencilState = NULL;
	pDepthStencilView = NULL;
	pRasterizerStateBackCull = NULL;
	pRasterizerStateFrontCull = NULL;
	pBlendState = NULL;

	gD3d = this;

}

DRX::DRX(const DRX&)
{

}

DRX::~DRX()
{

}

bool DRX::Initialize(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* pSwapChainBuffer;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterizerDesc;
	D3D11_BLEND_DESC blendStateDesc;

	HRESULT hResult = S_OK;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = (int)BACKBUFFER_WIDTH;
	swapChainDesc.BufferDesc.Height = (int)BACKBUFFER_HEIGHT;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;

	UINT flag = NULL;

#if _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	hResult = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, NULL, 
		NULL, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext );
	if (FAILED(hResult)) return false;

	hResult = pSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&pSwapChainBuffer);
	if (FAILED(hResult)) return false;

	hResult = pDevice->CreateRenderTargetView(pSwapChainBuffer, 0, &pRenderTargetView);
	if (FAILED(hResult)) return false;

	pSwapChainBuffer->Release();
	pSwapChainBuffer = NULL;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = NULL;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.Height = (int)BACKBUFFER_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.Width = (int)BACKBUFFER_WIDTH;

	hResult = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);
	if (FAILED(hResult)) return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hResult = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if (FAILED(hResult)) return false;

	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hResult = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);
	if (FAILED(hResult)) return false;

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = TRUE;
	rasterizerDesc.AntialiasedLineEnable = TRUE;

	hResult = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerStateBackCull);
	if (FAILED(hResult)) return false;

	rasterizerDesc.CullMode = D3D11_CULL_FRONT;

	hResult = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerStateFrontCull);
	if (FAILED(hResult)) return false;

	ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));
	blendStateDesc.AlphaToCoverageEnable = TRUE;
	blendStateDesc.IndependentBlendEnable = FALSE;
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hResult = pDevice->CreateBlendState(&blendStateDesc, &pBlendState);

	// COMMENT THIS LINE OUT IN ORDER TO TEST TRANPARENCY MASKING ON THE ALIENTREE WITHOUT THE AID OF ALPHA TO COVERAGE
	pDeviceContext->OMSetBlendState(pBlendState, NULL, 0xFFFFFFFF);

	ZeroMemory(&viewPort, sizeof(viewPort));
	hResult = pSwapChain->GetDesc(&swapChainDesc);
	if (FAILED(hResult)) return false;
	viewPort.Width = BACKBUFFER_WIDTH;
	viewPort.Height = BACKBUFFER_HEIGHT;
	viewPort.MaxDepth = 1;

	pDeviceContext->RSSetViewports(1, &viewPort);

	XMMATRIX projectionMatrix = XMMatrixIdentity();
	projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(VERTICAL_FOV), BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT, NEAR_PLANE, FAR_PLANE);
	XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);

	return true;
}
void DRX::Shutdown()
{
	SAFE_RELEASE(pSwapChain);
	pDeviceContext->ClearState();
	SAFE_RELEASE(pDeviceContext);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDepthStencilBuffer);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pRasterizerStateBackCull);
	SAFE_RELEASE(pRasterizerStateFrontCull);
	SAFE_RELEASE(pBlendState);

#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug* d3dDebug = nullptr;
	HRESULT hResult = pDevice->QueryInterface(_uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hResult))
		hResult = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SAFE_RELEASE(d3dDebug);
#endif

	SAFE_RELEASE(pDevice);

	return;
}

void DRX::ResetScene(float red, float green, float blue, float alpha)
{
	float clearColor[4];

	clearColor[0] = red;
	clearColor[1] = green;
	clearColor[2] = blue;
	clearColor[3] = alpha;

	pDeviceContext->ClearRenderTargetView(pRenderTargetView, clearColor);

	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, NULL);

	return;
}

void DRX::ClearDepthStencilView()
{
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, NULL);

	return;
}

void DRX::PresentScene()
{
	pSwapChain->Present(0, 0);
}

void DRX::ResizeWindow()
{
	if (pSwapChain)
	{
		HRESULT hResult;

		pDeviceContext->OMSetRenderTargets(0, 0, 0);

		pRenderTargetView->Release();
		pRenderTargetView = NULL;

		hResult = pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		ID3D11Texture2D* pSwapChainBuffer;

		hResult = pSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&pSwapChainBuffer);

		D3D11_TEXTURE2D_DESC windowDesc;
		pSwapChainBuffer->GetDesc(&windowDesc);

		hResult = pDevice->CreateRenderTargetView(pSwapChainBuffer, NULL, &pRenderTargetView);

		pSwapChainBuffer->Release();
		pSwapChainBuffer = NULL;

		pDepthStencilBuffer->Release();
		pDepthStencilBuffer = NULL;

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = NULL;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.Height = windowDesc.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.Width = windowDesc.Width;

		hResult = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);

		pDepthStencilView->Release();

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hResult = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);

		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		viewPort.Width = (float)windowDesc.Width;
		viewPort.Height = (float)windowDesc.Height;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		pDeviceContext->RSSetViewports(1, &viewPort);

		XMMATRIX projectionMatrix = XMLoadFloat4x4(&m_ProjectionMatrix);
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(VERTICAL_FOV), (float)windowDesc.Width / (float)windowDesc.Height, NEAR_PLANE, FAR_PLANE);
		XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
	}
}

ID3D11Device* DRX::GetDevice()
{
	return pDevice;
}

ID3D11DeviceContext* DRX::GetDeviceContext()
{
	return pDeviceContext;
}

ID3D11RasterizerState* DRX::GetRasterStateBackCull()
{
	return pRasterizerStateBackCull;
}

ID3D11RasterizerState* DRX::GetRasterStateFrontCull()
{
	return pRasterizerStateFrontCull;
}

XMFLOAT4X4 DRX::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}


ID3D11DepthStencilView* DRX::GetDepthStencilView()
{
	return pDepthStencilView;
}

void DRX::ResetRenderTargetView()
{
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}

D3D11_TEXTURE2D_DESC DRX::GetWindowDesc()
{
	D3D11_TEXTURE2D_DESC windowDesc;
	pDepthStencilBuffer->GetDesc(&windowDesc);
	return windowDesc;
}

void DRX::SetViewPort()
{
	pDeviceContext->RSSetViewports(1, &viewPort);

}

void DRX::SetBlendState()
{
	pDeviceContext->OMSetBlendState(pBlendState, NULL, 0xFFFFFFFF);
}
