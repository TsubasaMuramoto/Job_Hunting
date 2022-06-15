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
#include "LoadX.h"
#include "Ui.h"
#include "TextureFont.h"
#include "Goal.h"
#include "RemainCount.h"
#include "StringManager.h"
#include "ClearGauge.h"
#include "player.h"

//=========================================================
// �}�N����`
//=========================================================
#define START_FRAME		(60)
#define FONT_SIZE		(32)
#define UI_SIZE			(D3DXVECTOR3(1.0f,1.0f,0.0f))
#define SKY_SIZE		(D3DXVECTOR3(6000.0f,20000.0f,6000.0f))
#define CYLINDER_ROT	(0.003f)

//=========================================================
// �ÓI�����o�ϐ��̏�����
//=========================================================
CScore *CGame::m_pScore = nullptr;
CStage *CGame::m_pStage = nullptr;
bool	CGame::m_bRetry = false;

//=========================================================
// �R���X�g���N�^
//=========================================================
CGame::CGame()
{
	m_nFrame		= 0;
	m_CameraRot		= { 0.0f,0.0f,0.0f };
	m_pTimer		= nullptr;
	m_pMeshCylinder = nullptr;
	m_pMeshField	= nullptr;
	m_pMeshSphere	= nullptr;
	m_pMeshWall		= nullptr;
	m_pStage		= nullptr;
	m_pUI			= nullptr;
	m_bOnce			= false;
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
	//m_pMeshSphere = CMeshSphere::Create({ 0.0f,0.0f,0.0f }, SKY_SIZE, { 0.0f,0.0f,0.0f }, 10, 10, true, CScene::OBJTYPE_MESH);
	//m_pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));

	// �󐶐�
	m_pMeshCylinder = CMeshCylinder::Create({ 0.0f,0.0f,0.0f }, SKY_SIZE, { 0.0f,0.0f,0.0f }, 12, 12,CMeshCylinder::CYLINDER_TYPE::STAND_INSIDE);
	m_pMeshCylinder->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_CYBER"));

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

	// �N���A�Q�[�W
	CClearGauge::Create(GAUGE_POS, GAUGE_SIZE);

	// �e�N�X�`���t�H���g
	CStringManager::Create(FONT_POS, (float)FONT_SIZE, FONT_SIZE, "�p�ŃJ�����؂�ւ�","�l�r�S�V�b�N");
	CStringManager::Create({ FONT_POS.x,FONT_POS.y + FONT_SIZE,FONT_POS.z }, (float)FONT_SIZE, FONT_SIZE, "�e�N�X�`���t�H���g" );

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
	if (m_pMeshSphere)
	{
		m_pMeshSphere->AddRot({ 0.0f,-CYLINDER_ROT,0.0f });
		m_pMeshSphere->CMesh::SetPos(m_pMeshSphere,m_pStage->GetPlayer()->GetPos(), m_pMeshSphere->GetSize());
	}

	// �w�i�V�����_�[�̉�]�E�ړ�
	if (m_pMeshCylinder)
	{
		m_pMeshCylinder->AddRot({ 0.0f,-CYLINDER_ROT,0.0f });
		if (!m_pStage->GetPlayer()->GetDeath())
		{
			m_pMeshCylinder->CMesh::SetPos(m_pMeshCylinder, m_pStage->GetPlayer()->GetPos(), m_pMeshCylinder->GetSize());
		}
	}

#if (0)
	if (!m_bStart && !m_bEnd)
	{
		if (m_nFrame == 0)
		{
			CManager::GetInstance()->SetStop(true);
		}

		m_nFrame++;

		if (m_nFrame == START_FRAME)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_GAMESTART"));
		}
		else if (m_nFrame == START_FRAME * 2)
		{
			SetUiDelete();
			m_nFrame = 0;
			m_bStart = true;
			CManager::GetInstance()->SetStop(false);
		}
		
	}
#endif

#if(0)
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

void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}






