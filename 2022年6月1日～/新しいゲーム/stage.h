//========================================
// ステージ読み込み
// Author:村元翼
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"

//========================================
// マクロ定義
//========================================

//========================================
// 前方宣言
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
// ステージクラス
//========================================
class CStage
{
public:
	//------------------------------------
	// コンストラクタ、デストラクタ
	//------------------------------------
	CStage();
	~CStage();

	//------------------------------------
	// プロトタイプ宣言
	//------------------------------------
	HRESULT Init(void);								// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	void SetStage(const char *aStageFileName);		// モデルテキストを読み込む関数

private:
	vector<CPlayer>		m_pPlayer;
	vector<CGoal>		m_pGoal;
	vector<CObstacle>	m_pObstacle;
};
#endif