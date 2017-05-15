#pragma once
#include <D3DX11.h>
#include "Camera.h"
#include "Define.h"

#include <DirectXMath.h>

using namespace DirectX;


class DynamicCubeMap
{
public:
	DynamicCubeMap();
	~DynamicCubeMap();
	bool init(ID3D11Device* device);
	static const int m_sCubeMapSize = 256;
	ID3D11ShaderResourceView* getShaderResourceView();
	ID3D11RenderTargetView** getRenderTargetViews();
	Camera** buildCamrera(float x, float y, float z);
	ID3D11RenderTargetView** getRenderTargetView();

private:
	ID3D11RenderTargetView* m_pRenderTargetViews[6];
	ID3D11ShaderResourceView* m_srv;
	Camera* cam[6];
};

