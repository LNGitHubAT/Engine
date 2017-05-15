#include "Texture2D.h"
#include <string>


Texture2D::Texture2D()
{
}


Texture2D::~Texture2D()
{
}

ID3D11ShaderResourceView * Texture2D::getResourceView()
{
	return m_textureResourceView;
}

bool Texture2D::init(ID3D11Device * device, const WCHAR * fileName)
{
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_textureResourceView, NULL);
	if (FAILED(result))
	{
		std::wstring msg = L"Can not find ";
		msg = msg + fileName;
		MessageBox(0, msg.c_str(), 0, 0);
		
		return false;
	}
	return true;
}

///---------
//Create texture array form many file
///---------

bool Texture2D::init(ID3D11Device * device, ID3D11DeviceContext* deviceContext,std::vector<WCHAR*> fileNames, DXGI_FORMAT format, UINT filter, UINT mipFilter)
{
	UINT size = fileNames.size();
	HRESULT result;
	std::vector<ID3D11Texture2D*> sourceTexture(size);
	for (UINT i = 0; i < size; i++)
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX11_FROM_FILE;
		loadInfo.Height = D3DX11_FROM_FILE;
		loadInfo.Depth = D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel = 0;
		loadInfo.BindFlags = 0;
		loadInfo.MipLevels = D3DX11_FROM_FILE;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		loadInfo.MiscFlags = 0;
		loadInfo.pSrcInfo = 0;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.Filter = filter;
		loadInfo.Format = format;
		loadInfo.MipFilter = mipFilter;
		result = D3DX11CreateTextureFromFile(device, fileNames[i], &loadInfo, NULL, (ID3D11Resource**)&sourceTexture[i], NULL);

		if (FAILED(result))
		{
			std::wstring msg = L"Can not find ";
			msg = msg + fileNames[i];
			MessageBox(0, msg.c_str(), 0, 0);
			return false;
		}
	}

	D3D11_TEXTURE2D_DESC textureElementDesc;
	sourceTexture[0]->GetDesc(&textureElementDesc);

	D3D11_TEXTURE2D_DESC textureArrayDecs;
	/*textureArrayDecs.ArraySize = size;
	textureArrayDecs.Width = textureElementDesc.Width;
	textureArrayDecs.Height = textureElementDesc.Height;
	textureArrayDecs.MipLevels = textureElementDesc.MipLevels;

	textureArrayDecs.Format = textureElementDesc.Format;
	textureArrayDecs.Usage = D3D11_USAGE_DEFAULT;
	textureArrayDecs.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureArrayDecs.CPUAccessFlags = 0;
	textureArrayDecs.MiscFlags = 0;
	textureArrayDecs.SampleDesc.Count = 1;

	textureArrayDecs.SampleDesc.Quality = 0;*/

	textureArrayDecs.ArraySize = size;
	textureArrayDecs.Width = textureElementDesc.Width;
	textureArrayDecs.Height = textureElementDesc.Height;
	textureArrayDecs.MipLevels = textureElementDesc.MipLevels;
	
	textureArrayDecs.Format = textureElementDesc.Format;
	textureArrayDecs.SampleDesc.Count = 1;
	textureArrayDecs.SampleDesc.Quality = 0;
	textureArrayDecs.Usage = D3D11_USAGE_DEFAULT;
	textureArrayDecs.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureArrayDecs.CPUAccessFlags = 0;
	textureArrayDecs.MiscFlags = 0;
	

	ID3D11Texture2D* textureArray;
	result = device->CreateTexture2D(&textureArrayDecs, 0, &textureArray);
	if (FAILED(result))
	{
		std::wstring msg = L"Failed to create texture Array ";
		
		MessageBox(0, msg.c_str(), 0, 0);
		return false;
	}
	//
	// copy all of the texture element into array
	//
	for (UINT textureElementID = 0; textureElementID < size; textureElementID++)
	{
		for (UINT mipLevel  = 0; mipLevel < textureArrayDecs.MipLevels; mipLevel++)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			// get subresource in source texture at miplevel
			deviceContext->Map(sourceTexture[textureElementID],mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);

			deviceContext->UpdateSubresource(
				textureArray,
				D3D11CalcSubresource(mipLevel, textureElementID, textureElementDesc.MipLevels),
				NULL,
				mappedTex2D.pData,
				mappedTex2D.RowPitch,
				mappedTex2D.DepthPitch
			);
			
			deviceContext->Unmap(sourceTexture[textureElementID], mipLevel);

		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC textureArrayViewDecs;
	textureArrayViewDecs.Format = textureArrayDecs.Format;
	textureArrayViewDecs.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	textureArrayViewDecs.Texture2DArray.MostDetailedMip = 0;
	textureArrayViewDecs.Texture2DArray.MipLevels = textureArrayDecs.MipLevels;
	textureArrayViewDecs.Texture2DArray.FirstArraySlice = 0;
	textureArrayViewDecs.Texture2DArray.ArraySize = size;


	
	result = device->CreateShaderResourceView(textureArray, &textureArrayViewDecs,& m_textureResourceView);
	if (FAILED(result))
	{
		std::wstring msg = L"Failed to create shader resource view ";

		MessageBox(0, msg.c_str(), 0, 0);
		return false;
	}

	return true;
}



void Texture2D::clean()
{
	m_textureResourceView->Release();
	m_textureResourceView = NULL;
}
