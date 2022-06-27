//========================================
// ステージ読み込み
// Author:村元翼
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"

//========================================
// 前方宣言
//========================================
class CModel_Spawner;
class CPlayer;

//========================================
// ステージクラスの定義
//========================================
class CStage
{
public:
	CStage();				// コンストラクタ
	~CStage();				// デストラクタ

	HRESULT Init(void);								// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	void SetStage(const char *aStageFileName);		// モデルテキストを読み込む関数
};
#endif