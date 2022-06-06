//==================================
// ���b�V���X�t�B�A�̍쐬
// Author: ������
//==================================
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPHERE_RADIUS	(100.0f)													// �����a
#define SPHERE_SIZE		(D3DXVECTOR3(SPHERE_RADIUS,SPHERE_RADIUS,SPHERE_RADIUS))	// ���T�C�Y

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//------------------------------------------------------------------------------
//�N���X�̒�`
//------------------------------------------------------------------------------
class CMeshSphere : public CScene
{
public:
	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMeshSphere(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshSphere();

	//--------------------------------------------
	// ���\�b�h
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// �ݒ�E�擾����
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }			// �e�N�X�`���ݒ菈��
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);									// ���_���W�̐ݒ�
	void SetTex(int nAnim, int nPartU);													// �e�N�X�`���̐ݒ�
	void SetTex(float fSpeedX, float fSpeedY);											// �e�N�X�`���̐ݒ�
	void SetCol(D3DXCOLOR col);															// �J���[�̐ݒ�
	void SetRot(D3DXVECTOR3 rot)					{ m_rot = rot; }					// ��]�̐ݒ�
	void SetScale(D3DXVECTOR3 scale)				{ m_scale = scale; }				// �X�P�[���̐ݒ�
	D3DXVECTOR3 GetScale()							{ return m_scale; }
	D3DXMATRIX	GetMatrix()							{ return m_mtxWorld; }				// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetLocalVtx(int nID)				{ return m_vtxLocal[nID]; }			// ���[�J�����_���W�̎擾

	bool SphereCollisionSphere(float fRadius, CScene *pScene);
	static CMeshSphere *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical,bool bInsideDraw, CScene::OBJTYPE objtype);	// �V�[��3D�̐���
	bool LineCollisionMesh(CScene *pScene, const int *nVtx);

private:
	vector<D3DXVECTOR3> m_vtxLocal;								// ���[�J�����_���W
	vector<D3DXVECTOR3> m_vtxWorld;								// ���[���h���_���W
	vector<D3DXMATRIX>  m_mtxVec;								// ���[���h���_�}�g���b�N�X
	D3DXVECTOR3 m_pos;											// �ʒu
	D3DXVECTOR3 m_rot;											// ��]
	D3DXVECTOR3 m_size;											// �T�C�Y
	D3DXVECTOR3 m_scale;										// �X�P�[��
	int m_nLine;												// ���̃|���S����
	int m_nVertical;											// �c�̃|���S����
	bool m_bInsideDraw;											// �����ɕ`�悷�邩
	CScene::OBJTYPE objtype;									// �I�u�W�F�N�g�^�C�v

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff = nullptr;		// ���_�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;
};


#endif // !_MESHSPHERE_H_


