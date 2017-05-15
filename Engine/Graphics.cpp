
#include "Graphics.h"


Graphics::Graphics()
{

}


Graphics::Graphics(const Graphics& other)
{
	
}


Graphics::~Graphics()
{
}


bool Graphics::init(int screenWidth, int screenHeight, HWND hwnd)
{
	m_pD3DManager = new  D3DManager();
	bool  result  =  m_pD3DManager->init(screenWidth, screenHeight, false, false, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"init Direct3D Failed", L"Error", MB_OK);
		return false;
	}
	m_hwnd = hwnd;
	m_screenSize = XMINT2(screenWidth, screenHeight);
	

		
	return true;
}


void Graphics::clean()
{
	if (m_pD3DManager)
	{
		m_pD3DManager->clean();
		delete m_pD3DManager;
		m_pD3DManager = NULL;
	}
	return;
}


bool Graphics::update()
{
	return true;
}

bool Graphics::render(std::vector<GameObject*> gameObjectList, Camera * cam, PointLightObject* pointLight, Fog fog , bool isLowestQuality)
{
	
	std::vector<GameObject*> opaqueGameObjectList;
	std::vector<GameObject*> transparencyObjectList;

	std::vector<GameObject*> mirrorObject;
	std::vector<XMFLOAT4> shadowPlaneToCastOn;

	for (auto it = gameObjectList.begin(); it != gameObjectList.end(); it++)
	{


		if ((*it)->isMirror)
		{
			mirrorObject.push_back(*it);
		}
		else
		{
			if (!(*it)->isTransparent())
			{
				opaqueGameObjectList.push_back((*it));
			}
			else
			{
				transparencyObjectList.push_back((*it));
			}
		}
		if ((*it)->isPlane)
		{

			XMVECTOR plane;
			XMFLOAT4 planef4;
			XMVECTOR offset = XMLoadFloat3(&(*it)->getUp()) * 0.001f;
			plane = XMPlaneFromPointNormal(XMLoadFloat3(&(*it)->getPosition()) + offset, XMLoadFloat3(&(*it)->getUp()) );
			XMStoreFloat4(&planef4, plane);
			shadowPlaneToCastOn.push_back(planef4);
		}
	}

	//render opaque objects first

	transparent(false);
	for (auto it = opaqueGameObjectList.begin(); it != opaqueGameObjectList.end(); it++)
	{
		backFaceCulling( (*it)->isUseBackFaceCulling() );
		(*it)->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog, isLowestQuality);
		
	}
	backFaceCulling(false);

	// render plannar shadow
	// hard code
	if (m_shadowEnable)
	{
		for (UINT i = 0; i < shadowPlaneToCastOn.size(); i++)
		{
			renderShadow(gameObjectList, cam, pointLight, fog, shadowPlaneToCastOn[i]);
		}
	}
	




	transparent(true);

	sortGameObjectList(transparencyObjectList, cam->getPosition());

	for (auto it = transparencyObjectList.begin(); it != transparencyObjectList.end(); it++)
	{
		backFaceCulling((*it)->isUseBackFaceCulling());

		(*it)->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog);
		//(*it)->setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	transparent(false);
	backFaceCulling(true);
	// render reflection to every mirror
	//XMFLOAT3 oldPointLightPosition = pointLight->getPosition();
	
	for (UINT i = 0; i < mirrorObject.size(); i++)
	{
		renderReflection(gameObjectList, cam, pointLight, fog, mirrorObject[i]);
	}


	
	return true;
}


bool Graphics::render(std::vector<Terrain*> terrainList, Camera * cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality)
{
	//wireFrame(true);
	for (UINT i = 0; i < terrainList.size(); i++)
	{
		terrainList[i]->render(getDeviceContext(), cam, pointLight, fog , isLowestQuality);
	}
	//wireFrame(false);
	return true;
}

bool Graphics::render(std::vector<GameObject*> gameObjectList, std::vector<Terrain*> terrainList, SkyBox * skyBox, std::vector<GUI*> guiList, Camera * cam, PointLightObject * pointLight, Fog fog)
{
	
	m_pD3DManager->renderToCubeMap(true);

	for (UINT i = 0; i < gameObjectList.size(); i++)
	{
		if (gameObjectList[i]->isUseDynamicCubeMap())
		{
			// render to cube map
			ID3D11RenderTargetView** renderTargetViews = gameObjectList[i]->getCubeMapRenderTargetViews();
			Camera** cubeCam = gameObjectList[i]->getCubeMapCams();
			std::vector<GameObject*> cubeMapGameObjectList = gameObjectList;
			cubeMapGameObjectList.erase(cubeMapGameObjectList.begin() + i);
			//UINT cameraIndex = 1;

			for (UINT cameraIndex = 0; cameraIndex < 6; cameraIndex++)
			{
				setRenderTargetView(renderTargetViews[cameraIndex]);

				render(cubeMapGameObjectList, cubeCam[cameraIndex], pointLight, fog, true);

				render(terrainList, cubeCam[cameraIndex], pointLight, fog, true);

				render(skyBox, cubeCam[cameraIndex]);

			}

			getDeviceContext()->GenerateMips(gameObjectList[i]->m_dynamicCubeMap->getShaderResourceView());
			
		}
	}

	//skyBox->setSRV(gameObjectList[0]->m_dynamicCubeMap->getShaderResourceView());

	m_pD3DManager->renderToCubeMap(false);

	m_pD3DManager->setRenderTargetView(NULL); // render 

	
	prepareRendering();

	render(gameObjectList, cam, pointLight, fog);

	render(terrainList, cam, pointLight, fog);

	render(skyBox, cam);
	
	render(guiList);

	endRendering();
	return true;
}

bool Graphics::render(SkyBox * skyBox, Camera * cam)
{
	if (skyBox)

	{
		//wireFrame(true);
		backFaceCulling(false);
		skyBox->render(getDeviceContext(), cam);
		backFaceCulling(true);
		//wireFrame(false);
		return true;
	}
	
}


bool Graphics::renderToTexture(std::vector<GameObject*> gameObjectList, Camera * cam, PointLightObject * pointLight, Fog fog, ID3D11RenderTargetView * renderTargetView)
{

	setRenderTargetView(renderTargetView);
	render(gameObjectList, cam, pointLight, fog);

	setRenderTargetView(NULL);
	return true;
}

bool Graphics::render(std::vector<GUI*> guiList)
{
	for (int i = 0; i < guiList.size(); i++)
	{
		guiList[i]->render(getDeviceContext());
	}

	return true;
}

bool Graphics::renderReflection(std::vector<GameObject*> gameObjectList, Camera * cam, PointLightObject * pointLight, Fog fog, GameObject* mirrorObject)
{
	m_pD3DManager->clearStencilView();
	onlyWriteToStencilBuffer(true);

	XMVECTOR mirrorPlane;
	XMFLOAT4 planef4;
	mirrorPlane = XMPlaneFromPointNormal(XMLoadFloat3(&(mirrorObject)->getPosition()), XMLoadFloat3(&(mirrorObject)->getUp()));

	mirrorObject->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog);

	onlyWriteToStencilBuffer(false);



	std::vector<GameObject*> opaqueGameObjectList;
	std::vector<GameObject*> transparencyObjectList;

	std::vector<GameObject*> mirrorObjectList;
	std::vector<XMFLOAT4> shadowPlaneToCastOn;

	for (auto it = gameObjectList.begin(); it != gameObjectList.end(); it++)
	{
		{
			if (!(*it)->isTransparent())
			{
				opaqueGameObjectList.push_back((*it));
			}
			else
			{
				transparencyObjectList.push_back((*it));
			}
		}
		
	}



	
	
	//render  reflection
	XMFLOAT3 originalLightPos = pointLight->getPosition();
	XMFLOAT3 reflectedPointLightPos;
	XMVECTOR reflectedPointLightPosV = XMLoadFloat3(&originalLightPos);
	XMMATRIX reflectM = XMMatrixReflect(mirrorPlane);

	reflectedPointLightPosV = XMVector4Transform(reflectedPointLightPosV, reflectM);
	XMStoreFloat3(&reflectedPointLightPos, reflectedPointLightPosV);
	pointLight->setPosition(reflectedPointLightPos);
	
	drawReflectionSS(true);
	// render
	//backFaceCulling(false);
	transparent(false);
	for (UINT i = 0; i < opaqueGameObjectList.size(); i++)
	{
		if (opaqueGameObjectList[i]->isMirror)
			continue;
		backFaceCulling(opaqueGameObjectList[i]->isUseBackFaceCulling());
		opaqueGameObjectList[i]->setWorldMatrixFactor(reflectM);
		opaqueGameObjectList[i]->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog);
		opaqueGameObjectList[i]->setWorldMatrixFactor(XMMatrixIdentity());
	}
	sortGameObjectList(transparencyObjectList, cam->getPosition());
	transparent(true);
	for (UINT i = 0; i < transparencyObjectList.size(); i++)
	{
		if (transparencyObjectList[i]->isMirror)
			continue;
		backFaceCulling(transparencyObjectList[i]->isUseBackFaceCulling());
		transparencyObjectList[i]->setWorldMatrixFactor(reflectM);
		transparencyObjectList[i]->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog);
		transparencyObjectList[i]->setWorldMatrixFactor(XMMatrixIdentity());
	}
	backFaceCulling(true);
	//transparent(fa);
	drawReflectionSS(false);

	transparent(true);
	mirrorObject->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog);
	transparent(false);
	pointLight->setPosition(originalLightPos);

	
	
	return true;
}

bool Graphics::renderShadow(std::vector<GameObject*> gameObjectList, Camera * cam, PointLightObject * pointLight, Fog fog, XMFLOAT4 planeToCastShadowOn)
{

	preventDoubleBlending(true);

	XMMATRIX worldMatrixFactor;
	XMFLOAT3 lightPosF = pointLight->getPosition();
	XMVECTOR lightPos = XMVectorSet(lightPosF.x, lightPosF.y, lightPosF.z, 1.0f);

	//XMVectorSetW(lightPos, 1.0f);
	worldMatrixFactor = XMMatrixShadow(XMLoadFloat4(&planeToCastShadowOn), lightPos);

	Material mShadowMat;

	mShadowMat.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mShadowMat.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	mShadowMat.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);

	transparent(true);
	for (UINT i = 0; i < gameObjectList.size(); i++)
	{
		if (gameObjectList[i]->isPlane || gameObjectList[i]->isMirror)
			continue;
		Material oldMat = gameObjectList[i]->getMaterial();
		gameObjectList[i]->setWorldMatrixFactor(worldMatrixFactor);
		gameObjectList[i]->setMaterial(mShadowMat);
		gameObjectList[i]->render(m_pD3DManager->getDeviceContext(), cam, pointLight, fog);
		gameObjectList[i]->setMaterial(oldMat);
		gameObjectList[i]->setWorldMatrixFactor(XMMatrixIdentity());
	}
	transparent(false);
	preventDoubleBlending(false);

	return true;
}



void Graphics::backFaceCulling(bool isBackFaceCulling)
{
	m_pD3DManager->backFaceCulling(isBackFaceCulling);
}

void Graphics::setShadow(bool isEnable)
{
	m_shadowEnable = isEnable;
}

void Graphics::transparent(bool isTransparent)
{
	m_pD3DManager->transparent(isTransparent);
}

void Graphics::preventDoubleBlending(bool isPreventDoubleBlending)
{
	m_pD3DManager->preventDoubleBlending(isPreventDoubleBlending);
}

void Graphics::onlyWriteToStencilBuffer(bool isEnable)
{
	m_pD3DManager->onlyWriteStencilBuffer(isEnable);
}

void Graphics::drawReflectionSS(bool isRenderingReflection)
{
	m_pD3DManager->drawReflection(isRenderingReflection);
	m_pD3DManager->setFrontFaceCCW(isRenderingReflection);
}

void Graphics::setRenderTargetView(ID3D11RenderTargetView * renderTargetView)
{
	
	m_pD3DManager->setRenderTargetView(renderTargetView);
	
}





HWND Graphics::getHwnd()
{
	return m_hwnd;
}

XMINT2 Graphics::getScreenSize()
{
	return m_screenSize;
}

ID3D11Device * Graphics::getDevice()
{
	return m_pD3DManager->getDevice();
}

ID3D11DeviceContext * Graphics::getDeviceContext()
{
	return m_pD3DManager->getDeviceContext();
}

void Graphics::wireFrame(bool enable)
{
	m_pD3DManager->wireFrame(enable);
}

void Graphics::prepareRendering()
{
	m_pD3DManager->beginFrame(0.5f, 0.5f, 0.5f, 1.0f);
	//getDe0viceContext()->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pD3DManager->clearDepthStencilView();
}

void Graphics::endRendering()
{
	m_pD3DManager->endFrame();
}

void Graphics::sortGameObjectList(std::vector<GameObject*>& gameObjectList, XMFLOAT3 camPos)
{
	XMVECTOR camPosVector = XMLoadFloat3(&camPos);


	int size = gameObjectList.size();
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i; j < (size - 1); j++)
		{
			float length_jth;
			XMStoreFloat(&length_jth, (XMVector3Length( camPosVector -  XMLoadFloat3(&gameObjectList[j]->getPosition()))) );
			float length_jth1;
			XMStoreFloat(&length_jth1, (XMVector3Length(camPosVector - XMLoadFloat3(&gameObjectList[j + 1]->getPosition()))));
			if (length_jth < length_jth1)
			{
				std::swap(gameObjectList[j], gameObjectList[j + 1]);
			}
		}
	}
}
