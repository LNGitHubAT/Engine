#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Define.h"
#include <string>
#include "MeshLoader.h"
#include "AABBCollider.h"
using namespace DirectX;

class Mesh
{
public:
	Mesh();
	~Mesh();
	virtual void bindMesh(ID3D11DeviceContext * pDeviceContext);
	void unBindMesh(ID3D11DeviceContext * pDeviceContext);
	bool init(ID3D11Device * pDevice);
	bool init(ID3D11Device * pDevice, WCHAR* fileName, bool isComputeAABB = false, D3D11_USAGE flag = D3D11_USAGE_DEFAULT);
	bool initBox(ID3D11Device * pDevice, XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f));
	bool initSkyBox(ID3D11Device * pDevice, XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f));
	bool initPlane(ID3D11Device * pDevice, XMINT2 XZsize, XMFLOAT2 XZoffset, XMINT2 texCoordTile);
	bool initGUIQuad(ID3D11Device * pDevice, XMFLOAT2 size, XMFLOAT2 position);
	bool initTerrain(ID3D11Device * pDevice, UINT verticesPerCols, UINT verticesPerRows, float terrainWidth, float terrainHeight, std::vector<XMFLOAT2> yBoundaries, UINT numPatchFaces);
	virtual void update(float deltaTime);
	void clean();
	UINT getIndexCount();
	UINT getVertextCount();
	AABB getAABB();
private:
	//terrain helper fucntion
	bool buildPatchVertexBuffer(ID3D11Device * device, UINT verticesPerCols, UINT verticesPerRows, float terrainWidth, float terrainHeight, std::vector<XMFLOAT2> yBoundaries);
	bool buildPatchIndexBuffer(ID3D11Device * device, UINT numPatchFaces, UINT verticesPerCols, UINT verticesPerRows);
	
protected:
	bool createBuffer(ID3D11Device * pDevice, BasicVertex * vertices, UINT * indices, D3D11_USAGE flag = D3D11_USAGE_DEFAULT);
	ID3D11Buffer *	m_pVertexBuffer, *m_pIndexBuffer;
	UINT m_vertexCount, m_indexCount;
	UINT m_stride = sizeof(BasicVertex);
	UINT m_vertexBindOffset = 0;
	D3D11_PRIMITIVE_TOPOLOGY m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	AABB aabb;


};

