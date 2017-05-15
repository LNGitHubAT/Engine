#pragma once

#include "Mesh.h"
#include "Object.h"
#include "GUIShader.h"
#include "Texture2D.h"
// image only
class GUI
{
public:
	GUI();
	~GUI();
	bool init(ID3D11Device * device, XMFLOAT2 size, XMFLOAT2 position, GUIShader * shader, ID3D11ShaderResourceView * shaderResourceView);
	bool render(ID3D11DeviceContext* deviceContext);
	bool update();

private:
	Mesh* m_mesh;
	GUIShader * m_guiShader;
	ID3D11ShaderResourceView * m_shaderResourceView;
protected:
	bool bindGUIObject(ID3D11DeviceContext* deviceContext);
	bool unbindGUIObject(ID3D11DeviceContext* deviceContext);
};

