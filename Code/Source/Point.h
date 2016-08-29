#ifndef _POINT__H_
#define _POINT__H_

#include <d3d11.h>
#pragma comment(lib, "D3D11.Lib")

#include <DirectXMath.h>
using namespace DirectX;

#include "Texture.h"

#include <vector>

class Point
{
private:
	struct PointData
	{
		XMFLOAT4 Position;
	};

	ID3D11Buffer* pVertexBuffer;

	std::vector<PointData> vVertexArray;

	XMFLOAT4X4 m_WorldMatrix;
	Texture* pTexture;

	unsigned int vertexCount;

public:
	Point();
	Point(const Point&);
	~Point();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState);

	bool LoadTexture(ID3D11Device* device, WCHAR* fileName);

	XMFLOAT4X4 GetWorldMatrix();

	void PushBackPoint(XMFLOAT4 position);

private:
	bool InitializeBuffer(ID3D11Device* device);
	void ShutdownBuffer();
	void RenderBuffer(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState);
};

#endif