#include "Model.h"
#include "DRX.h"

#include <vector>
#include <fstream>
#include <stdio.h>

Model::Model()
{
	pVertexBuffer = NULL;
	pIndexBuffer = NULL;
	pInstanceBuffer = NULL;

	pTexture = NULL;

	vertexCount = 0;
	indexCount = 0;
	textureCount = 1;
	instanceCount = 0;
}

Model::Model(const Model&)
{

}

Model::~Model()
{

}

bool Model::Initialize(ID3D11Device* device)
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	if (!InitializeBuffers(device)) return false;

	return true;
}

void Model::Shutdown()
{
	ShutdownBuffers();

	return;
}

void Model::Render(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState)
{
	RenderBuffers(deviceContext, rasterizerState);

	return;
}

bool Model::LoadModelFromOBJ(const char* fileName)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<XMFLOAT4> tempVertices, tempUVs, tempNormals;

	FILE* file;
	fopen_s(&file, fileName, "r");

	if (file == NULL) return false;

	while (true)
	{
		char header[128];

		int result = fscanf_s(file, "%s", header, sizeof(header));

		if (result == EOF) break;

		if (strcmp(header, "v") == 0)
		{
			XMFLOAT4 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.w = 1.0f;
			tempVertices.push_back(vertex);
		}
		else if (strcmp(header, "vt") == 0)
		{
			XMFLOAT4 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = 1.0f - uv.y;
			uv.z = 0.0f;
			uv.w = 0.0f;
			tempUVs.push_back(uv);
		}
		else if (strcmp(header, "vn") == 0)
		{
			XMFLOAT4 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.w = 0.0f;
			tempNormals.push_back(normal);
		}
		else if (strcmp(header, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) return false;

			Basic_Vertex verts[3] = {
				{ tempVertices[vertexIndex[0] - 1], tempUVs[uvIndex[0] - 1], tempNormals[normalIndex[0] - 1], { 1.0f, 1.0f, 1.0f, 1.0f } },
				{ tempVertices[vertexIndex[1] - 1], tempUVs[uvIndex[1] - 1], tempNormals[normalIndex[1] - 1], { 1.0f, 1.0f, 1.0f, 1.0f } },
				{ tempVertices[vertexIndex[2] - 1], tempUVs[uvIndex[2] - 1], tempNormals[normalIndex[2] - 1], { 1.0f, 1.0f, 1.0f, 1.0f } }
			};

			for (unsigned int vert = 0; vert < 3; vert++)
			{
				unsigned int index = 0;
				for (; index < vVertexArray.size(); index++)
				{
					if (verts[vert] == vVertexArray[index])
						break;
				}

				if (index == vVertexArray.size())
					vVertexArray.push_back(verts[vert]);

				vIndexArray.push_back(index);
			}
		}
	}

	vertexCount = (unsigned int)vVertexArray.size();
	indexCount = (unsigned int)vIndexArray.size();

	return true;
}

unsigned int Model::GetVertexCount()
{
	return vertexCount;
}

unsigned int Model::GetIndexCount()
{
	return indexCount;
}

unsigned int Model::GetInstanceCount()
{
	return instanceCount;
}

void Model::Translate(float x, float y, float z)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(x, y, z));
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	return;
}

void Model::RotateOnX(float angle)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	worldMatrix = XMMatrixMultiply(XMMatrixRotationX(XMConvertToRadians(angle)), worldMatrix);
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	return;
}

void Model::RotateOnY(float angle)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(angle)), worldMatrix);
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	return;
}

void Model::RotateOnZ(float angle)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(XMConvertToRadians(angle)), worldMatrix);
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	return;
}

void Model::Scale(float x, float y, float z)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(x, y, z), worldMatrix);
	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);

	return;
}

void Model::OrbitOnY(float angle)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&m_WorldMatrix), XMMatrixRotationY(XMConvertToRadians(angle))));

	return;
}

XMFLOAT4X4 Model::GetWorldMatrix()
{
	return m_WorldMatrix;
}

void Model::SetWorldMatrix(XMFLOAT4X4& worldMatrix)
{
	m_WorldMatrix = worldMatrix;

	return;
}

void Model::PushBackInstanceMatrix(XMFLOAT4X4& worldInstance)
{
	InstanceData tempData;
	tempData.xAxis = { worldInstance._11, worldInstance._12, worldInstance._13, worldInstance._14 };
	tempData.yAxis = { worldInstance._21, worldInstance._22, worldInstance._23, worldInstance._24 };
	tempData.zAxis = { worldInstance._31, worldInstance._32, worldInstance._33, worldInstance._34 };
	tempData.wAxis = { worldInstance._41, worldInstance._42, worldInstance._43, worldInstance._44 };
	vInstanceArray.push_back(tempData);
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	HRESULT hResult;

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	D3D11_SUBRESOURCE_DATA instanceData;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Basic_Vertex) * vertexCount;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = sizeof(Basic_Vertex);

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &vVertexArray[0];

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(hResult)) return false;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDesc.CPUAccessFlags = NULL;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.StructureByteStride = sizeof(unsigned int);

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &vIndexArray[0];

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(hResult)) return false;

	if (instanceCount > 0)
	{
		vInstanceArray.reserve(instanceCount);

		ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
		instanceBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		instanceBufferDesc.ByteWidth = sizeof(InstanceData) * instanceCount;
		instanceBufferDesc.CPUAccessFlags = NULL;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.StructureByteStride = sizeof(InstanceData);

		ZeroMemory(&instanceData, sizeof(instanceData));
		instanceData.pSysMem = &vInstanceArray[0];

		hResult = device->CreateBuffer(&instanceBufferDesc, &instanceData, &pInstanceBuffer);
		if (FAILED(hResult)) return false;
	}

	return true;
}

void Model::ShutdownBuffers()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pInstanceBuffer);

	if (pTexture)
	{
		pTexture->Shutdown();
		delete pTexture;
		pTexture = NULL;
	}

	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rasterizerState)
{
	UINT stride, offset;

	if (pTexture)
	{
		if (textureCount > 1)
			pTexture->RenderMulti(deviceContext);
		else
			pTexture->Render(deviceContext);
	}

	deviceContext->RSSetState(rasterizerState);

	stride = sizeof(Basic_Vertex);
	offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (instanceCount > 0)
	{
		stride = sizeof(InstanceData);
		offset = 0;
		deviceContext->IASetVertexBuffers(1, 1, &pInstanceBuffer, &stride, &offset);

		deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
	}
	else
		deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* fileName)
{
	pTexture = new Texture;
	if (!pTexture) return false;

	if (!pTexture->Initialize(device, fileName)) return false;

	return true;
}

bool Model::LoadMultiTexture(ID3D11Device* device, WCHAR* fileName, WCHAR* altFilename)
{
	pTexture = new Texture;
	if (!pTexture) return false;

	if (!pTexture->InitializeMulti(device, fileName, altFilename)) return false;

	return true;
}

void Model::SetTextureCount(unsigned int textCount)
{
	textureCount = textCount;
}

void Model::SetInstanceCount(unsigned int instCount)
{
	instanceCount = instCount;
}

void Model::SetStarData()
{
	vertexCount = 12;
	indexCount = 60;

	vVertexArray.resize(vertexCount);
	vIndexArray.resize(indexCount);

	vVertexArray[0].Position = { 0.0f, 0.0f, -0.5f, 1.0f };
	vVertexArray[0].UV = { 0.0f, 0.0f, 0.0f, 0.0f };
	vVertexArray[0].Normal = { 0.0f, 0.0f, -1.0f, 0.0f };
	vVertexArray[0].Color = { 0.0f, 0.0f, 1.0f, 1.0f };

	vVertexArray[1].Position = { 0.0f, 0.0f, 0.5f, 1.0f };
	vVertexArray[1].UV = { 0.0f, 0.0f, 0.0f, 0.0f };
	vVertexArray[1].Normal = { 0.0f, 0.0f, -1.0f, 0.0f };
	vVertexArray[1].Color = { 0.0f, 0.0f, 1.0f, 1.0f };

	for (unsigned int vert = 2; vert < vertexCount; vert++)
	{
		vVertexArray[vert].Position = { (float)sin(XMConvertToRadians(((vert - 2) * 36.0f))), (float)cos(XMConvertToRadians(((vert - 2) * 36.0f))), 0.0f, 1.0f };
		vVertexArray[vert].UV = { 0.0f, 0.0f, -1.0f, 0.0f };
		vVertexArray[vert].Normal = { 0.0f, 0.0f, 0.0f, 0.0f };

		if (vert % 2 == 0)
		{
			vVertexArray[vert].Position.x *= 2.0f;
			vVertexArray[vert].Position.y *= 2.0f;
			vVertexArray[vert].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}
		else
			vVertexArray[vert].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	}

	unsigned int indice = 2;
	for (unsigned int index = 0; index < (indexCount >> 1); index++)
	{
		if (index % 3 == 0)
		{
			vIndexArray[index] = 0;

			indice--;

			if (indice < 2)
				indice = 11;
		}
		else
		{
			indice++;

			if (indice > 11)
				indice = 2;

			vIndexArray[index] = indice;
		}
	}

	indice = 2;
	for (unsigned int index = (indexCount >> 1); index < indexCount; index++)
	{
		if (index % 3 == 0)
		{
			vIndexArray[index] = 1;

			indice++;

			if (indice > 11)
				indice = 2;
		}
		else
		{
			indice--;

			if (indice < 2)
				indice = 11;

			vIndexArray[index] = indice;
		}
	}

	return;
}

void Model::SetNDCQuad()
{
	vertexCount = 4;
	indexCount = 6;

	vVertexArray.resize(vertexCount);
	vIndexArray.resize(indexCount);

	vVertexArray[0].Position = { -1.0f, 1.0f, 0.0f, 1.0f };
	vVertexArray[0].UV = { 0.0f, 0.0f, 0.0f, 0.0f };
	vVertexArray[1].Position = { 1.0f, 1.0f, 0.0f, 1.0f };
	vVertexArray[1].UV = { 1.0f, 0.0f, 0.0f, 0.0f };
	vVertexArray[2].Position = { -1.0f, -1.0f, 0.0f, 1.0f };
	vVertexArray[2].UV = { 0.0f, 1.0f, 0.0f, 0.0f };
	vVertexArray[3].Position = { 1.0f, -1.0f, 0.0f, 1.0f };
	vVertexArray[3].UV = { 1.0f, 1.0f, 0.0f, 0.0f };

	vIndexArray[0] = 0;
	vIndexArray[1] = 1;
	vIndexArray[2] = 2;
	vIndexArray[3] = 2;
	vIndexArray[4] = 1;
	vIndexArray[5] = 3;

	return;
}

bool Model::Basic_Vertex::operator==(const Basic_Vertex& other)
{
	if (other.Position.x != Position.x || other.Position.y != Position.y
		|| other.Position.z != Position.z || other.Position.w != Position.w) return false;

	if (other.UV.x != UV.x || other.UV.y != UV.y
		|| other.UV.z != UV.z || other.UV.w != UV.w) return false;

	if (other.Normal.x != Normal.x || other.Normal.y != Normal.y
		|| other.Normal.z != Normal.z || other.Normal.w != Normal.w) return false;

	if (other.Color.x != Color.x || other.Color.y != Color.y
		|| other.Color.z != Color.z || other.Color.w != Color.w) return false;

	return true;
}
