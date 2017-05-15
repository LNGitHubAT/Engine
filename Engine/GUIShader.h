#pragma once
#include "Shader.h"
class GUIShader :
	public Shader
{
public:
	virtual bool setPSParameter(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* shaderResourceView);
	
	GUIShader();
	~GUIShader();
};

