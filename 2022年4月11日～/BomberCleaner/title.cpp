//============================================
// �^�C�g���̏���
// Author:������
//============================================
#include "title.h"
#include "texture.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "bg.h"
#include "Ui.h"

//===============================================
// �}�N����`
//===============================================
#define NOT_SELECT_COLOR (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))	// �I������Ă��Ȃ����̐F

//============================================
// �R���X�g���N�^
//============================================
CTitle::CTitle()
{
	// �ϐ�������
	memset(m_pSelectUi, 0, sizeof(m_pSelectUi));
	m_pPressEnter	= nullptr;
	m_pTitleLogo	= nullptr;
	m_pTutorial		= nullptr;
	m_pBg			= nullptr;
	m_Page			= PAGE_01;
	m_nSelect		= 0;
}

//============================================
// �f�X�g���N�^
//============================================
CTitle::~CTitle()
{

}

//============================================
// ������
//============================================
HRESULT CTitle::Init(void)
{
	// �^�C�g���w�i�̐���
	m_pBg = CBg::Create();

	// press startUI�̐���
	if (!m_pPressEnter)
	{
		m_pPressEnter = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, 0.0f), CUi::UI_TYPE_BLINK);
		m_pPressEnter->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_PRESS_START"));
	}
	
	// �^�C�g��UI�̐���
	if (!m_pTitleLogo)
	{
		m_pTitleLogo = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), D3DXVECTOR3(1200.0f, 500.0f, 0.0f), CUi::UI_TYPE_NONE);
		m_pTitleLogo->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_TITLE"));
	}

	return S_OK;
}

//============================================
// �I��
//============================================
void CTitle::Uninit(void)
{
	//-------------------------------
	// �I�u�W�F�N�g�I������
	//-------------------------------
	for (int nCnt = 0; nCnt < TITLE_SELECT_MAX; nCnt++)
	{
		if (m_pSelectUi[nCnt])
		{
			m_pSelectUi[nCnt]->Uninit();
			m_pSelectUi[nCnt] = nullptr;
		}
	}

	if (m_pPressEnter)
	{
		m_pPressEnter->Uninit();
		m_pPressEnter = nullptr;
	}

	if (m_pTitleLogo)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = nullptr;
	}

	if (m_pBg)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}
}
//--------------------------------------------
//�X�V
//--------------------------------------------
void CTitle::Update(void)
{
	switch (m_Page)
	{
	case PAGE_01:
		if (CInput::PressAnyAction(CInput::ACTION_ENTER))
		{
			m_Page = PAGE_02;
			
			// �v���X�X�^�[�g��j��
			if (m_pPressEnter)
			{
				m_pPressEnter->Uninit();
				m_pPressEnter = nullptr;
			}

			// �Q�[�����[�h�Z���N�gUI�̐���
			D3DXVECTOR3 pos,size;
			string Tex;
			CUi::UI_TYPE UiType;
			for (int nCnt = 0; nCnt < TITLE_SELECT_MAX; nCnt++)
			{
				switch (nCnt)	// UI�̈ʒu�A�T�C�Y�A�e�N�X�`���ݒ�
				{
				case TITLE_SELECT_01:
					pos =	{ 300.0f,500.0f,0.0f };
					size =	{ 300.0f,200.0f,0.0f };
					Tex =	"TEX_TYPE_TITLE_SELECT00";
					UiType = CUi::UI_TYPE_NONE;
					break;

				case TITLE_SELECT_02:
					pos =	{ SCREEN_WIDTH / 2,600.0f,0.0f };
					size =	{ 300.0f,200.0f,0.0f };
					Tex =	"TEX_TYPE_TITLE_SELECT01";
					UiType = CUi::UI_TYPE_NONE;
					break;

				case TITLE_SELECT_03:
					pos =	{ 980.0f,500.0f,0.0f };
					size =	{ 300.0f,200.0f,0.0f };
					Tex =	"TEX_TYPE_TITLE_SELECT02";
					UiType = CUi::UI_TYPE_NONE;
					break;
				}

				if (!m_pSelectUi[nCnt])	// UI����
				{
					m_pSelectUi[nCnt] = CUi::Create(pos, size, CUi::UI_TYPE_NONE);
					m_pSelectUi[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(Tex));
					m_pSelectUi[nCnt]->SetCol(NOT_SELECT_COLOR);
				}
			}
			m_pSelectUi[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;

	case PAGE_02:

		if(CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			switch (m_nSelect)
			{
			case TITLE_SELECT_01:
				// �Q�[�����[�h�ֈڍs
				if (CManager::SetModeBool(CManager::MODE::GAME))
				{

				}
				break;
			case TITLE_SELECT_02:
				if (!m_pTutorial)
				{
					m_pTutorial = CUi::Create({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0.0f }, { SCREEN_WIDTH / 1.5f,SCREEN_HEIGHT / 1.5f,0.0f }, CUi::UI_TYPE_NONE);
				}
				else
				{
					m_pTutorial->Uninit();
					m_pTutorial = nullptr;
				}

				break;
			case TITLE_SELECT_03:
				PostQuitMessage(0);	// �I��
				break;
			}
		}

		else if (CInput::PressAnyAction(CInput::ACTION_LEFT))
		{
			m_pSelectUi[m_nSelect]->SetCol(NOT_SELECT_COLOR);

			if (m_nSelect > TITLE_SELECT_01)
			{
				m_nSelect--;
			}
			else
			{
				m_nSelect = TITLE_SELECT_03;
			}
		}

		else if (CInput::PressAnyAction(CInput::ACTION_RIGHT))
		{
			m_pSelectUi[m_nSelect]->SetCol(NOT_SELECT_COLOR);

			if (m_nSelect < TITLE_SELECT_03)
			{
				m_nSelect++;
			}
			else
			{
				m_nSelect = TITLE_SELECT_01;
			}
		}

		// ���ݑI������Ă���UI
		m_pSelectUi[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		break;
	}
}

//============================================
// �`��
//============================================
void CTitle::Draw(void)
{

}
//
//bool CTitle::PressAnyKey(TITLE_ACTION action)
//{
//
//}


