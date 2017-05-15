#include "GameObject.h"
#include "PointLightObject.h"


GameObject::GameObject()
	: Object()
{
	m_mesh = NULL;
	tag = "";
}

GameObject::GameObject(Mesh* mesh, Shader* shader, Material material, Texture2D * texture,
	ID3D11ShaderResourceView *cubeMapSRV,
	DynamicCubeMap* dynamicCubeMap,
	bool isUseAnphaClipTest,
	bool isTransparent,
	bool isUseBackCulling,
	bool isMirror,
	bool isPlane
)
{
	Object::Object();

	this->m_mesh = mesh;
	this->m_shader = shader;
	this->m_material = material;

	this->m_pTexture = texture;
	if (texture == NULL)
	{
		m_isUsetexture = false;
	}
	else
	{
		m_isUsetexture = true;
	}

	m_staticCubeMapSRV = cubeMapSRV;
	if (m_staticCubeMapSRV)
	{
		m_isUseCubeMap = true;
	}
	else
	{
		m_isUseCubeMap = false;
	}
	this->m_dynamicCubeMap = dynamicCubeMap;
	if (m_dynamicCubeMap)
	{
		m_isUseDynamicCubeMap = true;
		m_isUseCubeMap = true;
	}
	else
	{
		m_isUseDynamicCubeMap = false;
	}

	//this->m_isUseDynamicCubeMap = isUseDynamicCubeMap;
	this->m_isUseAnphaClipTest = isUseAnphaClipTest;
	this->m_isUseBackCulling = isUseBackCulling;
	this->m_isTransparent = isTransparent;
	this->isMirror = isMirror;
	this->isPlane = isPlane;
	m_worldMatrixFactor = XMMatrixIdentity();
	tag = "";
	m_isDestroyed = false;
}


GameObject::~GameObject()
{
}



void GameObject::bindObject(ID3D11DeviceContext * deviceContext)
{
	m_mesh->bindMesh(deviceContext);
	m_shader->bindShader(deviceContext);
}

void GameObject::unbindObject(ID3D11DeviceContext * deviceContext)
{
	m_mesh->unBindMesh(deviceContext);
	m_shader->unBindShader(deviceContext);
}

void GameObject::render(ID3D11DeviceContext * deviceContext, Camera* cam, PointLightObject* pointLight, Fog fog  , bool isLowestQuality)
{
	bindObject(deviceContext);
	Shader* shader = (Shader*)m_shader;
	shader->setVSParameter(deviceContext, getWorldMatrix() * m_worldMatrixFactor, cam->getViewMatrix(), cam->getProjectionMatrix(), getWorldMatrixInverseTranspose());
	if (m_isUseCubeMap  && !isLowestQuality)
	{
		if (m_isUseDynamicCubeMap)
		{
			shader->setPSParameter(deviceContext, m_material, pointLight->getData(), cam->getPosition(), m_pTexture, m_dynamicCubeMap->getShaderResourceView(), m_isUsetexture, true, m_isUseAnphaClipTest, fog);
		}
		else
		{
			shader->setPSParameter(deviceContext, m_material, pointLight->getData(), cam->getPosition(), m_pTexture, m_staticCubeMapSRV, m_isUsetexture, true, m_isUseAnphaClipTest, fog);
		}
	}
	else
	{
		shader->setPSParameter(deviceContext, m_material, pointLight->getData(), cam->getPosition(), m_pTexture, NULL, m_isUsetexture, false, m_isUseAnphaClipTest, fog);
	}
	deviceContext->DrawIndexed(m_mesh->getIndexCount(), 0, 0);
	unbindObject(deviceContext);
}

void GameObject::update(float deltaTime)
{
	m_mesh->update(deltaTime);
}

void GameObject::handleInput()
{
}

bool GameObject::isUseBackFaceCulling()
{
	return m_isUseBackCulling;
}

bool GameObject::isTransparent()
{
	return m_isTransparent;
}

bool GameObject::isUseDynamicCubeMap()
{
	return m_isUseDynamicCubeMap;
}

void GameObject::setWorldMatrixFactor(XMMATRIX worldMatrixFactor)
{
	m_worldMatrixFactor = worldMatrixFactor;
}

Material GameObject::getMaterial()
{
	return m_material;
}

void GameObject::setMaterial(Material m)
{
	m_material = m;
}

Camera* * GameObject::getCubeMapCams()
{
	return	m_dynamicCubeMap->buildCamrera(m_position.x, m_position.y, m_position.z);
}

ID3D11RenderTargetView** GameObject::getCubeMapRenderTargetViews()
{
	return m_dynamicCubeMap-> getRenderTargetView();
}

std::string GameObject::getTag()
{
	return tag;
}

void GameObject::setTag(std::string tag)
{
	this->tag = tag;
}

void GameObject::destroy()
{
	m_isDestroyed = true;
}

bool GameObject::isDestroyed()
{
	return m_isDestroyed;
}

