#pragma once
#include "GameObject.h"
#include "BiillBoardShader.h"
class TreeBillBoardObject :
	public GameObject
{
public:
	TreeBillBoardObject();
	TreeBillBoardObject(Mesh* mesh, Shader* shader, Material material, Texture2D * texture,
		bool isUseAnphaClipTest = false,
		bool isTransparent = false,
		bool isUseBackCulling = true,
		bool isMirror = false,
		bool isPlane = false);
	~TreeBillBoardObject();
	void render(ID3D11DeviceContext* deviceContext, Camera* cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality = false) override;

};

