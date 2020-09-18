#include "d3dclass.h"

D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{
}

D3DClass::~D3DClass()
{
}

// screenWidth, screenHeight: SystemClass에서 만든 윈도우 너비 높이
// hwnd: 윈도우에 대한 핸들 -> 만들어진 윈도우에 접근하는 용도
// screenDepth, screenNear: 윈도우에 그려질 3D 환경에서의 깊이(depth)
// vsync: 게임의 수직동기화 기능(모니터주사율에 맞춰 렌더링 vs 가능한 빠르게 다시 그리기)
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth,
	float screenNear)
{
	/// 하드웨어 관련
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	/// 스왑체인 관련
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	/// 버퍼 세팅 관련 (속도에 아주 큰 영향)
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	/// 레스터화기 관련
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	/// 마지막에 스와핑해서 출력
	
#pragma region [1단계: 하드웨어 스펙 검사] (+주사율 계산)
	// Store the vsync setting.
	// 수직동기화 설정 저장
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	// 그래픽카드 확인(없는경우도 있기때문에)
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	// 모니터에 대한 정보
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	// 사용 가능한 해상도가 무엇인지 확인
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	// 주사율의 분자, 분모 계산
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				// 현재에 맞는 주사율에 맞는 정보를 가져와 저장
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	/// 그래픽 카드 이름 및 사용가능 메모리 크기 추출
	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	/// 정보를 얻기 위해 사용한 구조체 및 인터페이스 해제
	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;
	
#pragma endregion

#pragma region [2단계: Double Buffering- 스왑체인] 픽셀 데이터를 어떻게 세팅할지
	/// 스왑 체인의 description 구조체 채우기
	// ┕실제로 렌더링이 기록되는 프론트버퍼와 백버퍼

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
	// Set to a single back buffer.
	// 렌더링 시 하나의 백버퍼만 사용
	swapChainDesc.BufferCount = 1;
	
	// Set the width and height of the back buffer.
	// 백버퍼의 너비 및 높이 설정(스크린 사이즈에 영향)
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 현재는 unsigned normal data type

	/// 주사율(초당 몇장의 백버퍼를 프론트 버퍼와 스왑하는지)
	// Set the refresh rate of the back buffer.
	// 백버퍼의 새로고침 비율 설정
	if (m_vsync_enabled)
	{
		// 모니터의 성능에 맞춘 속도로
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// 0. 뽑아낼 수 있는 최대속도
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	// 백버퍼의 용도 설정
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	// 렌더링이 이루어질 윈도우 핸들 설정
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	// 멀티샘플링 Off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	// Set to full screen or windowed mode.
	// 윈도우 모드 설정(풀스크린 On/Off)
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	// 스캔라인의 정렬 및 스캔라이닝을 지정하지 않음
	// 픽셀 물고기 그림 생각하기! 바꿀 일 거의 없음!
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	// 출력된 이후의 백버퍼의 내용 버림
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	// 추가 옵션 플래그 사용하지 않음
	swapChainDesc.Flags = 0;
	///스왑체인 세팅 끝!

	// Set the feature level to DirectX 11.
	// 피쳐 레벨(DirectX 버전)을 DirectX 11로 설정 (DirectX 11의 기능읆 쓸 거야!)
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	// Create the swap chain, Direct3D device, and Direct3D device context.
	// 스왑 체인, Direct3D 장치, Direct3D 장치 컨텍스트 생성
	/// 세팅한 값으로 설정하기!
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, 
		&m_deviceContext);		// m_deviceContext: 하드웨어에 데이터 전달
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	// 백버퍼의 포인터 가져옴
	/// 그래픽 카드에게 보내줄 내용 세팅
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	// 백버퍼 포인터로 렌더타겟 뷰 생성
	/// 그래픽 카드에게 보내줄 내용으로 무슨 작업을 할지 알려줌
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	// 백버퍼 포인터 해제
	backBufferPtr->Release();
	backBufferPtr = 0;

#pragma endregion

#pragma region [3단계: 버퍼(SystemMemory, GPU)] 특히 Depth buffer
	/// 3D공간에서 폴리곤을 올바르게 그리기 위해 깊이버퍼 생성
	// Initialize the description of the depth buffer.
	// 깊이 버퍼의 description 초기화
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	// 깊이 버퍼의 description 작성
	/// 가장 중요한 것은 사이즈와 들어갈 데이터가 무엇인지!
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	// 깊이/스텐실 버퍼 생성
	/// 스텐실 버퍼(모션블러, 볼류메트릭 그림자 등의 효과)를 깊이 버퍼에 연결
	/// GPU 메모리에 만들겠다!
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;			// depth가 더 작은걸로(가까운걸로!)
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	// 깊이/스텐실 상태 생성
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	/// Direct3D가 깊이 버퍼를 깊이-스텐실 텍스쳐로 인식하기 위한 작업
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc,
		&m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	// 렌더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
#pragma endregion
#pragma region [4단계: Rasterisation] 뷰포트 설정, 섀이더 설정
	// Setup the raster description which will determine how and what polygons will be drawn.
	// 래스터화기 상태 지정
	// ㄴ도형을 어떻게 그릴 것인지 결정(와이어프레임, 앞뒷면 모두 그리기 등)
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	// 레스터화기 상태 설정
	m_deviceContext->RSSetState(m_rasterState);	// Setup the viewport for rendering.
	// 뷰포트: 렌더타겟 공간에서 클리핑 수행(윈도우 전체 크기와 동일하게 설정)
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	// 투영 행렬 설정
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	// 3D 렌더링을 위한 투영 행렬 생성
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear,
		screenDepth);

	// Initialize the world matrix to the identity matrix.
	// 월드 행렬을 단위 행렬로 초기화
	D3DXMatrixIdentity(&m_worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	// 2D(UI와 같은 요소들) 렌더링에 사용될 직교 투영 행렬을 생성
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
#pragma endregion
	
	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	// 종료 전 윈도우 모드로 전환하지 않으면 스왑체인을 해제할 때 예외 발생
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

// 매 프레임의 시작마다 3D화면을 그리기 시작할 때 호출
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	// 버퍼를 어떤 색상으로 지울 것인지 설정
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	// 백버퍼 지움
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	// 깊이 버퍼 지움
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	return;
}

// 매 프레임의 마지막에 스왑 체인이 백버퍼에 그린 3D화면을 표시
void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	// (렌더링 완료->)백버퍼의 내용을 화면에 표시
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		// 새로고침 비율 고정
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		// 가능한 빠르게 표시
		m_swapChain->Present(0, 0);
	}

	return;
}

#pragma region 도우미 함수

// Direct3D 디바이스의 포인터 반환
ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

// 디바이스 컨텍스트의 포인터 반환
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

// 투영 행렬 반환
void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

// 월드 행렬 반환
void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

// 직교 투영 행렬 반환
void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

// 그래픽 카드의 이름과 사용가능한 메모리양 반환
void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

#pragma endregion

