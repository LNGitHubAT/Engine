#include "GUI.h"



GUI::GUI()
{
	
}


GUI::~GUI()
{
}

bool GUI::init(ID3D11Device * device, XMFLOAT2 size, XMFLOAT2 position, GUIShader * shader , ID3D11ShaderResourceView* shaderResourceView)
{
	m_mesh = new Mesh();
	m_mesh->initGUIQuad(device, size, position);
	m_guiShader = shader;
	m_shaderResourceView = shaderResourceView;
	return true;
}

bool GUI::render(ID3D11DeviceContext * deviceContext)
{
	bindGUIObject(deviceContext);
	m_guiShader->setPSParameter(deviceContext, m_shaderResourceView);
	deviceContext->DrawIndexed(m_mesh->getIndexCount(), 0, 0);
	unbindGUIObject(deviceContext);
	return true;
}

bool GUI::update()
{
	return false;
}

bool GUI::bindGUIObject(ID3D11DeviceContext * deviceContext)
{
	m_mesh->bindMesh(deviceContext);
	m_guiShader->bindShader(deviceContext);
	return true;
}

bool GUI::unbindGUIObject(ID3D11DeviceContext * deviceContext)
{
	m_mesh->unBindMesh(deviceContext);
	m_guiShader->unBindShader(deviceContext);
	return true;
}
