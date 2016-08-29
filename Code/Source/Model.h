#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#pragma comment(lib, "D3D11.Lib")

#include <DirectXMath.h>
using namespace DirectX;

#include "Texture.h"

#include <vector>

class Model
{
private:
	struct Basic_Vertex
	{
		XMFLOAT4 Position;
		XMFLOAT4 UV;
		XMFLOAT4 Normal;
		XMFLOAT4 Color;

		bool operator==(const Basic_Vertex& other);
	};

	struct InstanceData
	{
		XMFLOAT4 xAxis;
		XMFLOAT4 yAxis;
		XMFLOAT4 zAxis;
		XMFLOAT4 wAxis;
	};

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pInstanceBuffer;

	std::vector<Basic_Vertex> vVertexArray;
	std::vector<unsigned int> vIndexArray;
	std::vector<InstanceData> vInstanceArray;

	XMFLOAT4X4 m_WorldMatrix;
	Texture* pTexture;

	unsigned int vertexCount;
	unsigned int indexCount;
	unsigned int textureCount;
	unsigned int instanceCount;

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState);

	bool LoadModelFromOBJ(const char* fileName);
	bool LoadTexture(ID3D11Device* device, WCHAR* fileName);
	bool LoadMultiTexture(ID3D11Device* device, WCHAR* fileName, WCHAR* altFilename);

	void Translate(float x, float y, float z);
	void RotateOnX(float angle);
	void RotateOnY(float angle);
	void RotateOnZ(float angle);
	void Scale(float x, float y, float z);
	void OrbitOnY(float angle);

	unsigned int GetVertexCount();
	unsigned int GetIndexCount();
	unsigned int GetInstanceCount();

	XMFLOAT4X4 GetWorldMatrix();

	void SetInstanceCount(unsigned int instCount);
	void SetTextureCount(unsigned int textCount);

	void SetWorldMatrix(XMFLOAT4X4& worldMatrix);
	void PushBackInstanceMatrix(XMFLOAT4X4& worldInstance);

	void SetStarData();
	void SetNDCQuad();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState);
};

#endif