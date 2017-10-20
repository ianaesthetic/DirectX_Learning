#include "d3Util.h"
#include "d3dApp.h"

class IndirectD3DAPP :public D3DAPP {
	IndirectD3DAPP(HINSTANCE hIstance);
	~IndirectD3DAPP();

	bool Init(); 
	void UpdateScene(float dt);
	void DrawScene();
	void OnResize();
};

IndirectD3DAPP::IndirectD3DAPP(HINSTANCE hInstance) : D3DAPP(hInstance) {
}

bool IndirectD3DAPP::Init() {
	return D3DAPP::Init();
}
