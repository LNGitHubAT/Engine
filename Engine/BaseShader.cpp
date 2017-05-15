#include "BaseShader.h"



BaseShader::BaseShader()
{
}


BaseShader::~BaseShader()
{
}

void BaseShader::bindShader(ID3D11DeviceContext * deviceContext)
{

}

void BaseShader::unBindShader(ID3D11DeviceContext * deviceContext)
{
	
}

bool BaseShader::createInputLayout(ID3D11Device * device, ID3D10Blob * vertexShaderBuffer)
{
	return true;
}

bool BaseShader::createSampleState(ID3D11Device * device)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result = device->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (FAILED(result))
		return false;
	return true;
}

bool BaseShader::createConstantBuffers(ID3D11Device * device)
{
	return true;
}

bool BaseShader::createShaders(ID3D11Device * device, HWND hwnd)
{
	
	return true;
}

bool BaseShader::initialize(ID3D11Device * device , HWND hwnd)
{
	bool result;
	result = createShaders(device , hwnd);
	if (!result)
		return false;
	// create uniform buffer
	result = createConstantBuffers(device);
	if (!result)
		return false;
	// Create Texutre samplerState
	result = createSampleState(device);
	if (!result)
		return false;
	return true;
}

void BaseShader::outputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

