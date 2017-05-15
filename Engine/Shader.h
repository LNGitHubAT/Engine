#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Define.h"
#include <DirectXMath.h>
#include "Texture2D.h"
#include "BaseShader.h"
using namespace DirectX;
using namespace std;
class Shader : public BaseShader
{
public:
	Shader();
	~Shader();

	bool init(ID3D11Device * device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	//void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
	virtual void bindShader(ID3D11DeviceContext* deviceContext);
	virtual void unBindShader(ID3D11DeviceContext* deviceContext);
	virtual bool createInputLayout(ID3D11Device * device, ID3D10Blob* vertexShaderBuffer);
	virtual bool createConstantBuffers(ID3D11Device * device);
	virtual bool createShaders(ID3D11Device * device, HWND hwnd);
	virtual bool setVSParameter(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, XMMATRIX worldInverseTranspose) sealed;
	virtual bool setPSParameter(ID3D11DeviceContext* deviceContext, Material mat, PointLight pointLight, XMFLOAT3 campos, Texture2D* texture2D, ID3D11ShaderResourceView* cubeMap, bool isUseTexture, bool isUseCubeMap, bool isUseAlphaCliptest, Fog fog) sealed;

	void clean();

protected:
	ID3D11VertexShader * m_pVertexShader;
	ID3D11PixelShader * m_pPixelShader;
	
private:
	WCHAR* psFileName;
	WCHAR* vsFileName;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightBuffer;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX worldReverseTranspose;
	};

	struct LightBufferType
	{
		Material material;
		PointLight pointLight;
		XMFLOAT3 cameraPosition;
		int isUseTexture;
		int isUseAlphaClipTest;
		int isUseCubeMap;
		float fogStart;
		float fogRange;
		XMFLOAT4 fogColor;
	};




};

