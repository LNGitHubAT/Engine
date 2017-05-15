#pragma once
#include "Shader.h"
class TerrainShader : public BaseShader
{
public:
	TerrainShader();
	~TerrainShader();
	bool init(ID3D11Device * device, HWND hwnd, WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename ,WCHAR* psFilename);
	virtual void bindShader(ID3D11DeviceContext * deviceContext);
	virtual void unBindShader(ID3D11DeviceContext * deviceContext);
	virtual bool createInputLayout(ID3D11Device * device, ID3D10Blob* vertexShaderBuffer);
	// create constant buffer
	virtual bool createConstantBuffers(ID3D11Device * device);
	virtual bool createShaders(ID3D11Device * device, HWND hwnd);
	virtual bool setVSParameter(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix , ID3D11ShaderResourceView * heightMap);
	virtual bool setHSParameter(ID3D11DeviceContext * deviceContext, XMFLOAT3 cameraPosition, float minDist, float maxDist, float maxTess, float minTess, XMFLOAT4 frustumPlanes[6]);
	virtual bool setDSParameter(ID3D11DeviceContext * deviceContext, XMMATRIX wolrdMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float textureScale, ID3D11ShaderResourceView * heightMapSRV);
	virtual bool setPSParameter(ID3D11DeviceContext * deviceContext, Material material,	PointLight pointLight,	XMFLOAT3 cameraPosition, XMFLOAT4 fogColor,	float fogStart, float fogRange,	float worldCellSpace,XMFLOAT2 heightMapCellSpace , ID3D11ShaderResourceView * heightMapSRV, ID3D11ShaderResourceView * blendMapSRV, ID3D11ShaderResourceView * LayersMapSRV);

	//void clean();

private:
	WCHAR* vsFilename;
	WCHAR* hsFilename;
	WCHAR* dsFilename;
	WCHAR* psFilename;

	ID3D11VertexShader * m_pVertexShader;
	ID3D11HullShader   * m_pHullShader;
	ID3D11DomainShader * m_pDomainShader;
	ID3D11PixelShader * m_pPixelShader;
	
private:
	// constant buffer
	struct PerVertexBuffer
	{
		XMMATRIX worldMatrix;
	};

	struct PerPatchBuffer
	{
		// Tesselation depends on distant between point and camera
		XMFLOAT3 cameraPosition;
		float minDist;
		float maxDist;
		float minTess;
		float maxTess;
		float pad;
		XMFLOAT4 frustumPlane[6];

	};


	struct PerTesselatedPointBuffer
	{
		XMMATRIX wolrdMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
		float textureScale;
		XMFLOAT3 pad;
	};

	struct PerPixelBuffer
	{
		Material material;
		PointLight pointLight;
		XMFLOAT3 cameraPosition;
		float worldCellSpace;;
		XMFLOAT4 fogColor;
		float fogStart;
		float fogRange;
		XMFLOAT2 heightMapCellSpace;
	};

	ID3D11Buffer * perVertexBuffer;

	ID3D11Buffer * perPatchBuffer;

	ID3D11Buffer * perTesselatedPointBuffer;

	ID3D11Buffer * perPixelBuffer;


	

};

