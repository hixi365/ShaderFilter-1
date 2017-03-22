#pragma once

//======================================================================================
//	�w�b�_�E�t�@�C��
//======================================================================================
#include "Main.h"
#include "DeviceResources.h"

#include "CTechnique.h"
#include "CMesh11.h"

//======================================================================================
//	�}�N��
//======================================================================================
using namespace D2D1;
	
//======================================================================================
//	�N���XMainResources
//======================================================================================
class MainResources
{

private:

	//--------------------------------------------------------------------------------
	//	�f�o�C�X���\�[�X
	//--------------------------------------------------------------------------------
	std::shared_ptr<DeviceResources>	m_deviceResources;

	//--------------------------------------------------------------------
	//	�e�N�X�`��
	//--------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_InSRV;		// ����

	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_OutTex;		// �o��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_OutSRV;		// �o��
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_OutRTV;		// �o��

	//--------------------------------------------------------------------
	//	�e�N�j�b�N
	//--------------------------------------------------------------------
	std::shared_ptr<CTechnique>							m_TechMain;		// main
	std::shared_ptr<CTechnique>							m_TechBlur;		// blur
	std::shared_ptr<CTechnique>							m_TechGamma;	// gamma
	std::shared_ptr<CTechnique>							m_TechUnsharp;	// unsharp

	int													m_IndexTech;	// index
	std::vector<std::shared_ptr<CTechnique>>			m_TechVector;	// Vector 

	//--------------------------------------------------------------------
	//	���b�V��
	//--------------------------------------------------------------------
	std::unique_ptr<CMesh11>		m_MeshPlane;
	std::unique_ptr<CMesh11>		m_MeshParticlePack;

	//--------------------------------------------------------------------
	//	�ʏ�ϐ�
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