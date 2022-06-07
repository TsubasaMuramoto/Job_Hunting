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
#include "Goal.h"
#include "Obstacle.h"

//========================================
// �}�N����`
//========================================
#define PLAYER_POS (D3DXVECTOR3(-1000.0f, 50.0f, 0.0f))

//========================================
// �ÓI�����o�ϐ��̏�����
//========================================

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
						case CScene::MODTYPE_NORMAL:		// �ʏ탂�f��
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_GOAL:			// �S�[�����f��
							CGoal::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_OBSTACLE:		// ��Q�����f��
							CObstacle::Create(pos, rot, scale, nType);
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
			
			//--------------------------------------------------------------
			// �v���C���[�ݒu����
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					fscanf(pFile, "%s", &aStr[1]);					// ������ǂݍ���
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)		// �I�u�W�F�N�g�̐���
					{
						// �v���C���[�̓ǂݍ���
						CPlayer::Create(PLAYER_POS, { 0.0f,0.0f,0.0f }, 0);	
						CModel_Spawner::Create({ PLAYER_POS.x - 100.0f,0.0f,PLAYER_POS.z }, { 0.0f,0.0f,0.0f }, { 2.0f,3.0f,3.0f }, 9);
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

