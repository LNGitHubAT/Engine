#include "BluringPostProcess.h"



BluringPostProcess::BluringPostProcess(ID3D11Device* device, int width, int height, BlurringComputeShader * verticalBlur, BlurringComputeShader* horizontalBlur)
{

	D3D11_TEXTURE2D_DESC blurredTexDesc;
	blurredTexDesc.Width = width;
	blurredTexDesc.Height = height;
	blurredTexDesc.MipLevels = 1;
	blurredTexDesc.ArraySize = 1;
	blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	blurredTexDesc.SampleDesc.Count = 1;
	blurredTexDesc.SampleDesc.Quality = 0;
	blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
	blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	blurredTexDesc.CPUAccessFlags = 0;
	blurredTexDesc.MiscFlags = 0;

	ID3D11Texture2D* textureOutput;
	(device->CreateTexture2D(&blurredTexDesc, NULL, &textureOutput));


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	(device->CreateShaderResourceView(textureOutput, &srvDesc, &outputshaderResourceView));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	//device->CreateShaderResourceView(textureOutput, NULL, &outputshaderResourceView);
	device->CreateUnorderedAccessView(textureOutput, &uavDesc, &outputunorderedAccessView);
















	D3D11_TEXTURE2D_DESC textureDecs;
	textureDecs.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
	textureDecs.Width = width;
	textureDecs.Height = height;
	textureDecs.ArraySize = 1;
	textureDecs.CPUAccessFlags = 0;
	textureDecs.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDecs.MipLevels = 1;
	textureDecs.MiscFlags = 0;
	textureDecs.SampleDesc.Count = 1;
	textureDecs.SampleDesc.Quality = 0;
	textureDecs.Usage = D3D11_USAGE_DEFAULT;
	
	ID3D11Texture2D* texture;
	device->CreateTexture2D(&textureDecs, NULL, &texture);

	device->CreateRenderTargetView(texture, NULL, &renderTargetView);
	device->CreateShaderResourceView(texture, NULL, &shaderResourceView);
	device->CreateUnorderedAccessView(texture, NULL, &unorderedAccessView);


	

	
	



	




	textureOutput->Release();
	textureOutput = 0;

	texture->Release();
	texture = 0;
	this->verticalBlur = verticalBlur;
	this->horizontalBlur = horizontalBlur;
	m_width = width;
	m_height = height;
}


BluringPostProcess::~BluringPostProcess()
{
}

ID3D11RenderTargetView * BluringPostProcess::getRenderTargetView()
{
	return renderTargetView;
}

ID3D11ShaderResourceView * BluringPostProcess::getShaderResourceView()
{
	return shaderResourceView;
}

ID3D11UnorderedAccessView * BluringPostProcess::getUnorderedAccessView()
{
	return unorderedAccessView;
}

void BluringPostProcess::blur(ID3D11DeviceContext * deviceContext, int count)
{
	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
	ID3D11UnorderedAccessView* nullUAV[1] = { 0 };

	horizontalBlur->bindShader(deviceContext);

	// Blur horizontally count times
	//numthread in a thread group is fixed in compute shader ( 256 , 1 , 1)
	UINT numGroupsThreadX = (UINT)ceilf(m_width/ 256.0f);
	
	for (int i = 0; i < count; i++)
	{
		horizontalBlur->setCSParameter(deviceContext, shaderResourceView, outputunorderedAccessView);
		deviceContext->Dispatch(numGroupsThreadX, m_height, 1);
	}

	horizontalBlur->unBindShader(deviceContext);
	
	deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	deviceContext->CSSetShaderResources(0, 1, nullSRV);



	verticalBlur->bindShader(deviceContext);

	// Blur horizontally count times
	//numthread in a thread group is fixed in compute shader ( 256 , 1 , 1)
	UINT numGroupsTheardY = (UINT)ceilf(m_height / 256.0f);

	for (int i = 0; i < count; i++)
	{
		verticalBlur->setCSParameter(deviceContext, outputshaderResourceView, unorderedAccessView);
		deviceContext->Dispatch(m_width, numGroupsTheardY, 1);
	}

	verticalBlur->unBindShader(deviceContext);

	deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	deviceContext->CSSetShaderResources(0, 1, nullSRV);
	
}

