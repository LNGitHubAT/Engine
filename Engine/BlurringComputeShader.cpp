#include "BlurringComputeShader.h"



void BlurringComputeShader::setCSParameter(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView * srv, ID3D11UnorderedAccessView * uov)
{
	deviceContext->CSSetShaderResources(0, 1, &srv);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &uov, NULL);
}

BlurringComputeShader::BlurringComputeShader()
{
}


BlurringComputeShader::~BlurringComputeShader()
{
}
