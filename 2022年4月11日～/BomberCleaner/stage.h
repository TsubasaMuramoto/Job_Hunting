//========================================
// �X�e�[�W
// Author:������
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"

//========================================
// �}�N����`
//========================================
#define MAX_OBJECT		(50)			// �I�u�W�F�N�g�̍ő吔
#define MAX_PLAYER		(1)				// �v���C���[�̍ő吔

//========================================
// �O���錾
//========================================
class CModel_Spawner;
class CPlayer;

//------------------------------------------
// �X�e�[�W�N���X
//------------------------------------------
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
	void StageSet(const char *sStageFileName);		// �X�e�[�W�e�L�X�g��ǂݍ��ފ֐�
	void SetStage(const char *aStageFileName);		// ���f���e�L�X�g��ǂݍ��ފ֐�

private:
};
#endif _STAGE_H_