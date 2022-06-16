//===========================================
// ゲートのモデル
//===========================================
#ifndef _GATE_H_
#define _GATE_H_

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
class CGate : public CModel_Spawner
{
public:
	CGate(OBJTYPE nPriority = OBJTYPE_MODEL);			// コンストラクタ
	~CGate();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CGate *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);
	D3DXVECTOR3 GetScale() { return m_scale; }

private:
	D3DXVECTOR3 m_scale;			// スケール
	CUi			*m_pUI;				// 終了UI
	int			m_nFrame;			// フレーム数

};
#endif
