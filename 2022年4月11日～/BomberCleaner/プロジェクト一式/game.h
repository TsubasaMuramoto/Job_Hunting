//============================================================
// ゲームシーンの処理
// Author:村元翼
//============================================================
#ifndef _GAME_H_
#define _GAME_H_

//============================================================
// インクルードファイル
//============================================================
#include "main.h"

//============================================================
// 前方宣言
//============================================================
class CPlayer;
class CTime;
class CScore;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CStage;
class CUi;
class CTimerCount;

//============================================================
// クラスの生成
//============================================================
class CGame
{
public:

	CGame();				// コンストラクタ
	~CGame();				// デストラクタ
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	static CScore *GetScore() { return m_pScore; }	// スコアの取得
	void SetUiDelete(void);							// UIの削除

private:
	CTimerCount			*m_pTimer;			// タイマークラス
	CUi					*m_pUI;				// UIクラス
	CStage				*m_pStage;			// ステージクラス
	CMeshField			*m_pMeshField;		// メッシュフィールドクラス
	CMeshField			*m_pMeshWall;		// メッシュウォールクラス
	CMeshCylinder		*m_pMeshCylinder;	// メッシュシリンダークラス
	CMeshSphere			*m_pMeshSphere;		// メッシュスフィアクラス
	static CScore		*m_pScore;			// スコアクラス
	bool				m_bOnce;			// 一度のみ通る処理に使う変数
	bool				m_bStart;			// スタート判定
	int					m_nFrame;			// フレームカウント変数
};
#endif