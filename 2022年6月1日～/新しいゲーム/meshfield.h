//==================================
// ���b�V���t�B�[���h�̍쐬
// Author: ������
//==================================
#ifndef _MESHFIELD_H
#define _MESHFIELD_H
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "texture.h"

//==============================================
// �}�N����`
//==============================================
#define MESH_VTX			(4)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//------------------------------------------------------------------------------
//�N���X�̒�`
//------------------------------------------------------------------------------
class CMeshField : public CScene
{
public:
	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMeshField(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshField();

	//--------------------------------------------
	// ���\�b�h
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// �Z�b�^�[�E�Q�b�^�[
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture; }			// �e�N�X�`���ݒ菈��
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);									// ���_���W�̐ݒ�
	void SetTex(int nAnim, int nPartU);													// �e�N�X�`���̐ݒ�
	void SetTex(float fSpeedX, float fSpeedY);											// �e�N�X�`���̐ݒ�
	void SetCol(D3DXCOLOR col);															// �J���[�̐ݒ�
	D3DXMATRIX	GetMatrix()							{ return m_mtxWorld; }				// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetLocalVtx(int nID)				{ return m_vtxLocal[nID]; }			// ���[�J�����_���W�̎擾

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical);	// �V�[��3D�̐���
	void AllCollision(void);
	bool LineCollisionMesh(CScene *pScene, const int *pnVtx);													// ���ƃ��b�V���̓����蔻��
	bool ShadowCollisionMesh(CScene *pScene, const int *pnVtx);																	// �e�Ƃ̓����蔻��
	void ProcessByObjtype(CScene *pScene,D3DXVECTOR3 &pos);
	// �ݒ�E�擾����


private:
	vector<D3DXVECTOR3>			m_vtxLocal;								// ���[�J�����_���W
	vector<D3DXVECTOR3>			m_vtxWorld;								// ���[���h���_���W
	vector<D3DXMATRIX>			m_mtxVec;								// ���[���h���_�}�g���b�N�X
	D3DXVECTOR3					m_pos;									// �ʒu
	D3DXVECTOR3					m_rot;									// ��]
	D3DXVECTOR3					m_size;									// �T�C�Y
	int							m_nLine;								// ���̃|���S����
	int							m_nVertical;							// �c�̃|���S����

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff = nullptr;		// ���_�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;
};


#endif // !_MESHFIELD_H

