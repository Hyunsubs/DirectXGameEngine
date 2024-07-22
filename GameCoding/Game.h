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

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	// DX start
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	// ���� ü���� �� ���۸� �����
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	
	// Render Target View
	ComPtr<ID3D11RenderTargetView> _rtView = nullptr;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> _srView = nullptr;

	// MISC(���)
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };

private:
	// Geometry
	vector<Vertex> _vertices;
	vector<uint32> _indices;

	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// RS
	// DX 11.0 ����
	// �� ���� �������� ����̽��� �����Ͷ����� ������Ʈ�ڿ� ���ڸ� �ٿ� ������Ʈ �� ��ü�� ��� ����
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	// [CPU<->RAM] , [GPU<->VRAM] �� �Ϸ��� ���� ��
	// vertex �����ʹ� ���� Ram�� ����ִ� �ܰ���
	// �� ������ GPU�� ���������ؼ� ���۸� ����� �Ѱܾ� �Ѵ�.


	// Sampler, BlendState
	ComPtr<ID3D11SamplerState> _samplerState;
	ComPtr<ID3D11BlendState> _blendState;

private:
	// ���� SRT ��� ����
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;

};

