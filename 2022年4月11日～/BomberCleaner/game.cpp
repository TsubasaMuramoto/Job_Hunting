//=========================================================
//�Q�[����ʂ̏���
//=========================================================
#include "game.h"
#include "texture.h"
#include "manager.h"
#include "fade.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "timercount.h"
#include "stage.h"
#include "score.h"
#include "Scene3D.h"
#include "field.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "Bomb.h"
#include "BombSpawner.h"
#include "LoadX.h"
#include "Ui.h"

//=========================================================
// �}�N����`
//=========================================================
#define COUNT_DOWN_FRAME (60)

//=========================================================
// �ÓI�����o�ϐ��̏�����
//=========================================================
CScore *CGame::m_pScore = nullptr;

//=========================================================
// �R���X�g���N�^
//=========================================================
CGame::CGame()
{
	m_nFrame = 0;
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CGame::~CGame()
{

}

//=========================================================
// ������
//=========================================================
HRESULT CGame::Init(void)
{
	CBombSpawner::Create({ 800.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 5);
	//CBomb::Create(D3DXVECTOR3(0.0f, 10.0f, -200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), "data/MODEL/Bomb001.x");
	CBomb::Create({ 0.0f,	10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 0.0f,	10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));

	/*CMeshField::Create({ 600.0f,50.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 700.0f,120.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 900.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);*/

	// �X�e�[�W����
	if (!m_pStage)
	{
		m_pStage = new CStage;
		if (m_pStage)
		{
			m_pStage->Init();									// ������
			m_pStage->SetStage("data/TEXT/StageSet000.txt");	// ���f���ݒ�̃t�@�C����
		}
	}

	/*CMeshField::Create({ 100.0f,50.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 100.0f,120.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 100.0f,180.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 6, 6);
	CMeshField::Create({ -200.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);*/

	// �^�C�}�[�J�E���g����
	CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f), 10, true);

	// �X�R�A����
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f));

	return S_OK;
}

//=========================================================
// �I��
//=========================================================
void CGame::Uninit(void)
{

}

//=========================================================
//�X�V
//=========================================================
void CGame::Update(void)
{
#ifndef _DEBUG
	if (!m_bStart && !m_bEnd)
	{
		if (m_nFrame == 0)
		{
			CManager::GetInstance()->SetStop(true);
		}

		m_nFrame++;

		if (m_nFrame == COUNT_DOWN_FRAME) 
		{
			m_pUI = CUi::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_3RD"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 2) 
		{
			m_pUI->Uninit();
			delete m_pUI;
			m_pUI = nullptr;
			m_pUI = CUi::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_2ND"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 3) 
		{
			m_pUI->Uninit();
			delete m_pUI;
			m_pUI = nullptr;
			m_pUI = CUi::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_1ST"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 4) 
		{
			m_pUI->Uninit();
			delete m_pUI;
			m_pUI = nullptr;
			m_pUI = CUi::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_PRESS_START"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 5)
		{
			m_pUI->Uninit();
			delete m_pUI;
			m_pUI = nullptr;
			m_nFrame = 0;
			m_bStart = true;
			CManager::GetInstance()->SetStop(false);
		}
	}
#endif
	//if (CManager::SetModeBool(CManager::MODE::RESULT))
	//{

	//}
}
//=========================================================
// �`��
//--------------------------------------------
void CGame::Draw(void)
{

}

bool CGame::GetStart(void) {
	return m_bStart;
}
bool CGame::GetEnd(void) {
	return m_bEnd;
}




