#include "TreeBillBoard.h"



TreeBillBoard::TreeBillBoard()
{
}


TreeBillBoard::~TreeBillBoard()
{
}

bool TreeBillBoard::init(ID3D11Device * pDevice, XMFLOAT3 boundary, UINT quantity, XMFLOAT2 size, D3D11_USAGE flag)
{
	BillBoardVertex* vertices = new BillBoardVertex[quantity];
	MathUtility::seed();
	for (UINT i = 0; i < quantity; i++)
	{
		XMFLOAT3 positionR = XMFLOAT3(MathUtility::random(-boundary.x, boundary.x), boundary.y, MathUtility::random(-boundary.z, boundary.z));
		XMFLOAT2 sizeR = XMFLOAT2(MathUtility::random(0.8f, 1.0f) * size.x, MathUtility::random(0.8f, 1.0f) * size.y);
		vertices[i].position = positionR;
		vertices[i].size = sizeR;
	}
	//BillBoardVertex vertex;
	
	
	//vertices[0] = vertex;
	m_vertexCount = quantity;
	createBuffer(pDevice, vertices);

	return false;
}

void TreeBillBoard::bindMesh(ID3D11DeviceContext * pDeviceContext)
{
	UINT stride;
	UINT offset;
	stride = sizeof(BillBoardVertex);
	offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

bool TreeBillBoard::createBuffer(ID3D11Device * pDevice, BillBoardVertex * vertices, D3D11_USAGE flag)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData ;
	
	// Create vertexBuffer  desciption
	vertexBufferDesc.Usage = flag;
	vertexBufferDesc.ByteWidth = sizeof(BillBoardVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (flag == D3D11_USAGE_DYNAMIC)
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
	return true;
}
