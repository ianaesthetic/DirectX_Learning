#include "d3dApp.h"
#include "Grid.h"
#include <fstream>
ofstream fout;

struct Vertex {	
	XMFLOAT3 cPos;
	XMFLOAT4 cColor;

	Vertex() {}

	Vertex(const XMFLOAT3 pos, const XMVECTORF32 color) {
		cPos = pos;
		XMStoreFloat4(&cColor, XMVECTOR(color));
	}
};

class Hills : public D3DAPP {
public:
	Hills(HINSTANCE hInstace);
	~Hills();

	bool Init();
	void OnResize(); 
	void UpdateScene(float dt); 
	void DrawScene();

	/*void OnMouseDown(WPARAM btnState, int x, int y); 
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);*/
	
private:
	void BuildGeometryBuffer();
	void BuildInputLayout();
	void BuildShader();
	void LookAt(XMFLOAT4X4 &m); 
	void Projection(XMFLOAT4X4 &m);

	ID3DX11Effect* mFx;
	ID3DX11EffectTechnique* mTech;
	ID3D11Buffer* mVertexBuffer; 
	ID3D11Buffer* mIndexBuffer;
	ID3D11InputLayout* mInputLayout;

	XMFLOAT4X4 mLtoW;
	XMFLOAT4X4 mWtoV; 
	XMFLOAT4X4 mVtoH;

	float mTheta; 
	float mPhi;
	float mRadius;

	float mGridLeft; 
	float mGridRight; 
	float mGridUp;
	float mGridDown;
	int mGridW;	
	int mGridH;
	//POINT mLastMousePos; 
};


void Hills::OnResize() {
	D3DAPP::OnResize();
	Projection(mVtoH);
}

void Hills::LookAt(XMFLOAT4X4 &m) {
	float x, y, z; 
	x = mRadius * sinf(mPhi) * cosf(mTheta);
	y = mRadius * sinf(mPhi) * sinf(mTheta);
	z = mRadius * cosf(mPhi);
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m, V);
}

void Hills::Projection(XMFLOAT4X4 &m) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Pi, GetAspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m, P);
}

Hills::Hills(HINSTANCE hInstance) :
	D3DAPP(hInstance),
	mFx(0),
	mTech(0),
	mVertexBuffer(0),
	mIndexBuffer(0),
	mInputLayout(0),
	mTheta(1.5f * Pi),
	mPhi(0.5f * Pi),
	mRadius(5.0f),
	mGridLeft(-10.0f),
	mGridRight(+10.0f),
	mGridUp(+10.f),
	mGridDown(-10.f),
	mGridW(3),
	mGridH(3){

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mLtoW, I);
	LookAt(mWtoV);
	Projection(mVtoH);
}

Hills::~Hills() {
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mVertexBuffer); 
	ReleaseCOM(mIndexBuffer);
	ReleaseCOM(mFx);
	ReleaseCOM(mTech);
}

void Hills::UpdateScene(float dt) {
	LookAt(mWtoV);
}

void Hills::DrawScene() {
	//Clear View
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, Colors::Blue);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_STENCIL | D3D10_CLEAR_DEPTH, 1.0f, 0);

	//Input Layout
	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Buffers
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IAGetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Matrix 
	XMMATRIX LtoW = XMLoadFloat4x4(&mLtoW);
	XMMATRIX WtoV = XMLoadFloat4x4(&mWtoV);
	XMMATRIX VtoH = XMLoadFloat4x4(&mVtoH);
	XMMATRIX WVH = LtoW * WtoV * VtoH;

	XMFLOAT4X4 t;
	XMStoreFloat4x4(&t, WVH);

	fout << t._11 << " " << t._12 << " " << t._13 << " " << t._14 << endl
		<< t._21 << " " << t._22 << " " << t._23 << " " << t._24 << endl
		<< t._31 << " " << t._32 << " " << t._33 << " " << t._34 << endl
		<< t._41 << " " << t._42 << " " << t._43 << " " << t._44 << endl;
	fout << "***********\n"; 

	//Draw!
	D3DX11_TECHNIQUE_DESC techDesc; 
	ID3DX11EffectMatrixVariable* fxWVH;
	fxWVH = mFx->GetVariableByName("gWVP")->AsMatrix();
	fxWVH->SetMatrix(reinterpret_cast<float*>(&WVH));
	mFx->GetTechniqueByName("ColorTech")->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p) {
		mTech->GetPassByIndex(p)->Apply(0, mDeviceContext);
		//mDeviceContext->DrawIndexed(mGridH * mGridW * 6, 0, 0);
		mDeviceContext->DrawIndexed(36, 0, 0); 
	}
	HR(mSwapChain->Present(0,0));

}

bool Hills::Init() {
	if (!D3DAPP::Init()) {
		return 0;
	}
	BuildGeometryBuffer();
	BuildShader();
	BuildInputLayout();
	return 1; 
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance, PSTR cmdLine, int showCmd ) {
	fout.open("test.txt");
	Hills theAPP(hInstance);
	if (!theAPP.Init()) {
		return 0; 
	}
	return theAPP.Run();
}

void Hills::BuildGeometryBuffer() {
	/*Grid Terrain(mGridLeft, mGridRight, mGridUp, mGridDown, mGridW, mGridH);
	std::vector<Vertex> Vertices(Terrain.AmountOfVertex());
	for (int i = 0; i <= mGridH; ++i) {
		for (int j = 0; j <= mGridW; ++j) {
			int tIndex = Terrain.IndexQuery(j, i);
			float x, y, z;
			Terrain.PosQuery(x, y, z, j, i);
			Vertices[tIndex] = Vertex(XMFLOAT3(x, y, z), Colors::White);
		}
	}
	
	/*for (int i = 0; i < Terrain.AmountOfVertex(); ++i) {
		fout << Vertices[i].cPos.x << " " << Vertices[i].cPos.z << " "<< i << endl;
	}


	std::vector<UINT> Indices(Terrain.SizeofGrid() * 6);
	for (int i = 0; i < mGridH; ++i) {
		for (int j = 0; j < mGridW; ++j) {
			int s = 6 * (i * mGridW + j);
			Indices[s] = Terrain.IndexQuery(j, i);
			Indices[s + 1] = Terrain.IndexQuery(j, i + 1);
			Indices[s + 2] = Terrain.IndexQuery(j + 1, i);
			Indices[s + 3] = Terrain.IndexQuery(j + 1, i);
			Indices[s + 4] = Terrain.IndexQuery(j, i + 1);
			Indices[s + 5] = Terrain.IndexQuery(j + 1, i + 1);
			/*fout << s << " " << endl; 
			for (int l = 0; l < 6; ++l) {
				fout << Indices[s + l] << " ";
			}
			fout << endl;
		}
	}*/

	Vertex Vertices[] = {
		Vertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), Colors::White),
		Vertex(XMFLOAT3(-1.0f, +1.0f, -1.0f), Colors::Black),
		Vertex(XMFLOAT3(+1.0f, +1.0f, -1.0f), Colors::Red),
		Vertex(XMFLOAT3(+1.0f, -1.0f, -1.0f), Colors::Green),
		Vertex(XMFLOAT3(-1.0f, -1.0f, +1.0f), Colors::Blue),
		Vertex(XMFLOAT3(-1.0f, +1.0f, +1.0f), Colors::Yellow),
		Vertex(XMFLOAT3(+1.0f, +1.0f, +1.0f), Colors::Cyan),
		Vertex(XMFLOAT3(+1.0f, -1.0f, +1.0f), Colors::Magenta),
	};

	UINT Indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//VBDesc.ByteWidth = sizeof(Vertex) * Terrain.AmountOfVertex();
	VBDesc.ByteWidth = sizeof(Vertex) * 8;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = 0;
	VBDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA VData;
	VData.pSysMem = &Vertices[0];
	HR(mDevice->CreateBuffer(&VBDesc, &VData, &mVertexBuffer));

	D3D11_BUFFER_DESC IBDesc;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.ByteWidth = sizeof(UINT) * 36;
	//IBDesc.ByteWidth = sizeof(UINT) * Terrain.SizeofGrid() * 6;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = 0;
	IBDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA IData;
	IData.pSysMem = &Indices[0];
	HR(mDevice->CreateBuffer(&IBDesc, &IData, &mIndexBuffer));
}

void Hills::BuildInputLayout() {
	D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC PassDesc;
	mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	HR(mDevice->CreateInputLayout(InputDesc, 2, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mInputLayout));
}

void Hills::BuildShader() {
	ID3DBlob* Error = 0;
	HRESULT hr = 
		D3DX11CompileEffectFromFile(L"Color.fx", NULL, NULL, 
			D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, mDevice, &mFx, &Error);
	if (Error != 0) {
		MessageBoxA(0, (char*)Error->GetBufferPointer(), 0, 0);
		ReleaseCOM(Error);
	}

	if (FAILED(hr)) {
		DXTrace(__FILEW__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	mTech = mFx->GetTechniqueByName("ColorTech");
}