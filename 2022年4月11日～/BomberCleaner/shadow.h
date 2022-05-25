//==================================
// �e�̍쐬
// Author:������
//==================================
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"
#include "scene3D.h"

//==============================================================================
// �}�N��
//==============================================================================
#define GRAVITY (1000.0f)

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CShadow : public CScene3D
{
public:
	CShadow(OBJTYPE nPriority = OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);

	void SetGround(const float &X, const float &Z)	{ m_pos.x = X, m_pos.z = Z; }
	void SetHeight(const float &fHeight)			{ m_pos.y = fHeight; }
	void SetGravity(float GravitySpeed);

	//D3DXMATRIX *GetVtxMtx(void) { return &m_vtxMtxWorld[0]; }		// ���_���[���h�}�g���b�N�X�̎擾
	//void SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx);		// ���_���[���h�}�g���b�N�X�̐ݒ�

private:
	D3DXVECTOR3 m_OriginSize;
	//D3DXMATRIX m_vtxMtxWorld[FIELD_VTX];
	//D3DXVECTOR3 m_vtx[FIELD_VTX];
	//D3DXVECTOR3 m_vtxRot[FIELD_VTX];
};


#endif // !_FIELD_H_
