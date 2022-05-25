//===========================================
// 爆風エフェクト(メッシュ球)の作成
// Author : 村元翼
//===========================================
#ifndef _EFFECT_BLAST_H_
#define _EFFECT_BLAST_H_
#include "main.h"
#include "scene.h"

//===========================================
// マクロ定義
//===========================================
#define EXPLOSION_SPHERE_SIZE	(D3DXVECTOR3(100.0f,100.0f,100.0f))	// 爆発球サイズ
#define BLAST_SCALE_MAX			(5)
//===========================================
// 前方宣言
//===========================================
class CMeshSphere;

//===========================================
// クラス定義
//===========================================
class CBlast : public CScene
{
public:

	//---------------------------------
	// コンストラクタ・デストラクタ
	//---------------------------------
	CBlast(OBJTYPE nPriority = OBJTYPE_EFFECTMESH);
	~CBlast();

	//---------------------------------
	// メソッド
	//---------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	static CBlast *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLine, int nVertical);	// オブジェクト生成

private:
	CMeshSphere		*m_pSphere;			// メッシュ球クラス(爆発エフェクト用)
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_size;				// サイズ
	int				m_nFrame;			// フレーム
	float			m_scale;			// スケール
	float			m_fScalingSpeed;	// スケール変更速度
	D3DXCOLOR		m_col;				// 色
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
};

#endif