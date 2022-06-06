//============================================================
// �Q�[���V�[���̏���
// Author:������
//============================================================
#ifndef _GAME_H_
#define _GAME_H_

//============================================================
// �C���N���[�h�t�@�C��
//============================================================
#include "main.h"
#include "scene.h"
#include "manager.h"

//============================================================
// �O���錾
//============================================================
class CBg;
class CPlayer;
class CTime;
class CScore;
class CPolygon;
class CTarget;
class CGauge;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CStage;
class CUi;
class CTimerCount;

//============================================================
// �N���X�̐���
//============================================================
class CGame
{
public:

	CGame();				// �R���X�g���N�^
	~CGame();				// �f�X�g���N�^
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	static CScore *GetScore()	{ return m_pScore; }
	void SetUiDelete(void);				// UI�̂ݍ폜����Ƃ��̊֐�

	bool GetStart(void);
	bool GetEnd(void);

private:
	//�����o�ϐ�
	CTimerCount			*m_pTimer;			// �^�C�}�[�N���X
	CUi					*m_pUI;				// UI�N���X
	CStage				*m_pStage;			// �X�e�[�W�N���X
	CMeshField			*m_pMeshField;		// ���b�V���t�B�[���h�N���X
	CMeshField			*m_pMeshWall;		// ���b�V���E�H�[���N���X
	CMeshCylinder		*m_pMeshCylinder;	// ���b�V���V�����_�[�N���X
	CMeshSphere			*m_pMeshSphere;		// ���b�V���X�t�B�A�N���X
	static CScore		*m_pScore;			// �X�R�A�N���X
	D3DXVECTOR3			m_CameraRot;		// �J������]
	bool				m_bOnce;			// ��x�̂ݒʂ鏈��
	static bool			m_bRetry;			// ���g���C����

	int m_nFrame;
	bool m_bStart;
	bool m_bEnd;
};
#endif