//========================================
// �����G�t�F�N�g(���b�V����)�̍쐬
// Author : ������
//========================================
#include "effect_blast.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "LoadX.h"
#include "meshsphere.h"
#include "score.h"
#include "game.h"
#include "Bomb.h"

//===========================================
// �}�N����`
//===========================================
#define BLAST_SCALE_SPEED		(1.6f)	// �����X�P�[�����x
#define BLAST_ROLLING_SPEED		(0.5f)	// ������]���x
#define EXPLOSION_SCORE			(100)	// ���j�X�R�A
#define START_TRANSPARENT_FRAME (60)	// �����ɂȂ�n�߂�t���[����
#define ALPHA_SUB				(0.05f) // ���l������

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlast::CBlast(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pSphere, 0, sizeof(m_pSphere));				// ���N���X
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �F
	m_nFrame		= 0;									// �t���[����
	m_scale			= 0.0f;									// �X�P�[��
	m_fScalingSpeed = BLAST_SCALE_SPEED;					// �X�P�[���ύX���x
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlast::~CBlast()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBlast::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;		// �ʒu
	m_size = size;		// �T�C�Y
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBlast::Uninit(void)
{
	// ���b�V�����j��
	if (m_pSphere)
	{
		// ���S����
		if (!m_pSphere->GetDeath())
		{
			m_pSphere->Uninit();
			m_pSphere = nullptr;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBlast::Update(void)
{
	// �������̏���
	if (m_pSphere)
	{
		CScene *pScene = CScene::GetScene(OBJTYPE_MODEL);

		// �V�[����null�ɂȂ�܂Œʂ�
		while (pScene)
		{
			// ���̃V�[�����擾
			CScene *pSceneNext = CScene::GetSceneNext(pScene);

			CScene::MODTYPE modtype = pScene->GetModelType();
			switch(modtype)
			{
			case MODTYPE_TARGET:
				if (m_pSphere->SphereCollisionSphere((m_pSphere->GetSize().x * m_scale) / 2, pScene))
				{
					CScore *pScore = CGame::GetScore();
					pScore->AddScore(EXPLOSION_SCORE);
					pScene->Uninit();
				}
				break;
				
			case MODTYPE_BOMB:
				if (m_pSphere->SphereCollisionSphere((m_pSphere->GetSize().y * m_scale) / 2, pScene))
				{
					CBomb *pBomb = (CBomb*)pScene;
					pBomb->Explosion();
				}
				break;
			}
			

			// ���̃V�[�������݂̃V�[���ɂ���
			pScene = pSceneNext;
		}

		Spread();	// �����L����
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBlast::Draw(void)
{

}

//=============================================================================
// �����̍L������̏���
//=============================================================================
void CBlast::Spread(void)
{
	float Nextsub = m_fScalingSpeed;

	m_nFrame++;
	if (m_nFrame >= START_TRANSPARENT_FRAME)
	{
		m_col.a -= ALPHA_SUB;
	}
	else
	{
		m_scale += m_fScalingSpeed;
		m_fScalingSpeed -= Nextsub * 0.1f;
	}

	m_rot.y += BLAST_ROLLING_SPEED;
	m_pSphere->SetRot(m_rot);
	m_pSphere->SetScale({ m_scale,m_scale ,m_scale });
	m_pSphere->SetCol(m_col);

	if (m_col.a <= 0.0f)	// ���S�ɓ����ɂȂ�܂ŏI�����Ȃ�
	{
		Uninit();
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CBlast *CBlast::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLine, int nVertical)
{
	// �C���X�^���X����
	CBlast *pBlast = nullptr;
	if (!pBlast)
	{
		pBlast = new CBlast();
		if (pBlast)
		{
			pBlast->m_col = col;
			pBlast->Init(pos, size);				// ������

			if (!pBlast->m_pSphere)
			{
				// ���e���f���̐���
				pBlast->m_pSphere = CMeshSphere::Create(pos, size, {0.0f,0.0f,0.0f}, nLine, nVertical, false, CScene::OBJTYPE_EFFECTMESH);
			}
		}
	}
	return pBlast;
}