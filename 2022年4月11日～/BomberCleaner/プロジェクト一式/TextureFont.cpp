//=============================================================================
//
// 数字の処理 [TextureFont.cpp]
// Author : 村元翼
//
//=============================================================================
#include "TextureFont.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFont::CFont(OBJTYPE nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CFont::~CFont()
{
}

//=============================================================================
//　数字の生成
//=============================================================================
CFont *CFont::Create(D3DXVECTOR3 pos, int nfontSize, const char* str, const char* font)
{
	// インスタンス生成
	CFont *pFont = new CFont;

	// 数字の準備
	if (pFont)
	{
		pFont->m_font = font;					// フォント名
		pFont->m_str = str;						// 文字
		pFont->m_pos = pos;						// 位置
		pFont->m_nFontSize = nfontSize;			// フォントサイズ
		pFont->Init(pos, { 0.0f,0.0f,0.0f });	// 初期化
	}

	return pFont;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFont::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// シーン2D初期化
	CScene2D::Init(pos, size);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//------------------------------------
	// フォント情報の作成
	//------------------------------------
	HFONT hFont = CreateFont
	(
		m_nFontSize,				// 文字の高さ(フォントサイズ)
		0,							// 文字の横幅
		0,							// 文字送りの方向と角度指定(10分の1角度)
		0,							// 各文字の回転角度
		0,							// 文字の太さ(FW_BOLDで太字になる)
		FALSE,						// イタリック体(斜体)にするか
		TRUE,						// 下線をつけるか
		FALSE,						// 
		SHIFTJIS_CHARSET,			// キャラクタセット
		OUT_TT_ONLY_PRECIS,			// 出力精度
		CLIP_DEFAULT_PRECIS,		// クリッピング精度
		PROOF_QUALITY,				// 出力品質
		FIXED_PITCH | FF_MODERN,	// フォントのピッチ・ファミリ
		m_font						// フォント名
	);

	//------------------------------------
	// デバイスコンテキスト取得
	//------------------------------------
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(CManager::GetInstance()->GethWnd());
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	//------------------------------------
	// 文字コード取得
	//------------------------------------
	UINT code = 0;
	if (IsDBCSLeadByte(*m_str))
	{
		code = (BYTE)m_str[0] << 8 | (BYTE)m_str[1];
	}
	else
	{
		code = m_str[0];
	}

	//------------------------------------
	// ビットマップ取得
	//------------------------------------
	const MAT2 Mat2 = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	GLYPHMETRICS GM;
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);

	DWORD BitMap;	// フォントのビットマップ情報を格納する変数
	BitMap = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat2);

	BYTE *ptr = new BYTE[BitMap];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, BitMap, ptr, &Mat2);

	// デバイスコンテキストとフォントハンドルの解放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	//------------------------------------
	// ビットマップ取得終了
	//------------------------------------
	// テクスチャ作成
	pDevice->CreateTexture
	(
		GM.gmCellIncX,
		TM.tmHeight,
		1, 
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture,
		NULL
	);
	
	// フォントのビットマップの幅と高さ
	int iBmp_w, iBmp_h;
	iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	iBmp_h = GM.gmBlackBoxY;

	// 余白を含むフォント領域
	int iFnt_w, iFnt_h;
	iFnt_w = GM.gmCellIncX;
	iFnt_h = TM.tmHeight;

	// フォントの書き出し位置(左上)
	int iOfs_x, iOfs_y;
	iOfs_x = GM.gmptGlyphOrigin.x;
	iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;

	// α値の段階
	int level = 17;
	DWORD Alpha, Color;

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// バッファの生成
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - iFnt_w, m_pos.y + iFnt_h,	0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - iFnt_w, m_pos.y,			0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x,			m_pos.y + iFnt_h,	0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x,			m_pos.y,			0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	m_pVtxBuff->Unlock();

	// テクスチャサーフェイスをロック
	D3DLOCKED_RECT LockRect;
	m_pTexture->LockRect(0, &LockRect, NULL, D3DLOCK_DISCARD);

	// テクスチャサーフェイスの初期化
	FillMemory(LockRect.pBits, LockRect.Pitch * iFnt_h, 0);

	// フォント情報の書き込み
	for (int y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (int x = iOfs_x; x < iOfs_x + iBmp_w; x++)
		{
			Alpha = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy((BYTE*)LockRect.pBits + LockRect.Pitch*y + 4 * x, &Color, sizeof(DWORD));
		}
	}

	// テクスチャサーフェイスアンロック
	m_pTexture->UnlockRect(0);
	delete[] ptr;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFont::Uninit(void)
{
	// シーン2D終了
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFont::Update(void)
{
	// シーン2D更新
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CFont::Draw(void)
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);

	// テクスチャセット
	pDevice->SetTexture(0, m_pTexture);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); // 板ポリのα値を利用

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,																// 描画を開始する最初の頂点インデックス
		2																// ポリゴンの数
	);

	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);

}