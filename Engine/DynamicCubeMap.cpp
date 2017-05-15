#include "DynamicCubeMap.h"



DynamicCubeMap::DynamicCubeMap()
{
}


DynamicCubeMap::~DynamicCubeMap()
{
}

bool DynamicCubeMap::init(ID3D11Device* device)
{
	
	//
	// Cubemap is a special texture array with 6 elements. We
	// bind this as a render target to draw to the cube faces,
	// and also as a shader resource, so we can use it in a pixel shader.
	//

	HRESULT result;
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_sCubeMapSize;
	texDesc.Height = m_sCubeMapSize;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS |
		D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D* cubeTex = 0;
	result  = device->CreateTexture2D(&texDesc, 0, &cubeTex);
	if (FAILED(result))
	{
		return false;
	}

	
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = 0;
	// Only create a view to one array element.
	rtvDesc.Texture2DArray.ArraySize = 1;
	for (int i = 0; i < 6; ++i)
	{
		// Create a render target view to the ith element.
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		result = device->CreateRenderTargetView(cubeTex, &rtvDesc, &m_pRenderTargetViews[i]);

		if (FAILED(result))
			return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	result = device->CreateShaderResourceView( cubeTex, &srvDesc, &m_srv);

	if (FAILED(result))
		return false;


	for (UINT i = 0; i < 6; i++)
	{
		cam[i] = new Camera(XM_PI / 2, SCREEN_NEAR, SCREEN_DEPTH, 1.0f);
	}

	return true;
}

ID3D11ShaderResourceView* DynamicCubeMap::getShaderResourceView()
{
	return m_srv;
}

ID3D11RenderTargetView ** DynamicCubeMap::getRenderTargetViews()
{
	return m_pRenderTargetViews;
}

Camera** DynamicCubeMap::buildCamrera(float x, float y, float z)
{

	for (UINT i = 0; i < 6; i++)
	{
		cam[i]->reset();
		cam[i]->setPosition(XMFLOAT3(x, y, z));
	}
	

	cam[0]->yaw(XMConvertToRadians(90.0f));
	cam[1]->yaw(XMConvertToRadians(-90.0f));
	cam[2]->pitch(XMConvertToRadians(-90.0f));
	cam[3]->pitch(XMConvertToRadians(90.0f));
	//cam[4]->yaw(XMConvertToRadians(0.0ff)); cam4 naturally at z+
	cam[5]->yaw(XMConvertToRadians(180.0f));
	return cam;
}

ID3D11RenderTargetView ** DynamicCubeMap::getRenderTargetView()
{
	return m_pRenderTargetViews;
}


