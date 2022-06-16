//===========================================
// �Q�[�g�̃��f��
//===========================================
#ifndef _GATE_H_
#define _GATE_H_

//===========================================
// �C���N���[�h�t�@�C��
//===========================================
#include "main.h"
#include "model_spawner.h"

//===========================================
// �O���錾
//===========================================
class CUi;

//===========================================
// �N���X�̒�`
//===========================================
class CGate : public CModel_Spawner
{
public:
	CGate(OBJTYPE nPriority = OBJTYPE_MODEL);			// �R���X�g���N�^
	~CGate();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// ����
	static CGate *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);
	D3DXVECTOR3 GetScale() { return m_scale; }

private:
	D3DXVECTOR3 m_scale;			// �X�P�[��
	CUi			*m_pUI;				// �I��UI
	int			m_nFrame;			// �t���[����

};
#endif
