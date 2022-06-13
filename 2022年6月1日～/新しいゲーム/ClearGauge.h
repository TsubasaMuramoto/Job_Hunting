//============================================================
// クリアゲージの作成
// Author:村元翼
//============================================================
#ifndef _CLEARGAUGE_H_
#define _CLEARGAUGE_H_

//============================================================
// インクルードファイル
//============================================================
#include "main.h"
#include "scene.h"

//============================================================
// マクロ定義
//============================================================
#define MAX_PERCENT_DIGIT (3)
#define GAUGE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f,0.0f))
#define GAUGE_SIZE (D3DXVECTOR3(40.0f,20.0f,0.0f))

//============================================================
// 前方宣言
//============================================================
class CNumber;
class CUi;

//============================================================
// クラスの生成
//============================================================
class CClearGauge : public CScene
{
public:
	//--------------------------------
	// 構造体
	//--------------------------------
	typedef struct
	{
		CUi				*pFrame;							// ゲージのフレームUI
		CUi				*pGauge;							// ゲージのUI
		D3DXVECTOR3		posFrame;							// ゲージフレームの位置
		D3DXVECTOR3		posGauge;							// ゲージの位置
		D3DXVECTOR3		size;								// ゲージの大きさ
		float			MaxLength;							// ゲージの最大の長さ

	}GAUGE_UI_INFO;


	CClearGauge(OBJTYPE nPriority = OBJTYPE_UI);		// コンストラクタ
	~CClearGauge();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CClearGauge *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetPercent(void);	// パーセント設定

private:
	D3DXVECTOR3		m_pos;								// 位置
	D3DXVECTOR3		m_size;								// サイズ
	D3DXVECTOR3		m_ClearLength;						// クリアまでの距離
	int				m_nPercent;							// クリアパーセントの数字
	CNumber			*m_apPercent[MAX_PERCENT_DIGIT];	// クリアパーセントのポリゴン
	GAUGE_UI_INFO	m_GaugeUI;							// ゲージUI
};
#endif