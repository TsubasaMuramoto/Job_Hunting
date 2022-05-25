//=============================================================================
// コントロール処理 [control_spread.cpp]
// Author : ムラモトツバサ
//=============================================================================
#include "control_spread.h"
#include "effect.h"
#include <random>

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_SPEED (5.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
CControlSpread::CControlSpread()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CControlSpread::~CControlSpread()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CControlSpread::Init()
{
	// 噴射エフェクトの角度の最小値と最大値(0～360)
	const float minRange = 0.0f;
	const float maxRange = D3DX_PI * 2;
	float fAngle;

	std::random_device Randseed;												// 乱数生成器でシード値を完全ランダムに初期化する
	std::mt19937_64 mt(Randseed());												// 疑似乱数を作り出す
	std::uniform_real_distribution<float> Dist(minRange, maxRange);				// 指定した範囲内の数値を等確率で返す(最小値以上、最大値"未満")
	fAngle = Dist(mt);

#ifdef _DEBUG
	float fDegree = D3DXToDegree(fAngle);										// デバッグ時に角度を確認するための処理
#endif

	m_move.x = cosf(fAngle); //* sinf(fAngle);
	//m_move.y =				  cosf(fAngle);
	m_move.z = sinf(fAngle); //* sinf(fAngle);

	m_move *= MOVE_SPEED;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CControlSpread::Uninit()
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CControlSpread::Update(CScene *pObject)
{
	// インスタンス
	CEffect *pEffect = nullptr;
	if (!pEffect)
	{
		// オブジェクトをエフェクトクラスにキャスト
		pEffect = (CEffect*)pObject;
		if (pEffect)
		{
			// 上にふわふわ上昇する処理
			D3DXVECTOR3 pos = pEffect->GetPos();
			pos += m_move;		// 上昇していく
			pEffect->SetPos(pos);
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CControlSpread *CControlSpread::Create()
{
	// インスタンスの生成
	CControlSpread *pControlSpread = nullptr;
	if (!pControlSpread)
	{
		pControlSpread = new CControlSpread();
		if (pControlSpread)
		{
			pControlSpread->Init();	// 初期化
		}
	}
	return pControlSpread;
}

