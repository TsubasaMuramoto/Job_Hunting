//========================================
// �X�e�[�W
// Author:������
//========================================
#include "stage.h"
#include "model.h"
#include "texture.h"
#include "model_spawner.h"
#include "BombSpawner.h"
#include "Bomb.h"
#include "player.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "LoadX.h"
#include "manager.h"
#include "meshsphere.h"
#include "Target.h"

//========================================
// �}�N����`
//========================================
#define MAX_STAGE_MODEL (300)			// ���f��(�p�[�c)�̍ő吔
#define MAX_STAGE_ENEMY (50)			// ���f��(�p�[�c)�̍ő吔
#define MAX_STAGE		(3)				// �X�e�[�W��

//========================================
// �ÓI�����o�ϐ��̏�����
//========================================
//int		CStage::m_NumModel = 0;
//int		CStage::m_NumPlayer = 0;

//========================================
// �R���X�g���N�^
//========================================
CStage::CStage()
{

}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CStage::~CStage()
{
}

//------------------------------------
// �X�e�[�W������
//------------------------------------
HRESULT CStage::Init(void)
{
	return S_OK;
}

//--------------------------------------------
// �I��
//--------------------------------------------
void CStage::Uninit(void)
{
	//for (int nCnt = 0; nCnt < MAX_PLAYER ; nCnt++)
	//{
	//	if (m_pPlayer[nCnt])
	//	{
	//		m_pPlayer[nCnt]->Uninit();
	//		m_pPlayer[nCnt] = nullptr;
	//	}
	//}
}

//--------------------------------------------
// �X�V
//--------------------------------------------
void CStage::Update(void)
{

}

//------------------------------------
// �X�e�[�W�̓ǂݍ���
//------------------------------------
void CStage::SetStage(const char *aStageFileName)
{
	// �t�@�C���ǂݍ���
	FILE *pFile;
	// NULL�`�F�b�N
	if (pFile = fopen(aStageFileName, "r"))
	{
		char aStr[6][255] = { NULL };		// �ǂݍ��݂Ɏg���ϐ�
		int nNumPlayer = 0 ;

		// END_SCRIPT���Ă΂��܂Ń��[�v����
		// �P�P���ǂݍ���
		while(fscanf(pFile, "%s", &aStr[0]))
		{
			//--------------------------------------------------------------
			// ���b�V���ݒu����
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "MESH_SET") == 0)
			{
				// ���[�J�����b�V�����
				int nTexType,nMeshType;
				int nBlockX, nBlockY;
				bool bInSide = false;
				D3DXVECTOR3 pos, rot, size;

				fscanf(pFile, "%s", &aStr[1]);				// ������ǂݍ���
				if (strcmp(aStr[1], "MESH_TYPE") == 0)		// ���b�V���^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nMeshType);
				}
				if (strcmp(aStr[1], "TEX_TYPE") == 0)		// �e�N�X�`���^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nTexType);
				}
				if (strcmp(aStr[1], "POS") == 0)			// �ʒu�擾
				{
					fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(aStr[1], "ROT") == 0)			// ��]�擾
				{
					fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(aStr[1], "SIZE") == 0)			// �T�C�Y�擾
				{
					fscanf(pFile, "%*s%f %f %f", &size.x, &size.y, &size.z);
				}
				if (strcmp(aStr[1], "BLOCK") == 0)			// �������擾
				{
					fscanf(pFile, "%*s%d %d", &nBlockX, &nBlockY);
				}
				if (strcmp(aStr[1], "SIDE") == 0)			// �`��T�C�h�擾
				{
					fscanf(pFile, "%*s%d", (int*)&bInSide);
				}
				if (strcmp(aStr[1], "END_MESH_SET") == 0)	// ���b�V���̐���
				{
					switch (nMeshType)
					{
					case CScene::MESH_FIELD:
						CMeshField::Create(pos, D3DXVECTOR3(size.x, 0.0f, size.y), rot, nBlockX, nBlockY);
						break;

					case CScene::MESH_WALL:
						CMeshField::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), rot, nBlockX, nBlockY);
						break;

					case CScene::MESH_CYLINDER:
						CMeshCylinder::Create(pos, size, rot, nBlockX, nBlockY);
						break;

					case CScene::MESH_SPHERE:
						CMeshSphere::Create(pos, size, rot, nBlockX, nBlockY,bInSide,CScene::OBJTYPE_MESH);
						break;

					default:
						break;
					}
					break;
				}
			}

			//--------------------------------------------------------------
			// �I�u�W�F�N�g�ݒu����
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "MODEL_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					// ���[�J�����f�����
					int nType, nFunc;
					D3DXVECTOR3 pos, rot, scale;

					fscanf(pFile, "%s", &aStr[1]);					// ������ǂݍ���
					if (strcmp(aStr[1], "FUNC") == 0)				// ���f���@�\�擾
					{
						fscanf(pFile, "%*s%d", &nFunc);
					}
					if (strcmp(aStr[1], "TYPE") == 0)				// �^�C�v�擾
					{
						fscanf(pFile, "%*s%d", &nType);
					}
					if (strcmp(aStr[1], "POS") == 0)				// �ʒu�擾
					{
						fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr[1], "ROT") == 0)				// ��]�擾
					{
						fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(aStr[1], "SCALE") == 0)				// �X�P�[���擾
					{
						fscanf(pFile, "%*s%f %f %f", &scale.x, &scale.y, &scale.z);
					}
					if (strcmp(aStr[1], "END_MODEL_SET") == 0)		// �I�u�W�F�N�g�̐���
					{
						switch (nFunc)
						{
						case CScene::MODTYPE_NORMAL:		// �����̃��f��
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_BOMB:			// ���e���f��
							CBomb::Create(pos, rot, scale,nType);
							break;

						case CScene::MODTYPE_BOMBSPAWNER:	// ���e�X�|�i�[���f��
							CBombSpawner::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_TARGET:		// �^�[�Q�b�g���f��
							CTarget::Create(pos, rot, scale, nType);
							break;

						default :							// �����̃��f��
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;
						}
						
						break;
					}
				}

				else
				{
					printf("�ǂݍ��ݍς݂�X�t�@�C�������݂��܂���");
				}
			}
			
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					fscanf(pFile, "%s", &aStr[1]);					// ������ǂݍ���
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)		// �I�u�W�F�N�g�̐���
					{
						// �v���C���[�̓ǂݍ���
						CPlayer::Create({ 0.0f,50.0f,0.0f }, {0.0f,0.0f,0.0f}, 0);		// �v���C���[����
						break;
					}
				}

				else
				{
					printf("�ǂݍ��ݍς݂�X�t�@�C�������݂��܂���");
				}
			}
			
			// �ǂݍ��݂Ɣz�u�I��
			if (strcmp(aStr[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}

	else
	{
		// �G���[
		perror("�t�@�C�����J���܂���ł���");
	}

	// �t�@�C�������
	fclose(pFile);
}

//------------------------------------
// �X�e�[�W�̓ǂݍ��ݐݒu
//------------------------------------
void CStage::StageSet(const char *sStageFileName)
{
	//char sString[6][255];	// �ǂݍ��ݗp�̕ϐ�
	//int nCntNumModel = 0;
	//int nCntModel = 0;
	//int nCntField = 0;
	//D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//int nBlockX = 0;
	//int nBlockY = 0;
	//int nType = 0;

	//// �t�@�C���ǂݍ���
	//FILE *pFile = fopen(sStageFileName, "r");

	//// NULL�`�F�b�N
	//if (pFile != NULL)
	//{
	//	// END_SCRIPT���Ă΂��܂Ń��[�v����
	//	while (1)
	//	{
	//		// �P�P���ǂݍ���
	//		fscanf(pFile, "%s", &sString[0]);

	//		// �p�[�c���̓ǂݍ���
	//		if (strcmp(sString[0], "NUM_MODEL") == 0)
	//		{

	//			fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
	//			fscanf(pFile, "%d", &m_NumModel);	// �p�[�c����ǂݍ���
	//			m_sModelFileName = new char*[m_NumModel];

	//			for (int nCnt = 0; nCnt < m_NumModel; nCnt++)
	//			{
	//				m_sModelFileName[nCnt] = new char[64];
	//			}
	//		}

	//		if (strcmp(sString[0], "MODEL_FILENAME") == 0)
	//		{
	//			//���f���t�@�C���̃p�X�ǂݍ���
	//			fscanf(pFile, "%s", &sString[1]);
	//			fscanf(pFile, "%s", m_sModelFileName[nCntNumModel]);
	//			nCntNumModel++;
	//		}
	//		if (strcmp(sString[0], "NUM_FIELD") == 0)
	//		{
	//			int Num = 0;
	//			//���f���t�@�C���̃p�X�ǂݍ���
	//			fscanf(pFile, "%s", &sString[1]);
	//			fscanf(pFile, "%d", &Num);
	//			m_pField = new CField*[Num];
	//			m_NumMaxField = Num;

	//		}

	//		// ���b�V���t�B�[���h�̓ǂݍ���
	//		while (strcmp(sString[0], "FIELDSET") == 0)
	//		{

	//			fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "BLOCK") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%d %d", &nBlockX, &nBlockY);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "SIZE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f", &size.x, &size.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "END_FIELDSET") == 0)
	//			{
	//				m_pField[nCntField] = CField::Create(pos, size, rot, nBlockX, nBlockY, CTexture::FIELD);

	//				nCntField++;
	//				break;
	//			}
	//		}
	//		// ���f���̓ǂݍ���
	//		while (strcmp(sString[0], "MODELSET") == 0)
	//		{
	//			fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
	//			if (strcmp(sString[1], "TYPE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%d", &nType);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "SIZE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f", &size.x, &size.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "END_MODELSET") == 0)
	//			{
	//				m_pStageModel[nCntModel] = CModel_Spawner::Create(pos / 2.0f, rot, m_sModelFileName[nType]);
	//				if (nType == 5)
	//				{
	//					m_pStageModel[nCntModel]->SetBoolRot(true);
	//				}
	//				nCntModel++;
	//				m_NumMaxModel++;
	//				break;
	//			}
	//		}
	//		// �G�̓ǂݍ���
	//		while (strcmp(sString[0], "ENEMYSET") == 0)
	//		{
	//			fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
	//			if (strcmp(sString[1], "ENEMYTYPE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%d", &nType);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "END_ENEMYSET") == 0)
	//			{
	//				switch (nType)
	//				{
	//				case 0:
	//					m_pEnemy[m_NumMaxEnemy] = CEnemy::Create(pos, rot);
	//					m_NumMaxEnemy++;
	//					break;
	//				case 1:
	//					m_pEnemy_baum[m_NumMaxEnemyBaum] = CEnemy_Baum::Create(pos, rot);
	//					m_NumMaxEnemyBaum++;
	//					break;
	//				}
	//				break;
	//			}

	//		}
	//		// ��Q���̓ǂݍ���
	//		while (strcmp(sString[0], "BARRIERSET") == 0)
	//		{
	//			fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
	//			if (strcmp(sString[1], "BARRIERTYPE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%d", &nType);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// �C�R�[�������܂���
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// �C�R�[�������܂���
	//			}
	//			if (strcmp(sString[1], "END_BARRIERSET") == 0)
	//			{
	//				switch (nType)
	//				{
	//				case 0:
	//					CBarrier_Block::Create(pos, rot);
	//					break;
	//				case 1:
	//					CBarrier_wood::Create(pos, rot);
	//					break;
	//				}
	//				break;
	//			}

	//		}

	//		if (strcmp(sString[0], "END_SCRIPT") == 0)
	//		{
	//			break;
	//		}

	//	}
	//}

	//else
	//{
	//	// �G���[�o��
	//	perror("�t�@�C�����J���܂���ł���");
	//}

	//// �t�@�C�������
	//fclose(pFile);
}

