#include "D3DManager.h"




D3DManager::D3DManager()
{
	m_pDevice = NULL;
	m_pDeviceContext = NULL;
	m_pSwapChain = NULL;
	m_pDepthStencilBuffer = NULL;
	m_pRenderTargetView = NULL;
	m_pDepthStencilView = NULL;
}


D3DManager::~D3DManager()
{
}

bool D3DManager::init(int screenWidth, int screenHeight, bool enable4xMsAA, bool vsync, HWND hwnd,bool  fullscreen, float screenDepth, float screenNear)
{
	IDXGIFactory* factory;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D * pBackBuffer;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_BLEND_DESC blendDecs;
	UINT m4xMsaaQuality;
	HRESULT result;


	//Create swapChainDesc
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;


	//(md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality)); assert(m4xMsaaQuality > 0);

	if (!enable4xMsAA)
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;

	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	featureLevel = D3D_FEATURE_LEVEL_11_0;


	result  = D3D11CreateDeviceAndSwapChain(NULL,
								D3D_DRIVER_TYPE_HARDWARE,
								NULL,
								0,
								&featureLevel, 1,
								D3D11_SDK_VERSION,
								&swapChainDesc, &m_pSwapChain,
								&m_pDevice, NULL, &m_pDeviceContext);

	if (FAILED(result))
	{
		return false;
	}

	//get Back Buffer Pointer
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (VOID**)&pBackBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//Create RenderTargetView withh default desc
	result =  m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}
	//Realease pointer to backBuffer
	pBackBuffer->Release();
	pBackBuffer = NULL;

	//Create DepthBuffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result =  m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);

	if (FAILED(result))
	{
		return false;
	}

	// Create DepthStencil View
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;



	result =   m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}




	float CubeMapSize = 256;


	D3D11_TEXTURE2D_DESC cubeMapTexutureDecs;
	cubeMapTexutureDecs.Width = CubeMapSize;
	cubeMapTexutureDecs.Height = CubeMapSize;
	cubeMapTexutureDecs.MipLevels = 1;
	cubeMapTexutureDecs.ArraySize = 1;
	cubeMapTexutureDecs.SampleDesc.Count = 1;
	cubeMapTexutureDecs.SampleDesc.Quality = 0;
	cubeMapTexutureDecs.Format = DXGI_FORMAT_D32_FLOAT;
	cubeMapTexutureDecs.Usage = D3D11_USAGE_DEFAULT;
	cubeMapTexutureDecs.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	cubeMapTexutureDecs.CPUAccessFlags = 0;
	cubeMapTexutureDecs.MiscFlags = 0;
	ID3D11Texture2D* cubeMapTexture = 0;

	result = m_pDevice->CreateTexture2D(&cubeMapTexutureDecs, 0, &cubeMapTexture);
	if (FAILED(result))
	{
		return false;
	}
	// Create the depth stencil view for the entire buffer.
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = cubeMapTexutureDecs.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	result = m_pDevice->CreateDepthStencilView(cubeMapTexture,
		&dsvDesc, &m_pDynamicCubeMapDepthStencilView);

	if (FAILED(result))
	{
		return false;
	}

	cubeMapTexture->Release();
	cubeMapTexture = NULL;





	result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDynamicCubeMapDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}




	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);



	// Create  rasterize state desc
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;




	//
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pDefaultRasterState);
	if (FAILED(result))
	{
		return false;
	}
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pWireFrameRasterState);
	if (FAILED(result))
	{
		return false;
	}
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;

	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterStateNoBackCulling);
	if (FAILED(result))
	{
		return false;
	}

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = true;
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pReflectionRasterState);
	if (FAILED(result))
	{
		return false;
	}

	rasterDesc.CullMode = D3D11_CULL_NONE;
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pReflectionRasterStateNoBackCulling);
	if (FAILED(result))
	{
		return false;
	}



	m_pDeviceContext->RSSetState(m_pDefaultRasterState);




	// transparent blend state

	blendDecs.AlphaToCoverageEnable = false;
	blendDecs.IndependentBlendEnable = false;
	blendDecs.RenderTarget[0].BlendEnable = true;
	blendDecs.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDecs.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDecs.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDecs.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDecs.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDecs.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDecs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;



	result = m_pDevice->CreateBlendState(&blendDecs, &m_BlendStateTransparent);
	if (FAILED(result))
	{
		return false;
	}
	// no write to render Target
	blendDecs.RenderTarget[0].RenderTargetWriteMask = 0;

	result = m_pDevice->CreateBlendState(&blendDecs, &m_noWriteToRenderTarget);
	if (FAILED(result))
	{
		return false;
	}
	// no double blending
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	
	depthStencilStateDesc.StencilEnable = true;
	depthStencilStateDesc.StencilReadMask = 0xff;
	depthStencilStateDesc.StencilWriteMask = 0xff;

	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	
	m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &preventDoubleBlendingStencilDepthState);

	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pOnlyWriteStencilDepthStencilState);

	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_pReflectionDepthStencilState);




	// View port
	mScreenViewport.Width = (float)screenWidth;
	mScreenViewport.Height = (float)screenHeight;
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;
	mScreenViewport.TopLeftX = 0.0f;
	mScreenViewport.TopLeftY = 0.0f;


	
	m_cubeMapViewport.TopLeftX = 0.0f;
	m_cubeMapViewport.TopLeftY = 0.0f;
	m_cubeMapViewport.Width = (float)CubeMapSize;
	m_cubeMapViewport.Height = (float)CubeMapSize;
	m_cubeMapViewport.MinDepth = 0.0f;
	m_cubeMapViewport.MaxDepth = 1.0f;


	// set the viewport.
	m_pDeviceContext->RSSetViewports(1, &mScreenViewport);

	m_backFaceCulling = true;
	m_frontFaceCCW = false;
	m_transparent = false;
	m_preventDoubleBlending = false;
	m_isRenderingToCubeMap = false;

	return true;

}

void D3DManager::beginFrame(float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	
}

void D3DManager::endFrame()
{
	m_pSwapChain->Present(0, 0);
}

void D3DManager::clearDepthStencilView()
{
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3DManager::clearStencilView()
{
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,  D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3DManager::clearDepthView()
{
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11Device * D3DManager::getDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext * D3DManager::getDeviceContext()
{
	return m_pDeviceContext;
}

void D3DManager::clean()
{
}

void D3DManager::backFaceCulling(bool isBackFaceCulling)
{
	m_backFaceCulling = isBackFaceCulling;
	if (!m_backFaceCulling)
	{
		if (!m_frontFaceCCW)
		{
			m_pDeviceContext->RSSetState(m_pRasterStateNoBackCulling);
		}	
		else
		{
			m_pDeviceContext->RSSetState(m_pReflectionRasterStateNoBackCulling);
		}
	}
	else
	{
		if (!m_frontFaceCCW)
		{
			m_pDeviceContext->RSSetState(m_pDefaultRasterState);
		}
		else
		{
			m_pDeviceContext->RSSetState(m_pReflectionRasterState);
		}
	}
}

void D3DManager::transparent(bool isTransparent)
{
	m_transparent = isTransparent;
	float factor[] = { 0.0f,0.0f,0.0f,0.0f };
	if (isTransparent)
	{
		m_pDeviceContext->OMSetBlendState(m_BlendStateTransparent, factor, 0xffffffff);
	}
	else
	{
		//default
		m_pDeviceContext->OMSetBlendState(NULL, factor, 0xffffffff);
	}
}

void D3DManager::setFrontFaceCCW(bool frontFaceCCW)
{
	m_frontFaceCCW = frontFaceCCW;
	if (!m_backFaceCulling)
	{
		if (!m_frontFaceCCW)
		{
			m_pDeviceContext->RSSetState(m_pRasterStateNoBackCulling);
		}
		else
		{
			m_pDeviceContext->RSSetState(m_pReflectionRasterStateNoBackCulling);
		}
	}
	else
	{
		if (!m_frontFaceCCW)
		{
			m_pDeviceContext->RSSetState(m_pDefaultRasterState);
		}
		else
		{
			m_pDeviceContext->RSSetState(m_pReflectionRasterState);
		}
	}
}

void D3DManager::onlyWriteStencilBuffer(bool onlyWriteStencilBuffer)
{
	m_onlyWriteStencilBuffer = onlyWriteStencilBuffer;
	float blendFactor[] = { 0.0f,0.0f,0.0f,0.0f };
	if (m_onlyWriteStencilBuffer)
	{
		m_pDeviceContext->OMSetBlendState(m_noWriteToRenderTarget, blendFactor, 0xffffffff);
		m_pDeviceContext->OMSetDepthStencilState(m_pOnlyWriteStencilDepthStencilState, 1);
	}
	else
	{
		m_pDeviceContext->OMSetBlendState(NULL, blendFactor, 0xffffffff);
		m_pDeviceContext->OMSetDepthStencilState(NULL, 0);
	}
}

void D3DManager::preventDoubleBlending(bool isEnable)
{
	m_preventDoubleBlending = isEnable;
	if (m_preventDoubleBlending)
	{
		m_pDeviceContext->OMSetDepthStencilState(preventDoubleBlendingStencilDepthState, 0);
	}
	else
	{
		m_pDeviceContext->OMSetDepthStencilState(0, 0);
	}
}

void D3DManager::drawReflection(bool isDrawR)
{
	m_drawReflection = isDrawR;
	if (m_drawReflection)
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pReflectionDepthStencilState, 1);
	}
	else
	{
		m_pDeviceContext->OMSetDepthStencilState(0, 0);
	}
}

void D3DManager::setRenderTargetView(ID3D11RenderTargetView * renderTargetView)
{
	
	if (renderTargetView)
	{
		float color[4];
		color[0] = 1.0f;
		color[1] = 0.05f;
		color[2] = 0.05f;
		color[3] = 0.05f;
		m_pDeviceContext->ClearRenderTargetView(renderTargetView, color);

		if (!m_isRenderingToCubeMap)
		{
			clearDepthStencilView();
			m_pDeviceContext->OMSetRenderTargets(1, &renderTargetView, m_pDepthStencilView);
		}
		else
		{

			m_pDeviceContext->ClearDepthStencilView(m_pDynamicCubeMapDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			m_pDeviceContext->OMSetRenderTargets(1, &renderTargetView, m_pDynamicCubeMapDepthStencilView);
		}
	}
	else
	{
		float color[4];
		color[0] = 1.0f;
		color[1] = 1.05f;
		color[2] = 1.05f;
		color[3] = 1.05f;
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	}
		
}

void D3DManager::wireFrame(bool enable)
{
	if (enable)
	{
		m_pDeviceContext->RSSetState(m_pWireFrameRasterState);
	}
	else
	{
		m_pDeviceContext->RSSetState(m_pDefaultRasterState);
	}
}

void D3DManager::renderToCubeMap(bool enable)
{
	m_isRenderingToCubeMap = enable;
	if (m_isRenderingToCubeMap)
	{
		m_pDeviceContext->RSSetViewports(1, &m_cubeMapViewport);
	}
	else
	{
		m_pDeviceContext->RSSetViewports(1, &mScreenViewport);
	}
	
}

