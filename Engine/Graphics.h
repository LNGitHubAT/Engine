#pragma once
#include <windows.h>
#include "D3DManager.h"
#include <DirectXMath.h>


#include "GameObject.h"
#include "Shader.h"
#include "Camera.h"
#include "PointLightObject.h"
#include <list>
#include "GUI.h"
#include "Terrain.h"
#include "SkyBox.h"
using namespace DirectX;
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
//const float SCREEN_DEPTH = 1000.0f;
//const float SCREEN_NEAR = 0.1f;
//const float FOV = XMConvertToRadians(45.0f);

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool init(int, int, HWND);
	void clean();
	bool update();
	bool render(std::vector<GameObject*> gameObjectList, Camera* cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality = false);
	bool renderToTexture(std::vector<GameObject*> gameObjectList, Camera* cam, PointLightObject* pointLight, Fog fog, ID3D11RenderTargetView* renderTargetView);
	bool render(std::vector<GUI*> guiList);
	bool render(std::vector<Terrain*> terrainList, Camera * cam, PointLightObject* pointLight, Fog fog , bool isLowestQuality = false);

	bool render(std::vector<GameObject*> gameObjectList, std::vector<Terrain*> terrainList, SkyBox * skyBox, std::vector<GUI*> guiList, Camera * cam, PointLightObject* pointLight, Fog fog);

	bool render(SkyBox * skyBox, Camera * cam);

	void backFaceCulling(bool);
	void setShadow(bool isEnable);
	void transparent(bool);
	void preventDoubleBlending(bool);
	void onlyWriteToStencilBuffer(bool);
	void drawReflectionSS(bool);
	void setRenderTargetView(ID3D11RenderTargetView* renderTargetView);
	HWND getHwnd();
	XMINT2 getScreenSize();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	void wireFrame(bool enable);
	void prepareRendering();

	void endRendering();
	
	
private:
	bool renderReflection(std::vector<GameObject*> gameObjectList, Camera* cam, PointLightObject* pointLight, Fog fog, GameObject* mirrorObject); //temp
	bool renderShadow(std::vector<GameObject*> gameObjectList, Camera* cam, PointLightObject* pointLight, Fog fog, XMFLOAT4 planeToCastShadowOn); // temp
	void sortGameObjectList(std::vector<GameObject*>& gameObjectList, XMFLOAT3 camPos);
	D3DManager* m_pD3DManager;
	HWND  m_hwnd;
	XMINT2 m_screenSize;
	bool m_shadowEnable;

	
};
