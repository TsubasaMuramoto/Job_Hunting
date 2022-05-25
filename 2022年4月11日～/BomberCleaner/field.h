//==================================
// �t�B�[���h�̍쐬
// Author:������
//==================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "scene3D.h"
#include "texture.h"

//==============================================================================
// �}�N����`
//==============================================================================
//#define _OX_EPSILON_		(0.000001f) // �덷
//#define ALLOWABLE_ERROR		(5.0f)	// ���e�덷(�����Ζʂł������Ȃ��悤�ɂ���)


//==============================================================================
// �O���錾
//==============================================================================

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CField : public CScene3D
{
public:
	CField();
	~CField();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
	bool DotCollisionField(CScene *pScene);
	bool LineCollisionField(CScene *pScene);
	D3DXMATRIX *GetVtxMtx(void)	{ return &m_vtxMtxWorld[0]; }		// ���_���[���h�}�g���b�N�X�̎擾
	void SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx);		// ���_���[���h�}�g���b�N�X�̐ݒ�

private:
	D3DXMATRIX m_vtxMtxWorld[FIELD_VTX];
	D3DXVECTOR3 m_vtx[FIELD_VTX];
	D3DXVECTOR3 m_vtxRot[FIELD_VTX];
	CScene3D	*m_pScene3D;
};


#endif // !_FIELD_H_
