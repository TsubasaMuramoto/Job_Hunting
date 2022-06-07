//=============================================================================
//
// �c�@�J�E���g�̃N���X [RemainCount.cpp]
// Author : ������
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "RemainCount.h"
#include "number.h"
#include "fade.h"
#include "Ui.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_REMAIN (2)
#define NEXT_NUMBER_SPACE (100)

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
int		CRemainCount::m_nRemain = MAX_REMAIN;
bool	CRemainCount::m_bChange = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRemainCount::CRemainCount(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_pos = { 0.0f,0.0f,0.0f };
	m_size = { 0.0f,0.0f,0.0f };
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_bFade = false;
	m_bChange = false;
	m_nFrame = 0;
	m_RemainMode = REMAIN::MODE_RETRY;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRemainCount::~CRemainCount()
{

}

//=============================================================================
//����������
//=============================================================================
HRESULT CRemainCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CRemainCount::Uninit(void)
{
	// 2D�|���S���̔j��
	for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRemainCount::Update(void)
{
	// �c�@��0�ɂȂ�(�Q�[���I�[�o�[)
	if (m_bChange)
	{
		RemainChange();
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CRemainCount::Draw(void)
{
	// 2D�|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//=============================================================================
// ���Ԃ̐���
//=============================================================================
CRemainCount *CRemainCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ���Ԃ̃C���X�^���X����
	CRemainCount *pRemainCount = new CRemainCount;

	// ���Ԃ̏���
	if (pRemainCount)
	{
		pRemainCount->Init(pos, size);			// ������
	}

	return pRemainCount;
}

//=============================================================================
// ���Ԃ̐���
//=============================================================================
void CRemainCount::RemainChange(void)
{
	if (m_nFrame == 0)
	{
		// �ω��O�̎c�@
		for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
		{
			if (!m_apNumber[nCnt])
			{
				m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (m_size.x * nCnt), m_pos.y, 0.0f), D3DXVECTOR3(m_size.x, m_size.y, 0.0f));
				m_apNumber[nCnt]->SetNumber((m_nRemain) % (int)pow(10, MAX_REMAIN_DIGIT - nCnt) / (int)pow(10, (MAX_REMAIN_DIGIT - 1) - nCnt));
			}
		}
		m_nRemain--;	// �c�@�����炷
	}

	else if (m_nFrame == GO_RESULT_FRAME)
	{
		// �ω���̎c�@
		for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
		{
			if (m_apNumber[nCnt])
			{
				m_apNumber[nCnt]->SetNumber(m_nRemain % (int)pow(10, MAX_REMAIN_DIGIT - nCnt) / (int)pow(10, (MAX_REMAIN_DIGIT - 1) - nCnt));
			}
		}
	}

	else if (m_nFrame == GO_RESULT_FRAME * 2)
	{
		// �t�F�[�h�̎擾
		CFade *pFade = CManager::GetInstance()->GetFade();

		CManager::GetInstance()->SetStop(true);
		m_bChange = false;

		if (m_nRemain <= 0)
		{
			m_RemainMode = REMAIN::MODE_GAMEOVER;	// �Q�[���I�[�o�[
		}

		switch (m_RemainMode)
		{
		case REMAIN::MODE_RETRY:	// ���g���C
			pFade->SetFade(CManager::MODE::GAME);
			break;

		case REMAIN::MODE_GAMEOVER:	// �Q�[���I�[�o�[
			pFade->SetFade(CManager::MODE::RESULT);
			break;
		}
	
	}

	m_nFrame++; // �t���[����i�߂�
}
