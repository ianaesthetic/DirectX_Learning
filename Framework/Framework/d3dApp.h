#pragma once

#include "d3Util.h"
#include "GameTimer.h" 

class D3APP {
public: 

protected:

	bool InitDirect3D();

	//WinAPI component
	HWND mhMainWnd;

	//DiretX component
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext; 
	DXGI_SWAP_CHAIN_DESC* mSwapChain; 
	ID3D11Texture2D* _DepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView; 
	ID3D11DepthStencilView* mDepthStencilView; 
	D3D11_VIEWPORT mViewport; 

	//Controlling parameter
	D3D_DRIVER_TYPE mDriverType; 
	int mClientWindowWidth;
	int mClientWindowHeight;
	bool mMsaa4XEnabled; 
	UINT mMsaaQualityLevel;
};