#include "d3dApp.h"
#include "GameTimer.h"

namespace {
	D3DAPP* gd3dApp;
}

LRESULT CALLBACK WndProc(HWND hwnd, MSG msg, LPARAM lParam, WPARAM wParam) {
	return gd3dApp->MsgProc(hwnd, msg, lParam, wParam); 
}

D3DAPP::D3DAPP(HINSTANCE hinstance) :
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
	mDriverType(D3D_DRIVER_TYPE_HARDWARE),
	mMsaaQualityLevel(0) {	
	ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));
	gd3dApp = this;
}

D3DAPP::~D3DAPP() {
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mRenderTargetView); 
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mSwapChain); 
	if (mDeviceContext) mDeviceContext->ClearState(); 
	ReleaseCOM(mDeviceContext); 
	ReleaseCOM(mDevice);
}

HINSTANCE D3DAPP::GetAppInstance() const {
	return mhAppInstance; 
}

HWND D3DAPP::GetAppWindow() const {
	return mhMainWnd; 
}

float D3DAPP::GetAspectRatio() const {
	return (float)mClientWindowWidth / (float)mClientWindowHeight; 
}

bool D3DAPP::Init() {
	if (!InitWindow()) {
		return 0; 
	}
	if (!InitDirect3D()) {
		return 0; 
	}
	return 1; 
}

int D3DAPP::Run() {
	MSG msg; 

	mTimer.Reset();
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (!mAppPaused) {
				mTimer.Tick();
				if (!mAppPaused) {
					UpdateScene(mTimer.DeltaTime());
					DrawScene();
				}
			}
			else {
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;

}

bool D3DAPP::InitDirect3D() {
	UINT createDeviceFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Creating device and 
	
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
		&mDeviceContext
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
	assert(mMsaaQualityLevel > 0);

	//SwapChain Description
	DXGI_SWAP_CHAIN_DESC sDec;
	sDec.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sDec.BufferDesc.Width					= mClientWindowWidth;
	sDec.BufferDesc.Height					= mClientWindowHeight;
	sDec.BufferDesc.RefreshRate.Numerator	= 60;
	sDec.BufferDesc.RefreshRate.Denominator = 1; 
	sDec.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED; 
	sDec.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sDec.BufferCount = 1; 
	sDec.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sDec.Flags = 0;
	sDec.OutputWindow = mhMainWnd;
	DXGI_SAMPLE_DESC sd; 
	if (mMsaa4XEnabled) {
		sd.Count = 4; 
		sd.Quality = mMsaaQualityLevel - 1; 
	}
	else {
		sd.Count = 1; 
		sd.Quality = 0;
	}
	sDec.SampleDesc = sd; 
	sDec.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sDec.Windowed = 1; 

	//Creating SwapChain with interfaces
	IDXGIDevice* dxgiDevice;
	mDevice->QueryInterface(_uuidof(dxgiDevice), (void **) &dxgiDevice);

	IDXGIAdapter* dxgiAdapter; 
	dxgiDevice->GetAdapter(&dxgiAdapter);

	IDXGIFactory* dxgiFactory; 
	dxgiAdapter->GetParent(_uuidof(dxgiFactory), (void **)&dxgiFactory); 

	dxgiFactory->CreateSwapChain(mDevice, &sDec, &mSwapChain);

	ReleaseCOM(dxgiFactory);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiDevice);

	OnResize();
	return 1; 
}

void D3DAPP::OnResize() {
	assert(mDevice);
	assert(mDeviceContext);
	assert(mSwapChain);

	//Describing DepthBuffer

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mDepthStencilView);

	//Creating RenderTargetView

	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, _uuidof(backBuffer), reinterpret_cast<void**>(backBuffer));
	mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	ReleaseCOM(backBuffer);

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.ArraySize		= 1;
	depthBufferDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Width			= mClientWindowWidth;
	depthBufferDesc.Height			= mClientWindowHeight;
	depthBufferDesc.CPUAccessFlags	= 0;
	depthBufferDesc.MipLevels		= 1;
	depthBufferDesc.MiscFlags		= 0;
	if (mMsaa4XEnabled) {
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = mMsaaQualityLevel;
	}
	else {
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0; 
	}
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	//Create DepthBuffer
	HR(mDevice->CreateTexture2D(&depthBufferDesc, 0, &mDepthStencilBuffer));
	HR(mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));
	
	//Bindint Buffer to output Merger Stage
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mViewport.Width		= static_cast<float>(mClientWindowWidth);
	mViewport.Height	= static_cast<float>(mClientWindowHeight);
	mViewport.MinDepth	= 0; 
	mViewport.MaxDepth	= 1; 
	mViewport.TopLeftX	= 0;
	mViewport.TopLeftY	= 0; 

	mDeviceContext->RSSetViewports(1, &mViewport);

}

bool D3DAPP::InitWindow() {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "(0. 0   )";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); 
}

LRESULT D3DAPP::MsgProc(HWND hwnd, MSG msg, LPARAM lParam, WPARAM wParam) {
}