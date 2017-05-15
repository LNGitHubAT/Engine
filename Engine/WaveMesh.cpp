#include "WaveMesh.h"



WaveMesh::WaveMesh()
{
	currentTime = 0;
}


WaveMesh::~WaveMesh()
{
}

bool WaveMesh::init(ID3D11Device * pDevice, XMINT2 size, XMFLOAT2 XZlamda, float period, float magnitude)
{
	m_size = size;
	m_period = period;
	m_XZlamda = XZlamda;
	m_omega = 2 * PI_F / period;
	m_vertexCount = size.x * size.y;
	m_indexCount = (size.x - 1) * (size.y - 1) * 2 * 3;
	m_vertices = new BasicVertex[m_vertexCount];
	m_indices = new UINT[m_indexCount];
	m_magnitude = magnitude;

	XMFLOAT2 offset;
	offset.x = XZlamda.x / 2;
	offset.y = XZlamda.y / 2;

	XMFLOAT2 UV;
	UV.x = 1.0f / (size.y - 1);
	UV.y = 1.0f / (size.x - 1);

	for (UINT row = 0; row < size.x; row++)
	{
		for (UINT col = 0; col < size.y; col++)
		{
			m_vertices[row * size.y + col] = BasicVertex((float)row * offset.x, 0.0f, (float)col * offset.y, 0.0f, 1.0f, 0.0f, (float)row * UV.x, (float)col * UV.y);
		}
	}


	UINT face = 0;
	for (UINT row = 0; row < size.x - 1; ++row)
	{
		for (UINT col = 0; col < size.y - 1; ++col)
		{
			m_indices[face] = row * size.y + col;
			m_indices[face + 1] = row* size.y + col + 1;
			m_indices[face + 2] = (row + 1)* size.y + col;

			m_indices[face + 3] = (row + 1)*size.y + col;
			m_indices[face + 4] = row* size.y + col + 1;
			m_indices[face + 5] = (row + 1)* size.y + col + 1;

			face += 6; // next quad
		}
	}

	createBuffer(pDevice, m_vertices, m_indices , D3D11_USAGE_DYNAMIC);

	return true;
}

void WaveMesh::update(float deltaTime)
{
	currentTime += deltaTime;
	if (currentTime >= m_period)
	{
		currentTime -= m_period;
	}
	float currentAngle = 2 * PI_F / m_period * currentTime;
	for (UINT row = 0; row < m_size.x; row++)
	{
		for (UINT col = 0; col < m_size.y; col++)
		{
			XMFLOAT3 position = m_vertices[row * m_size.y + col].position;
			position.y = m_magnitude * (cos((m_XZlamda.x * position.x) + (m_XZlamda.y + position.z) + currentAngle));
			m_vertices[row * m_size.y + col].position = position;
		}
	}

	for (UINT row = 0; row < m_size.x; row++)
	{
		for (UINT col = 0; col < m_size.y; col++)
		{
			m_vertices[row * m_size.y + col].normal =  calculateNormal(row, col);
			m_vertices[row * m_size.y + col].textureCoord.y -= 0.1f * deltaTime;
		}
	}
	

}

void WaveMesh::bindMesh(ID3D11DeviceContext * pDeviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	UINT bufferNumber;
	BasicVertex* vertices;
	result = pDeviceContext->Map(m_pVertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	bufferNumber = 0;
	vertices = (BasicVertex*)mappedSubResource.pData;
	
	memcpy(vertices, m_vertices, sizeof(BasicVertex) * m_vertexCount);

	pDeviceContext->Unmap(m_pVertexBuffer, 0);

	
	Mesh::bindMesh(pDeviceContext);
}

XMFLOAT3 WaveMesh::calculateNormal(UINT row, UINT col)
{
	XMFLOAT3 normal;
	float hL = getY(row - 1, col);
	float hR = getY(row + 1, col);
	float hD = getY(row , col - 1);
	float hU = getY(row , col + 1);

	normal = XMFLOAT3(hL - hR, 2.0f, hD - hU);
	 XMVECTOR normalVector = (XMVector3Normalize(XMLoadFloat3(&normal)));
	 XMStoreFloat3(&normal, normalVector);
	 return normal;
	
}

float WaveMesh::getY(int row, int col)
{
	if (row < 0 || row >= m_size.x || col < 0 || col >= m_size.y)
		return 0;
	else
	{
		return m_vertices[row * m_size.y + col].position.y;
	}
}


