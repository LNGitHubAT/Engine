#pragma once
#include "BaseShader.h"
class SkyBoxShader :
	public BaseShader
{
public:
	bool init(ID3D11Device * device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void bindShader(ID3D11DeviceContext * deviceContext);
	virtual void unBindShader(ID3D11DeviceContext * deviceContext);
	virtual bool createInputLayout(ID3D11Device * device, ID3D10Blob* vertexShaderBuffer);
	// create constant buffer
	virtual bool createConstantBuffers(ID3D11Device * device);
	virtual bool createShaders(ID3D11Device * device, HWND hwnd);
	bool setVSParameters(ID3D11DeviceContext * deviceContext, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	bool setPSParameters(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * cubeMapSRV);
	SkyBoxShader();
	~SkyBoxShader();
private:
	ID3D11VertexShader * m_pVertexShader;
	ID3D11PixelShader * m_pPixelShader;

	WCHAR* vsFilename;
	WCHAR* psFilename;

	struct PerVertexBuffer
	{
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	ID3D11Buffer * m_perVertexBuffer;
};

