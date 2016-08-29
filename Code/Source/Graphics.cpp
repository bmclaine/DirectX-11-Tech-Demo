#include "Graphics.h"
extern Graphics* gGraphics;

Graphics::Graphics()
{
	pDirectX = NULL;
	pCamera = NULL;
	pAltCamera = NULL;
	pGroundQuad = NULL;
	pSceneQuad = NULL;
	pPostProcessQuad = NULL;
	pGroundPoint = NULL;
	pSkybox = NULL;
	pStarModel = NULL;
	pStarFighter = NULL;
	pSpaceChicken = NULL;
	pDeathClaw = NULL;
	pOrbLight = NULL;
	pAlienTree = NULL;
	pAlienTreeLeft = NULL;
	pAlienTreeRight = NULL;
	pPointToQuadShader = NULL;
	pPostProcessShader = NULL;
	pSkyboxShader = NULL;
	pColorShader = NULL;
	pColorLightShader = NULL;
	pTextureShader = NULL;
	pTextureLightShader = NULL;
	pDirectionalLight = NULL;
	pAltDirectionalLight = NULL;
	pPointLight = NULL;
	pAltPointLight = NULL;
	pSpotLight = NULL;
	pAltSpotLight = NULL;
	pDCSpotLight = NULL;

	gGraphics = this;
}

Graphics::Graphics(const Graphics&)
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(HWND hwnd)
{
	dirLightRot = 0;

	pDirectX = new DRX;
	if (!pDirectX) return false;
	if (!pDirectX->Initialize(hwnd)) return false;

	pCamera = new Camera;
	if (!pCamera) return false;
	pCamera->SetWindow(hwnd);
	pCamera->RotateOnX(15.0f);
	pCamera->Translate(0.0f, 3.0f, -10.0f);

	pAltCamera = new Camera;
	if (!pAltCamera) return false;
	pAltCamera->SetWindow(hwnd);
	pAltCamera->Translate(0.0f, 4.0f, -8.0f);

	pGroundPoint = new Point;
	if (!pGroundPoint) return false;
	pGroundPoint->PushBackPoint({ 0.0f, 0.0f, 0.0f, 1.0f });
	if (!pGroundPoint->Initialize(pDirectX->GetDevice())) return false;
	pGroundPoint->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/CraterScape.dds");

	pGroundQuad = new Model;
	if (!pGroundQuad) return false;
	pGroundQuad->LoadModelFromOBJ("./Assets/Models/Groundquad.obj");
	if (!pGroundQuad->Initialize(pDirectX->GetDevice())) return false;
	pGroundQuad->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/CraterScape.dds");

	pSceneQuad = new Model;
	if (!pSceneQuad) return false;
	pSceneQuad->LoadModelFromOBJ("./Assets/Models/Scenequad.obj");
	if (!pSceneQuad->Initialize(pDirectX->GetDevice())) return false;
	pSceneQuad->Translate(0.0f, 0.0f, -10.0f);

	pPostProcessQuad = new Model;
	if (!pPostProcessQuad) return false;
	pPostProcessQuad->SetNDCQuad();
	if (!pPostProcessQuad->Initialize(pDirectX->GetDevice())) return false;

	pSkybox = new Model;
	if (!pSkybox) return false;
	pSkybox->LoadModelFromOBJ("./Assets/Models/Skybox.obj");
	if (!pSkybox->Initialize(pDirectX->GetDevice())) return false;
	pSkybox->SetTextureCount(2);
	if (!pSkybox->LoadMultiTexture(pDirectX->GetDevice(), L"./Assets/Textures/Skybox.dds", L"./Assets/Textures/AltSkybox.dds")) return false;

	pStarModel = new Model;
	if (!pStarModel) return false;
	pStarModel->SetStarData();
	if (!pStarModel->Initialize(pDirectX->GetDevice())) return false;
	pStarModel->Translate(0.0f, 2.0f, 0.0f);

	pSpaceChicken = new Model;
	if (!pSpaceChicken) return false;
	pSpaceChicken->LoadModelFromOBJ("./Assets/Models/Spacechicken.obj");
	pSpaceChicken->SetInstanceCount(150);

	XMFLOAT4X4 tempWorld =
	{
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float xPos = -14.0f;
	float yPos = 0.0f;
	float zPos = 18.0f;
	for (unsigned int instance = 0; instance < 150; instance++)
	{
		tempWorld._41 = xPos;
		tempWorld._42 = yPos;
		tempWorld._43 = zPos;

		pSpaceChicken->PushBackInstanceMatrix(tempWorld);

		xPos += 2.0f;
		if (xPos > 14.0f)
		{
			xPos = -14.0f;
			zPos -= 2.0f;
		}
	}
	if (!pSpaceChicken->Initialize(pDirectX->GetDevice())) return false;
	if (!pSpaceChicken->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/Spacechicken.dds")) return false;
	//pSpaceChicken->RotateOnY(180.0f);
	pSpaceChicken->Translate(0.0f, 0.0f, 10.0f);

	pDeathClaw = new Model;
	if (!pDeathClaw) return false;
	pDeathClaw->LoadModelFromOBJ("./Assets/Models/DeathClaw.obj");
	if (!pDeathClaw->Initialize(pDirectX->GetDevice())) return false;
	if (!pDeathClaw->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/DeathClaw.dds")) return false;
	pDeathClaw->RotateOnY(180.0f);

	pStarFighter = new Model;
	if (!pStarFighter) return false;
	pStarFighter->LoadModelFromOBJ("./Assets/Models/Starfighter.obj");
	if (!pStarFighter->Initialize(pDirectX->GetDevice())) return false;
	if (!pStarFighter->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/Starfighter.dds")) return false;
	pStarFighter->RotateOnY(225.0f);
	pStarFighter->Translate(4.0f, 0.0f, 26.0f);

	pOrbLight = new Model;
	if (!pOrbLight) return false;
	pOrbLight->LoadModelFromOBJ("./Assets/Models/OrbLight.obj");
	if (!pOrbLight->Initialize(pDirectX->GetDevice())) return false;
	if (!pOrbLight->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/OrbLight.dds")) return false;
	pOrbLight->Translate(0.0f, 2.0f, 2.0f);

	pAlienTree = new Model;
	if (!pAlienTree) return false;
	pAlienTree->LoadModelFromOBJ("./Assets/Models/Alientree.obj");
	if (!pAlienTree->Initialize(pDirectX->GetDevice())) return false;
	if (!pAlienTree->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/AlienTree.dds")) return false;
	pAlienTree->Translate(-4.0f, 0.0f, 26.0f);

	pAlienTreeLeft = new Model;
	if (!pAlienTreeLeft) return false;
	pAlienTreeLeft->LoadModelFromOBJ("./Assets/Models/Alientree.obj");
	if (!pAlienTreeLeft->Initialize(pDirectX->GetDevice())) return false;
	if (!pAlienTreeLeft->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/AlienTree.dds")) return false;
	pAlienTreeLeft->Translate(-4.0f, 0.0f, 4.0f);

	pAlienTreeRight = new Model;
	if (!pAlienTreeRight) return false;
	pAlienTreeRight->LoadModelFromOBJ("./Assets/Models/Alientree.obj");
	if (!pAlienTreeRight->Initialize(pDirectX->GetDevice())) return false;
	if (!pAlienTreeRight->LoadTexture(pDirectX->GetDevice(), L"./Assets/Textures/AlienTree.dds")) return false;
	pAlienTreeRight->Translate(4.0f, 0.0f, 4.0f);

	pPointToQuadShader = new Shader;
	if (!pPointToQuadShader) return false;
	if (!pPointToQuadShader->Initialize(pDirectX->GetDevice(), VSPointToQuadShader, PSTextureLightShader)) return false;

	pPostProcessShader = new Shader;
	if (!pPostProcessShader) return false;
	if (!pPostProcessShader->Initialize(pDirectX->GetDevice(), VSPostProcessShader, PSPostProcessShader)) return false;

	pSkyboxShader = new Shader;
	if (!pSkyboxShader) return false;
	if (!pSkyboxShader->Initialize(pDirectX->GetDevice(), VSSkyboxShader, PSSkyboxShader)) return false;

	pInstancedShader = new Shader;
	if (!pInstancedShader) return false;
	if (!pInstancedShader->Initialize(pDirectX->GetDevice(), VSInstanceShader, PSTextureLightShader)) return false;

	pColorShader = new Shader;
	if (!pColorShader) return false;
	if (!pColorShader->Initialize(pDirectX->GetDevice(), VSMatrixShader, PSColorShader)) return false;

	pColorLightShader = new Shader;
	if (!pColorLightShader) return false;
	if (!pColorLightShader->Initialize(pDirectX->GetDevice(), VSMatrixShader, PSColorLightShader)) return false;

	pTextureShader = new Shader;
	if (!pTextureShader) return false;
	if (!pTextureShader->Initialize(pDirectX->GetDevice(), VSMatrixShader, PSTextureShader)) return false;

	pTextureLightShader = new Shader;
	if (!pTextureLightShader) return false;
	if (!pTextureLightShader->Initialize(pDirectX->GetDevice(), VSMatrixShader, PSTextureLightShader)) return false;

	pDirectionalLight = new Light;
	if (!pDirectionalLight) return false;
	pDirectionalLight->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	pDirectionalLight->SetAmbience({ 0.2f, 0.0f, 0.0f, 0.0f });

	pAltDirectionalLight = new Light;
	if (!pAltDirectionalLight) return false;
	pAltDirectionalLight->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	pAltDirectionalLight->SetAmbience({ 0.2f, 0.0f, 0.0f, 0.0f });
	pAltDirectionalLight->SetDirection({ 0.0f, -1.0f, 1.0f, 1.0f });

	pPointLight = new Light;
	if (!pPointLight) return false;
	pPointLight->SetRadius({ 10.0f, 0.0f, 0.0f, 0.0f });
	pPointLight->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	pPointLight->SetAmbience({ 0.0f, 0.0f, 0.0f, 0.0f });

	pAltPointLight = new Light;
	if (!pAltPointLight) return false;
	pAltPointLight->SetRadius({ 0.0f, 0.0f, 0.0f, 0.0f });
	pAltPointLight->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	pAltPointLight->SetAmbience({ 0.0f, 0.0f, 0.0f, 0.0f });

	pSpotLight = new Light;
	if (!pSpotLight) return false;
	pSpotLight->SetPosition({ 0.0f, 10.0f, 10.0f, 1.0f });
	pSpotLight->SetDirection({ 0.0f, -1.0f, 0.0f, 1.0f });
	pSpotLight->SetInnerConeRatio({ 0.95f, 0.0f, 0.0f, 0.0f });
	pSpotLight->SetOuterConeRatio({ 0.90f, 0.0f, 0.0f, 0.0f });
	pSpotLight->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	pSpotLight->SetAmbience({ 0.0f, 0.0f, 0.0f, 0.0f });

	pAltSpotLight = new Light;
	if (!pAltSpotLight) return false;
	pAltSpotLight->SetPosition({ 0.0f, 2.0f, -8.0f, 1.0f });
	pAltSpotLight->SetDirection({ 0.0f, 1.0f, 1.0f, 1.0f });
	pAltSpotLight->SetInnerConeRatio({ 0.95f, 0.0f, 0.0f, 0.0f });
	pAltSpotLight->SetOuterConeRatio({ 0.90f, 0.0f, 0.0f, 0.0f });
	pAltSpotLight->SetColor({ 2.0f, 0.0f, 0.0f, 1.0f });
	pAltSpotLight->SetAmbience({ 0.0f, 0.0f, 0.0f, 0.0f });

	pDCSpotLight = new Light;
	if (!pDCSpotLight) return false;
	pDCSpotLight->SetPosition({ 0.0f, 2.0f, -4.0f, 1.0f });
	pDCSpotLight->SetDirection({ 0.0f, 1.0f, -1.0f, 1.0f });
	pDCSpotLight->SetInnerConeRatio({ 0.95f, 0.0f, 0.0f, 0.0f });
	pDCSpotLight->SetOuterConeRatio({ 0.90f, 0.0f, 0.0f, 0.0f });
	pDCSpotLight->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	pDCSpotLight->SetAmbience({ 0.0f, 0.0f, 0.0f, 0.0f });

	// render to texture stuff - read only
	D3D11_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(textDesc));
	textDesc.ArraySize = 1;
	textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textDesc.CPUAccessFlags = NULL;
	textDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textDesc.Height = pDirectX->GetWindowDesc().Height;
	textDesc.MipLevels = 1;
	textDesc.SampleDesc.Count = 4;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.Width = pDirectX->GetWindowDesc().Width;

	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pTempTexture);
	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pPostProcessTexture);

	textDesc.SampleDesc.Count = 1;

	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pResolvedTexture);
	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pResolvedPostProcessTexture);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

	pDirectX->GetDevice()->CreateRenderTargetView(pTempTexture, &rtvDesc, &pTempRTV);

	pDirectX->GetDevice()->CreateRenderTargetView(pPostProcessTexture, &rtvDesc, &pPostProcessRTV);

	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	postProcessEnable = false;

	return true;
}

void Graphics::Shutdown()
{
	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pAltCamera)
	{
		delete pAltCamera;
		pAltCamera = NULL;
	}

	if (pGroundPoint)
	{
		pGroundPoint->Shutdown();
		delete pGroundPoint;
		pGroundPoint = NULL;
	}

	if (pGroundQuad)
	{
		pGroundQuad->Shutdown();
		delete pGroundQuad;
		pGroundQuad = NULL;
	}

	if (pSceneQuad)
	{
		pSceneQuad->Shutdown();
		delete pSceneQuad;
		pSceneQuad = NULL;
	}

	if (pPostProcessQuad)
	{
		pPostProcessQuad->Shutdown();
		delete pPostProcessQuad;
		pPostProcessQuad = NULL;
	}

	if (pSkybox)
	{
		pSkybox->Shutdown();
		delete pSkybox;
		pSkybox = NULL;
	}

	if (pStarModel)
	{
		pStarModel->Shutdown();
		delete pStarModel;
		pStarModel = NULL;
	}

	if (pSpaceChicken)
	{
		pSpaceChicken->Shutdown();
		delete pSpaceChicken;
		pSpaceChicken = NULL;
	}

	if (pDeathClaw)
	{
		pDeathClaw->Shutdown();
		delete pDeathClaw;
		pDeathClaw = NULL;
	}

	if (pStarFighter)
	{
		pStarFighter->Shutdown();
		delete pStarFighter;
		pStarFighter = NULL;
	}

	if (pOrbLight)
	{
		pOrbLight->Shutdown();
		delete pOrbLight;
		pOrbLight = NULL;
	}

	if (pAlienTree)
	{
		pAlienTree->Shutdown();
		delete pAlienTree;
		pAlienTree = NULL;
	}

	if (pAlienTreeLeft)
	{
		pAlienTreeLeft->Shutdown();
		delete pAlienTreeLeft;
		pAlienTreeLeft = NULL;
	}

	if (pAlienTreeRight)
	{
		pAlienTreeRight->Shutdown();
		delete pAlienTreeRight;
		pAlienTreeRight = NULL;
	}

	if (pPointToQuadShader)
	{
		pPointToQuadShader->Shutdown();
		delete pPointToQuadShader;
		pPointToQuadShader = NULL;
	}

	if (pPostProcessShader)
	{
		pPostProcessShader->Shutdown();
		delete pPostProcessShader;
		pPostProcessShader = NULL;
	}

	if (pSkyboxShader)
	{
		pSkyboxShader->Shutdown();
		delete pSkyboxShader;
		pSkyboxShader = NULL;
	}

	if (pInstancedShader)
	{
		pInstancedShader->Shutdown();
		delete pInstancedShader;
		pInstancedShader = NULL;
	}

	if (pColorShader)
	{
		pColorShader->Shutdown();
		delete pColorShader;
		pColorShader = NULL;
	}

	if (pColorLightShader)
	{
		pColorLightShader->Shutdown();
		delete pColorLightShader;
		pColorLightShader = NULL;
	}

	if (pTextureShader)
	{
		pTextureShader->Shutdown();
		delete pTextureShader;
		pTextureShader = NULL;
	}

	if (pTextureLightShader)
	{
		pTextureLightShader->Shutdown();
		delete pTextureLightShader;
		pTextureLightShader = NULL;
	}

	if (pDirectionalLight)
	{
		delete pDirectionalLight;
		pDirectionalLight = NULL;
	}

	if (pAltDirectionalLight)
	{
		delete pAltDirectionalLight;
		pAltDirectionalLight = NULL;
	}

	if (pPointLight)
	{
		delete pPointLight;
		pPointLight = NULL;
	}

	if (pAltPointLight)
	{
		delete pAltPointLight;
		pAltPointLight = NULL;
	}

	if (pSpotLight)
	{
		delete pSpotLight;
		pSpotLight = NULL;
	}

	if (pAltSpotLight)
	{
		delete pAltSpotLight;
		pAltSpotLight = NULL;
	}

	if (pDCSpotLight)
	{
		delete pDCSpotLight;
		pDCSpotLight = NULL;
	}

	SAFE_RELEASE(pTempTexture);
	SAFE_RELEASE(pPostProcessTexture);
	SAFE_RELEASE(pResolvedTexture);
	SAFE_RELEASE(pResolvedPostProcessTexture);
	SAFE_RELEASE(pTempRTV);
	SAFE_RELEASE(pTempSRV);
	SAFE_RELEASE(pPostProcessRTV);
	SAFE_RELEASE(pPostProcessSRV);

	if (pDirectX)
	{
		pDirectX->Shutdown();
		delete pDirectX;
		pDirectX = NULL;
	}

	return;
}

bool Graphics::Update()
{
	timer.Signal();
	totalTime = (float)timer.TotalTime();

	if (GetAsyncKeyState('P') & 0x1)
		postProcessEnable = !postProcessEnable;

	dirLightRot += (float)timer.Delta();

	pCamera->Input((float)timer.Delta(), pDirectX->GetWindowDesc().Height, pDirectX->GetWindowDesc().Width);

	// silly little star
	//pStarModel->RotateOnY((float)timer.Delta());d

	pOrbLight->RotateOnY(-(float)timer.Delta() * 100.0f);
	pOrbLight->OrbitOnY((float)timer.Delta() * 50.0f);

	pDirectionalLight->SetDirection({ cos(dirLightRot), sin(dirLightRot), 0.0f, 1.0f });
	pSpotLight->SetDirection({ -cos(dirLightRot * 2.0f), -1.0f, -sin(dirLightRot * 2.0f), 1.0f });

	if (!Render()) return false;

	return true;
}

void Graphics::Resize()
{
	D3D11_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(textDesc));
	textDesc.ArraySize = 1;
	textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textDesc.CPUAccessFlags = NULL;
	textDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textDesc.Height = pDirectX->GetWindowDesc().Height;
	textDesc.MipLevels = 1;
	textDesc.SampleDesc.Count = 4;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.Width = pDirectX->GetWindowDesc().Width;

	SAFE_RELEASE(pTempTexture);
	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pTempTexture);
	SAFE_RELEASE(pPostProcessTexture);
	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pPostProcessTexture);

	textDesc.SampleDesc.Count = 1;

	SAFE_RELEASE(pResolvedTexture);
	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pResolvedTexture);
	SAFE_RELEASE(pResolvedPostProcessTexture);
	pDirectX->GetDevice()->CreateTexture2D(&textDesc, NULL, &pResolvedPostProcessTexture);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

	SAFE_RELEASE(pTempRTV);
	pDirectX->GetDevice()->CreateRenderTargetView(pTempTexture, &rtvDesc, &pTempRTV);

	SAFE_RELEASE(pPostProcessRTV);
	pDirectX->GetDevice()->CreateRenderTargetView(pPostProcessTexture, &rtvDesc, &pPostProcessRTV);

	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
}

bool Graphics::Render()
{
	pDirectX->GetDeviceContext()->OMSetRenderTargets(1, &pTempRTV, pDirectX->GetDepthStencilView());
	pDirectX->SetViewPort();
	pDirectX->SetBlendState();

	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pDirectX->GetDeviceContext()->ClearRenderTargetView(pTempRTV, color);
	pDirectX->GetDeviceContext()->ClearDepthStencilView(pDirectX->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, NULL);

	firstPass = true;
	RenderToTexture();

	pDirectX->GetDeviceContext()->ResolveSubresource(pResolvedTexture, 0, pTempTexture, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
	pDirectX->GetDevice()->CreateShaderResourceView(pResolvedTexture, &srvDesc, &pTempSRV);

	if (postProcessEnable)
		pDirectX->GetDeviceContext()->OMSetRenderTargets(1, &pPostProcessRTV, pDirectX->GetDepthStencilView());
	else
		pDirectX->ResetRenderTargetView();

	firstPass = false;
	RenderScene();

	if (postProcessEnable)
	{
		pDirectX->GetDeviceContext()->ResolveSubresource(pResolvedPostProcessTexture, 0, pPostProcessTexture, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
		pDirectX->GetDevice()->CreateShaderResourceView(pResolvedPostProcessTexture, &srvDesc, &pPostProcessSRV);

		pDirectX->ResetRenderTargetView();
		RenderPostProcess();
	}

	pDirectX->PresentScene();

	pDirectX->GetDeviceContext()->ClearState();

	return true;
}

void Graphics::RenderScene()
{
	pDirectX->ResetScene(0.0f, 0.0f, 0.5f, 1.0f);

	XMFLOAT4X4 tempMat = pCamera->GetInverseViewMatrix();
	tempMat._41 = 0.0f;
	tempMat._42 = 0.0f;
	tempMat._43 = 0.0f;
	pSkyboxShader->Render(pDirectX->GetDeviceContext(), pSkybox->GetWorldMatrix(), tempMat, pDirectX->GetProjectionMatrix(), NULL, NULL, NULL, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
	pSkybox->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateFrontCull());

	pDirectX->ClearDepthStencilView();

	// put the orblight in the space chickens space
	XMFLOAT4X4 orbWorld;
	XMMATRIX tempA = XMLoadFloat4x4(&pOrbLight->GetWorldMatrix());
	XMMATRIX tempB = XMLoadFloat4x4(&pSpaceChicken->GetWorldMatrix());
	XMStoreFloat4x4(&orbWorld, XMMatrixMultiply(tempA, tempB));

	// place the point light where the orblight is
	pPointLight->SetPosition({ orbWorld._41, orbWorld._42, orbWorld._43, 1.0f });
	pSpotLight->SetPosition({ orbWorld._41, 5.0f, orbWorld._43, 1.0f });

	XMFLOAT4X4 camWorldMat = pCamera->GetViewMatrix();
	XMFLOAT4 camWorldPos = { camWorldMat._41, camWorldMat._42, camWorldMat._43, camWorldMat._44 };

	pPointToQuadShader->Render(pDirectX->GetDeviceContext(), pGroundPoint->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pDirectionalLight, pPointLight, pSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pGroundPoint->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateFrontCull());
	pGroundPoint->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());
	pDirectX->GetDeviceContext()->GSSetShader(NULL, NULL, NULL);

	//pTextureLightShader->Render(pDirectX->GetDeviceContext(), pGroundQuad->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pDirectionalLight, pPointLight, pSpotLight, camWorldPos);
	//pGroundQuad->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateFrontCull());
	//pGroundQuad->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	// silly little star..
	//pColorShader->Render(pDirectX->GetDeviceContext(), pStarModel->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), NULL);
	//pStarModel->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pInstancedShader->Render(pDirectX->GetDeviceContext(), pSpaceChicken->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pDirectionalLight, pPointLight, pSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pSpaceChicken->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), orbWorld, pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pDirectionalLight, pPointLight, pSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pOrbLight->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), pStarFighter->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pDirectionalLight, pPointLight, pSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pStarFighter->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), pAlienTree->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pDirectionalLight, pPointLight, pSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pAlienTree->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	if (firstPass == false)
	{
		pDirectX->GetDeviceContext()->PSSetShaderResources(0, 1, &pTempSRV);
		SAFE_RELEASE(pTempSRV);
		pTextureLightShader->Render(pDirectX->GetDeviceContext(), pSceneQuad->GetWorldMatrix(), pCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pAltDirectionalLight, pAltPointLight, pDCSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
		pSceneQuad->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());
	}

	return;
}

void Graphics::RenderToTexture()
{
	pDirectX->ResetScene(0.0f, 0.0f, 0.5f, 1.0f);

	XMFLOAT4X4 tempMat = pAltCamera->GetInverseViewMatrix();
	tempMat._41 = 0.0f;
	tempMat._42 = 0.0f;
	tempMat._43 = 0.0f;
	pSkyboxShader->Render(pDirectX->GetDeviceContext(), pSkybox->GetWorldMatrix(), tempMat, pDirectX->GetProjectionMatrix(), NULL, NULL, NULL, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
	pSkybox->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateFrontCull());

	pDirectX->ClearDepthStencilView();

	XMFLOAT4X4 camWorldMat = pAltCamera->GetViewMatrix();
	XMFLOAT4 camWorldPos = { camWorldMat._41, camWorldMat._42, camWorldMat._43, camWorldMat._44 };

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), pGroundQuad->GetWorldMatrix(), pAltCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pAltDirectionalLight, pAltPointLight, pAltSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pGroundQuad->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateFrontCull());
	pGroundQuad->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), pDeathClaw->GetWorldMatrix(), pAltCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pAltDirectionalLight, pAltPointLight, pAltSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pDeathClaw->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), pAlienTreeLeft->GetWorldMatrix(), pAltCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pAltDirectionalLight, pAltPointLight, pAltSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pAlienTreeLeft->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	pTextureLightShader->Render(pDirectX->GetDeviceContext(), pAlienTreeRight->GetWorldMatrix(), pAltCamera->GetInverseViewMatrix(), pDirectX->GetProjectionMatrix(), pAltDirectionalLight, pAltPointLight, pAltSpotLight, camWorldPos, { 0.0f, 0.0f, 0.0f, 0.0f });
	pAlienTreeRight->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());

	return;
}

void Graphics::RenderPostProcess()
{
	pDirectX->ResetScene(0.0f, 0.0f, 0.5f, 1.0f);

	pDirectX->GetDeviceContext()->PSSetShaderResources(0, 1, &pPostProcessSRV);
	SAFE_RELEASE(pPostProcessSRV);
	pPostProcessShader->Render(pDirectX->GetDeviceContext(), pGroundQuad->GetWorldMatrix(), pGroundQuad->GetWorldMatrix(), pGroundQuad->GetWorldMatrix(), NULL, NULL, NULL, { 0.0f, 0.0f, 0.0f, 0.0f }, { totalTime, 0.0f, 0.0f, 0.0f });
	pPostProcessQuad->Render(pDirectX->GetDeviceContext(), pDirectX->GetRasterStateBackCull());
}
