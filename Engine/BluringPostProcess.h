#pragma once
#include <d3d11.h>
#include "BlurringComputeShader.h"
class BluringPostProcess
{
public:
	BluringPostProcess( ID3D11Device* device, int width, int height, BlurringComputeShader * verticalBlur, BlurringComputeShader * horizontalBlur);
	~BluringPostProcess();
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11ShaderResourceView* getShaderResourceView();
	ID3D11UnorderedAccessView* getUnorderedAccessView();

	void blur(ID3D11DeviceContext * deviceContext, int count);
private:
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11UnorderedAccessView* unorderedAccessView;
	ID3D11ShaderResourceView* outputshaderResourceView;
	ID3D11UnorderedAccessView* outputunorderedAccessView;
	BlurringComputeShader * verticalBlur;
	BlurringComputeShader * horizontalBlur;

	int m_width;
	int m_height;
};

