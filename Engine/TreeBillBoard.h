#pragma once
#include "Mesh.h"
#include "TreeBillBoard.h"
#include "MathUtility.h"
class TreeBillBoard :
	public Mesh
{
public:
	TreeBillBoard();
	~TreeBillBoard();
	bool init(ID3D11Device * pDevice, XMFLOAT3 boundary, UINT quantity,  XMFLOAT2 size, D3D11_USAGE flag = D3D11_USAGE_DEFAULT);
	virtual void bindMesh(ID3D11DeviceContext * pDeviceContext) override;
	
private:
	struct BillBoardVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 size;
	};
	bool  createBuffer(ID3D11Device * pDevice, BillBoardVertex* vertices, D3D11_USAGE flag = D3D11_USAGE_DEFAULT);

};

