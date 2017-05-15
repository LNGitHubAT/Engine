#pragma once
#include <d3d11.h>
#include <d3dx11tex.h>
#include <vector>

class Texture2D
{
public:
	Texture2D();
	~Texture2D();
	ID3D11ShaderResourceView* getResourceView();
	bool init(ID3D11Device* device, const WCHAR* fileName);
	bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext,  std::vector<WCHAR*> fileNames, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT filter = D3DX11_FILTER_LINEAR, UINT mipFilter = D3DX11_FILTER_LINEAR);
	void clean();
private:
	ID3D11ShaderResourceView* m_textureResourceView;
};

