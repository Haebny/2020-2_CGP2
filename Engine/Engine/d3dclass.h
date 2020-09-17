#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// Linking
// DirectX 초기화, 3D그래픽 렌더링, 그래픽카드 정보 등과 같은 하드웨어 통신에 필요한 모든 Direct3D의 기능
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

// DirectX includes
// 링크한 라이브러리, DirectX 타입 정의 등에 대한 헤더
#include <dxgi.h>
#include <d3dcommon.h>		// directX에서 자주 쓰이는 함수
#include <d3d11.h>
#include <d3dx10math.h>

// 캡슐화된 클래스
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);
	
	void GetVideoCardInfo(char*, int&);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif