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

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();
	CreateSRV();

}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();

	// 본격적인 렌더 작업
	// IA - VS - RS - PS - OM
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _srView.GetAddressOf());

		// OM
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

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
	CHECK(hr);
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


// 도형의 기하학적 정의
void Game::CreateGeometry()
{
	// VertexData

	// Triangle
	{
		_vertices.resize(4);
		
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[0].UV = Vec2(0.f, 1.f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
		_vertices[1].UV = Vec2(0.f, 0.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);
		_vertices[2].UV = Vec2(1.f, 1.f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[3].UV = Vec2(1.f, 0.f);
	}

	// Index
	{
		_indices = { 0, 1, 2, 2, 1, 3 };
	}

	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		// 버퍼 용도에 대한 설명은 Notion에 정리 해놓았음
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data(); // 시스템 메모리에 초기 데이터 설정

		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		// 버퍼 용도에 대한 설명은 Notion에 정리 해놓았음
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data(); // 시스템 메모리에 초기 데이터 설정

		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}

// GPU에 보낸 버텍스 버퍼가 어떻게 생겨먹은건지 알려주는 정보
void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC); // 보낼 레이아웃 갯수
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf()); // Shader에 대한 정보를 만들어야 함
}

void Game::CreateVS()
{
	// 블롭은 중요하지 않음
	LoadShaderFromFile(L"Standard.hlsl", "VS_Main", "vs_5_0", _vsBlob);
	_device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Standard.hlsl", "PS_Main", "ps_5_0", _psBlob);
	_device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
}

void Game::CreateSRV()
{
	//DirectXTex
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT hr = DirectX::LoadFromWICFile(L"채원갓.jpg", DirectX::WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = DirectX::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _srView.GetAddressOf());
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}
