#include "Shape.h"
#include <d3dApp.h>

class Present :public D3DAPP {
public:
	Present(HINSTANCE hInstance);
	~Present(); 

	bool Init();
	void OnResize();
	void UpdateScene();
	void DrawScene();
private:
	void BuildBuffer();
	void BuildLayout();
	void BuildShader();

	void LookAt(XMFLOAT4X4 &m); 
	void Projection(XMFLOAT4X4 &m);

private:
	ID3D11Buffer *mVBuffer;
	ID3D11Buffer *mIBuffer;
	ID3DX11Effect *mFx; 
	ID3DX11EffectTechnique *mTech;
	D3DX11_TECHNIQUE_DESC mtechDesc;

	XMFLOAT4X4 mLtoW;
	XMFLOAT4X4 mWtoV;
	XMFLOAT4X4 mVtoH;

	XMFLOAT3 mEye;

};

void Present::LookAt(XMFLOAT4X4 &m) {
	XMVECTOR pos = XMVectorSet(mEye.x, mEye.y, mEye.z, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m, V);
}

void Present::Projection(XMFLOAT4X4 &m) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25 * Pi, GetAspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m, P);
}

Present::Present(HINSTANCE hInstance) : D3DAPP(hInstance), 
	mVBuffer(0), 
	mIBuffer(0), 
	mFx(0), 
	mTech(0){ 

	mEye.x = -2.0f; 
	mEye.y = 0.0f; 
	mEye.z = 0.0f; 

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mLtoW, I);
	LookAt(mWtoV);
	Projection(mVtoH);
}

Present::~Present() {
	ReleaseCOM(mTech);
	ReleaseCOM(mFx);
	ReleaseCOM(mVBuffer);
	ReleaseCOM(mIBuffer);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PreInstace, 
	PSTR cmdLine, int showCmd) {
}