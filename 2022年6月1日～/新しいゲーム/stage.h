//========================================
// �X�e�[�W�ǂݍ���
// Author:������
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"

//========================================
// �}�N����`
//========================================

//========================================
// �O���錾
//========================================
class CModel_Spawner;
class CPlayer;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CModel_Spawner;
class CGoal;
class CObstacle;
class CTarget;

//========================================
// �X�e�[�W�N���X
//========================================
class CStage
{
public:
	//------------------------------------
	// �R���X�g���N�^�A�f�X�g���N�^
	//------------------------------------
	CStage();
	~CStage();

	//------------------------------------
	// �v���g�^�C�v�錾
	//------------------------------------
	HRESULT Init(void);								// ������
	void Uninit(void);								// �I��
	void Update(void);								// �X�V
	void SetStage(const char *aStageFileName);		// ���f���e�L�X�g��ǂݍ��ފ֐�

private:
	vector<CPlayer>		m_pPlayer;
	vector<CGoal>		m_pGoal;
	vector<CObstacle>	m_pObstacle;
};
#endif