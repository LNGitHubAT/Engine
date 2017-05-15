#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Define.h"
#include <DirectXMath.h>
#include "Texture2D.h"
using namespace DirectX;
using namespace std;
class BaseShader
{
public:
	BaseShader();
	~BaseShader();
	virtual void bindShader(ID3D11DeviceContext * deviceContext);
	virtual void unBindShader(ID3D11DeviceContext * deviceContext);
	virtual bool createInputLayout(ID3D11Device * device, ID3D10Blob* vertexShaderBuffer);
	virtual bool createSampleState(ID3D11Device * device);
	// create constant buffer
	virtual bool createConstantBuffers(ID3D11Device * device);
	virtual bool createShaders(ID3D11Device * device , HWND hwnd);
protected:
	virtual bool initialize(ID3D11Device * device, HWND hwnd) sealed ;
	ID3D11InputLayout * m_pInputLayout;
	ID3D11SamplerState* m_pSamplerState;
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
	
};

