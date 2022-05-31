//=============================================================================
// コントロール処理 [control_fade.cpp]
// Author : ムラモトツバサ
//=============================================================================
#include "control_fade.h"
#include "billboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CControlFade::CControlFade()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CControlFade::~CControlFade()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlFade::Init()
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlFade::Uninit()
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CControlFade::Update(CScene *pObject)
{
	// インスタンス
	CBillboard *pBillboard = nullptr;
	if (!pBillboard)
	{
		// オブジェクトをBillboardクラスにキャスト
		pBillboard = (CBillboard*)pObject;

		if(pBillboard)
		{
			// 徐々に消える処理
			D3DXCOLOR alpha = pBillboard->GetCol();
			D3DXVECTOR3 pos = pBillboard->GetPos();
			D3DXVECTOR3 size = pBillboard->GetSize();

			// 透明度のフェードアウト
			if (alpha.a > 0.0f)
			{
				alpha.a -= 0.01f;
				pBillboard->SetColVal(alpha);
			}

			// サイズの縮小
			if(size.x > 0.0f && size.y > 0.0f)
			{ 
				size.x -= 0.1f;
				size.y -= 0.1f;
				pBillboard->SetPos(pos,size);
			}

			// ビルボードの終了
			if(alpha.a <= 0.0f || (size.x <= 0.0f && size.y <= 0.0f))
			{
				pBillboard->Uninit();
			}
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CControlFade *CControlFade::Create()
{
	// インスタンスの生成
	CControlFade *pControlFade = nullptr;
	if (!pControlFade)
	{
		pControlFade = new CControlFade();
		if (pControlFade)
		{
			pControlFade->Init();	// 初期化
		}
	}
	return pControlFade;
}