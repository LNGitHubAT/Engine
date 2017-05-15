#pragma once
#include <windows.h>
#include "Define.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class MeshGenerator
{
public:
	MeshGenerator();
	~MeshGenerator();
	static bool getDataFromFile(WCHAR* fileName, BasicVertex** vertices, UINT &vertexCount, UINT** indices, UINT& indexCount);
	static bool createPlane(XMINT2 XZsize, XMFLOAT2 XZoffset, XMINT2 texCoordTile, BasicVertex** vertices, UINT &vertexCount, UINT** indices, UINT& indexCount);
};

