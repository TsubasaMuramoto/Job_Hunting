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
#include "scene.h"

//============================================================
// 前方宣言
//============================================================
class CBg;
class CPlayer;
class CTime;
class CScore;
class CPolygon;
class CTarget;
class CGauge;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CStage;
class CUi;

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

	static CScore *GetScore() { return m_pScore; }

	bool GetStart(void);
	bool GetEnd(void);

private:
	//メンバ変数
	CUi					*m_pUI;
	CStage				*m_pStage;			// ステージクラス
	CMeshField			*m_pMeshField;		// メッシュフィールドクラス
	CMeshField			*m_pMeshWall;		// メッシュウォールクラス
	CMeshCylinder		*m_pMeshCylinder;	// メッシュシリンダークラス
	CMeshSphere			*m_pMeshSphere;		// メッシュスフィアクラス
	static CScore		*m_pScore;			// スコアクラス

	int m_nFrame;
	bool m_bStart;
	bool m_bEnd;
};
#endif //_GAME_H_