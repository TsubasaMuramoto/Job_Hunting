//===========================================
// 爆弾スポナーの作成
// Author : 村元翼
//===========================================
#ifndef _BOMBSPAWNER_H_
#define _BOMBSPAWNER_H_
#include "main.h"
#include "scene.h"

//===========================================
// マクロ定義
//===========================================

//===========================================
// 前方宣言
//===========================================
class CModel;
class CBomb;

//===========================================
// クラス定義
//===========================================
class CBombSpawner : public CScene
{
public:
	//---------------------------------
	// コンストラクタ・デストラクタ
	//---------------------------------
	CBombSpawner(OBJTYPE nPriority = OBJTYPE_MODEL);
	~CBombSpawner();

	//---------------------------------
	// メソッド
	//---------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	static CBombSpawner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);	// オブジェクト生成

private:
	CModel			*m_pModel;			// モデルクラス
	CBomb			*m_pBomb;			// 爆弾クラス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_scale;			// スケール
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	int				m_nSpawnTime;		// スポーンタイム
	bool			m_bIsThere;			// そこに存在しているか
};
#endif