#include "Terrain.h"


Terrain::Terrain() : GameObject()
{
	
}


Terrain::~Terrain()
{
}

bool Terrain::init(ID3D11Device * device, ID3D11DeviceContext * deviceContext, InitInfo initInfo, TerrainShader * shader, float textureScale)
{
	// Build VB
	//
	bool result;

	m_initInfo = initInfo;

	m_numVerticesPerPatchRows = ((initInfo.HeightmapWidth - 1) / maxCellPerPatch) + 1;
	m_numVerticesPerPatchCols = ((initInfo.HeightmapHeight -1)/ maxCellPerPatch) + 1;

	m_numPatchVertices = m_numVerticesPerPatchCols * m_numVerticesPerPatchRows;
	m_numPatchFaces = (m_numVerticesPerPatchCols - 1)* (m_numVerticesPerPatchRows - 1);

	heightMapCellSpacing.x = 1.0f / (initInfo.HeightmapWidth - 1);

	heightMapCellSpacing.y = 1.0f / (initInfo.HeightmapHeight - 1);

	m_textureScale = textureScale;

	// Build height map
	result = loadHeightMap();

	if (!result)
	{
		
		return false;
	}
	// Build Boundary Y
	calculateYBoundaries();


	m_mesh = new Mesh();
	result =  m_mesh->initTerrain(device, m_numVerticesPerPatchCols, m_numVerticesPerPatchRows, getWidth(), getDepth(), m_patchYBoundary, m_numPatchFaces);

	
	if (!result)
	{
		MessageBox(0, L"Failed to create terrain", 0, 0);
		return false;
	}

	buildHeightMapSRV(device);
	// Build texture array and blend map
	buildLayersBlendMap(device, deviceContext);

	m_shader = shader;

	return true;
}

void Terrain::render(ID3D11DeviceContext * deviceContext, Camera * cam, PointLightObject * pointLight, Fog fog , bool isLowestQuality)
{
	bindObject(deviceContext);
	TerrainShader* shader = (TerrainShader*)m_shader;
	shader->setVSParameter(deviceContext, m_worldMatrix, m_pHeightMapSRV);
	XMFLOAT4X4 viewProjectionMatrixF44;
	XMStoreFloat4x4(&viewProjectionMatrixF44, XMMatrixMultiply(cam->getViewMatrix(), cam->getProjectionMatrix())) ;

	XMFLOAT4 *planes = MathUtility::extractFrustum(viewProjectionMatrixF44);

	if (!isLowestQuality)
	{
		shader->setHSParameter(deviceContext, cam->getPosition(), minDist, maxDist, maxTess, minTess, MathUtility::extractFrustum(viewProjectionMatrixF44));
	}
	else
		shader->setHSParameter(deviceContext, cam->getPosition(), minDist, maxDist, 0.0f, 0.0f, MathUtility::extractFrustum(viewProjectionMatrixF44));
	
	shader->setDSParameter(deviceContext, m_worldMatrix, cam->getViewMatrix(), cam->getProjectionMatrix(), m_textureScale, m_pHeightMapSRV);
	shader->setPSParameter(deviceContext, m_material, pointLight->getData(), cam->getPosition(), fog.color, fog.start, fog.range, m_initInfo.CellSpacing, heightMapCellSpacing, m_pHeightMapSRV, m_pBlendMapsSRV, m_pLayesSRV);
	deviceContext->DrawIndexed(m_mesh->getIndexCount(), 0, 0);
	unbindObject(deviceContext);
}

float Terrain::getWidth() const
{
	return (m_initInfo.HeightmapWidth - 1) * m_initInfo.CellSpacing;
}

float Terrain::getDepth() const
{
	return (m_initInfo.HeightmapHeight - 1) * m_initInfo.CellSpacing;
}

void Terrain::getHeightAndNormal(float x, float z, float & height, XMFLOAT3 & normal) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*getWidth()) / m_initInfo.CellSpacing;
	float d = (z - 0.5f*getDepth()) / -m_initInfo.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);


	float A = m_HeightMapStored[row*m_initInfo.HeightmapWidth + col];
	float B = m_HeightMapStored[row*m_initInfo.HeightmapWidth + col + 1];
	float C = m_HeightMapStored[(row + 1)*m_initInfo.HeightmapWidth + col];
	float D = m_HeightMapStored[(row + 1)*m_initInfo.HeightmapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		height =  A + s*uy + t*vy;

		// calculate normal


		XMFLOAT3 AB = XMFLOAT3(m_initInfo.CellSpacing, B - A, 0.0f);
		XMFLOAT3 AC = XMFLOAT3(0.0f, C - A, -m_initInfo.CellSpacing);

		XMVECTOR faceNormal = XMVector3Cross(XMLoadFloat3(&AB), XMLoadFloat3(&AC) );

		faceNormal = XMVector3Normalize(faceNormal);

		XMStoreFloat3(&normal, faceNormal);

	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		height =  D + (1.0f - s)*uy + (1.0f - t)*vy;

		// calculate normal

		XMFLOAT3 BC = XMFLOAT3(-m_initInfo.CellSpacing, C - B, -m_initInfo.CellSpacing);
		XMFLOAT3 BD = XMFLOAT3(0.0f, D - B, -m_initInfo.CellSpacing);

		XMVECTOR faceNormal = XMVector3Cross(XMLoadFloat3(&BD), XMLoadFloat3(&BC));

		faceNormal = XMVector3Normalize(faceNormal);

		XMStoreFloat3(&normal, faceNormal);
	}
}

bool Terrain::loadHeightMap()
{
	std::vector<unsigned char > in(m_initInfo.HeightmapHeight * m_initInfo.HeightmapWidth);
	std::ifstream ifstream;
	ifstream.open(m_initInfo.HeightMapFilename, std::ios_base::binary);

	if (ifstream)
	{
		// Read the RAW bytes.
		//inFile.read((char*)&in[0], (std::streamsize)in.size());
		ifstream.read((char*)&in[0], (std::streamsize)in.size());
		ifstream.close();
	}
	else
	{
		
			std::wstring msg = L"Can not find ";
			msg = msg + m_initInfo.HeightMapFilename;
			MessageBox(0, msg.c_str(), 0, 0);
			return false;
	}

	m_HeightMapStored.resize(m_initInfo.HeightmapHeight * m_initInfo.HeightmapWidth, 0.0f);
	// for every vertices in heightmap
	for (UINT i = 0; i < m_initInfo.HeightmapHeight * m_initInfo.HeightmapWidth; i++)
	{
		// height value between 0 and heightScale
		m_HeightMapStored[i] = ((float)in[i] / 255.0f) * m_initInfo.HeightScale;
	}
	
	smoothHeightMap();

	float A = m_HeightMapStored[0];
	float B = m_HeightMapStored[1];
	float C = m_HeightMapStored[2048];

	return true;
}

void Terrain::smoothHeightMap()
{
	std::vector<float> smoothedHeightMap(m_HeightMapStored.size());
	for (UINT col = 0; col < m_initInfo.HeightmapWidth; col++)
	{
		for (UINT row = 0; row < m_initInfo.HeightmapHeight; row++)
		{
			smoothedHeightMap[row * m_initInfo.HeightmapWidth + col] = average(col, row);
		}
	}
	m_HeightMapStored = smoothedHeightMap;
}

float Terrain::average(UINT col, UINT row)
{
	float result = 0;
	UINT numberOfNeighBorVertex = 0;

	for (int currentCol = (int)col - 1; currentCol <= (int)col + 1; currentCol++)
	{
		for (int currentRow = (int)row - 1; currentRow <= (int)row + 1; currentRow++)
		{
			if (isInBound(currentCol, currentRow))
			{
				result += m_HeightMapStored[currentRow * m_initInfo.HeightmapWidth + currentCol];
				numberOfNeighBorVertex++;
			}
		}
	}

	return result / (float)numberOfNeighBorVertex;
}

bool Terrain::isInBound(UINT col, UINT row)
{
	return (col >= 0 && col < m_initInfo.HeightmapWidth)
		&& (row >= 0 && row < m_initInfo.HeightmapHeight);
}

bool Terrain::buildPatchVertexBuffer(ID3D11Device * device)
{
	std::vector<Vertex::TerrainVertex> vertices(m_numVerticesPerPatchCols * m_numVerticesPerPatchRows);
	float haftWidth = 0.5f * getWidth();
	float haftDepth = 0.5f * getDepth();
	float patchWidth = getWidth() / (m_numVerticesPerPatchCols - 1);
	float patchDepth = getDepth() / (m_numVerticesPerPatchRows - 1);
	float du = 1.0f / (m_numVerticesPerPatchCols - 1);
	float dv = 1.0f / (m_numVerticesPerPatchRows - 1);
	for (UINT row = 0; row < m_numVerticesPerPatchRows; row++)
	{
		float z = -haftDepth + row * patchDepth;
		for (UINT col = 0; col < m_numVerticesPerPatchCols; col++)
		{
			float x = -haftWidth + col * patchWidth;
			UINT ID = row * m_numVerticesPerPatchCols + col;
			vertices[ID].position = XMFLOAT3(x, 0.0f, z);
			vertices[ID].textureCoord = XMFLOAT2(col * du, row * dv);
		}
	}

	//only store the boundary in the upper left conner of a patch
	for (UINT row = 0; row < m_numVerticesPerPatchRows - 1; row++)
	{
		for (UINT col = 0; col < m_numVerticesPerPatchCols - 1; col++)
		{
			UINT ID = row * m_numVerticesPerPatchCols + col;
			vertices[ID].boundaryY = m_patchYBoundary[ID];
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
	result = device->CreateBuffer(&vbd, &vinitData, &m_pQuadVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool Terrain::buildPatchIndexBuffer(ID3D11Device * device)
{
	// 4 indices per patch (quad)
	std::vector<UINT> indices(m_numPatchFaces * 4);

	int QuadID = 0;
	for (UINT i = 0; i < m_numVerticesPerPatchRows - 1; ++i)
	{
		for (UINT j = 0; j < m_numVerticesPerPatchCols - 1; ++j)
		{
			// upper left
			indices[QuadID] = i*m_numVerticesPerPatchCols + j;
			// upper right
			indices[QuadID + 1] = i*m_numVerticesPerPatchCols + j + 1;

			// lower left
			indices[QuadID + 2] = (i + 1)*m_numVerticesPerPatchCols + j;
			// lower right
			indices[QuadID + 3] = (i + 1)*m_numVerticesPerPatchCols + j + 1;

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

void Terrain::calculateYBoundaries()
{
	m_patchYBoundary.resize(m_numPatchFaces);
	for (UINT col = 0; col < m_numVerticesPerPatchCols - 1; col++)
	{
		for (UINT row = 0; row < m_numVerticesPerPatchRows - 1 ; row++)
		{
			calcuateYBoundary(col, row);
		}
	}
}

void Terrain::calcuateYBoundary(int col, int row)
{
	// Loop through all vertices of a patch to find min and max heights
	XMFLOAT2 yBoundary(MathUtility::maximumFloatNumber(), -MathUtility::maximumFloatNumber());
	UINT startColIndex = col * maxCellPerPatch;
	UINT endColIndex = (col + 1) * maxCellPerPatch;
	UINT startRowIndex = row * maxCellPerPatch;
	UINT endRowIndex = (row + 1) * maxCellPerPatch;

	for (int vertCol = startColIndex; vertCol < endColIndex; vertCol++)
	{
		for (int vertRow = startRowIndex; vertRow < endRowIndex; vertRow++)
		{
			float height = m_HeightMapStored[vertRow * m_initInfo.HeightmapWidth + vertCol];
			yBoundary.x = MathUtility::minf(yBoundary.x, height);
			yBoundary.y = MathUtility::maxf(yBoundary.y, height);
		}
	}

	int patchID = row * (m_numVerticesPerPatchCols - 1) + col;
	m_patchYBoundary[patchID] = yBoundary;
}

bool Terrain::buildHeightMapSRV(ID3D11Device * device)
{
	/*bool result;

	Texture2D* heightmapTexture = new Texture2D();
	const WCHAR* fileName = m_initInfo.HeightMapFilename.c_str();
	result = heightmapTexture->init(device, fileName);

	if (!result)
	{

		return false;
	}
	else
	{
		m_pHeightMapSRV = heightmapTexture->getResourceView();
	}

	return true;*/

	HRESULT result;


	//std::vector<DirectX::PackedVector::HALF> hmap(m_HeightMapStored.size());
	//std::transform(m_HeightMapStored.begin(), m_HeightMapStored.end(), hmap.begin(), DirectX::PackedVector::XMConvertFloatToHalf);


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_initInfo.HeightmapWidth;
	texDesc.Height = m_initInfo.HeightmapHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// HALF is defined in xnamath.h, for storing 16-bit float.
	

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &m_HeightMapStored[0];
	data.SysMemPitch = m_initInfo.HeightmapWidth * sizeof(float);
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	result = (device->CreateTexture2D(&texDesc, &data, &hmapTex));

	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	result = (device->CreateShaderResourceView(hmapTex, &srvDesc, &m_pHeightMapSRV));

	if (FAILED(result))
	{
		return false;
	}
	// SRV saves reference.
	hmapTex->Release();
	hmapTex = 0;
	//ReleaseCOM(hmapTex);
	return true;
}

bool Terrain::buildLayersBlendMap(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{

	bool result;

	Texture2D* textureArray = new Texture2D();
	result = textureArray->init(device, deviceContext, m_initInfo.layerMapFileNames);

	if (!result)
	{

		return false;
	}
	else
	{
		m_pLayesSRV = textureArray->getResourceView();
	}

	Texture2D* blendMap = new Texture2D();

	result = blendMap->init(device, m_initInfo.BlendMapFilename.c_str());

	if (!result)
	{

		return false;
	}
	else
	{
		m_pBlendMapsSRV = blendMap->getResourceView();
	}
	return true;
}

