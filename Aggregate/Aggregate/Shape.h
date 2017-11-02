#include "Geometry.h"
#pragma once

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

class Box :public Geometry {
	Box(float x, float y, float z, 
		vector<Vertex> &v, vector<UINT> &i, int &vb, int &ib, 
		float posX, float posY, float posZ); 
	~Box();
};


