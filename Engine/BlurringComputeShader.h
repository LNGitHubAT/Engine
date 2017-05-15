#pragma once
#include "ComputeShader.h"
class BlurringComputeShader : public ComputeShader
{
public:
	void setCSParameter( ID3D11DeviceContext* deviceContext,ID3D11ShaderResourceView * srv, ID3D11UnorderedAccessView* uov);
public:
	BlurringComputeShader();
	~BlurringComputeShader();
};

