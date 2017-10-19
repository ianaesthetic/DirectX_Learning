#include "d3dApp.h"
#include "GameTimer.h"

D3APP::D3APP(HINSTANCE hinstance) :
	mDevice(0),
	mDeviceContext(0),
	mSwapChain(0), 
	mDepthStencilBuffer(0),
	mRenderTargetView(0), 
	mDepthStencilView(0),

	mAppPaused(false), 
	mMsaa4XEnabled(false), 
	mClientWindowWidth(800), 
	mClientWindowHeight(600),
	mDriverType()
	
{}

bool D3APP::InitDirect3D() {
	UINT createDeviceFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,
		mDriverType,
		0,
		createDeviceFlag,
		0, 0,
		D3D11_SDK_VERSION,
		&mDevice,
		&featureLevel,
		&mContext
	);
	if (FAILED(hr)) {
		MessageBox(NULL, "Device Initialization Failed!", "Error", MB_OK);
		return 0;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_1) {
		MessageBox(NULL, "D3D11 Not Supported!", 0, MB_OK);
		return 0;
	}

	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &mMsaaQualityLevel);
}