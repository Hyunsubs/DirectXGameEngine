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
	void CreateGeometry();
	void CreateInputLayout();

	void CreateVS();
	void CreatePS();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	// DX start
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	// 스왑 체인은 백 버퍼를 담당함
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	


	//Render Target View
	ComPtr<ID3D11RenderTargetView> _rtView = nullptr;

	// MISC(잡다)
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };

private:
	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	// [CPU<->RAM] , [GPU<->VRAM] 이 일련의 과정 중
	// vertex 데이터는 아직 Ram에 들어있는 단계임
	// 이 정보를 GPU로 보내기위해선 버퍼를 만들어 넘겨야 한다.


};

