//=============================================================================
//
// 数字表示のクラス [number.h]
// Author : 土居大輝
//
//=============================================================================
// プリプロセッサ
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "manager.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CNumber : public CScene2D
{
public:
	CNumber(OBJTYPE nPriority = OBJTYPE_NUMBER);	// コンストラクタ
	~CNumber();										// デストラクタ

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	void SetColor(D3DXCOLOR col);					// 色の設定
	void SetNumber(int nNumber);					// 数字設定
	//void SetPos(D3DXVECTOR3 pos);					// 位置設定

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// バッファ
	D3DXCOLOR					m_col;				// 色
};

#endif // !_NUMBER_H_
