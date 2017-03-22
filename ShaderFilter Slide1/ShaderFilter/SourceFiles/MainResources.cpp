
//--------------------------------------------------------------------------------
//	ヘッダ・ファイル
//--------------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>

#include "pch.h"
#include "MainResources.h"

#include <WICTextureLoader.h>
#include <ScreenGrab.h>

//--------------------------------------------------------------------------------
//	大域変数
//--------------------------------------------------------------------------------

extern UINT g_WindowWidth;
extern UINT g_WindowHeight;

//extern std::vector<IDXGIOutputDuplication*>	g_pDeskDuplications;

//extern int counterCaptureWindow;

//========================================================================================================
//	コンストラクタ
//========================================================================================================
MainResources::MainResources(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{

	// シェーダの作成
	CreateTechResources();

	// メッシュの作成
	CreateMeshResources();

	// キャプチャ用テクスチャの作成
	CreateCaptureResources();

}

//========================================================================================================
//	デストラクタ
//========================================================================================================
MainResources::~MainResources()
{

}

//========================================================================================================
//	リソースの更新
//========================================================================================================
void MainResources::Update()
{
	HRESULT hr = S_OK;

	auto context = m_deviceResources->GetD3DDeviceContext();

	UpdateTexture();

}

//========================================================================================================
//	Shader関係のリソース
//========================================================================================================
void MainResources::CreateTechResources()
{
	HRESULT hr = S_OK;

	auto device = m_deviceResources->GetD3DDevice();

	m_TechMain = std::make_shared<CTechnique>(m_deviceResources);
	m_TechMain->Init(TECH_2D_MAIN);

	m_TechBlur = std::make_shared<CTechnique>(m_deviceResources);
	m_TechBlur->Init(TECH_2D_BLUR);

	m_TechGamma = std::make_shared<CTechnique>(m_deviceResources);
	m_TechGamma->Init(TECH_2D_GAMMA);

	m_TechUnsharp = std::make_shared<CTechnique>(m_deviceResources);
	m_TechUnsharp->Init(TECH_2D_UNSHARP);

	// Vector突っ込み
	m_IndexTech = 0;
	m_TechVector.clear();
	m_TechVector.push_back(m_TechMain);
	m_TechVector.push_back(m_TechBlur);
	m_TechVector.push_back(m_TechGamma);
	m_TechVector.push_back(m_TechUnsharp);

}

//========================================================================================================
//	Mesh関係のリソース
//========================================================================================================
void MainResources::CreateMeshResources()
{
	HRESULT hr = S_OK;

	auto device = m_deviceResources->GetD3DDevice();

	m_MeshPlane = std::unique_ptr<CMesh11>(new CMesh11(m_deviceResources));
	m_MeshPlane->CreatePlane(1.0f);

}

//========================================================================================================
//	Capture関係のリソース
//========================================================================================================
void MainResources::CreateCaptureResources()
{
	HRESULT hr = S_OK;

	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	////------------------------------------------------------------------------------------
	////　2Dテクスチャの作成
	////------------------------------------------------------------------------------------
	//{

	//	D3D11_TEXTURE2D_DESC descTex;
	//	descTex.Width = g_WindowWidth;
	//	descTex.Height = g_WindowHeight;
	//	descTex.MipLevels = 1;
	//	descTex.ArraySize = 1;
	//	descTex.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UINT; // DXGI_FORMAT_R32G32B32A32_FLOAT; //DXGI_FORMAT_R8G8B8A8_UNORM;//
	//	descTex.SampleDesc.Count = 1;
	//	descTex.SampleDesc.Quality = 0;
	//	descTex.Usage = D3D11_USAGE_DEFAULT;
	//	descTex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//	descTex.CPUAccessFlags = 0;
	//	descTex.MiscFlags = 0;

	//	// RTV
	//	D3D11_RENDER_TARGET_VIEW_DESC descRTV;
	//	descRTV.Format = descTex.Format;
	//	descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//	descRTV.Texture2D.MipSlice = 0;

	//	// SRV
	//	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	//	descSRV.Format = descTex.Format;
	//	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//	descSRV.Texture2D.MipLevels = 1;
	//	descSRV.Texture2D.MostDetailedMip = 0;

	////	hr = device->CreateTexture2D(&descTex, nullptr, m_WinCapUTex.GetAddressOf());
	////	hr = device->CreateRenderTargetView(m_WinCapUTex.Get(), &descRTV, m_WinCapURTV.GetAddressOf());
	////	hr = device->CreateShaderResourceView(m_WinCapUTex.Get(), &descSRV, m_WinCapUSRV.GetAddressOf());

	//}

	//------------------------------------------------------------------------------------
	//　2Dテクスチャの作成
	//------------------------------------------------------------------------------------
	{

		D3D11_TEXTURE2D_DESC descTex;
		descTex.Width = TEXTURE_WIDTH;
		descTex.Height = TEXTURE_HEIGHT;
		descTex.MipLevels = 1;
		descTex.ArraySize = 1;
		descTex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descTex.SampleDesc.Count = 1;
		descTex.SampleDesc.Quality = 0;
		descTex.Usage = D3D11_USAGE_DEFAULT;
		descTex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		descTex.CPUAccessFlags = 0;
		descTex.MiscFlags = 0;

		// RTV
		D3D11_RENDER_TARGET_VIEW_DESC descRTV;
		descRTV.Format = descTex.Format;
		descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		descRTV.Texture2D.MipSlice = 0;

		// SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		descSRV.Format = descTex.Format;
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MipLevels = 1;
		descSRV.Texture2D.MostDetailedMip = 0;

		// Out
		hr = device->CreateTexture2D(&descTex, nullptr, m_OutTex.GetAddressOf());
		hr = device->CreateRenderTargetView(m_OutTex.Get(), &descRTV, m_OutRTV.GetAddressOf());
		hr = device->CreateShaderResourceView(m_OutTex.Get(), &descSRV, m_OutSRV.GetAddressOf());

	}

	hr = DirectX::CreateWICTextureFromFile(device, L"./texture/input.png", nullptr, m_InSRV.GetAddressOf());		// 白テクスチャ(SRVがnullのとき使用)

}

//========================================================================================================
//	テクスチャ処理
//========================================================================================================
void MainResources::UpdateTexture()
{

	auto context = m_deviceResources->GetD3DDeviceContext();

	D3D11_VIEWPORT vp = { 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, 1 };
	D3D11_VIEWPORT pVPs[] = { vp };
	context->RSSetViewports(1, pVPs);

	CBuffer CData;
	CData.float4A.x = m_floatAX;
	float color[4] = { 0, 0, 0, 1 };

	ID3D11RenderTargetView   *pRTVs[] = { m_OutRTV.Get() };
	ID3D11ShaderResourceView *pSRVs[] = { m_InSRV.Get() };

	context->OMSetRenderTargets(1, pRTVs, nullptr);
	context->ClearRenderTargetView(pRTVs[0], color);

	STATESET states = CTechnique::GetStates(RS_SOLID_CULLBACK, DS_DISABLEDEPTHTESTWRITE, BS_ALPHA);
	auto pSampler = CTechnique::GetSampler(SS_ANISOTROPIC_CLAMP);

	m_MeshPlane->Render(m_TechVector[m_IndexTech], states, &CData, pSRVs, 1, &pSampler, 1);

}

//====================================================================================================
//	テクスチャを保存
//====================================================================================================
void MainResources::SaveTexture()
{

	auto context = m_deviceResources->GetD3DDeviceContext();
	HRESULT hr = S_OK;

	{
		WCHAR filename[256], tstamp[256];

		// 現在時刻を取得
		SYSTEMTIME	t;
		GetLocalTime(&t);

		// 保存フォルダ作成
		CreateDirectory(L"./Output", NULL);

		// タイムスタンプ
		swprintf_s(tstamp, 256, L"Screen-%04d%02d%02d%02d%02d%02d",
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex = nullptr;

		swprintf_s(filename, 256, L"./Output/%s-out.png", tstamp);
		pTex = m_OutTex.Get();

		hr = DirectX::SaveWICTextureToFile(context, (ID3D11Resource*)pTex.Get(), GUID_ContainerFormatPng, filename);

	}

}

//========================================================================================================
//	Capture
//========================================================================================================
void MainResources::UpdateCapture()
{

	//HRESULT hr = S_OK;

	//auto context = m_deviceResources->GetD3DDeviceContext();

	//IDXGIResource* resource = nullptr;
	//DXGI_OUTDUPL_FRAME_INFO frameInfo;

	//IDXGIOutputDuplication* pDeplication = g_pDeskDuplications[counterCaptureWindow];

	//const UINT timeout = 1; // ms
	//hr = pDeplication->AcquireNextFrame(timeout, &frameInfo, &resource);
	//if (hr != S_OK)
	//	return;

	//ID3D11Texture2D* texture;
	//hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));
	//if (hr != S_OK)
	//	return;

	//D3D11_TEXTURE2D_DESC descTex;
	//texture->GetDesc(&descTex);
	//context->CopyResource(m_WinCapUTex.Get(), texture);
	//resource->Release();

	//hr = pDeplication->ReleaseFrame();
	//if (hr != S_OK)
	//	return;

	//D3D11_VIEWPORT vp = { 0, 0, g_WindowWidth, g_WindowHeight, 0, 1 };
	//D3D11_VIEWPORT pVPs[] = { vp };
	//context->RSSetViewports(1, pVPs);

	//CBuffer CData;
	//float color[4] = { 0, 0, 0, 1 };

	//ID3D11RenderTargetView   *pRTVs[] = { m_WinCapRTV.Get() };
	//ID3D11ShaderResourceView *pSRVs[] = { m_WinCapUSRV.Get() };

	//context->OMSetRenderTargets(1, pRTVs, nullptr);
	//context->ClearRenderTargetView(pRTVs[0], color);

	//STATESET states = CTechnique::GetStates(RS_SOLID_CULLBACK, DS_DISABLEDEPTHTESTWRITE, BS_ALPHA);
	//auto pSampler = CTechnique::GetSampler(SS_ANISOTROPIC_CLAMP);

	//m_MeshPlane->Render(m_TechCapture, states, &CData, pSRVs, 1, &pSampler, 1);

}