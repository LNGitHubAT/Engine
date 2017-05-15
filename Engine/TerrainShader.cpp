#include "TerrainShader.h"



TerrainShader::TerrainShader()
{
}


TerrainShader::~TerrainShader()
{
}

bool TerrainShader::init(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * hsFilename, WCHAR * dsFilename, WCHAR * psFilename)
{
	this->vsFilename = vsFilename;
	this->hsFilename = hsFilename;
	this->dsFilename = dsFilename;
	this->psFilename = psFilename;
	
	return initialize(device, hwnd);
}

void TerrainShader::bindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(m_pInputLayout);
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->HSSetShader(m_pHullShader, NULL, 0);
	deviceContext->DSSetShader(m_pDomainShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	//deviceContext->VSSetSamplers(0, 1, &m_pSamplerState);
	//deviceContext->DSSetSamplers(0, 1, &m_pSamplerState);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
}

void TerrainShader::unBindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(NULL);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->HSSetShader(NULL, NULL, 0);
	deviceContext->DSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);

	//deviceContext->PSSetSamplers(0, 1, NULL);

	//deviceContext->VSSetSamplers(0, 1, NULL);
	//deviceContext->DSSetSamplers(0, 1, NULL);
	//deviceContext->PSSetSamplers(0, 1, NULL);
}



bool TerrainShader::createInputLayout(ID3D11Device * device, ID3D10Blob * vertexShaderBuffer)
{
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3];
	UINT numberElements;

	inputLayoutDesc[0].SemanticName = "POSITION";
	inputLayoutDesc[0].SemanticIndex = 0;
	inputLayoutDesc[0].InputSlot = 0;
	inputLayoutDesc[0].AlignedByteOffset = 0;
	inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[0].InstanceDataStepRate = 0;


	inputLayoutDesc[1].SemanticName = "TEXCOORD";
	inputLayoutDesc[1].SemanticIndex = 0;
	inputLayoutDesc[1].InputSlot = 0;
	inputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[1].InstanceDataStepRate = 0;

	inputLayoutDesc[2].SemanticName = "TEXCOORD";
	inputLayoutDesc[2].SemanticIndex = 1;
	inputLayoutDesc[2].InputSlot = 0;
	inputLayoutDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[2].InstanceDataStepRate = 0;

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


bool TerrainShader::createConstantBuffers(ID3D11Device * device)
{

	D3D11_BUFFER_DESC perVertexBufferDesc;
	//D3D11_BUFFER_DESC lightBufferDesc;

	perVertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	perVertexBufferDesc.ByteWidth = sizeof(PerVertexBuffer);
	perVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perVertexBufferDesc.MiscFlags = 0;
	perVertexBufferDesc.StructureByteStride = 0;

	// Create buffer

	HRESULT result = device->CreateBuffer(&perVertexBufferDesc, NULL, &perVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC perPatchBufferDesc;
	//D3D11_BUFFER_DESC lightBufferDesc;

	perPatchBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perPatchBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	perPatchBufferDesc.ByteWidth = sizeof(PerPatchBuffer);
	perPatchBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perPatchBufferDesc.MiscFlags = 0;
	perPatchBufferDesc.StructureByteStride = 0;

	// Create buffer

	result = device->CreateBuffer(&perPatchBufferDesc, NULL, &perPatchBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC perTesselatedPointBufferDesc;
	//D3D11_BUFFER_DESC lightBufferDesc;

	perTesselatedPointBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perTesselatedPointBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	perTesselatedPointBufferDesc.ByteWidth = sizeof(PerTesselatedPointBuffer);
	perTesselatedPointBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perTesselatedPointBufferDesc.MiscFlags = 0;
	perTesselatedPointBufferDesc.StructureByteStride = 0;

	// Create buffer

	result = device->CreateBuffer(&perTesselatedPointBufferDesc, NULL, &perTesselatedPointBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC perPixelBufferDesc;
	//D3D11_BUFFER_DESC lightBufferDesc;

	perPixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perPixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	perPixelBufferDesc.ByteWidth = sizeof(PerPixelBuffer);
	perPixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perPixelBufferDesc.MiscFlags = 0;
	perPixelBufferDesc.StructureByteStride = 0;

	// Create buffer

	result = device->CreateBuffer(&perPixelBufferDesc, NULL, &perPixelBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TerrainShader::createShaders(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;
	D3D11_SAMPLER_DESC samplerDesc;

	// 
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



	result = D3DCompileFromFile(hsFilename,
		NULL, NULL,
		"main", "hs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&hullShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, hsFilename);
		}
		else
		{
			MessageBox(hwnd, hsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}



	result = D3DCompileFromFile(dsFilename,
		NULL, NULL,
		"main", "ds_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&domainShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, dsFilename);
		}
		else
		{
			MessageBox(hwnd, dsFilename, L"Missing Shader File", MB_OK);
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



	result = device->CreateHullShader(hullShaderBuffer->GetBufferPointer(),
		hullShaderBuffer->GetBufferSize(),
		NULL,
		&m_pHullShader);
	if (FAILED(result))
	{
		return false;
	}


	result = device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(),
		domainShaderBuffer->GetBufferSize(),
		NULL,
		&m_pDomainShader);
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
	createInputLayout(device, vertexShaderBuffer);


	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	hullShaderBuffer->Release();
	hullShaderBuffer = 0;

	domainShaderBuffer->Release();
	domainShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

bool TerrainShader::setVSParameter(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix, ID3D11ShaderResourceView * heightMapSRV)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	PerVertexBuffer * dataPointer;
	UINT bufferNumber;
	result = deviceContext->Map(perVertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;
	bufferNumber = 0;
	dataPointer = (PerVertexBuffer*)mappedSubResource.pData;
	dataPointer->worldMatrix = worldMatrix;
	deviceContext->Unmap(perVertexBuffer, 0);
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &perVertexBuffer);

	deviceContext->VSSetShaderResources(2, 1,	&heightMapSRV);

	return true;
}

bool TerrainShader::setHSParameter(ID3D11DeviceContext * deviceContext, XMFLOAT3 cameraPosition, float minDist, float maxDist, float maxTess, float minTess, XMFLOAT4 frustumPlanes[6])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	PerPatchBuffer * dataPointer;
	UINT bufferNumber;

	result = deviceContext->Map(perPatchBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;
	bufferNumber = 0;
	dataPointer = (PerPatchBuffer*)mappedSubResource.pData;
	dataPointer->cameraPosition = cameraPosition;
	dataPointer->maxDist = maxDist;
	dataPointer->minDist = minDist;
	dataPointer->maxTess = maxTess;
	dataPointer->minTess = minTess;
	memcpy(dataPointer->frustumPlane, frustumPlanes, sizeof(XMFLOAT4) * 6);
	//dataPointer->frustumPlane = frustumPlanes;
	deviceContext->Unmap(perPatchBuffer, 0);

	deviceContext->HSSetConstantBuffers(bufferNumber, 1, &perPatchBuffer);

	//deviceContext->VSSetShaderResources(0, 1, &heightMapSRV);

	return true;
}

bool TerrainShader::setDSParameter(ID3D11DeviceContext * deviceContext, XMMATRIX wolrdMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float textureScale, ID3D11ShaderResourceView * heightMapSRV)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	PerTesselatedPointBuffer * dataPointer;
	UINT bufferNumber;

	result = deviceContext->Map(perTesselatedPointBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;
	bufferNumber = 0;
	dataPointer = (PerTesselatedPointBuffer*)mappedSubResource.pData;
	dataPointer->projectionMatrix = projectionMatrix;
	dataPointer->viewMatrix = viewMatrix;
	dataPointer->wolrdMatrix = wolrdMatrix;
	dataPointer->textureScale = textureScale;
	
	deviceContext->Unmap(perTesselatedPointBuffer, 0);

	deviceContext->DSSetConstantBuffers(bufferNumber, 1, &perTesselatedPointBuffer);

	deviceContext->DSSetShaderResources(3, 1, &heightMapSRV);


	return true;
}

bool TerrainShader::setPSParameter(ID3D11DeviceContext * deviceContext, Material material, PointLight pointLight, XMFLOAT3 cameraPosition, XMFLOAT4 fogColor, float fogStart, float fogRange, float worldCellSpace, XMFLOAT2 heightMapCellSpace, ID3D11ShaderResourceView * heightMapSRV, ID3D11ShaderResourceView * blendMapSRV, ID3D11ShaderResourceView * LayersMapSRV)
{


	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	PerPixelBuffer * dataPointer;
	UINT bufferNumber;

	result = deviceContext->Map(perPixelBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;

	bufferNumber = 0;
	dataPointer = (PerPixelBuffer*)mappedSubResource.pData;

	dataPointer->cameraPosition = cameraPosition;
	dataPointer->fogColor = fogColor;
	dataPointer->fogRange = fogRange;
	dataPointer->fogStart = fogStart;
	dataPointer->heightMapCellSpace = heightMapCellSpace;
	dataPointer->material = material;
	dataPointer->pointLight = pointLight;
	dataPointer->worldCellSpace = worldCellSpace;

	deviceContext->Unmap(perPixelBuffer, 0);

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &perPixelBuffer);


	deviceContext->PSSetShaderResources(0, 1, &LayersMapSRV);
	deviceContext->PSSetShaderResources(1, 1, &blendMapSRV);
	deviceContext->PSSetShaderResources(4, 1, &heightMapSRV);

	//deviceContext->VSSetShaderResources(0, 1, &heightMapSRV);

	return true;
}




