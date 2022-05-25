//========================================================================
// scene.cppの作成
// Author:村元翼
//========================================================================
#include "scene.h"
#include "renderer.h"
#include "manager.h"

//========================================================================
// 静的メンバ変数の初期化
//========================================================================
CScene *CScene::m_pCur[OBJTYPE_MAX] = {};		// 最後のオブジェクト
CScene *CScene::m_pTop[OBJTYPE_MAX] = {};		// 最初のオブジェクト
int CScene::m_nNumAll[OBJTYPE_MAX] = {};		// 各種オブジェクト数

//========================================================================
// コンストラクタ
//========================================================================
CScene::CScene()
{

}

//========================================================================
// オーバーロードコンストラクタ
//========================================================================
CScene::CScene(OBJTYPE nPriority)
{
	// 1週目
	if (!m_pTop[nPriority] && !m_pCur[nPriority])
	{
		m_pTop[nPriority] = this;	// thisは最初のオブジェクトである
		m_pCur[nPriority] = this;	// thisは最後のオブジェクトである
		m_pNext = nullptr;
		m_pPrev = nullptr;
	}

	// 2週目以降
	else
	{
		m_pCur[nPriority]->m_pNext = this;		// 現在のオブジェクト(A)から見た次のオブジェクト(B)は自分である
		this->m_pPrev = m_pCur[nPriority];		// 自分の一個前(BからみたA)はm_pCurである
		m_pCur[nPriority] = this;				// 現在のAはBである
		this->m_pNext = nullptr;				// 次は無し
	}

	m_nNumAll[nPriority]++;						// 数をカウント
	m_nPriority = nPriority;					// 優先順位の値を代入
	m_objType = nPriority;						// オブジェクトタイプの代入

	//------------------------------------------
	// 変数初期化
	//------------------------------------------
	m_pos		= { 0.0f,0.0f,0.0f };
	m_posOld	= { 0.0f,0.0f,0.0f };
	m_size		= { 0.0f,0.0f,0.0f };
	m_col		= { 0.0f,0.0f,0.0f,0.0f };
	m_Tex		= { 0.0f,0.0f };
	m_bDeath	= false;

}

//========================================================================
// デストラクタ(個別のオブジェクトの破棄)
//========================================================================
CScene::~CScene()
{
	// 消されるオブジェクトが最初かつ最後のオブジェクトだったら(オブジェクトが1個のみだったら)
	if (this == m_pTop[m_nPriority] && this == m_pCur[m_nPriority])
	{
		// 全て空にする
		m_pTop[m_nPriority] = nullptr;
		m_pCur[m_nPriority] = nullptr;
		m_pNext = nullptr;
		m_pPrev = nullptr;
	}

	else if (this == m_pTop[m_nPriority])
	{
		m_pNext->m_pPrev = nullptr;		// 次のオブジェクトが持つ前のオブジェクト情報を空にする
		m_pTop[m_nPriority] = m_pNext;	// 最初のオブジェクト情報を次のオブジェクトにずらす

	}
	else if (this == m_pCur[m_nPriority])
	{
		m_pPrev->m_pNext = nullptr;
		m_pCur[m_nPriority] = m_pPrev;

	}
	else
	{
		
		m_pPrev->m_pNext = m_pNext;	// 自分の前のオブジェクトが持っている次のオブジェクト情報を自分の次のオブジェクト情報にする
		m_pNext->m_pPrev = m_pPrev;	// 自分の次のオブジェクトが持っている前のオブジェクト情報を自分の前のオブジェクト情報にする
	}

	// オブジェクト数のカウントを減らす
	m_nNumAll[m_nPriority]--;
}


//========================================================================
// 全てのオブジェクト破棄
//========================================================================
void CScene::ReleaseAll()
{	
	CScene *pScene = nullptr;		// 消すオブジェクト
	CScene *pSaveScene = nullptr;	// 保持用オブジェクト

	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		// フェードのみ全モードで使うので消さない
		if (nObj != OBJTYPE_FADE)
		{
			pScene = m_pTop[nObj];

			// 最後のオブジェクトの次のNextがNULLでなかったら
			// 無限ループ
			while (pScene)
			{
				// トップの次のオブジェクトを格納
				pSaveScene = pScene->m_pNext;

				// トップの情報を消す
				pScene->Uninit();
				if (pScene->m_bDeath)
				{
					delete pScene;
					pScene = nullptr;
				}

				//トップの情報を次のオブジェクトにする
				pScene = pSaveScene;
			}
		}
	}
}

//========================================================================
// 全てのオブジェクト更新
//========================================================================
void CScene::UpdateAll()
{
	CScene *pNextUpdate = nullptr;									// 更新処理用オブジェクト

		for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
		{
			if (!CManager::GetInstance()->GetStop() && !CManager::GetInstance()->GetPause())					// ポーズ状態がfalseなら
			{
				if (m_pTop[nObj])
				{
					pNextUpdate = m_pTop[nObj];						// 次に更新するオブジェクトを保存する変数

					while (pNextUpdate)
					{
						pNextUpdate->Update();
						pNextUpdate = pNextUpdate->m_pNext;
					}
				}
			}

			// ポーズ中に更新するオブジェクト
			else
			{
				if (nObj == OBJTYPE_PAUSE || nObj == OBJTYPE_UI || nObj == OBJTYPE_FADE)	// オブジェクトがフェードorUIorポーズなら
				{
					if (m_pTop[nObj])
					{
						CScene *m_pUpdate = m_pTop[nObj];					// 次に更新するオブジェクトの格納

						while (m_pUpdate)									// m_pUpdateにNULLが入るまで
						{
							if (!m_pUpdate->m_bDeath)
							{
								m_pUpdate->Update();
							}

							m_pUpdate = m_pUpdate->m_pNext;
						}
					}
				}
			}
		}

	CScene *pSave = nullptr;	//	保存用オブジェクト

		// オブジェクトが死亡しているかの判定
		for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
		{
			if (m_pTop[nObj])
			{
				pNextUpdate = m_pTop[nObj];

				while (pNextUpdate)
				{
					// 次のデータを保存
					pSave = pNextUpdate->m_pNext;

					// 死亡判定
					if (pNextUpdate->m_bDeath)
					{
						delete pNextUpdate;
						pNextUpdate = nullptr;
					}

					// 次のオブジェクトにする
					pNextUpdate = pSave;
				}
			}
		}
}

//========================================================================
// 全ての描画
//========================================================================
void CScene::DrawAll()
{
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		if (m_pTop[nObj])
		{
			// 次に描画するオブジェクトの格納
			CScene *pNextDraw = m_pTop[nObj];

			while (pNextDraw)
			{
				if (!pNextDraw->m_bDeath)
				{
					pNextDraw->Draw();
				}
				pNextDraw = pNextDraw->m_pNext;
			}
		}
	}
}

//========================================================================
// 自身の破棄
//========================================================================
void CScene::Release()
{
	// 死亡フラグをtrueにする
	m_bDeath = true;
}