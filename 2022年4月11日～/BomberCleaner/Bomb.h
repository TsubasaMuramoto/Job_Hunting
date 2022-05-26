//===========================================
// 爆弾の作成
// Author : 村元翼
//===========================================
#ifndef _BOMB_H_
#define _BOMB_H_
#include "main.h"
#include "scene.h"

//===========================================
// マクロ定義
//===========================================

//===========================================
// 前方宣言
//===========================================
class CModel;
class CControl;
class CShadow;

//===========================================
// クラス定義
//===========================================
class CBomb : public CScene
{
public:

	//---------------------------------
	// コンストラクタ・デストラクタ
	//---------------------------------
	CBomb(OBJTYPE nPriority = OBJTYPE_MODEL);
	~CBomb();

	//---------------------------------
	// メソッド
	//---------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	static CBomb *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);	// オブジェクト生成
	void Ignition(void);					// 発火状態の処理
	void Explosion(void);					// 爆発状態の処理
	void Throw(D3DXVECTOR3 ThrowDir);		// 投げた時の処理

	// 設定・取得処理
	void SetThrow(bool bThrow)	{ m_bThrow = bThrow; }
	bool GetThrow()				{ return m_bThrow; }
	D3DXVECTOR3 GetThrowDir()	{ return m_ThrowDir; }

private:
	CShadow			*m_pShadow;			// シャドウクラス
	CControl		*m_pControl;		// コントロールクラス
	CModel			*m_pModel;			// モデルクラス(爆弾のモデル)
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_scale;			// スケール
	D3DXVECTOR3		m_ThrowDir;			// 爆弾の飛ぶ向き
	D3DXCOLOR		m_Emissive;			// 発光色
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	int				m_nBombTime;		// 爆発までのタイム
	bool			m_bIgnition;		// 着火したか
	bool			m_bShrink;			// 縮むか
	bool			m_bBlink;			// 点滅するか
	bool			m_bThrow;			// 投げられている状態か
	float			m_fOriginScaleX;	// 生成時のスケールX(元のスケール)を保存する変数
	float			m_fSphereSize;		// 球サイズ
	float			m_fSphereRot;		// 球回転
	float			m_fSphereAlpha;		// 球透明度
};

#endif