#include "d3dApp.h"
#include <fstream>
ofstream fout; 

struct Vertex {
	XMFLOAT3 cPos; 
	XMFLOAT4 cColor; 

	Vertex(const XMFLOAT3 pos, const XMVECTORF32 color) {
		cPos = pos;
		XMStoreFloat4(&cColor, XMVECTOR(color));
	}
};

class BoxDemo:public D3DAPP {
public:
	BoxDemo(HINSTANCE); 
	~BoxDemo(); 

	bool Init();
	void UpdateScene(float dt); 
	void DrawScene();
	void OnResize(); 

private: 
	void BuildInputLayout(); 
	void BuildFx(); 
	void BuildVertexBuffer();

	void OnMouseDown(WPARAM wParam, int x, int y);
	void OnMouseUp(WPARAM wParam, int x, int y);
	void OnMouseMove(WPARAM wParam, int x, int y);

	void LookAt(XMFLOAT4X4& view);

private:
	ID3DX11Effect* mFx; 
	ID3DX11EffectTechnique* mTech; 
	ID3D11InputLayout* mInputLayout; 
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	XMFLOAT4X4 mLocalWorld;
	XMFLOAT4X4 mWorldView;
	XMFLOAT4X4 mViewProj;

	// Dealing With Moving;
	float mTheta;
	float mPhi; 
	float mRadius; 
	POINT mLastMousePos;
};

void BoxDemo::LookAt(XMFLOAT4X4& view) {
	float x, y, z; 
	x = mRadius * sinf(mPhi) * cosf(mTheta);
	y = mRadius * sinf(mPhi) * sinf(mTheta); 
	z = mRadius * cosf(mPhi);
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&view, V);
}

BoxDemo::BoxDemo(HINSTANCE hInstance) :
	D3DAPP(hInstance),
	mFx(0),
	mTech(0),
	mInputLayout(0),
	mVertexBuffer(0),
	mIndexBuffer(0), 
	mTheta(0.0f * Pi), 
	mPhi(0.5f * Pi), 
	mRadius(5.0f){
	
	mLastMousePos.x = 0; 
	mLastMousePos.y = 0; 

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mLocalWorld, I);
	LookAt(mWorldView);
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Pi, GetAspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mViewProj, P);
}

BoxDemo::~BoxDemo() {
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mTech);
	ReleaseCOM(mFx);
}

bool BoxDemo::Init() {
	if (!D3DAPP::Init()) return 0; 
	BuildFx(); 
	BuildVertexBuffer(); 
	BuildInputLayout(); 
	return 1; 
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance, PSTR cmdLine, int showCmd) {
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	fout.open("box.log");

	BoxDemo theApp(hInstance);
	if (!theApp.Init())
		return 0; 
	return theApp.Run();
}

void BoxDemo::UpdateScene(float dt) {
	XMMATRIX V;
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float y = mRadius * sinf(mPhi) * sinf(mTheta); 
	float z = mRadius * cosf(mPhi);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mWorldView, V);
}

void BoxDemo::OnResize() {
	D3DAPP::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Pi, D3DAPP::GetAspectRatio(), 1.0f, 1000.0f);	
	XMStoreFloat4x4(&mViewProj, P);
}

/*void BoxDemo::DrawScene() {
//Clear View
mDeviceContext->ClearRenderTargetView(mRenderTargetView, Colors::Black);
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
XMMATRIX LtoW = XMLoadFloat4x4(&mLocalWorld);
XMMATRIX WtoV = XMLoadFloat4x4(&mWorldView);
XMMATRIX VtoH = XMLoadFloat4x4(&mViewProj);
XMMATRIX WVH = LtoW * WtoV * VtoH;

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
HR(mSwapChain->Present(0, 0));

}*/


void BoxDemo :: DrawScene() {
	//Clear View
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, Colors::Black);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_STENCIL | D3D10_CLEAR_DEPTH, 1.0f, 0);

	//Input Layout
	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IAGetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	/*
	//Buffers
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	*/
	XMMATRIX localWorld = XMLoadFloat4x4(&mLocalWorld);
	XMMATRIX worldView = XMLoadFloat4x4(&mWorldView);
	XMMATRIX viewProj = XMLoadFloat4x4(&mViewProj);
	XMMATRIX WVP = localWorld * worldView * viewProj;

	ID3DX11EffectMatrixVariable* cWVP;
	cWVP = mFx->GetVariableByName("gWorldViewProj")->AsMatrix();
	cWVP->SetMatrix(reinterpret_cast<float*>(&WVP));

	//Draw!
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i) {
		mTech->GetPassByIndex(i)->Apply(0, mDeviceContext);
		mDeviceContext->DrawIndexed(36, 0, 0);
	}
	HR(mSwapChain->Present(0,0));
}	

/*
void BoxDemo::DrawScene() {
	//Clear Scene
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, Colors::Black);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//LayoutSet
	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Vertex Buffer & Index Buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//MatrixSetting

	XMMATRIX localWorld = XMLoadFloat4x4(&mLocalWorld);
	XMMATRIX worldView = XMLoadFloat4x4(&mWorldView);
	XMMATRIX viewProj = XMLoadFloat4x4(&mViewProj);
	XMMATRIX WVP = localWorld * worldView * viewProj;

	ID3DX11EffectMatrixVariable* cWVP;
	cWVP = mFx->GetVariableByName("gWorldViewProj")->AsMatrix();
	cWVP->SetMatrix(reinterpret_cast<float*>(&WVP));

	//Draw!
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i) {
		mTech->GetPassByIndex(i)->Apply(0, mDeviceContext);
		mDeviceContext->DrawIndexed(36, 0, 0);
	}
	HR(mSwapChain->Present(0, 0));
}*/

void BoxDemo::BuildFx() {
	ID3DBlob* ErrorMsg = 0;
	HRESULT hr = D3DX11CompileEffectFromFile(L"Color.fx", NULL, NULL,
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION,
		0, mDevice, &mFx, &ErrorMsg);
	if (ErrorMsg != 0) {
		MessageBoxA(0, (char*)ErrorMsg->GetBufferPointer(), 0, 0);
		ReleaseCOM(ErrorMsg);
	}

	if (FAILED(hr)) {
		DXTrace(__FILEW__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	mTech = mFx->GetTechniqueByName("ColorTech");
}

void BoxDemo::BuildVertexBuffer() {

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

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;; 
	VBDesc.ByteWidth = 8 * sizeof(Vertex);
	VBDesc.CPUAccessFlags = 0; 
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = 0; 
	VBDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;

	HR(mDevice->CreateBuffer(&VBDesc, &VertexData, &mVertexBuffer));

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

	D3D11_BUFFER_DESC IBDESC; 
	IBDESC.BindFlags = D3D11_BIND_INDEX_BUFFER; 
	IBDESC.ByteWidth = sizeof(UINT) * 36; 
	IBDESC.CPUAccessFlags = 0; 
	IBDESC.MiscFlags = 0; 
	IBDESC.StructureByteStride = 0;
	IBDESC.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA IndexData; 
	IndexData.pSysMem = Indices; 
	
	HR(mDevice->CreateBuffer(&IBDESC, &IndexData, &mIndexBuffer));
}

void BoxDemo::BuildInputLayout() {
	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC PassDesc; 
	mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	HR(mDevice->CreateInputLayout(VertexInputDesc, 2, 
		PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mInputLayout));
}

void BoxDemo::OnMouseDown(WPARAM btnState, int x, int y) {
	mLastMousePos.x = x; 
	mLastMousePos.y = y; 
	SetCapture(mhMainWnd);
}

void BoxDemo::OnMouseUp(WPARAM btnState, int x, int y) {
	ReleaseCapture();
}

void BoxDemo::OnMouseMove(WPARAM btnState, int x, int y) {
	if ((btnState & MK_LBUTTON) != 0) {
		float dx = 0.025f * static_cast<float>(x - mLastMousePos.x); 
		float dy = 0.025f * static_cast<float>(y - mLastMousePos.y);
		mTheta += dx; 
		mPhi -= dy; 
		//mPhi = Clamp(0.0f, Pi - 0.001f, mPhi);
	}
	else if ((btnState & MK_RBUTTON) != 0) {
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x); 
		float dy = 0.005f* static_cast<float>(y - mLastMousePos.y);

		mRadius += dy - dx;
		mRadius = Clamp(3.0f, 15.0f, mRadius);
	}
	mLastMousePos.x = x;
	mLastMousePos.y = y; 
}