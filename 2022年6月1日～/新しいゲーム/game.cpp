//=========================================================
// �Q�[����ʂ̏���
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
#include "TextureFont.h"
#include "Goal.h"
#include "RemainCount.h"
#include "StringManager.h"

//=========================================================
// �}�N����`
//=========================================================
#define SKY_SIZE (D3DXVECTOR3())
#define COUNT_DOWN_FRAME (60)
#define UI_SIZE (D3DXVECTOR3(1.0f,1.0f,0.0f))
#define LETTER_SPACE (50.0f)

//=========================================================
// �ÓI�����o�ϐ��̏�����
//=========================================================
CScore *CGame::m_pScore = nullptr;
bool	CGame::m_bRetry = false;

//=========================================================
// �R���X�g���N�^
//=========================================================
CGame::CGame()
{
	m_nFrame = 0;
	m_CameraRot = { 0.0f,0.0f,0.0f };
	m_pTimer = nullptr;
	m_pMeshCylinder = nullptr;
	m_pMeshField = nullptr;
	m_pMeshSphere = nullptr;
	m_pMeshWall = nullptr;
	m_pStage = nullptr;
	m_pUI = nullptr;
	m_bOnce = false;
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
	// �X�g�b�v����
	CManager::GetInstance()->SetStop(false);

	// �󐶐�
	m_pMeshSphere = CMeshSphere::Create({ 0.0f,0.0f,0.0f }, { 10000.0f,10000.0f,10000.0f }, { 0.0f,0.0f,0.0f }, 10, 10, true, CScene::OBJTYPE_MESH);
	m_pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_SKY"));

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

// �^�C�}�[�J�E���g����
#ifdef _DEBUG
	m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f), 10, true);
#else
	m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f), 60, false);
#endif

	// �X�R�A����
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f));

	// �c�@�J�E���g����
	CRemainCount::Create(SCREEN_CENTER, REMAIN_SIZE);

	// �e�N�X�`���t�H���g
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 0),FONT_POS.y ,FONT_POS.z }, 64, "Q");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 1),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 2),FONT_POS.y ,FONT_POS.z }, 64, "�J");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 3),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 4),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 5),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 6),FONT_POS.y ,FONT_POS.z }, 64, "�]");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 7),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 8),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 9),FONT_POS.y ,FONT_POS.z }, 64, "��");
	//CFont::Create({FONT_POS.x + (LETTER_SPACE * 10),FONT_POS.y,FONT_POS.z }, 64, "��");

	CStringManager::Create(FONT_POS, LETTER_SPACE, 64, "Q�ŃJ�����؂�ւ�");

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
#if (0)
	if (!m_bStart && !m_bEnd)
	{
		//CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
		//m_CameraRot = pCamera->GetRot();
		//m_CameraRot.y += 0.1f;
		//pCamera->SetRot(m_CameraRot);

		if (m_nFrame == 0)
		{
			CManager::GetInstance()->SetStop(true);
		}

		m_nFrame++;

		if (m_nFrame == COUNT_DOWN_FRAME) 
		{
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER_THREE"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 2) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER_TWO"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 3) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER_ONE"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 4) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, {UI_SIZE.x * 3,UI_SIZE.y,0.0f}, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_GAMESTART"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 5)
		{
			SetUiDelete();
			m_nFrame = 0;
			m_bStart = true;
			CManager::GetInstance()->SetStop(false);
		}
	}
	if (m_pTimer->GetLimit())
	{
		if (!m_bOnce)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_END"));
			CManager::GetInstance()->SetStop(true);
			m_bOnce = true;
		}
		m_nFrame++;
		if (m_nFrame == 90)
		{
			SetUiDelete();
			CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
		}
	}
#endif

}
//=========================================================
// �`��
//=========================================================
void CGame::Draw(void)
{

}

bool CGame::GetStart(void) {
	return m_bStart;
}
bool CGame::GetEnd(void) {
	return m_bEnd;
}

void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}






