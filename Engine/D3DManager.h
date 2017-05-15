#pragma once
#include <d3d11.h>
class D3DManager
{
public:
	D3DManager();
	~D3DManager();
	bool init(int screenWidth, int screenHeight, bool enable4xMsAA, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void beginFrame(float red, float green, float blue, float alpha);
	void endFrame();
	void clearDepthStencilView();
	void clearStencilView();
	void clearDepthView();
	ID3D11Device * getDevice();
	ID3D11DeviceContext * getDeviceContext();
	void clean();
	void backFaceCulling(bool);
	void transparent(bool);
	void setFrontFaceCCW(bool);
	void onlyWriteStencilBuffer(bool);
	void preventDoubleBlending(bool);
	void drawReflection(bool);
	void setRenderTargetView(ID3D11RenderTargetView* renderTargetView);
	void wireFrame(bool);
	void renderToCubeMap(bool enable);
private:
	ID3D11Device * m_pDevice;
	ID3D11DeviceContext * m_pDeviceContext;

	IDXGISwapChain* m_pSwapChain;

	ID3D11Texture2D *m_pDepthStencilBuffer;

	ID3D11RenderTargetView *m_pRenderTargetView;

	ID3D11DepthStencilView *m_pDepthStencilView;
	ID3D11DepthStencilView *m_pDynamicCubeMapDepthStencilView;

	ID3D11DepthStencilState* preventDoubleBlendingStencilDepthState;
	ID3D11DepthStencilState* m_pOnlyWriteStencilDepthStencilState;
	ID3D11DepthStencilState* m_pReflectionDepthStencilState;


	ID3D11RasterizerState* m_pWireFrameRasterState;
	ID3D11RasterizerState* m_pDefaultRasterState;
	ID3D11RasterizerState* m_pRasterStateNoBackCulling;
	ID3D11RasterizerState* m_pReflectionRasterState;
	ID3D11RasterizerState* m_pReflectionRasterStateNoBackCulling;

	ID3D11BlendState* m_pDefaultBlendState;
	ID3D11BlendState* m_BlendStateTransparent;
	ID3D11BlendState* m_noWriteToRenderTarget;

	D3D11_VIEWPORT mScreenViewport;

	D3D11_VIEWPORT m_cubeMapViewport;
	bool mEnable4xMsaa;

private:

	bool m_backFaceCulling;
	bool m_transparent;
	bool m_frontFaceCCW;
	bool m_onlyWriteStencilBuffer;
	bool m_preventDoubleBlending;
	bool m_drawReflection;
	bool m_isRenderingToCubeMap;
};

