//===========================================
// �S�[���n�_�̃��f��
//===========================================
#include "Goal.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "Ui.h"

//===========================================
// �R���X�g���N�^
//===========================================
CGoal::CGoal(OBJTYPE nPriority) : CModel_Spawner(nPriority)
{
	m_pUI = nullptr;
	m_nFrame = 0;
}

//===========================================
// �f�X�g���N�^
//===========================================
CGoal::~CGoal()
{

}

//===========================================
// �����֐�
//===========================================
CGoal *CGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���X�^���X����
	CGoal *pGoal = nullptr;

	if (!pGoal)
	{
		pGoal = new CGoal();
		// NULL�`�F�b�N
		if (pGoal)
		{
			pGoal->Init(pos, rot);		// ������

			// �p�x�����W�A���ɕϊ�
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pGoal->m_pModel)
			{
				// ���f���̐���
				pGoal->m_pModel = CModel::Create({ 0.0f, 0.0f, 0.0f }, { rot_x, rot_y, rot_z }, scale, nXType);
				// �e���f����NULL�ɐݒ�
				pGoal->m_pModel->SetParent(NULL);
				// ���f���^�C�v�̐ݒ�
				pGoal->CScene::SetModelType(CScene::MODTYPE_NORMAL);
				// �V���O�����f���T�C�Y�̐ݒ�
				pGoal->SetSize(pGoal->m_pModel->GetSize());
			}
		}
	}

	return pGoal;
}


//===========================================
// �������֐�
//===========================================
HRESULT CGoal::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel_Spawner::Init(pos, rot);
	return S_OK;
}

//===========================================
// �I���֐�
//===========================================
void CGoal::Uninit(void)
{
	CModel_Spawner::Uninit();

	// UI�폜
	if(m_pUI)
	{ 
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}

//===========================================
// �X�V�֐�
//===========================================
void CGoal::Update(void)
{
	if (m_pModel)
	{
		m_pModel->Update();

#if(1)
		if (m_pModel->GetPlayerHitBool())		// �S�[���ɓ�����ƃ��U���g�ɂ���
		{
			// �S�[�����b�Z�[�W�̕\��
			if (m_nFrame == 0)
			{
				m_pUI = CUi::Create(SCREEN_CENTER, { 3.0f, 1.0f, 0.0f }, CUi::UI_TYPE_ZOOM);
				m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_END"));
			}

			// ���U���g�֍s��
			if (m_nFrame == GO_RESULT_FRAME)
			{
				CManager::GetInstance()->SetStop(true);
				CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
			}

			// �t���[����i�߂�
			m_nFrame++;
		}
#endif
	}
}

//===========================================
// �`��֐�
//===========================================
void CGoal::Draw(void)
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
