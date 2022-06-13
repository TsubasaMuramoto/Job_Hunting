//===========================================
// ��Q�����f��
//===========================================
#include "Obstacle.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "fade.h"
#include "Ui.h"
#include "RemainCount.h"
#include "effect_blast.h"

//===========================================
// �R���X�g���N�^
//===========================================
CObstacle::CObstacle(OBJTYPE nPriority) : CModel_Spawner(nPriority)
{
	m_pUI = nullptr;
	m_nFrame = 0;
	m_bDoOnce = false;
	m_bRetry = false;
}

//===========================================
// �f�X�g���N�^
//===========================================
CObstacle::~CObstacle()
{

}

//===========================================
// ����
//===========================================
CObstacle *CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���X�^���X����
	CObstacle *pObstacle = nullptr;

	if (!pObstacle)
	{
		pObstacle = new CObstacle();
		// NULL�`�F�b�N
		if (pObstacle)
		{
			pObstacle->Init(pos, rot);		// ������

			// �p�x�����W�A���ɕϊ�
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pObstacle->m_pModel)
			{
				// ���f���̐���
				pObstacle->m_pModel = CModel::Create({ 0.0f, 0.0f, 0.0f }, { rot_x, rot_y, rot_z }, scale, nXType);
				// �e���f����NULL�ɐݒ�
				pObstacle->m_pModel->SetParent(NULL);
				// ���f���^�C�v�̐ݒ�
				pObstacle->CScene::SetModelType(CScene::MODTYPE_NORMAL);
				// �V���O�����f���T�C�Y�̐ݒ�
				pObstacle->SetSize(pObstacle->m_pModel->GetSize());
			}
		}
	}

	return pObstacle;
}

//===========================================
// �������֐�
//===========================================
HRESULT CObstacle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel_Spawner::Init(pos, rot);
	return S_OK;
}


//===========================================
// �I���֐�
//===========================================
void CObstacle::Uninit()
{
	CModel_Spawner::Uninit();

	// UI�폜
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}

//===========================================
// �X�V�֐�
//===========================================
void CObstacle::Update()
{
	if (m_pModel)
	{
		m_pModel->Update();

		if (m_pModel->GetPlayerHitBool() && !m_bDoOnce)
		{
			m_bDoOnce = true;
			CPlayer *pPlayer = (CPlayer*)CScene::GetScene(OBJTYPE_PLAYER);
			if (pPlayer)
			{
				pPlayer->SetUninit();
				CBlast::Create(pPlayer->GetPos(), pPlayer->GetSize(), { 0.0f,1.0f,1.0f,1.0f }, 20, 20);
				CRemainCount::SetRemainChange();
			}
		}
	}
}

//===========================================
// �`��֐�
//===========================================
void CObstacle::Draw()
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���݂̃}�e���A�����擾
	D3DXMATRIX mtxRotModel, mtxTransModel;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;					// �e�̃}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	if (m_pModel)
	{
		m_pModel->Draw();
	}
}
