//=============================================================================
//テクスチャ処理
//Author:加藤瑞葵
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//================================================
//静的メンバ変数宣言
//================================================
std::vector<LPDIRECT3DTEXTURE9> CTexture::m_apTexture = {};
std::vector<std::string> CTexture::m_aPas;
map<string, int> CTexture::m_texType;
int CTexture::m_nNumTex = 0;

//=============================================================================
//コンストラクタ
//=============================================================================
CTexture::CTexture()
{
	m_texType.clear();
}

//=============================================================================
//デストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
//テクスチャの生成
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスを取得する

	// textファイル読み込み
	FILE *pFile = fopen("data/TEXTURE/SCRIPT/texture.txt", "r");
	if (pFile != NULL)
	{
		char cString[128];
		// 一行ずつ保存
		while (fgets(cString, 128, pFile) != NULL)
		{
			//文字列を保存
			fscanf(pFile, "%s", cString);
			//文字列の中にTEX_NUMがあったら
			if (strncmp("TEX_NUM", cString, 8) == 0)
			{
				//テクスチャ最大数読み込み
				fscanf(pFile, "%*s%d", &m_nNumTex);

				int nNum = 0;
				//一行ずつ保存
				while (fgets(cString, 128, pFile) != NULL)
				{
					//文字列を保存
					fscanf(pFile, "%s", cString);
					//文字列の中にPASがあったら
					if (strncmp("PAS", cString, 4) == 0)
					{
						//パスの取得
						char cString[128];
						fscanf(pFile, "%*s%s", &cString[0]);
						m_aPas.push_back(&cString[0]);

						//名前の取得
						char cName[1][128];
						fscanf(pFile, "%*s%*s%s", cName[0]);
						//名前と数の割り当て
						m_texType[cName[0]] = nNum;
						nNum++;
					}
				}
			}
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}
	fclose(pFile);

	for (int nCntTex = 0; nCntTex < m_nNumTex; nCntTex++)
	{
		LPDIRECT3DTEXTURE9 pTexBuffer = nullptr;
		//テクスチャの生成
		D3DXCreateTextureFromFile
		(
			pDevice,
			m_aPas[nCntTex].c_str(),
			&pTexBuffer
		);

		m_apTexture.push_back(pTexBuffer);
	}
}

//=============================================================================
//終了
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNumTex; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt])
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//=============================================================================
//テクスチャ割り当て
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(std::string texType)
{
	return m_apTexture[m_texType[texType]];
}