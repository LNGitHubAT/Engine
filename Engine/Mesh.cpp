#include "Mesh.h"



Mesh::Mesh()
{
	
}


Mesh::~Mesh()
{
}

void Mesh::bindMesh(ID3D11DeviceContext * pDeviceContext)
{

	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_vertexBindOffset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(m_topology);
}

void Mesh::unBindMesh(ID3D11DeviceContext * pDeviceContext)
{
	//pDeviceContext->IASetVertexBuffers(0, 1, NULL, NULL, NULL);
	//pDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
}

bool Mesh::init(ID3D11Device * pDevice)
{
	BasicVertex * vertices;
	UINT *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// fixed vertices and indices array

	m_vertexCount = 3;
	m_indexCount = 3;
	vertices = new BasicVertex[m_vertexCount];
	indices = new UINT[m_indexCount];

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Create vertexBuffer  desciption
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BasicVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Create vertex  Data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Create VertexBuffer

	pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);


	// Create indexBuffer  desciption
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Create indices Data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);

	delete vertices;
	vertices = NULL;
	delete indices;
	indices = NULL;

	return true;
}

bool Mesh::init(ID3D11Device * pDevice, WCHAR* fileName , bool isComputeAABB , D3D11_USAGE flag)
{
	BasicVertex * vertices = NULL;
	UINT *indices = NULL;
	bool result;
	MeshGenerator::getDataFromFile(fileName, &vertices, m_vertexCount, &indices, m_indexCount);
	result = createBuffer(pDevice, vertices, indices, flag);
	if (!result)
		return false;
	

	if (isComputeAABB)
	{
		XMFLOAT3 vMinf3(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
		XMFLOAT3 vMaxf3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
		XMVECTOR vMin = XMLoadFloat3(&vMinf3);
		XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
		//std::vector<Vertex::Basic32> vertices(vcount);
		for (UINT i = 0; i < m_vertexCount; ++i)
		{

			XMVECTOR P = XMLoadFloat3(&vertices[i].position);
			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
		}
		XMStoreFloat3(&aabb.center, 0.5f*(vMin + vMax));
		XMStoreFloat3(&aabb.extends, 0.5f*(vMax - vMin));
	}
	




	delete vertices;
	vertices = NULL;
	delete indices;
	indices = NULL;
	return true;
}

bool Mesh::initBox(ID3D11Device * pDevice, XMFLOAT3 size)
{
	//
	// Create the vertices.
	//

	BasicVertex vertices[24];

	float w2 = 0.5f*size.x;
	float h2 = 0.5f*size.y;
	float d2 = 0.5f*size.z;

	

	// Fill in the front face vertex data.
	vertices[0] = BasicVertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[1] = BasicVertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[2] = BasicVertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[3] = BasicVertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	vertices[4] = BasicVertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f);
	vertices[5] = BasicVertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f);
	vertices[6] = BasicVertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f);
	vertices[7] = BasicVertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f);

	// Fill in the top face vertex data.
	vertices[8] = BasicVertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[9] = BasicVertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[10] = BasicVertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[11] = BasicVertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	vertices[12] = BasicVertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	vertices[13] = BasicVertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[14] = BasicVertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[15] = BasicVertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	vertices[16] = BasicVertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f);
	vertices[17] = BasicVertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f);
	vertices[18] = BasicVertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f);
	vertices[19] = BasicVertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f);

	// Fill in the right face vertex data.
	vertices[20] = BasicVertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[21] = BasicVertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[22] = BasicVertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[23] = BasicVertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);


	//
	// Create the indices.
	//

	UINT indices[36];

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	m_vertexCount = 24;
	m_indexCount = 36;

	createBuffer(pDevice, vertices, indices);
	return true;
	
}

bool Mesh::initSkyBox(ID3D11Device * pDevice, XMFLOAT3 size)
{
	//
	// Create the vertices.
	//
	m_stride = sizeof(Vertex::SkyBox);

	Vertex::SkyBox vertices[24];

	float w2 = 0.5f*size.x;
	float h2 = 0.5f*size.y;
	float d2 = 0.5f*size.z;



	// Fill in the front face vertex data.
	vertices[0] = Vertex::SkyBox(-w2, -h2, -d2);
	vertices[1] = Vertex::SkyBox(-w2, +h2, -d2);
	vertices[2] = Vertex::SkyBox(+w2, +h2, -d2);
	vertices[3] = Vertex::SkyBox(+w2, -h2, -d2);

	// Fill in the back face vertex data.
	vertices[4] = Vertex::SkyBox(-w2, -h2, +d2);
	vertices[5] = Vertex::SkyBox(+w2, -h2, +d2);
	vertices[6] = Vertex::SkyBox(+w2, +h2, +d2);
	vertices[7] = Vertex::SkyBox(-w2, +h2, +d2);

	// Fill in the top face vertex data.
	vertices[8] = Vertex::SkyBox(-w2, +h2, -d2);
	vertices[9] = Vertex::SkyBox(-w2, +h2, +d2);
	vertices[10] = Vertex::SkyBox(+w2, +h2, +d2);
	vertices[11] = Vertex::SkyBox(+w2, +h2, -d2);

	// Fill in the bottom face vertex data.
	vertices[12] = Vertex::SkyBox(-w2, -h2, -d2);
	vertices[13] = Vertex::SkyBox(+w2, -h2, -d2);
	vertices[14] = Vertex::SkyBox(+w2, -h2, +d2);
	vertices[15] = Vertex::SkyBox(-w2, -h2, +d2);

	// Fill in the left face vertex data.
	vertices[16] = Vertex::SkyBox(-w2, -h2, +d2);
	vertices[17] = Vertex::SkyBox(-w2, +h2, +d2);
	vertices[18] = Vertex::SkyBox(-w2, +h2, -d2);
	vertices[19] = Vertex::SkyBox(-w2, -h2, -d2);

	// Fill in the right face vertex data.
	vertices[20] = Vertex::SkyBox(+w2, -h2, -d2);
	vertices[21] = Vertex::SkyBox(+w2, +h2, -d2);
	vertices[22] = Vertex::SkyBox(+w2, +h2, +d2);
	vertices[23] = Vertex::SkyBox(+w2, -h2, +d2);


	//
	// Create the indices.
	//

	UINT indices[36];

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	m_vertexCount = 24;
	m_indexCount = 36;

	//createBuffer(pDevice, vertices, indices);




	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	// Create vertexBuffer  desciption
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex::SkyBox) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	vertexBufferDesc.CPUAccessFlags = 0;

	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Create vertex  Data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Create VertexBuffer

	pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	// Create indexBuffer  desciption
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Create indices Data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	return true;






	return true;
}

bool Mesh::initPlane(ID3D11Device * pDevice, XMINT2 XZsize, XMFLOAT2 XZoffset, XMINT2 texCoordTile)
{
	BasicVertex * vertices = NULL;
	UINT *indices = NULL;
	bool result;
	result = MeshGenerator::createPlane(XZsize, XZoffset, texCoordTile,&vertices, m_vertexCount, &indices, m_indexCount);
	if (!result)
	{
		return false;
	}
	result = createBuffer(pDevice, vertices, indices);

	if (!result)
	{
		return false;
	}
	return true;
}

bool Mesh::initGUIQuad(ID3D11Device * pDevice, XMFLOAT2 size, XMFLOAT2 position)
{
	bool result;
	XMVECTOR sizeV = XMLoadFloat3(&XMFLOAT3(size.x, size.y, 0.0f));
	XMVECTOR positionV = XMLoadFloat3(&XMFLOAT3(position.x, position.y, 0.0f));
	//
	XMVECTOR vertexPosVector[4] =
	{
		XMLoadFloat3(&XMFLOAT3(-1.0f, -1.0f, 0.0f)) *sizeV + positionV,
		XMLoadFloat3(&XMFLOAT3(1.0f, 1.0f, 0.0f))	*sizeV + positionV,
		XMLoadFloat3(&XMFLOAT3(-1.0f, 1.0f, 0.0f))	*sizeV + positionV,
		XMLoadFloat3(&XMFLOAT3(1.0f, -1.0f, 0.0f))	*sizeV + positionV
	};
	
	XMFLOAT3 vertexPos[4];
	for (int i = 0; i < 4; i++)
	{
		XMStoreFloat3(&vertexPos[i], vertexPosVector[i]);
	}
	
	UINT indices[6] =
	{
		0,2,1, 0 , 1, 3
	};

	XMFLOAT2 textureCoord[4] =
	{
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f)
	};

	XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, -1.0f);



	BasicVertex vertices[4];
	for (int i = 0; i < 4; i++)
	{
		vertices[i] = BasicVertex(vertexPos[i].x, vertexPos[i].y, vertexPos[i].z, normal.x, normal.y, normal.z, textureCoord[i].x, textureCoord[i].y);
	}

	m_vertexCount = 4;
	m_indexCount = 6;

	result = createBuffer(pDevice, vertices, indices);

	if (!result)
	{
		return false;
	}
	return true;

}

bool Mesh::initTerrain(ID3D11Device * pDevice, UINT verticesPerCols, UINT verticesPerRows, float terrainWidth, float terrainDepth, std::vector<XMFLOAT2> yBoundaries, UINT numPatchFaces)
{

	m_stride = sizeof(Vertex::TerrainVertex);

	m_vertexBindOffset = 0;

	m_topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;


	bool result;
	result = buildPatchVertexBuffer(pDevice, verticesPerCols, verticesPerRows, terrainWidth, terrainDepth,yBoundaries);

	if (!result)
	{
		return false;
	}
	result = buildPatchIndexBuffer(pDevice, numPatchFaces, verticesPerCols, verticesPerRows);

	if (!result)
	{
		return false;
	}
	return true;
}


void Mesh::update(float deltaTime)
{
}

void Mesh::clean()
{
	// Release the index buffer.
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

}

UINT Mesh::getIndexCount()
{
	return m_indexCount;
}

UINT Mesh::getVertextCount()
{
	return m_vertexCount;
}

AABB Mesh::getAABB()
{
	return aabb;
}

bool Mesh::buildPatchVertexBuffer(ID3D11Device * device, UINT verticesPerCols, UINT verticesPerRows, float terrainWidth, float terrainDepth, std::vector<XMFLOAT2> yBoundaries)
{
	m_vertexCount = verticesPerCols * verticesPerRows;
	std::vector<Vertex::TerrainVertex> vertices(m_vertexCount);
	float haftWidth = 0.5f * terrainWidth;
	float haftDepth = 0.5f * terrainDepth;
	float patchWidth = terrainWidth / (verticesPerCols - 1);
	float patchDepth = terrainDepth / (verticesPerRows - 1);
	float du = 1.0f / (verticesPerCols - 1);
	float dv = 1.0f / (verticesPerRows - 1);
	for (UINT row = 0; row < verticesPerRows; row++)
	{
		float z = haftDepth - row * patchDepth;
		for (UINT col = 0; col < verticesPerCols; col++)
		{
			float x = -haftWidth + col * patchWidth;
			UINT ID = row * verticesPerCols + col;
			vertices[ID].position = XMFLOAT3(x, 0.0f, z);
			vertices[ID].textureCoord = XMFLOAT2(col * du, row * dv);
		}
	}

	//only store the boundary in the upper left conner of a patch
	for (UINT row = 0; row < verticesPerRows - 1; row++)
	{
		for (UINT col = 0; col < verticesPerCols - 1; col++)
		{
			UINT ID = row * (verticesPerCols - 1) + col;
			vertices[row * verticesPerCols + col].boundaryY = yBoundaries[ID];
		}
	}


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::TerrainVertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HRESULT result;
	result = device->CreateBuffer(&vbd, &vinitData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}





bool Mesh::buildPatchIndexBuffer(ID3D11Device * device, UINT numPatchFaces, UINT verticesPerCols, UINT verticesPerRows )
{
		m_indexCount = numPatchFaces * 4;
		// 4 indices per patch (quad)
		std::vector<UINT> indices(m_indexCount);

		int QuadID = 0;
		for (UINT i = 0; i < verticesPerRows - 1; ++i)
		{
			for (UINT j = 0; j < verticesPerCols - 1; ++j)
			{
				// upper left
				indices[QuadID] = i*verticesPerCols + j;
				// upper right
				indices[QuadID + 1] = i*verticesPerCols + j + 1;

				// lower left
				indices[QuadID + 2] = (i + 1)*verticesPerCols + j;
				// lower right
				indices[QuadID + 3] = (i + 1)*verticesPerCols + j + 1;

				QuadID += 4; // next quad
			}
		}

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &indices[0];
		HRESULT result = device->CreateBuffer(&ibd, &initData, &m_pIndexBuffer);

		if (FAILED(result))
		{
			return false;
		}

		return true;
}

bool Mesh::createBuffer(ID3D11Device * pDevice, BasicVertex * vertices, UINT * indices, D3D11_USAGE flag)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	// Create vertexBuffer  desciption
	vertexBufferDesc.Usage = flag;
	vertexBufferDesc.ByteWidth = sizeof(BasicVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if(flag == D3D11_USAGE_DYNAMIC)
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
	{
		vertexBufferDesc.CPUAccessFlags = 0;
	}
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Create vertex  Data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Create VertexBuffer
	if ((flag != D3D11_USAGE_DYNAMIC))
	{
		pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	}
	else
	{
		pDevice->CreateBuffer(&vertexBufferDesc, NULL, &m_pVertexBuffer);
	}


	




	// Create indexBuffer  desciption
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Create indices Data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	return true;
}

