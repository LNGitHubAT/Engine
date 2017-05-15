#pragma once
#include "Mesh.h"
class WaveMesh :  public Mesh
{
public:
	WaveMesh();
	~WaveMesh();
	bool init(ID3D11Device * pDevice, XMINT2 size, XMFLOAT2 XZlamda, float period , float magnitude);
	virtual void update(float deltaTime);
	virtual void bindMesh(ID3D11DeviceContext * pDeviceContext);
	
private:
	XMFLOAT3 calculateNormal(UINT row, UINT col);
	float getY(int row, int col);
	XMINT2 m_size;
	XMFLOAT2 m_XZlamda;
	float m_period;
	float m_omega;
	BasicVertex *  m_vertices;
	UINT * m_indices;
	float m_magnitude;
	float currentTime;
};

