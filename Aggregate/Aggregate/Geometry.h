#pragma once
#include <DirectXMath.h>
#include <vector>
#include <Windows.h>

using namespace DirectX;
using namespace std;

struct Vertex {
	XMFLOAT3 cPos; 
	XMFLOAT3 cNormal;
	XMFLOAT4 cColor;

	Vertex(FXMVECTOR pos, FXMVECTOR normal, XMVECTORF32 color) {
		XMStoreFloat3(&cPos, pos);
		XMStoreFloat3(&cNormal, normal); 
		XMStoreFloat4(&cColor, color);
	}

	Vertex(XMFLOAT3 pos, XMFLOAT3 normal, XMVECTORF32 color) {
		cPos = pos, cNormal = normal;
		XMStoreFloat4(&cColor, color);
	}
};

class Geometry {
public:
	Geometry();
	~Geometry() {};
	
	virtual int VertexCount(); 
	virtual int IndexCount();
	virtual int SizeofVertex();
	virtual int SizeofIndex();
	virtual Vertex* GetVertexData();
	virtual UINT*  GetIndexData(); 

protected:
	int mVCount;
	int mICount;
	int mVSize; 
	int mISize; 

	XMFLOAT4X4 mLtoW; 
	
	vector <Vertex> mVData; 
	vector <UINT>	mIData;
};
