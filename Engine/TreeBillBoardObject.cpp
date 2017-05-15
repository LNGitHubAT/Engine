#include "TreeBillBoardObject.h"
#include "PointLightObject.h"


TreeBillBoardObject::TreeBillBoardObject()
{
}

TreeBillBoardObject::TreeBillBoardObject(Mesh * mesh, Shader * shader, Material material, Texture2D * texture, bool isUseAnphaClipTest, bool isTransparent, bool isUseBackCulling, bool isMirror, bool isPlane)
	:
	GameObject(mesh, shader, material, texture, NULL, NULL,isUseAnphaClipTest, isTransparent, isUseBackCulling, isMirror, isPlane)
{
}


TreeBillBoardObject::~TreeBillBoardObject()
{
}

void TreeBillBoardObject::render(ID3D11DeviceContext * deviceContext, Camera * cam, PointLightObject * pointLight, Fog fog, bool isLowestQuality)
{
	bindObject(deviceContext);
	BillBoardShader* bbshader = (BillBoardShader*)m_shader;
	//m_shader->setVSParameter(deviceContext, getWorldMatrix() , cam->getViewMatrix(), cam->getProjectionMatrix(), getWorldMatrixInverseTranspose());
	XMMATRIX viewprojectionMatrix = XMMatrixMultiply(cam->getViewMatrix(), cam->getProjectionMatrix());
	bbshader->setGSParameters(deviceContext, viewprojectionMatrix, cam->getPosition());
	bbshader->setPSParameter(deviceContext, m_material, pointLight->getData(), cam->getPosition(), m_pTexture, NULL, m_isUsetexture, m_isUseCubeMap, m_isUseAnphaClipTest, fog);
	deviceContext->Draw(m_mesh->getVertextCount(), 0);
	unbindObject(deviceContext);
}
