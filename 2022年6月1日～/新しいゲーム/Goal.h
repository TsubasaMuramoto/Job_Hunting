//===========================================
// ゴール地点のモデル
//===========================================
#ifndef _GOAL_H_
#define _GOAL_H_

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
class CGoal : public CModel_Spawner
{
public:
	CGoal(OBJTYPE nPriority = OBJTYPE_MODEL);			// コンストラクタ
	~CGoal();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);

private:
	CUi		*m_pUI;					// 終了UI
	int		m_nFrame;				// フレーム数

};
#endif
