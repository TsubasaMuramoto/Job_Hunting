//================================================================
// ライトの作成
// Author:村元翼
//================================================================
#ifndef _LIGHT_H
#define _LIGHT_H

//================================================================
// インクルードファイル
//================================================================
#include "main.h"
#include "scene.h"

//================================================================
// クラスの定義
//================================================================
class CLight
{
public:
	CLight();																// コンストラクタ
	~CLight();																// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXVECTOR3 vecDir);	// 初期化

private:
	D3DLIGHT9 m_light;				// ライトの情報
	static int m_nCntLight;			// 生成数のカウント
	int m_nId;						// 生成数
};
#endif // !_LIGHT_H
