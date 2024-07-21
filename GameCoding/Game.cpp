#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = WINDOW_WIDTH;
	_height = WINDOW_HEIGHT;


	// TODO
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();

}

void Game::Update()
{
	
}

void Game::Render()
{
	RenderBegin();

	// 본격적인 렌더 작업
	

	RenderEnd();
}

// 도화지를 초기화 하는 느낌
void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _rtView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_rtView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

// 그린 도화지를 제출하는 느낌
void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // memset(&desc, 0 ,sizeof(desc));
	{
		desc.BufferDesc.Width = _width; // 화면 크기와 버퍼 크기를 동일하게 설정
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟 전용
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd; // 타겟 창(윈도우 핸들값)
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	

	// 디바이스와 스왑체인 생성
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf() // Get은 데이터를 얻어오고 싶을때 GetAddressOf는 주소를 넘기고 싶을때
	);

	// assert 매크로
	CHECK(hr);

}



void Game::CreateRenderTargetView()
{
	HRESULT hr;

	// 스왑체인으로부터 백 버퍼를 가져옴
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 디바이스로 렌더타겟 뷰를 생성하고 받아온 백버퍼를 넘겨주면 rendertarget에 액세스 하기 위한 뷰를 받을 수 있음
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _rtView.GetAddressOf()); 
	CHECK(hr);

}

void Game::SetViewPort()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;

}
