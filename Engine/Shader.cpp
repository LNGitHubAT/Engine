#include "Shader.h"



bool Shader::init(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	this-> psFileName = psFilename;
	this-> vsFileName = vsFilename;
	return initialize(device, hwnd);
}

//void Shader::outputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
//{
//	char* compileErrors;
//	unsigned long long bufferSize, i;
//	ofstream fout;
//
//
//	// Get a pointer to the error message text buffer.
//	compileErrors = (char*)(errorMessage->GetBufferPointer());
//
//	// Get the length of the message.
//	bufferSize = errorMessage->GetBufferSize();
//
//	// Open a file to write the error message to.
//	fout.open("shader-error.txt");
//
//	// Write out the error message.
//	for (i = 0; i<bufferSize; i++)
//	{
//		fout << compileErrors[i];
//	}
//
//	// Close the file.
//	fout.close();
//
//	// Release the error message.
//	errorMessage->Release();
//	errorMessage = 0;
//
//	// Pop a message up on the screen to notify the user to check the text file for compile errors.
//	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
//}

void Shader::bindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(m_pInputLayout);
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
}

void Shader::unBindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(NULL);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShaderResources(0, 0, NULL);
}

bool Shader::createInputLayout(ID3D11Device * device, ID3D10Blob * vertexShaderBuffer)
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


	inputLayoutDesc[1].SemanticName = "NORMAL";
	inputLayoutDesc[1].SemanticIndex = 0;
	inputLayoutDesc[1].InputSlot = 0;
	inputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[1].InstanceDataStepRate = 0;

	inputLayoutDesc[2].SemanticName = "TEXCOORD";
	inputLayoutDesc[2].SemanticIndex = 0;
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

bool Shader::createConstantBuffers(ID3D11Device * device)
{
	// Create matrixBufferDesc
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create buffer

	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
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
	return true;
}

bool Shader::createShaders(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;
	D3D11_SAMPLER_DESC samplerDesc;

	result = D3DCompileFromFile(vsFileName,
		NULL, NULL,
		"main", "vs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&vertexShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		else
		{
			MessageBox(hwnd, vsFileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}


	result = D3DCompileFromFile(psFileName,
		NULL, NULL, "main", "ps_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&pixelShaderBuffer,
		&errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFileName);
		}
		else
		{
			MessageBox(hwnd, psFileName, L"Missing Shader File", MB_OK);
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




void Shader::clean()
{
	m_pVertexShader->Release();
	m_pVertexShader = NULL;

	m_pPixelShader->Release();
	m_pPixelShader = NULL;

	m_pInputLayout->Release();
	m_pInputLayout = NULL;

	m_pMatrixBuffer->Release();
	m_pMatrixBuffer = NULL;
}

bool Shader::setVSParameter(ID3D11DeviceContext * deviceContext, 
						XMMATRIX worldMatrix, 
						XMMATRIX viewMatrix, 
						XMMATRIX projectionMatrix, 
						XMMATRIX worldReverseTranspose
						)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	MatrixBufferType * dataPointer;
	UINT bufferNumber;
	result = deviceContext->Map(m_pMatrixBuffer, 0,
								D3D11_MAP_WRITE_DISCARD,
								0,
								& mappedSubResource
		);

	if (FAILED(result))
		return false;;
	bufferNumber = 0;
	dataPointer = (MatrixBufferType*)mappedSubResource.pData;
	dataPointer->projection = projectionMatrix;
	dataPointer->view = viewMatrix;
	dataPointer->world = worldMatrix;
	dataPointer->worldReverseTranspose = worldReverseTranspose;
	deviceContext->Unmap(m_pMatrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	return true;

}

bool Shader::setPSParameter(ID3D11DeviceContext * deviceContext, Material mat, PointLight pointLight, XMFLOAT3 campos, Texture2D* texture2D, ID3D11ShaderResourceView* cubeMap,  bool isUseTexture, bool isUseCubeMap,  bool isUseAlphaCliptest,
								Fog fog)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	LightBufferType * dataPointer;
	UINT bufferNumber;
	result = deviceContext->Map(m_pLightBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubResource
	);

	if (FAILED(result))
		return false;;
	bufferNumber = 0;
	dataPointer = (LightBufferType*)mappedSubResource.pData;
	dataPointer->material = mat;
	dataPointer->pointLight = pointLight;
	dataPointer->cameraPosition = campos;
	dataPointer->isUseCubeMap = isUseCubeMap;
	dataPointer->isUseTexture = isUseTexture;
	dataPointer->isUseAlphaClipTest = isUseAlphaCliptest;
	dataPointer->fogColor = fog.color;
	dataPointer->fogRange = fog.range;
	dataPointer->fogStart = fog.start;
	deviceContext->Unmap(m_pLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pLightBuffer);
	

	// bind Texture
	//ID3D11ShaderResourceView* m_textureResourceView = NULL;
	if (texture2D)
	{
		ID3D11ShaderResourceView* textureResourceView = texture2D->getResourceView();
		deviceContext->PSSetShaderResources(0, 1, &(textureResourceView));
	}

	if (cubeMap)
	{
		deviceContext->PSSetShaderResources(1, 1, &(cubeMap));
	}
	// set samplerState
	
	return true;
}



Shader::Shader()
{
}


Shader::~Shader()
{
}
