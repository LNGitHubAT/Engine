#include "SkyBox.h"



SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

bool SkyBox::init( ID3D11ShaderResourceView * cubeMapSRV, SkyBoxShader* shader, Mesh * mesh)
{
	m_cubeMapSRV = cubeMapSRV;
	m_shader = shader;
	m_skyBoxMesh = mesh;
	return true;
}

bool SkyBox::render(ID3D11DeviceContext* deviceContext, Camera * cam)
{
	bindSkyBox(deviceContext);

	SkyBoxShader* shader = (SkyBoxShader*)m_shader;
	
	XMMATRIX viewMatrix = cam->getViewMatrix();
	XMFLOAT4X4 viewMatrixData;
	XMStoreFloat4x4(&viewMatrixData, viewMatrix);
	
	viewMatrixData._41 = 0;
	viewMatrixData._42 = 0;
	viewMatrixData._43 = 0;

	viewMatrix = XMLoadFloat4x4(&viewMatrixData);

	shader->setVSParameters(deviceContext, viewMatrix, cam->getProjectionMatrix());
	shader->setPSParameters(deviceContext, m_cubeMapSRV);

	deviceContext->DrawIndexed(m_skyBoxMesh->getIndexCount(), 0 , 0);

	unbindSkyBox(deviceContext);
	return true;
}

void SkyBox::setSRV(ID3D11ShaderResourceView * cubeMapSRV)
{
	m_cubeMapSRV = cubeMapSRV;
}

void SkyBox::bindSkyBox(ID3D11DeviceContext * deviceContext)
{
	m_skyBoxMesh->bindMesh(deviceContext);
	m_shader->bindShader(deviceContext);
}

void SkyBox::unbindSkyBox(ID3D11DeviceContext * deviceContext)
{
	m_skyBoxMesh->bindMesh(deviceContext);
	m_shader->bindShader(deviceContext);
}


