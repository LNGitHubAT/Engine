#pragma once
#include "Graphics.h"
#include "Timer.h"
#include "GameObject.h"
#include "Shader.h"
#include <list>
#include "Camera.h"
#include "PointLightObject.h"
#include "Input.h"
#include "FirstPersonCamera.h"
#include <vector>
#include "TreeBillBoardObject.h"
#include "TreeBillBoard.h"
#include "GUI.h"
#include "BluringPostProcess.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "TankPlayerObject.h"

class Game
{
public:
	Game();
	~Game();
	bool init(int screenWidth, int screenHeight, HWND hwnd, Input* input);
	bool render();
	bool update(float deltaTime);
	void processInput(float deltaTime);
	void clean();
private:
	BluringPostProcess* blur;

	void addGameObject(GameObject* gameObject);
	void addGUI(GUI* guiObject);
	void addTerrain(Terrain* terrain);
	void setSkyBox(SkyBox* skyBox);
	std::vector<GameObject*> m_gameObjectList;
	std::vector<GUI*> m_GUIList;
	std::vector<Terrain*> m_terrainList;
	PointLightObject* pointLight;
	Camera * m_pCam;


	//FirstPersonCamera * m_FPSCam;

	Graphics* m_pGraphics;
	Input* m_Input;
	SkyBox* m_skyBox;
	Fog m_fog;
	bool terrainWireFrame = false;

};

