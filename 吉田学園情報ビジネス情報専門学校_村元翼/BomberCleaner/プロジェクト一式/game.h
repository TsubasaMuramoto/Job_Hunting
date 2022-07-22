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

	CScore *GetScore() { return m_pScore; }									// スコアの取得
	void StartGame(void);													// ゲームスタート
	void EndGame(void);														// ゲーム終了
	void SetUiCreate(const char* TexStr = "",float fSizeMulti = 1);	// UIの生成
	void SetUiDelete(void);													// UIの削除

private:
	CTimerCount			*m_pTimer;			// タイマークラス
	CUi					*m_pUI;				// UIクラス
	CStage				*m_pStage;			// ステージクラス
	CMeshField			*m_pMeshField;		// メッシュフィールドクラス
	CMeshField			*m_pMeshWall;		// メッシュウォールクラス
	CMeshCylinder		*m_pMeshCylinder;	// メッシュシリンダークラス
	CMeshSphere			*m_pMeshSphere;		// メッシュスフィアクラス
	CScore				*m_pScore;			// スコアクラス
	CScore				*m_pAddScore;		// 追加スコアクラス
	bool				m_bOnce;			// 一度のみ通る処理に使う変数
	bool				m_bStart;			// スタート判定
	bool				m_bEnd;				// エンド判定
	bool				m_bFrameCount;		// フレームカウント判定
	int					m_nFrame;			// フレームカウント変数
};
#endif