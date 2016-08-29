#include "Point.h"
#include "DRX.h"

Point::Point()
{
	pVertexBuffer = NULL;
	pTexture = NULL;

	vertexCount = 1;
}

Point::Point(const Point&)
{

}

Point::~Point()
{

}

bool Point::Initialize(ID3D11Device* device)
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	if (!InitializeBuffer(device)) return false;

	return true;
}

void Point::Shutdown()
{
	ShutdownBuffer();

	return;
}

void Point::Render(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState)
{
	RenderBuffer(deviceContext, rasterizerState);

	return;
}

bool Point::LoadTexture(ID3D11Device* device, WCHAR* fileName)
{
	pTexture = new Texture;
	if (!pTexture) return false;

	if (!pTexture->Initialize(device, fileName)) return false;

	return true;
}

XMFLOAT4X4 Point::GetWorldMatrix()
{
	return m_WorldMatrix;
}

void Point::PushBackPoint(XMFLOAT4 position)
{
	PointData tempData;
	tempData.Position = position;
	vVertexArray.push_back(tempData);

	return;
}

bool Point::InitializeBuffer(ID3D11Device* device)
{
	HRESULT hResult;

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(PointData) * vertexCount;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = sizeof(PointData);

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &vVertexArray[0];

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(hResult)) return false;

	return true;
}

void Point::ShutdownBuffer()
{
	SAFE_RELEASE(pVertexBuffer);

	if (pTexture)
	{
		pTexture->Shutdown();
		delete pTexture;
		pTexture = NULL;
	}

	return;
}

void Point::RenderBuffer(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState)
{
	UINT stride, offset;

	pTexture->Render(deviceContext);

	deviceContext->RSSetState(rasterizerState);

	stride = sizeof(PointData);
	offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	deviceContext->Draw(vertexCount, 0);

	return;
}

