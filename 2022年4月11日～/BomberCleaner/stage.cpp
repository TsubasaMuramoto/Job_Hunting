//========================================
// ステージ
// Author:村元翼
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
// マクロ定義
//========================================
#define MAX_STAGE_MODEL (300)			// モデル(パーツ)の最大数
#define MAX_STAGE_ENEMY (50)			// モデル(パーツ)の最大数
#define MAX_STAGE		(3)				// ステージ数

//========================================
// 静的メンバ変数の初期化
//========================================
//int		CStage::m_NumModel = 0;
//int		CStage::m_NumPlayer = 0;

//========================================
// コンストラクタ
//========================================
CStage::CStage()
{

}

//------------------------------------
// デストラクタ
//------------------------------------
CStage::~CStage()
{
}

//------------------------------------
// ステージ初期化
//------------------------------------
HRESULT CStage::Init(void)
{
	return S_OK;
}

//--------------------------------------------
// 終了
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
// 更新
//--------------------------------------------
void CStage::Update(void)
{

}

//------------------------------------
// ステージの読み込み
//------------------------------------
void CStage::SetStage(const char *aStageFileName)
{
	// ファイル読み込み
	FILE *pFile;
	// NULLチェック
	if (pFile = fopen(aStageFileName, "r"))
	{
		char aStr[6][255] = { NULL };		// 読み込みに使う変数
		int nNumPlayer = 0 ;

		// END_SCRIPTが呼ばれるまでループする
		// １単語を読み込む
		while(fscanf(pFile, "%s", &aStr[0]))
		{
			//--------------------------------------------------------------
			// メッシュ設置処理
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "MESH_SET") == 0)
			{
				// ローカルメッシュ情報
				int nTexType,nMeshType;
				int nBlockX, nBlockY;
				bool bInSide = false;
				D3DXVECTOR3 pos, rot, size;

				fscanf(pFile, "%s", &aStr[1]);				// 文字列読み込み
				if (strcmp(aStr[1], "MESH_TYPE") == 0)		// メッシュタイプ取得
				{
					fscanf(pFile, "%*s%d", &nMeshType);
				}
				if (strcmp(aStr[1], "TEX_TYPE") == 0)		// テクスチャタイプ取得
				{
					fscanf(pFile, "%*s%d", &nTexType);
				}
				if (strcmp(aStr[1], "POS") == 0)			// 位置取得
				{
					fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(aStr[1], "ROT") == 0)			// 回転取得
				{
					fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(aStr[1], "SIZE") == 0)			// サイズ取得
				{
					fscanf(pFile, "%*s%f %f %f", &size.x, &size.y, &size.z);
				}
				if (strcmp(aStr[1], "BLOCK") == 0)			// 分割数取得
				{
					fscanf(pFile, "%*s%d %d", &nBlockX, &nBlockY);
				}
				if (strcmp(aStr[1], "SIDE") == 0)			// 描画サイド取得
				{
					fscanf(pFile, "%*s%d", (int*)&bInSide);
				}
				if (strcmp(aStr[1], "END_MESH_SET") == 0)	// メッシュの生成
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
			// オブジェクト設置処理
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "MODEL_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// サイズチェック
				{
					// ローカルモデル情報
					int nType, nFunc;
					D3DXVECTOR3 pos, rot, scale;

					fscanf(pFile, "%s", &aStr[1]);					// 文字列読み込み
					if (strcmp(aStr[1], "FUNC") == 0)				// モデル機能取得
					{
						fscanf(pFile, "%*s%d", &nFunc);
					}
					if (strcmp(aStr[1], "TYPE") == 0)				// タイプ取得
					{
						fscanf(pFile, "%*s%d", &nType);
					}
					if (strcmp(aStr[1], "POS") == 0)				// 位置取得
					{
						fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr[1], "ROT") == 0)				// 回転取得
					{
						fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(aStr[1], "SCALE") == 0)				// スケール取得
					{
						fscanf(pFile, "%*s%f %f %f", &scale.x, &scale.y, &scale.z);
					}
					if (strcmp(aStr[1], "END_MODEL_SET") == 0)		// オブジェクトの生成
					{
						switch (nFunc)
						{
						case CScene::MODTYPE_NORMAL:		// ただのモデル
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_BOMB:			// 爆弾モデル
							CBomb::Create(pos, rot, scale,nType);
							break;

						case CScene::MODTYPE_BOMBSPAWNER:	// 爆弾スポナーモデル
							CBombSpawner::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_TARGET:		// ターゲットモデル
							CTarget::Create(pos, rot, scale, nType);
							break;

						default :							// ただのモデル
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;
						}
						
						break;
					}
				}

				else
				{
					printf("読み込み済みのXファイルが存在しません");
				}
			}
			
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// サイズチェック
				{
					fscanf(pFile, "%s", &aStr[1]);					// 文字列読み込み
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)		// オブジェクトの生成
					{
						// プレイヤーの読み込み
						CPlayer::Create({ 0.0f,50.0f,0.0f }, {0.0f,0.0f,0.0f}, 0);		// プレイヤー生成
						break;
					}
				}

				else
				{
					printf("読み込み済みのXファイルが存在しません");
				}
			}
			
			// 読み込みと配置終了
			if (strcmp(aStr[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}

	else
	{
		// エラー
		perror("ファイルが開けませんでした");
	}

	// ファイルを閉じる
	fclose(pFile);
}

//------------------------------------
// ステージの読み込み設置
//------------------------------------
void CStage::StageSet(const char *sStageFileName)
{
	//char sString[6][255];	// 読み込み用の変数
	//int nCntNumModel = 0;
	//int nCntModel = 0;
	//int nCntField = 0;
	//D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//int nBlockX = 0;
	//int nBlockY = 0;
	//int nType = 0;

	//// ファイル読み込み
	//FILE *pFile = fopen(sStageFileName, "r");

	//// NULLチェック
	//if (pFile != NULL)
	//{
	//	// END_SCRIPTが呼ばれるまでループする
	//	while (1)
	//	{
	//		// １単語を読み込む
	//		fscanf(pFile, "%s", &sString[0]);

	//		// パーツ数の読み込み
	//		if (strcmp(sString[0], "NUM_MODEL") == 0)
	//		{

	//			fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
	//			fscanf(pFile, "%d", &m_NumModel);	// パーツ数を読み込む
	//			m_sModelFileName = new char*[m_NumModel];

	//			for (int nCnt = 0; nCnt < m_NumModel; nCnt++)
	//			{
	//				m_sModelFileName[nCnt] = new char[64];
	//			}
	//		}

	//		if (strcmp(sString[0], "MODEL_FILENAME") == 0)
	//		{
	//			//モデルファイルのパス読み込み
	//			fscanf(pFile, "%s", &sString[1]);
	//			fscanf(pFile, "%s", m_sModelFileName[nCntNumModel]);
	//			nCntNumModel++;
	//		}
	//		if (strcmp(sString[0], "NUM_FIELD") == 0)
	//		{
	//			int Num = 0;
	//			//モデルファイルのパス読み込み
	//			fscanf(pFile, "%s", &sString[1]);
	//			fscanf(pFile, "%d", &Num);
	//			m_pField = new CField*[Num];
	//			m_NumMaxField = Num;

	//		}

	//		// メッシュフィールドの読み込み
	//		while (strcmp(sString[0], "FIELDSET") == 0)
	//		{

	//			fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "BLOCK") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%d %d", &nBlockX, &nBlockY);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "SIZE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f", &size.x, &size.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "END_FIELDSET") == 0)
	//			{
	//				m_pField[nCntField] = CField::Create(pos, size, rot, nBlockX, nBlockY, CTexture::FIELD);

	//				nCntField++;
	//				break;
	//			}
	//		}
	//		// モデルの読み込み
	//		while (strcmp(sString[0], "MODELSET") == 0)
	//		{
	//			fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
	//			if (strcmp(sString[1], "TYPE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%d", &nType);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "SIZE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f", &size.x, &size.z);	// イコールを噛ませる
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
	//		// 敵の読み込み
	//		while (strcmp(sString[0], "ENEMYSET") == 0)
	//		{
	//			fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
	//			if (strcmp(sString[1], "ENEMYTYPE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%d", &nType);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// イコールを噛ませる
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
	//		// 障害物の読み込み
	//		while (strcmp(sString[0], "BARRIERSET") == 0)
	//		{
	//			fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
	//			if (strcmp(sString[1], "BARRIERTYPE") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%d", &nType);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "POS") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);	// イコールを噛ませる
	//			}
	//			if (strcmp(sString[1], "ROT") == 0)
	//			{
	//				fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
	//				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);	// イコールを噛ませる
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
	//	// エラー出力
	//	perror("ファイルが開けませんでした");
	//}

	//// ファイルを閉じる
	//fclose(pFile);
}

