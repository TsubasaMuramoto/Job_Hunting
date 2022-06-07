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
#include "Goal.h"
#include "Obstacle.h"

//========================================
// マクロ定義
//========================================
#define PLAYER_POS (D3DXVECTOR3(-1000.0f, 50.0f, 0.0f))

//========================================
// 静的メンバ変数の初期化
//========================================

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
						case CScene::MODTYPE_NORMAL:		// 通常モデル
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_GOAL:			// ゴールモデル
							CGoal::Create(pos, rot, scale, nType);
							break;

						case CScene::MODTYPE_OBSTACLE:		// 障害物モデル
							CObstacle::Create(pos, rot, scale, nType);
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
			
			//--------------------------------------------------------------
			// プレイヤー設置処理
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// サイズチェック
				{
					fscanf(pFile, "%s", &aStr[1]);					// 文字列読み込み
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)		// オブジェクトの生成
					{
						// プレイヤーの読み込み
						CPlayer::Create(PLAYER_POS, { 0.0f,0.0f,0.0f }, 0);	
						CModel_Spawner::Create({ PLAYER_POS.x - 100.0f,0.0f,PLAYER_POS.z }, { 0.0f,0.0f,0.0f }, { 2.0f,3.0f,3.0f }, 9);
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

