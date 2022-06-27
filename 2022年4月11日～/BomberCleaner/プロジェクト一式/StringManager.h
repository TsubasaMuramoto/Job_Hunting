//=============================================================================
//
// 文字列管理のクラス [StringManager.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _STRINGMANAGER_H_
#define _STRINGMANAGER_H_

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFont;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStringManager
{
public:
	CStringManager();		// コンストラクタ
	~CStringManager();		// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSpace, int nfontSize, const char* str, const char* font);		// 初期化
	void Uninit(void);																					// 終了
	void Update(void);																					// 更新

	static CStringManager *Create(D3DXVECTOR3 pos,float fSpace, int nfontSize, const char* str, const char* font = "ＭＳ 明朝");	// 生成関数

private:
	vector<CFont*> m_vFont;		// フォントクラスの動的配列
};
#endif

