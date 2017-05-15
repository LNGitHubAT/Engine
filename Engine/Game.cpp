#include "Game.h"

#include "WaveMesh.h"
#include <sstream>



Game::Game()
{
}


Game::~Game()
{
}

bool Game::init(int screenWidth, int screenHeight, HWND hwnd, Input* input)
{
	bool result;


	m_pGraphics = new Graphics();
	m_pGraphics->init(screenWidth, screenHeight, hwnd);
	
	m_pCam = new FirstPersonCamera(FOV, SCREEN_NEAR, SCREEN_DEPTH, (float)screenWidth / (float)screenHeight);
	m_pCam->setPosition(XMFLOAT3(0.0f, 40.0f, -10.0f));

	m_pCam->yaw(PI_F / 2);


	Material material;


	std::vector<WCHAR*> layersFileNames;
	layersFileNames.push_back(L"Texture\\grass.dds");
	layersFileNames.push_back(L"Texture\\darkdirt.dds");
	layersFileNames.push_back(L"Texture\\stone.dds");
	layersFileNames.push_back(L"Texture\\lightdirt.dds");
	layersFileNames.push_back(L"Texture\\snow.dds");

	Terrain::InitInfo tii;
	tii.HeightMapFilename = L"Texture\\terrain.raw";
	tii.layerMapFileNames = layersFileNames;
	tii.BlendMapFilename = L"Texture\\blend.dds";
	tii.HeightScale = 50.0f;
	tii.HeightmapWidth = 2049;
	tii.HeightmapHeight = 2049;
	tii.CellSpacing = 0.5f;

	Terrain * terrainObject = new Terrain();

	TerrainShader * terrainShader = new TerrainShader();

	result = terrainShader->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram//TerrainVertexShader.vs", L"ShaderProgram//TerrainHullShader.hs", L"ShaderProgram//TerrainDomainShader.ds", L"ShaderProgram//TerrainPixelShader.ps");
	if (!result)
		return false;

	terrainObject->init(m_pGraphics->getDevice(), m_pGraphics->getDeviceContext(), tii, terrainShader, 50.0f);

	material.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.reflect = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 64.0f);

	terrainObject->setMaterial(material);

	addTerrain(terrainObject);

	Texture2D* cubeMapTexture = new Texture2D();
	result = cubeMapTexture->init(m_pGraphics->getDevice(), L"Texture\\EnviromentMap.dds");
	if (!result)
	{
		return false;
	}



	pointLight = new PointLightObject(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
		XMFLOAT4(0.9f, 0.9f, 0.3f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		50000.0f,
		XMFLOAT3(1.0f, 0.0014f, 0.000007f)
	);
	pointLight->setPosition(XMFLOAT3(0, 300.0f, 0.0f));
	this->m_Input = input;
	input->showCursor(false);
	m_Input->setMouseSensivity(XMFLOAT2(0.001f, 0.001f));


	Mesh* mesh = new Mesh();
	result = mesh->init(m_pGraphics->getDevice(), L"Model/skull.txt", true);
	//mesh->initBox(m_pGraphics->getDevice());
	//mesh->createBox(m_pGraphics->getDevice());

	if (!result)
	{
		MessageBox(hwnd, L"init Mesh failed", L"Error", MB_OK);
		return false;
	}

	

	Shader* shader = new Shader();
	result = shader->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\StandardShader.vs", L"ShaderProgram\\StandardShader.ps");

	if (!result)
	{
		MessageBox(hwnd, L"init Shader failed", L"Error", MB_OK);
		return false;
	}

	
	material.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 16.0f);
                          

	material.reflect = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f);
	material.refract = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	material.fresnelPower = 15.0f;


	Texture2D* texture = new Texture2D();
	result = texture->init(m_pGraphics->getDevice(), L"Texture\\barrel.png");

	if (!result)
	{
		MessageBox(hwnd, L"Can not load texture", L"Error", MB_OK);
		return false;
	}


	// AABB object
	Mesh *AABBMesh = new Mesh();
	XMFLOAT3 boxSize = mesh->getAABB().extends;
	boxSize.x *= 2;
	boxSize.y *= 2;
	boxSize.z *= 2;
	AABBMesh->initBox(m_pGraphics->getDevice(), boxSize);
	Shader* aabbShader = new Shader();
	aabbShader->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\StandardShader.vs", L"ShaderProgram\\StandardShader.ps");
	GameObject * AABBObject = new GameObject(AABBMesh, shader, material);
	GameObject * AABBObject2 = new GameObject(AABBMesh, shader, material);
	//addGameObject(AABBObject);
	//



	DynamicCubeMap * dynamicCubeMap = new DynamicCubeMap();
	dynamicCubeMap->init(m_pGraphics->getDevice());
	AABBCollider collider;
	collider.aabb = mesh->getAABB();
	//GameObject* barrel = new GameObject(mesh, shader, material, NULL, cubeMapTexture, false, false, true);
	GameObject* barrel = new TankPlayerObject(terrainObject, AABBObject, collider, m_Input, mesh, shader, material, NULL, cubeMapTexture->getResourceView(), dynamicCubeMap);
	barrel->setPosition(XMFLOAT3(0.0f, 40.0f, 0.0f));
	//barrel->rotate(XMFLOAT3(-PI_F / 2, 0.0f, 0.0f));
	barrel->setTag("Player");
	addGameObject(barrel);

	Mesh* dragonMesh = new Mesh();

	//result = dragonMesh->init(m_pGraphics->getDevice(), L"Model/dragon.obj");
	//result = dragonMesh->init(m_pGraphics->getDevice(), L"Model/skull.txt");
	//mesh->createBox(m_pGraphics->getDevice());

	if (!result)
	{
		MessageBox(hwnd, L"init Mesh failed", L"Error", MB_OK);
		return false;
	}

	material.reflect = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.refract = XMFLOAT4(0.9f, 0.9f, 0.9f, 0.9f);
	material.fresnelPower = 1.0f;

	DynamicCubeMap * dynamicCubeMap2 = new DynamicCubeMap();
	dynamicCubeMap2->init(m_pGraphics->getDevice());
	GameObject* dragon = new TankObject(terrainObject, AABBObject2, collider, mesh, shader, material, NULL, cubeMapTexture->getResourceView(), NULL);
	dragon->setPosition(XMFLOAT3(10.0f, 40.0f, 0.0f));
	//barrel->rotate(XMFLOAT3(-PI_F / 2, 0.0f, 0.0f));
	dragon->setTag("Enemy");
	addGameObject(dragon);

	material.specular.w = 64.0f;
	material.diffuse.w = 0.5f;
	WaveMesh* wavesMesh = new WaveMesh();
	wavesMesh->init(m_pGraphics->getDevice(), XMINT2( 100,100), XMFLOAT2(0.5f, 0.5f), 2.0f, 0.09f);
	Texture2D* water = new Texture2D();
	water->init(m_pGraphics->getDevice(), L"Texture\\water2.dds");
	GameObject* waves = new GameObject(wavesMesh, shader, material, water, NULL, NULL, false, true, false);
	waves->setPosition(XMFLOAT3(0.0f, -0.5f, 0.0f));

	//addGameObject(waves);

	Mesh * boxMesh = new Mesh();
	boxMesh->initBox(m_pGraphics->getDevice());
	
	Texture2D* wireFence = new Texture2D();
	result = wireFence->init(m_pGraphics->getDevice(), L"Texture\\WireFence.dds");
	if (!result)
	{
		return false;
	}

	material.diffuse.w = 1.0f;
	material.specular.w = 64.0f;
	GameObject *wireFenceO = new GameObject(boxMesh, shader, material, wireFence, NULL, NULL, true, false, false);
	wireFenceO->setPosition(XMFLOAT3(1.0f, 20.0f, 7.0f));
	addGameObject(wireFenceO);


	m_fog.color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_fog.start = 1000.5f;
	m_fog.range = 50.0f;
	

	Mesh* plane = new Mesh();
	plane->initPlane(m_pGraphics->getDevice(), XMINT2(2, 2), XMFLOAT2(40.0f, 40.0f), XMINT2(4, 4));
	Texture2D* floorTexture = new Texture2D();
	result =  floorTexture->init(m_pGraphics->getDevice(), L"Texture\\grass.dds");
	if (!result)
	{
		return false;
	}


	material.diffuse.w = 1.0f;
	material.specular.w = 64.0f;
	GameObject *floorR = new GameObject(plane, shader, material, floorTexture, NULL, false, false, false, false, true);
	//floorR->setPosition(XMFLOAT3(-10.0f, -1.0f, -10.0f));
	floorR->setPosition(XMFLOAT3(-20, 35, -20));
	//addGameObject(floorR);


	Mesh* mirrorPlane = new Mesh();
	mirrorPlane->initPlane(m_pGraphics->getDevice(), XMINT2(2, 2), XMFLOAT2(20.0f, 20.0f), XMINT2(1, 1));
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	GameObject *mirror = new GameObject(mirrorPlane, shader, material, NULL, NULL, false, true, true, true, false);

	mirror->setPosition(XMFLOAT3(10.0f, -1, -10.0f));
	mirror->rotate(XMFLOAT3(0.0f, 0.0f, PI_F / 2));
	//addGameObject(mirror);
	GameObject *mirror2 = new GameObject(mirrorPlane, shader, material, NULL, NULL, false, true, true, true, false);

	mirror2->setPosition(XMFLOAT3(-10.0f, -1, 10.0f));
	mirror2->rotate(XMFLOAT3(-PI_F / 2, 0.0f, 0.0f));

	//addGameObject(mirror2);


	Texture2D* textureArray = new Texture2D();
	std::vector<WCHAR*> fileNames;
	fileNames.push_back(L"Texture\\tree0.dds");
	fileNames.push_back(L"Texture\\tree1.dds");
	fileNames.push_back(L"Texture\\tree2.dds");
	fileNames.push_back(L"Texture\\tree3.dds");
	textureArray->init(m_pGraphics->getDevice(), m_pGraphics->getDeviceContext(), fileNames);

	TreeBillBoard* treeBillBoardMesh = new TreeBillBoard();
	treeBillBoardMesh->init(m_pGraphics->getDevice(), XMFLOAT3(10.0f, 2.3f, 10.0f), 100, XMFLOAT2(5.0f, 5.0f));
	BillBoardShader* billBoardShader = new BillBoardShader();
	billBoardShader->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\TreeBillBoard.vs", L"ShaderProgram\\TreeBillBoard.gs", L"ShaderProgram\\TreeBillBoard.ps");

	
	material.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.reflect = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 64.0f);
	
	TreeBillBoardObject* treeBilllBoard = new TreeBillBoardObject(treeBillBoardMesh, billBoardShader, material, textureArray, true, false, false);

	//addGameObject(treeBilllBoard);

	m_pGraphics->setShadow(false);



	GUIShader* guiShader = new GUIShader();
	guiShader->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\GUIShader.vs", L"ShaderProgram\\GUIShader.ps");


	BlurringComputeShader* horizontalBlur = new BlurringComputeShader();
	horizontalBlur->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\HorizontalBlur.css");

	BlurringComputeShader* verticalBlur = new BlurringComputeShader();
	verticalBlur->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\VerticalBlur.css");
	blur = new BluringPostProcess(m_pGraphics->getDevice(), screenWidth, screenHeight, verticalBlur, horizontalBlur);
	GUI * guiObject = new GUI();
	XMFLOAT2 size(0.5f, 0.5f);
	XMFLOAT2 position(1.0f - size.x, -1.0f + size.y);
	guiObject->init(m_pGraphics->getDevice(), size, position, guiShader, blur->getShaderResourceView());
	//addGUI(guiObject);

	


	

	



	//Texture2D* cubeMapTexture = new Texture2D();
	SkyBoxShader* skyBoxShader = new SkyBoxShader();
	result =  skyBoxShader->init(m_pGraphics->getDevice(), hwnd, L"ShaderProgram\\SkyboxShader.vs", L"ShaderProgram\\SkyboxShader.ps");
	if (!result)
	{
		return false;
	}
	
	SkyBox *skyBox = new SkyBox();
	Mesh* skyBoxMesh = new Mesh();
	skyBoxMesh->initBox(m_pGraphics->getDevice(), XMFLOAT3(600.0f, 600.0f, 600.0f));
	skyBox->init(cubeMapTexture->getResourceView(), skyBoxShader, skyBoxMesh);
	setSkyBox(skyBox);
	


	return true;
}

bool Game::render()
{

	//m_pGraphics->renderToTexture(m_gameObjectList, m_pCam, pointLight, m_fog, blur->getRenderTargetView());

	//blur->blur(m_pGraphics->getDeviceContext(), 1);

	

	m_pGraphics->render(m_gameObjectList, m_terrainList, m_skyBox, m_GUIList, m_pCam, pointLight, m_fog);

	
	return true;
}

bool Game::update(float deltaTime)
{
	processInput(deltaTime);
	for (UINT i = 0; i < m_gameObjectList.size(); i++)
	{
		m_gameObjectList[i]->handleInput();
	}
	//m_gameObjectList[1]->rotate(XMFLOAT3(0.0f, -PI_F / 2 * deltaTime, 0.0f));
	//m_gameObjectList.front()->rotate(XMFLOAT3(0.0f, -PI_F / 2 * deltaTime, 0.0f) );

	// clear gameObjectList
	for (UINT i = 0; i < m_gameObjectList.size(); i++)
	{
		if (m_gameObjectList[i]->isDestroyed())
		{
			m_gameObjectList.erase(m_gameObjectList.begin() + i);
			i--;
		}
	}



	for (UINT i = 0; i < m_gameObjectList.size() - 1; i++)
	{
		float collidePreditedTime = MathUtility::infinityF();

		for (UINT j = i + 1; j < m_gameObjectList.size(); j++)
		{

			if (
				m_gameObjectList[i]->getTag() == "" || m_gameObjectList[i]->isDestroyed()
				||
				m_gameObjectList[j]->getTag() == "" || m_gameObjectList[j]->isDestroyed()
				)
			{
				continue;
			}
			else
			{
				if (

					(m_gameObjectList[i]->getTag() == "Player" &&
						m_gameObjectList[j]->getTag() == "BulletP")
					||
					(m_gameObjectList[i]->getTag() == "Enemy" &&
						m_gameObjectList[j]->getTag() == "BulletE")
					||
					(m_gameObjectList[i]->getTag() == "BulletP" &&
							m_gameObjectList[j]->getTag() == "Player")
					||
					(m_gameObjectList[i]->getTag() == "BulletE" &&
						m_gameObjectList[j]->getTag() == "Enemy")
					||
					(m_gameObjectList[i]->getTag() == "BulletE" &&
						m_gameObjectList[j]->getTag() == "BulletE")
					)
				{
					continue;
				}
				else
				{
					
					PhysicsObject* obj1 = (PhysicsObject*)m_gameObjectList[i];
					PhysicsObject* obj2 = (PhysicsObject*)m_gameObjectList[j];

					AABB aabb1 = obj1->getAABBCollider().getAABBBoardPhrase();
					AABB aabb2 = obj2->getAABBCollider().getAABBBoardPhrase();

					if (aabb1.isAABBintersection(aabb2))
					{
						//(PhysicsObject*)(m_gameObjectList[i])
						float collideTime = obj1->getAABBCollider().sweepAABB(obj2->getAABBCollider());
						if (collideTime <= deltaTime)
						{
							obj1->onCollitionEnter(obj2->getTag(), collideTime);
							obj2->onCollitionEnter(obj1->getTag(), collideTime);
						}
					}

					
				}
			}


			//float collideTime = 

		}
	}














	for (auto it = m_gameObjectList.begin(); it != m_gameObjectList.end(); it++)
	{
		(*it)->update(deltaTime);
	}

	



	m_Input->clear();
	return true;
}

void Game::processInput(float deltaTime)
{
	XMFLOAT2 delta = m_Input->getCusorDelta();
	m_pCam->pitch(delta.y * m_Input->getMouseSensivity().y);
	m_pCam->yaw(delta.x * m_Input->getMouseSensivity().x);

	float hVec = 50.0f * deltaTime;
	if (m_Input->IsKeyDown(0x57))
	{
		m_pCam->direct(hVec);
	}
	if (m_Input->IsKeyDown(0x53))
	{
		m_pCam->direct(-hVec);
	}
	if (m_Input->IsKeyDown(0x44))
	{
		m_pCam->straft(hVec);
	}
	if (m_Input->IsKeyDown(0x41))
	{
		m_pCam->straft(-hVec);
	}

	if (m_Input->IsKeyDown(0x41))
	{
		m_pCam->straft(-hVec);
	}

	/*while (!m_Input->isEmpty())
	{
		KeyEvent e =  m_Input->readKey();

		if (e.getCode() == VK_SPACE && e.isPress())
		{
			terrainWireFrame = !terrainWireFrame;
		}
	}*/

	

}

void Game::addGameObject(GameObject* gameObject)
{
	m_gameObjectList.push_back(gameObject);
}

void Game::addGUI(GUI * guiObject)
{
	m_GUIList.push_back(guiObject);
}

void Game::addTerrain(Terrain * terrain)
{
	m_terrainList.push_back(terrain);
}

void Game::setSkyBox(SkyBox * skyBox)
{
	m_skyBox = skyBox;
}

void Game::clean()
{
	m_pGraphics->clean();
}
