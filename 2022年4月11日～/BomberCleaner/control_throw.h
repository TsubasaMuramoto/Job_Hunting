//=============================================================================
// コントロール処理 [control_throw.h]
//=============================================================================
#ifndef _CONTROL_THROW_H_
#define _CONTROL_THROW_H_
#include "main.h"
#include "control.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene;

//*****************************************************************************
// コントロールクラス
//*****************************************************************************
class CControlThrow : public CControl
{
public:
	CControlThrow();							// コンストラクタ
	~CControlThrow();						// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(CScene *pObject);			// 更新処理
	static CControlThrow *Create();			// 生成処理

private:
	float m_fGravity;
	bool m_bDoOnce;							// 一度のみ通る処理
};

#endif