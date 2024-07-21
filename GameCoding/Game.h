#pragma once
class Game
{
public:
	Game();
	~Game();


public:
	void Init(HWND hwnd);
	void Update();
	void Render();


private:
	void RenderBegin();
	void RenderEnd();


private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewPort();

private:
	// DX start
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	// ���� ü���� �� ���۸� �����
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	


	//Render Target View
	ComPtr<ID3D11RenderTargetView> _rtView = nullptr;

	// MISC(���)
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
};

