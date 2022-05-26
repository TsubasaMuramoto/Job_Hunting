//=============================================================================
//
// �r���{�[�h�N���X [billboard.cpp]
//
//=============================================================================
#include "Scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CBillboard::CBillboard(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_colval = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CBillboard::~CBillboard()
{

}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	Release();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CBillboard::Update(void)
{
	m_pos = CScene::GetPos();

	if (m_pVtxBuff)
	{
		SetPos(m_pos, m_size);
		SetColVal(m_colval);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;											// �v�Z�p�}�g���b�N�X

	CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);		// �r���{�[�h�����f���̑O�ɕ`�悳��Ȃ��悤�ɂ���
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true,pDevice);	// �A���t�@�u�����h�ɂ����Z����
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);							// ���C�g�𖳌���

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);  //�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));  //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);							// ���C�e�B���O�L��
	CManager::GetInstance()->GetRenderer()->SetZTest(false, pDevice);		// Z�e�X�g����
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);	// ���e�X�g����
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(false, pDevice);// ���Z��������
}

//*****************************************************************************
// ��������
//*****************************************************************************
CBillboard *CBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CBillboard *pBillboard = nullptr;
	if (!pBillboard)
	{
		pBillboard = new CBillboard;
		if (pBillboard)
		{
			pBillboard->m_colval = col;
			pBillboard->Init(pos, size);																		// ������
			pBillboard->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_EFFECT_MOVE"));	// �e�N�X�`���擾
		}
	}
	return pBillboard;
}

//*****************************************************************************
// �ʒu�ݒ�
//*****************************************************************************
void CBillboard::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	m_pos = pos;
	m_size = scale;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	VERTEX_3D *pVtx; //���_�o�b�t�@�ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// �F�ݒ�
//*****************************************************************************
void CBillboard::SetColVal(D3DCOLORVALUE colval)
{
	m_colval = colval;
	SetCol(m_colval);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_���W�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W���X�V
	pVtx[0].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);
	pVtx[1].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);
	pVtx[2].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);
	pVtx[3].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
