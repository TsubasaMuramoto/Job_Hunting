//==================================
// 影の作成
// Author:村元翼
//==================================
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"
#include "scene3D.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define GRAVITY (1000.0f)

//------------------------------------------------------------------------------
// クラスの定義
//------------------------------------------------------------------------------
class CShadow : public CScene3D
{
public:
	CShadow(OBJTYPE nPriority = OBJTYPE_SHADOW);									// コンストラクタ
	~CShadow();																		// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);								// 初期化
	void Uninit(void);																// 終了
	void Update(void);																// 更新
	void Draw(void);																// 描画
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);		// 生成

	void SetGround(const float &X, const float &Z)	{ m_pos.x = X, m_pos.z = Z; }	// 位置設定
	void SetHeight(const float &fHeight)			{ m_pos.y = fHeight; }			// 高さ設定
	void SetGravity(float GravitySpeed);											// 重力設定

private:
	D3DXVECTOR3 m_OriginSize;														// 生成時のサイズ
};


#endif	