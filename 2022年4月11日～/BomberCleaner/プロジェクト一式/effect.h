//=============================================================================
//
// エフェクト [effect.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EXPLOSION_NUM	(128)								// 爆発エフェクト数
#define EXPLOSION_SUB	(0.5f)								// 爆発エフェクト縮小量
#define EXPLOSION_SIZE	(D3DXVECTOR3(50.0f,50.0f,0.0f))		// 爆発エフェクトサイズ
#define EXPLOSION_COL	(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))	// 爆発エフェクトカラー

#define IGNITION_SUB	(0.2f)								// 発火エフェクト縮小量
#define IGNITION_SIZE	(D3DXVECTOR3(5.0f, 5.0f, 0.0f))		// 発火エフェクトサイズ
#define IGNITION_COL	(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))	// 発火エフェクトカラー

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CControl;

//*****************************************************************************
// エフェクトクラスの定義
//*****************************************************************************
class CEffect : public CScene
{
public:
	//---------------------------------------
	// エフェクトタイプの列挙型構造体
	//---------------------------------------
	typedef enum
	{
		EFFECT_NONE = 0,
		EFFECT_FADE,
		EFFECT_BURN,
		EFFECT_EXPLOSION,
		EFFECT_MAX

	}EFFECT_TYPE;

	CEffect(OBJTYPE nPriority = OBJTYPE_EFFECT);	// コンストラクタ
	~CEffect();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, float fSub,int nType);		// 生成

	//---------------------------------------
	// Setter & Getter
	//---------------------------------------
	void SetSize(D3DXVECTOR3 size);
	void SetCol(D3DXCOLOR col);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture = nullptr;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = nullptr;	// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;					// 位置
	D3DXVECTOR3				m_move;					// 移動量
	D3DXVECTOR3				m_size;					// サイズ
	D3DXCOLOR				m_col;					// 色
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	CControl				*m_pControl;			// コントロールクラス
	float					m_fSub;					// 減少量

};
#endif //!_EFFECT_H_
