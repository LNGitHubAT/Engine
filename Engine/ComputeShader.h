#pragma once
#include <d3d11.h>
#include "BaseShader.h"
#include <d3dcompiler.h>
#include <fstream>
#include "Define.h"
#include <DirectXMath.h>
#include "Texture2D.h"

using namespace DirectX;
using namespace std;
class ComputeShader : public BaseShader
{
public:
	ComputeShader();
	~ComputeShader();
	bool init(ID3D11Device * device, HWND hwnd, WCHAR* csFileName);
	void bindShader(ID3D11DeviceContext* deviceContext) override;
	void unBindShader(ID3D11DeviceContext* deviceContext) override;

private:
	ID3D11ComputeShader* m_pComputeShader;
};

