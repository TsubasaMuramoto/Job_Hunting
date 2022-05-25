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

	static CScore *GetScore() { return m_pScore; }

	bool GetStart(void);
	bool GetEnd(void);

private:
	//�����o�ϐ�
	CUi					*m_pUI;
	CStage				*m_pStage;			// �X�e�[�W�N���X
	CMeshField			*m_pMeshField;		// ���b�V���t�B�[���h�N���X
	CMeshField			*m_pMeshWall;		// ���b�V���E�H�[���N���X
	CMeshCylinder		*m_pMeshCylinder;	// ���b�V���V�����_�[�N���X
	CMeshSphere			*m_pMeshSphere;		// ���b�V���X�t�B�A�N���X
	static CScore		*m_pScore;			// �X�R�A�N���X

	int m_nFrame;
	bool m_bStart;
	bool m_bEnd;
};
#endif //_GAME_H_