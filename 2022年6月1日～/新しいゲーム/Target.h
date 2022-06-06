//===========================================
// 破壊する対象の処理の作成
// Author : 村元翼
//===========================================
#ifndef _TARGET_H_
#define _TARGET_H_
#include "main.h"
#include "scene.h"

//===========================================
// マクロ定義
//===========================================

//===========================================
// 前方宣言
//===========================================
class CModel;

//===========================================
// クラス定義
//===========================================
class CTarget : public CScene
{
public:

	//---------------------------------
	// コンストラクタ・デストラクタ
	//---------------------------------
	CTarget(OBJTYPE nPriority = OBJTYPE_MODEL);
	~CTarget();

	//---------------------------------
	// メソッド
	//---------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	static CTarget *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);	// オブジェクト生成

private:
	CModel			*m_pModel;			// モデルクラス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_scale;			// スケール
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
};

#endif