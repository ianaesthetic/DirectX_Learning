#include "d3Util.h"
#include "d3dApp.h"

class IndirectD3DAPP :public D3DAPP {
public:
	IndirectD3DAPP(HINSTANCE hIstance);
	~IndirectD3DAPP();

	bool Init(); 
	void UpdateScene(float dt);
	void DrawScene();
	void OnResize();

	int Run(); 
};

IndirectD3DAPP::IndirectD3DAPP(HINSTANCE hInstance) : D3DAPP(hInstance) {
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd) {
	IndirectD3DAPP theAPP(hInstance);
	if (!theAPP.Init()) return 0;
	return theAPP.Run(); 
}

IndirectD3DAPP::~IndirectD3DAPP() {
	D3DAPP::~D3DAPP();
}

bool IndirectD3DAPP::Init() {
	return D3DAPP::Init();
}

void IndirectD3DAPP::UpdateScene(float dt) {

}


void IndirectD3DAPP::OnResize()
{
	D3DAPP::OnResize();
}

void IndirectD3DAPP::DrawScene() {
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	HR(mSwapChain->Present(0, 0));
}

int IndirectD3DAPP::Run() {
	return D3DAPP::Run();
}
