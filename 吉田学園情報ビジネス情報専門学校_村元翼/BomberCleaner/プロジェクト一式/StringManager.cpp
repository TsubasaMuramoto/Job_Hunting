//=============================================================================
//
// 文字列管理のクラス [StringManager.cpp]
// Author : 村元翼
//
//=============================================================================
#include "StringManager.h"
#include "TextureFont.h"
#include <stdlib.h>
#include <mbstring.h>

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CStringManager::CStringManager()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CStringManager::~CStringManager()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CStringManager::Init(D3DXVECTOR3 pos, float fSpace, int nfontSize, const char* str, const char* font)
{
	if (m_vFont.empty())
	{
		CFont *pFont = nullptr;

		// 2バイト文字はnCntを2個進めるので、次の文字までのfSpaceが2個分となるので、空白が1文字分できてしまうのを埋める変数を作る
		int nFill = 0;
		int nLen = strlen(str);
		for (int nCnt = 0; nCnt < nLen; nCnt++)
		{
			// フォント生成
			pFont = CFont::Create({ pos.x + (fSpace*(nCnt - nFill)),pos.y,pos.z }, nfontSize, &str[nCnt], font);
			m_vFont.emplace_back(pFont);
	
			// 2バイト文字(全角文字)か判定を行う
			if (_mbclen((BYTE*)&str[nCnt]) == 2)
			{
				nCnt++;		// 配列を2つ進める
				nFill++;	// 空いた隙間を埋めるための変数
			}
		}
	}

	else
	{
		return S_FALSE;
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CStringManager::Uninit()
{
	// 空判定
	if (!m_vFont.empty())
	{
		// コンテナに格納されているフォントクラスを全て破棄
		for (CFont *pDeleteFont : m_vFont)
		{
			if (pDeleteFont)
			{
				pDeleteFont->Uninit();
				pDeleteFont = nullptr;
			}
		}

		// フォントクラスコンテナを破棄
		m_vFont.clear();
	}
}

//=============================================================================
// 更新
//=============================================================================
void CStringManager::Update()
{

}

//=============================================================================
// 生成
//=============================================================================
CStringManager *CStringManager::Create(D3DXVECTOR3 pos, float fSpace, int nfontSize, const char* str, const char* font)
{
	// インスタンス生成
	CStringManager *pStrMng = nullptr;
	if (!pStrMng)
	{
		pStrMng = new CStringManager;
		if (pStrMng)
		{
			// 初期化
			pStrMng->Init(pos, fSpace, nfontSize, str, font);
		}
	}

	return pStrMng;
}