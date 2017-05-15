#include "ComputeShader.h"



ComputeShader::ComputeShader()
{
}


ComputeShader::~ComputeShader()
{
}

bool ComputeShader::init(ID3D11Device * device, HWND hwnd, WCHAR * csFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* computeShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	errorMessage = NULL;
	computeShaderBuffer = NULL;
	pixelShaderBuffer = NULL;
	D3D11_SAMPLER_DESC samplerDesc;

	result = D3DCompileFromFile(csFileName,
		NULL, NULL,
		"main", "cs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&computeShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, csFileName);
		}
		else
		{
			MessageBox(hwnd, csFileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}


	

	result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(),
		computeShaderBuffer->GetBufferSize(),
		NULL,
		&m_pComputeShader);
	if (FAILED(result))
	{
		return false;
	}

	
	return false;
}

void ComputeShader::bindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->CSSetShader(m_pComputeShader, 0, 0);
}

void ComputeShader::unBindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->CSSetShader(0, 0, 0);
}
