#pragma once

#include "d3Util.h"
#include "GameTimer.h" 

class D3DAPP {
public: 
	//Class Initilization 
	D3DAPP(HINSTANCE hinstance);
	virtual ~D3DAPP(); 

	HINSTANCE GetAppInstance() const;
	HWND GetAppWindow() const; 
	float GetAspectRatio() const; 

	int Run(); 

	virtual bool Init(); 
	virtual void OnResize();
	virtual void UpdateScene(float t) = 0;
	virtual void DrawScene() = 0; 

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

protected:

	bool InitWindow(); 
	bool InitDirect3D();

	//WinAPI component
	HINSTANCE mhAppInstance; 
	HWND mhMainWnd;
	bool mMinimized; 
	bool mMaximized; 
	bool mResizing; 

	//DiretX component
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext; 
	IDXGISwapChain* mSwapChain; 
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView; 
	ID3D11DepthStencilView* mDepthStencilView; 
	D3D11_VIEWPORT mViewport; 

	//Controlling parameter

	D3D_DRIVER_TYPE mDriverType; 
	int mClientWindowWidth;
	int mClientWindowHeight;
	bool mMsaa4XEnabled; 
	UINT mMsaaQualityLevel;

	//Timer 
	GameTimer mTimer; 
	bool mAppPaused; 
};