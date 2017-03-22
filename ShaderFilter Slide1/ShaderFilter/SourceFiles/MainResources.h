#pragma once

//======================================================================================
//	ヘッダ・ファイル
//======================================================================================
#include "Main.h"
#include "DeviceResources.h"

#include "CTechnique.h"
#include "CMesh11.h"

//======================================================================================
//	マクロ
//======================================================================================
using namespace D2D1;
	
//======================================================================================
//	クラスMainResources
//======================================================================================
class MainResources
{

private:

	//--------------------------------------------------------------------------------
	//	デバイスリソース
	//--------------------------------------------------------------------------------
	std::shared_ptr<DeviceResources>	m_deviceResources;

	//--------------------------------------------------------------------
	//	テクスチャ
	//--------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_InSRV;		// 入力

	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_OutTex;		// 出力
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_OutSRV;		// 出力
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_OutRTV;		// 出力

	//--------------------------------------------------------------------
	//	テクニック
	//--------------------------------------------------------------------
	std::shared_ptr<CTechnique>							m_TechMain;		// main
	std::shared_ptr<CTechnique>							m_TechBlur;		// blur
	std::shared_ptr<CTechnique>							m_TechGamma;	// gamma
	std::shared_ptr<CTechnique>							m_TechUnsharp;	// unsharp

	int													m_IndexTech;	// index
	std::vector<std::shared_ptr<CTechnique>>			m_TechVector;	// Vector 

	//--------------------------------------------------------------------
	//	メッシュ
	//--------------------------------------------------------------------
	std::unique_ptr<CMesh11>		m_MeshPlane;
	std::unique_ptr<CMesh11>		m_MeshParticlePack;

	//--------------------------------------------------------------------
	//	通常変数
	//--------------------------------------------------------------------
	float m_floatAX = 1.0f;

private:

	void CreateTechResources();
	void CreateMeshResources();
	void CreateCaptureResources();

	void UpdateTexture();
	void UpdateCapture();


public:
								MainResources(const std::shared_ptr<DeviceResources>& deviceResources);
								~MainResources();
	
	
	ID3D11ShaderResourceView*	GetOutSRV() { return m_OutSRV.Get(); }

	void						Update();

	void						ToggleTech() { m_IndexTech = m_IndexTech < m_TechVector.size() - 1 ? m_IndexTech + 1 : 0; }

	void						AddFloat(float x) { m_floatAX += x; }
	void						SetFloat(float x) { m_floatAX = x; }
	void						RateFloat(float x) { m_floatAX *= x; }

	void						SaveTexture();

};