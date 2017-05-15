#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
#include <d3d11.h>
#include "Camera.h"
#include "SkyBoxShader.h"
class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	bool init(ID3D11ShaderResourceView * cubeMapSRV, SkyBoxShader* shader, Mesh * mesh);
	bool render(ID3D11DeviceContext* deviceContext, Camera* cam);
	void setSRV(ID3D11ShaderResourceView * cubeMapSRV);
private:
	void bindSkyBox(ID3D11DeviceContext* deviceContext);
	void unbindSkyBox(ID3D11DeviceContext* deviceContext);
	ID3D11ShaderResourceView * m_cubeMapSRV;
	Mesh* m_skyBoxMesh;

	BaseShader * m_shader;
};

