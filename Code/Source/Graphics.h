#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "DRX.h"
#include "Camera.h"
#include "Model.h"
#include "Point.h"
#include "Shader.h"
#include "Light.h"

#include "../Utilities/XTime.h"

class Graphics
{
private:
	DRX* pDirectX;
	Camera* pCamera;
	Camera* pAltCamera;
	Point* pGroundPoint;
	Model* pGroundQuad;
	Model* pSceneQuad;
	Model* pPostProcessQuad;
	Model* pSkybox;
	Model* pStarModel;
	Model* pSpaceChicken;
	Model* pDeathClaw;
	Model* pStarFighter;
	Model* pOrbLight;
	Model* pAlienTree;
	Model* pAlienTreeLeft;
	Model* pAlienTreeRight;
	Shader* pPointToQuadShader;
	Shader* pPostProcessShader;
	Shader* pSkyboxShader;
	Shader* pInstancedShader;
	Shader* pColorShader;
	Shader* pColorLightShader;
	Shader* pTextureShader;
	Shader* pTextureLightShader;
	Light* pDirectionalLight;
	Light* pAltDirectionalLight;
	Light* pPointLight;
	Light* pAltPointLight;
	Light* pSpotLight;
	Light* pAltSpotLight;
	Light* pDCSpotLight;

	ID3D11Texture2D* pTempTexture;
	ID3D11Texture2D* pResolvedTexture;

	ID3D11RenderTargetView* pTempRTV;
	ID3D11ShaderResourceView* pTempSRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ID3D11Texture2D* pPostProcessTexture;
	ID3D11Texture2D* pResolvedPostProcessTexture;
	ID3D11RenderTargetView* pPostProcessRTV;
	ID3D11ShaderResourceView* pPostProcessSRV;

	XTime timer;
	float totalTime;

	float dirLightRot;
	bool firstPass;
	bool postProcessEnable;

public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(HWND hwnd);
	void Shutdown();
	bool Update();

	void Resize();

private:
	bool Render();
	void RenderScene();
	void RenderToTexture();
	void RenderPostProcess();

};

#endif