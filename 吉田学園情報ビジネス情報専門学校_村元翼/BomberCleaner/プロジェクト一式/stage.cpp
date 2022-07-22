//========================================
// ステージの作成
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
// コンストラクタ
//========================================
CStage::CStage()
{
}

//========================================
// デストラクタ
//========================================
CStage::~CStage()
{
}

//========================================
// ステージ初期化
//========================================
HRESULT CStage::Init(void)
{
	return S_OK;
}

//========================================
// 終了
//========================================
void CStage::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CStage::Update(void)
{

}

//========================================
// ステージの読み込み
//========================================
void CStage::SetStage(const char *aStageFileName)
{
	// ファイル読み込み
	FILE *pFile;

	// NULLチェック
	if (pFile = fopen(aStageFileName, "r"))
	{
		char aStr[6][255] = { NULL };		// 読み込みに使う変数

		// ローカル情報
		int nType = 0;
		int nFunc = 0;
		int nTexType = 0;
		int nMeshType = 0;
		int nBlockX = 0;
		int nBlockY = 0;
		bool bInSide = false;
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 size = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 scale = { 1.0f,1.0f,1.0f };

		// END_SCRIPTが呼ばれるまでループする
		// １単語を読み込む
		while(fscanf(pFile, "%s", &aStr[0]))
		{
			//=====================================================
			// オブジェクト設置処理
			//=====================================================
			while (strcmp(aStr[0], "MODELSET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// サイズチェック
				{
					fscanf(pFile, "%s", &aStr[1]);					// 文字列読み込み

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
					if (strcmp(aStr[1], "END_MODELSET") == 0)		// オブジェクトの生成
					{
						// タイプ別生成
						switch (nType)
						{
						case TypeBomb:
							CBomb::Create(pos, rot, scale, nType);
							break;

						case TypeBombspawner:
							CBombSpawner::Create(pos, rot, scale, nType);
							break;

						case TypeTargetBag:
							CTarget::Create(pos, rot, scale, nType);
							break;

						case TypeTargetWoodBox:
							CTarget::Create(pos, rot, scale, nType);
							break;

						default:
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

			//=====================================================
			// メッシュ設置処理
			//=====================================================
			while (strcmp(aStr[0], "FIELDSET") == 0)
			{
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
				if (strcmp(aStr[1], "END_FIELDSET") == 0)	// メッシュの生成
				{
					// タイプ別生成
					CMeshField::Create(pos, D3DXVECTOR3(size.x, 0.0f, size.y), rot, nBlockX, nBlockY);
					break;
				}
			}

			//=====================================================
			// メッシュ設置処理
			//=====================================================
			while (strcmp(aStr[0], "WALLSET") == 0)
			{
				// ローカルメッシュ情報
				int nTexType = 0;
				int nMeshType = 0;
				int nBlockX = 0;
				int nBlockY = 0;
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
				if (strcmp(aStr[1], "END_WALLSET") == 0)	// メッシュの生成
				{
					// タイプ別生成
					CMeshField::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), rot, nBlockX, nBlockY);
					break;
				}
			}
			
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// サイズチェック
				{
					fscanf(pFile, "%s", &aStr[1]);				// 文字列読み込み
					if (strcmp(aStr[1], "POS") == 0)			// 位置取得
					{
						fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr[1], "ROT") == 0)			// 回転取得
					{
						fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)		// オブジェクトの生成
					{
						// プレイヤーの読み込み
						CPlayer::Create(pos, rot, 0);
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