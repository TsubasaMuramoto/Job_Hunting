//===========================================
// ��Q��
//===========================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

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
class CObstacle : public CModel_Spawner
{
public:
	CObstacle(OBJTYPE nPriority = OBJTYPE_MODEL);		// �R���X�g���N�^
	~CObstacle();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// ����
	static CObstacle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);

private:
	CUi		*m_pUI;			// �c�@�\���pUI
	int		m_nFrame;		// �t���[���v���p
	bool	m_bDoOnce;		// ��x�̂ݒʂ鏈���Ɏg���ϐ�
	bool	m_bRetry;		// ���g���C���Ɏg���ϐ�
};
#endif
