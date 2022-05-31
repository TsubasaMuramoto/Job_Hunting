//=============================================================================
//テクスチャ処理
//Author:村元翼
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#include "main.h"

//=============================================================================
// クラス定義
//=============================================================================
class CTexture
{
public:
	CTexture();						//コンストラクタ
	~CTexture();					//デストラクタ

	static void Init(void);															// テクスチャの生成
	static void Uninit(void);														// 終了
	static LPDIRECT3DTEXTURE9 GetTexture(std::string texType);						// テクスチャの割り当て
	static LPDIRECT3DTEXTURE9 GetTexture(int nCnt) { return m_apTexture[nCnt]; }	// テクスチャの割り当て
	static std::string GetPas(int nCntTex) { return m_aPas[nCntTex]; }				// テクスチャのパス取得処理
	static int GetNum(void) { return m_nNumTex; }									// テクスチャの総数取得処理

private:
	static std::vector<LPDIRECT3DTEXTURE9> m_apTexture;		//テクスチャ
	static std::vector<std::string>	m_aPas;					//テクスチャのパス
	static map<string, int> m_texType;						//テクスチャの種類
	static int m_nNumTex;									//テクスチャの総数
};

#endif