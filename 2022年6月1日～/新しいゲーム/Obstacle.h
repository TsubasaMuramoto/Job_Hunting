//===========================================
// 障害物
//===========================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//===========================================
// インクルードファイル
//===========================================
#include "main.h"
#include "model_spawner.h"

//===========================================
// 前方宣言
//===========================================
class CUi;

//===========================================
// クラスの定義
//===========================================
class CObstacle : public CModel_Spawner
{
public:
	// 障害物の種類分け
	enum class OBSTACLE
	{
		NEEDLE = 0,
		CUBE,
		MAX
	};


	CObstacle(OBJTYPE nPriority = OBJTYPE_MODEL);		// コンストラクタ
	~CObstacle();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CObstacle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, OBSTACLE obstype);
	// プレイヤー消去
	void DeletePlayer(void);

private:
	CUi			*m_pUI;			// 残機表示用UI
	int			m_nFrame;		// フレーム計測用
	bool		m_bDoOnce;		// 一度のみ通る処理に使う変数
	bool		m_bRetry;		// リトライ時に使う変数
	OBSTACLE	m_ObsType;		// 障害物の種類
};
#endif
