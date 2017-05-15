#include "BiillBoardShader.h"



BillBoardShader::BillBoardShader()
{
}


BillBoardShader::~BillBoardShader()
{
}

bool BillBoardShader::init(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * gsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* geometryShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[2];
	UINT numberElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
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

	result = D3DCompileFromFile(gsFilename,
		NULL, NULL,
		"main", "gs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&geometryShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, gsFilename);
		}
		else
		{
			MessageBox(hwnd, gsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	result = D3DCompileFromFile(psFilename,
		NULL, NULL, 
		"main", "ps_5_0",
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

	result = device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(),
		geometryShaderBuffer->GetBufferSize(),
		NULL,
		&m_pGeometryShader);
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


	// Create Inputlayout desc
	inputLayoutDesc[0].SemanticName = "POSITION";
	inputLayoutDesc[0].SemanticIndex = 0;
	inputLayoutDesc[0].InputSlot = 0;
	inputLayoutDesc[0].AlignedByteOffset = 0;
	inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[0].InstanceDataStepRate = 0;


	inputLayoutDesc[1].SemanticName = "SIZE";
	inputLayoutDesc[1].SemanticIndex = 0;
	inputLayoutDesc[1].InputSlot = 0;
	inputLayoutDesc[1].AlignedByteOffset = 12;
	inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[1].InstanceDataStepRate = 0;

	// get number of element
	numberElements = sizeof(inputLayoutDesc) / sizeof(inputLayoutDesc[0]);
	//Create input layout
	result = device->CreateInputLayout(inputLayoutDesc, numberElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_pInputLayout);

	if (FAILED(result))
	{
		return false;
	}


	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// Create matrixBufferDesc

	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create buffer

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}





	// Create lightBufferDesc

	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create buffer

	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
	if (FAILED(result))
	{
		return false;
	}



	// Create Texutre samplerState
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

	result = device->CreateSamplerState(&samplerDesc, &m_pSamplerState);

	return true;
}

void BillBoardShader::bindShader(ID3D11DeviceContext * deviceContext)
{
	//Shader::bindShader(deviceContext);
	deviceContext->IASetInputLayout(m_pInputLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);

	deviceContext->GSSetShader(m_pGeometryShader, NULL, 0);

	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	
}

void BillBoardShader::unBindShader(ID3D11DeviceContext * deviceContext)
{
	//Shader::unBindShader(deviceContext);
	deviceContext->IASetInputLayout(NULL);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, NULL);
	deviceContext->PSSetShader(NULL, NULL, 0);
}

bool BillBoardShader::setGSParameters(ID3D11DeviceContext * deviceContext, XMMATRIX viewProjectionMatrix, XMFLOAT3 camposition)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	MatrixBufferType * dataPointer;
	UINT bufferNumber;
	result = deviceContext->Map(m_pMatrixBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;;
	bufferNumber = 0;
	dataPointer = (MatrixBufferType*)mappedSubResource.pData;
	dataPointer->viewProjectionMatrix = viewProjectionMatrix;
	dataPointer->camposition = camposition;
	deviceContext->Unmap(m_pMatrixBuffer, 0);
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	return true;
}
