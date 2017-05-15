#include "SkyBoxShader.h"



bool SkyBoxShader::init(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	this->vsFilename = vsFilename;
	this->psFilename = psFilename;
	return initialize(device, hwnd);
}

void SkyBoxShader::bindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(m_pInputLayout);
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
}

void SkyBoxShader::unBindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(NULL);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShaderResources(0, 0, NULL);
}

bool SkyBoxShader::createInputLayout(ID3D11Device * device, ID3D10Blob * vertexShaderBuffer)
{
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[1];
	UINT numberElements;

	inputLayoutDesc[0].SemanticName = "POSITION";
	inputLayoutDesc[0].SemanticIndex = 0;
	inputLayoutDesc[0].InputSlot = 0;
	inputLayoutDesc[0].AlignedByteOffset = 0;
	inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[0].InstanceDataStepRate = 0;

	// get number of element
	numberElements = sizeof(inputLayoutDesc) / sizeof(inputLayoutDesc[0]);
	//Create input layout
	HRESULT result = device->CreateInputLayout(inputLayoutDesc, numberElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_pInputLayout);

	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool SkyBoxShader::createConstantBuffers(ID3D11Device * device)
{
	// Create matrixBufferDesc
	D3D11_BUFFER_DESC perVertexDesc;

	perVertexDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	perVertexDesc.ByteWidth = sizeof(PerVertexBuffer);
	perVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perVertexDesc.MiscFlags = 0;
	perVertexDesc.StructureByteStride = 0;

	// Create buffer

	HRESULT result = device->CreateBuffer(&perVertexDesc, NULL, &m_perVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;

}

bool SkyBoxShader::createShaders(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;
	D3D11_SAMPLER_DESC samplerDesc;

	result = D3DCompileFromFile(vsFilename,
		NULL, NULL,
		"main", "vs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&vertexShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}


	result = D3DCompileFromFile(psFilename,
		NULL, NULL, "main", "ps_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&pixelShaderBuffer,
		&errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		NULL,
		&m_pVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_pPixelShader);
	if (FAILED(result))
	{
		return false;
	}



	result = createInputLayout(device, vertexShaderBuffer);
	if (!result)
	{
		return false;
	}
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	vertexShaderBuffer = 0;

	return true;
}

bool SkyBoxShader::setVSParameters(ID3D11DeviceContext * deviceContext, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	PerVertexBuffer * dataPointer;
	UINT bufferNumber;
	result = deviceContext->Map(m_perVertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;;
	bufferNumber = 0;
	dataPointer = (PerVertexBuffer*)mappedSubResource.pData;
	dataPointer->projectionMatrix = projectionMatrix;
	dataPointer->viewMatrix = viewMatrix;
	deviceContext->Unmap(m_perVertexBuffer, 0);
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_perVertexBuffer);

	return true;
}

bool SkyBoxShader::setPSParameters(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * cubeMapSRV)
{
	deviceContext->PSSetShaderResources(0, 1, &cubeMapSRV);
	return true;
}

SkyBoxShader::SkyBoxShader()
{
}


SkyBoxShader::~SkyBoxShader()
{
}
