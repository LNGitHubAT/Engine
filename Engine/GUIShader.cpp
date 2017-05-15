#include "GUIShader.h"




bool GUIShader::setPSParameter(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * shaderResourceView)
{
	if (shaderResourceView)
	{
		deviceContext->PSSetShaderResources(0, 1, &(shaderResourceView));
	}
	return true;
}

GUIShader::GUIShader()
{
}


GUIShader::~GUIShader()
{
}
