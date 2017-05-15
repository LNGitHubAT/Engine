#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "Texture2D.h"
#include "DynamicCubeMap.h"

using namespace DirectX;
class PointLightObject;
class GameObject : public Object
{
public:
	GameObject();
	GameObject(Mesh* mesh, Shader* shader, Material material, Texture2D * texture = NULL,
											ID3D11ShaderResourceView * cubeMapSRV = NULL,
											DynamicCubeMap* dynamicCubeMap = NULL,
							bool isUseAnphaClipTest = false,
							bool isTransparent = false,
							bool isUseBackCulling = true,
							bool isMirror = false,
							bool isPlane = false);
							
	~GameObject();
	
	virtual void render(ID3D11DeviceContext* deviceContext, Camera* cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality = false);
	virtual void update(float deltaTime);
	virtual void handleInput();
	bool isUseBackFaceCulling();
	bool isTransparent();
	bool isUseDynamicCubeMap();
	void setWorldMatrixFactor(XMMATRIX worldMatrixFactor);
	bool isMirror; // temporary
	bool isPlane; // temporary
	Material getMaterial();
	void setMaterial(Material material);
	Camera ** getCubeMapCams();
	ID3D11RenderTargetView** getCubeMapRenderTargetViews();
	DynamicCubeMap* m_dynamicCubeMap;
	std::string getTag();
	void setTag(std::string tag);
	void destroy();
	bool isDestroyed();
protected:

	Mesh* m_mesh;
	Material m_material;
	Texture2D * m_pTexture;
	ID3D11ShaderResourceView * m_staticCubeMapSRV;
	XMMATRIX m_worldMatrixFactor;
	bool m_isUsetexture;
	bool m_isUseAnphaClipTest;
	bool m_isUseBackCulling;
	bool m_isTransparent;
	bool m_isUseCubeMap;
	bool m_isUseDynamicCubeMap;
	bool m_isDestroyed;

protected:

	std::string tag;
	
protected:
	void bindObject(ID3D11DeviceContext* deviceContext);
	void unbindObject(ID3D11DeviceContext* deviceContext);
	BaseShader* m_shader;
};

