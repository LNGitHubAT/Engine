#include "MeshLoader.h"

std::vector<std::string> split(const std::string& text, const std::string& delims)
{
	std::vector<std::string> tokens;
	std::size_t start = text.find_first_not_of(delims), end = 0;

	while ((end = text.find_first_of(delims, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = text.find_first_not_of(delims, end);
	}
	if (start != std::string::npos)
		tokens.push_back(text.substr(start));

	return tokens;
}


MeshGenerator::MeshGenerator()
{
}


MeshGenerator::~MeshGenerator()
{
}

bool MeshGenerator::getDataFromFile(WCHAR* fileName, BasicVertex** ppVertices, UINT &vertexCount, UINT** ppIndices, UINT& indexCount)
{
	std::vector<XMFLOAT3> vertexPosistion;
	std::vector<XMFLOAT3> vertexNormal;
	std::vector<XMFLOAT2> vertexTexCoord;

	std::vector<int> vindices;
	std::vector<int> nindices;
	std::vector<int> tindices;


	std::wstring ws(fileName);
	// your new String
	std::string strFileName(ws.begin(), ws.end());

	std::ifstream fin(fileName, std::ifstream::in);

	



	if (strFileName.find(".obj") != std::string::npos)
	{
		if (!fin)
		{
			std::wstring msg = L"Can not find ";
			msg = msg + fileName;
			MessageBox(0, msg.c_str(), 0, 0);
			return false;
		}
		else
		{
			std::string lineBuffer;

			while (std::getline(fin, lineBuffer))
			{
				std::istringstream ss(lineBuffer);
				std::string cmd;
				ss >> cmd;
				if (cmd == "v")
				{
					
					XMFLOAT3 pos;
					ss >> pos.x >> pos.y >> pos.z;
					pos.z *= -1.0f;
					vertexPosistion.push_back(pos);
				}
				if (cmd == "vt")
				{
					XMFLOAT2 texCord;
					ss >> texCord.x >> texCord.y;
					texCord.y = 1.0f - texCord.y;
					vertexTexCoord.push_back(texCord);
				}
				if (cmd == "vn")
				{
					XMFLOAT3 normal;
					ss >> normal.x >> normal.y >> normal.z;
					normal.z *= -1.0f;
					vertexNormal.push_back(normal);
				}

				if (cmd == "f")
				{
					
					std::vector<std::string> vertexStrings;
					ss >> cmd; 
					vertexStrings.push_back(cmd);
					ss >> cmd;
					vertexStrings.push_back(cmd);
					ss >> cmd;
					vertexStrings.push_back(cmd);
					for (int i = 0; i < 3; i++)
					{
						std::vector<std::string> strs;
						std::string vertexString = vertexStrings[2 - i];
						strs = split(vertexString, "/");
						
						vindices.push_back(atoi(strs[0].c_str()) - 1);
						tindices.push_back(atoi(strs[1].c_str()) - 1);
						nindices.push_back(atoi(strs[2].c_str()) - 1);
					}
					
					
				}
			}
			
			vertexCount = vertexPosistion.size();
			indexCount = vindices.size();
			*ppVertices = new BasicVertex[vertexCount];
			*ppIndices = new UINT[indexCount];

			for (int i = 0; i < indexCount; i++)
			{
				BasicVertex newVertex;

				newVertex.position = vertexPosistion[vindices[i]];
				newVertex.normal = vertexNormal[nindices[i]];
				newVertex.textureCoord = vertexTexCoord[tindices[i]];
				(*ppVertices)[vindices[i]] = newVertex;
				(*ppIndices)[i] = vindices[i];
			}

			int x = 10;

		}
	}
	else
	{

		UINT vcount = 0;
		UINT tcount = 0;
		std::string ignore;

		fin >> ignore >> vcount;
		fin >> ignore >> tcount;
		fin >> ignore >> ignore >> ignore >> ignore;

		XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

		*ppVertices = new BasicVertex[vcount];
		for (UINT i = 0; i < vcount; ++i)
		{
		fin >> (*ppVertices)[i].position.x >> (*ppVertices)[i].position.y >> (*ppVertices)[i].position.z;
		fin >> (*ppVertices)[i].normal.x >> (*ppVertices)[i].normal.y >> (*ppVertices)[i].normal.z;
		}

		fin >> ignore;
		fin >> ignore;
		fin >> ignore;

		*ppIndices = new UINT[tcount * 3];
		for (UINT i = 0; i < tcount; ++i)
		{
			fin >> (*ppIndices)[i * 3 + 0] >> (*ppIndices)[i * 3 + 1] >> (*ppIndices)[i * 3 + 2];
		}

		indexCount = tcount * 3;
		vertexCount = vcount;
	}

	fin.close();

	return true;

}

bool MeshGenerator::createPlane(XMINT2 XZsize, XMFLOAT2 XZoffset, XMINT2 texCoordTile,BasicVertex ** vertices, UINT & vertexCount, UINT ** indices, UINT & indexCount)
{
	vertexCount = XZsize.x * XZsize.y;
	(*vertices) = new BasicVertex[vertexCount];
	indexCount = (XZsize.x - 1) * (XZsize.y - 1) * 2 * 3;
	*indices = new UINT[indexCount];

	XMFLOAT2 UV;
	UV.x = 1.0f / (XZsize.y - 1) * texCoordTile.x;
	UV.y = 1.0f / (XZsize.x - 1) * texCoordTile.y;

	for (UINT row = 0; row < XZsize.x; row++)
	{
		for (UINT col = 0; col < XZsize.y; col++)
		{
			(*vertices)[row * XZsize.y + col] = BasicVertex((float)row * XZoffset.x, 0.0f, (float)col * XZoffset.y, 0.0f, 1.0f, 0.0f, (float)row * UV.x, (float)col * UV.y);
		}
	}

	UINT face = 0;
	for (UINT row = 0; row < XZsize.x - 1; ++row)
	{
		for (UINT col = 0; col < XZsize.y - 1; ++col)
		{
			(*indices)[face] = row * XZsize.y + col;
			(*indices)[face + 1] = row* XZsize.y + col + 1;
			(*indices)[face + 2] = (row + 1)* XZsize.y + col;

			(*indices)[face + 3] = (row + 1)*XZsize.y + col;
			(*indices)[face + 4] = row* XZsize.y + col + 1;
			(*indices)[face + 5] = (row + 1)* XZsize.y + col + 1;

			face += 6; // next quad
		}
	}
	return true;
}


