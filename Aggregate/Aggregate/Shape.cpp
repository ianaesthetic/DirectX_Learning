#include "Shape.h"
#include "d3Util.h"
#define _x(a) XMVectorGetX(a)
#define _y(a) XMVectorGetY(a)
#define _z(a) XMVectorGetZ(a)
#define _w(a) XMVectorGetW(a)

#define _index(a, b, c) ((a << 2) | (b << 1) | c)

Box::Box() { 
	mVCount = 8;
	mICount = 36; 
	mVSize = mVCount * sizeof(Vertex);
	mISize = mICount * sizeof(UINT);

	mVData.push_back(Vertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::White));
	mVData.push_back(Vertex(XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Black));
	mVData.push_back(Vertex(XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Red));
	mVData.push_back(Vertex(XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Green));
	mVData.push_back(Vertex(XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Blue));
	mVData.push_back(Vertex(XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Yellow));
	mVData.push_back(Vertex(XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Cyan));
	mVData.push_back(Vertex(XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), Colors::Magenta));
	
	int s = 0;
	mIData[s + 0] = 0, mIData[s + 1] = 1, mIData[s + 2] = 2; 
	mIData[s + 3] = 0, mIData[s + 4] = 2, mIData[s + 5] = 3;
	s += 6; 

	mIData[s + 0] = 4, mIData[s + 1] = 6, mIData[s + 2] = 5;
	mIData[s + 3] = 4, mIData[s + 4] = 7, mIData[s + 5] = 6;
	s += 6;

	mIData[s + 0] = 4, mIData[s + 1] = 5, mIData[s + 2] = 1;
	mIData[s + 3] = 4, mIData[s + 4] = 1, mIData[s + 5] = 0;
	s += 6;

	mIData[s + 0] = 3, mIData[s + 1] = 2, mIData[s + 2] = 6;
	mIData[s + 3] = 3, mIData[s + 4] = 6, mIData[s + 5] = 7;
	s += 6;

	mIData[s + 0] = 1, mIData[s + 1] = 5, mIData[s + 2] = 6;
	mIData[s + 3] = 1, mIData[s + 4] = 6, mIData[s + 5] = 2;
	s += 6;

	mIData[s + 0] = 4, mIData[s + 1] = 0, mIData[s + 2] = 3;
	mIData[s + 3] = 4, mIData[s + 4] = 3, mIData[s + 5] = 7;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mLtoW, I);
}