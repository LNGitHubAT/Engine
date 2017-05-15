#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Define.h"
#include <vector>
#include <sstream>
#include <fstream>
#include "MathUtility.h"
#include "Texture2D.h"
#include "Object.h"
#include "PointLightObject.h"
#include "Camera.h"
#include "TerrainShader.h"
#include <DirectXPackedVector.h>
#include <algorithm>


using namespace DirectX;
class Terrain  : public GameObject
{
public:
	Terrain();
	~Terrain();
	struct InitInfo
	{
		std::wstring HeightMapFilename;
		std::vector<WCHAR*> layerMapFileNames;
		std::wstring BlendMapFilename;
		float HeightScale;
		UINT HeightmapWidth;
		UINT HeightmapHeight;
		float CellSpacing;
	};
	bool init(ID3D11Device* device, ID3D11DeviceContext* dc, InitInfo initInfo, TerrainShader * shader, float textureScale);
	void render(ID3D11DeviceContext * deviceContext, Camera* cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality = false);
	float getWidth()const;
	float getDepth()const;
	void getHeightAndNormal(float x, float z, float& height, XMFLOAT3& normal)const;
protected:
	
private:
	// store height value to CPU mem
	bool loadHeightMap();
	// smooth height map after load it
	void smoothHeightMap();
	// get the average height value by Averaging all of vertex (i , j) 's neighbors height
	float average(UINT col, UINT row);
	// if current index is valid
	bool isInBound(UINT col, UINT row);
	// build vertex buffer
	bool buildPatchVertexBuffer(ID3D11Device* device);
	// build index buffer 
	bool buildPatchIndexBuffer(ID3D11Device* device);
	// find boundary in y
	void calculateYBoundaries();
	// find boundary in y of a patch
	void calcuateYBoundary(int col, int row);
	// build height map SRV
	bool buildHeightMapSRV(ID3D11Device* device);
	// build texture array and blend map
	bool buildLayersBlendMap(ID3D11Device* device, ID3D11DeviceContext * deviceContext);


private:
	const int maxCellPerPatch = 64.0f;

	ID3D11Buffer* m_pQuadVertexBuffer;

	ID3D11Buffer* m_pIndexBuffer;

	InitInfo m_initInfo;

	//----
	//Texture
	//------
	// 5 layers : main texture + 4 rbga layers map
	ID3D11ShaderResourceView* m_pLayesSRV; 
	// blend map
	ID3D11ShaderResourceView* m_pBlendMapsSRV;
	// height map
	ID3D11ShaderResourceView* m_pHeightMapSRV;
	// number of vertices in lowest level of terrain
	UINT m_numPatchVertices;
	// a patch is a quad. Number of patch
	UINT m_numPatchFaces;
	// number verrtices along all patch in a row
	UINT m_numVerticesPerPatchRows;
	// number verrtices along all patch in a col
	UINT m_numVerticesPerPatchCols;

	//Material m_material;
	// read height map and store in a vector
	std::vector<float> m_HeightMapStored;
	// compute min max y of a patch for frustum cullling
	std::vector<XMFLOAT2> m_patchYBoundary;

	float minDist = 20.0f;
	
	float maxDist = 500.0f;

	float maxTess = 6.0f;

	float minTess = 1.0f;

	float m_textureScale = 50.0f;

	XMFLOAT2 heightMapCellSpacing;
};

