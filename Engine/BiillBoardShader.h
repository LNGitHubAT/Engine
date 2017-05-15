#pragma once
#include "Shader.h"
class BillBoardShader : public Shader
{
public:
	BillBoardShader();
	~BillBoardShader();
	bool init(ID3D11Device * device, HWND hwnd, WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);
	void bindShader(ID3D11DeviceContext* deviceContext) override;
	void unBindShader(ID3D11DeviceContext* deviceContext) override;
	//bool setPSParameter(ID3D11DeviceContext* deviceContext, Material mat, PointLight pointLight,  bool isUseTexture, bool isUseAlphaCliptest, Fog fog);
	bool setGSParameters(ID3D11DeviceContext * deviceContext, XMMATRIX viewProjectionMatrix, XMFLOAT3 camposition);
private:
	ID3D11GeometryShader* m_pGeometryShader;

	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightBuffer;

	struct MatrixBufferType
	{
		XMMATRIX viewProjectionMatrix;
		XMFLOAT3 camposition;
		float pad;
	};

	struct LightBufferType
	{
		Material material;
		PointLight pointLight;
		XMFLOAT3 cameraPosition;
		int isUseTexture;
		int isUseAlphaClipTest;
		float fogStart;
		float fogRange;
		float pad;
		XMFLOAT4 fogColor;
	};
};

