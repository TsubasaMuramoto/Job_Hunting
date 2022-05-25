//======================================================
// Xファイルの読み込み
// Author:村元翼
//======================================================
#include "LoadX.h"
#include "manager.h"
#include "renderer.h"

//======================================================
// 静的メンバ変数の初期化
//======================================================
vector<CLoadX::XData>	CLoadX::m_vDataX;
map<string, int>		CLoadX::m_NameX;
int						CLoadX::m_nNum = 0;

//======================================================
// コンストラクタ
//======================================================
CLoadX::CLoadX()
{

}

//======================================================
// デストラクタ
//======================================================
CLoadX::~CLoadX()
{

}

//======================================================
// 初期化
//======================================================
HRESULT CLoadX::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	FILE *pFile = nullptr;
	if (pFile = fopen("data/TEXT/ModelLoad000.txt", "r"))	// ファイルを開く
	{
		char cStr[128];
		while (fgets(cStr, 128, pFile))					// 一行ずつ読み込む
		{
			fscanf(pFile, "%s", cStr);					// 文字列を保存
			if(strncmp(cStr, "NUM_MODEL",10) == 0)
			{
				fscanf(pFile, "%*s%d", &m_nNum);		// モデル数を保存
				m_vDataX.resize(m_nNum);				// 配列(要素)数の確定

				int nCnt = 0;							// カウント用変数
				while (fgets(cStr, 128, pFile))			// 一行ずつ読み込む
				{
					fscanf(pFile, "%s", cStr);			// 文字列を保存
					if (strcmp(cStr, "PAS") == 0)
					{
						char aPas[64];
						fscanf(pFile, "%*s%s", &aPas[0]);	// ファイルパス取得
						m_vDataX[nCnt].m_XFilePas += aPas;	// ファイルパス格納
					}

					else if (strcmp(cStr, "NAME") == 0)
					{
						char aName[64];
						fscanf(pFile, "%*s%s", &aName[0]);			// ファイルネーム取得
						m_NameX[&aName[0]] = nCnt;					// ファイルネームと番号を結びつける
						nCnt++;
					}

					// 全てのモデルを読み込む
					else if (strcmp(cStr, "END_SCRIPT") == 0)
					{
						// Xファイルの読み込み
						for (int nMod = 0; nMod < m_nNum; nMod++)
						{
							D3DXLoadMeshFromX
							(
								m_vDataX[nMod].m_XFilePas.c_str(),
								D3DXMESH_SYSTEMMEM, pDevice,
								NULL,
								&m_vDataX[nMod].m_pBuffMat,
								NULL,
								&m_vDataX[nMod].m_nNumMat,
								&m_vDataX[nMod].m_pMesh
							);

							// マテリアル情報に対するポインタを取得
							m_vDataX[nMod].m_pMat = (D3DXMATERIAL*)m_vDataX[nMod].m_pBuffMat->GetBufferPointer();

							// マテリアル読み込み
							for (int nMat = 0; nMat < (int)m_vDataX[nMod].m_nNumMat; nMat++)
							{
								LPDIRECT3DTEXTURE9 pTexBuffer = nullptr;	// ローカルテクスチャ構造体

								if (m_vDataX[nMod].m_pMat[nMat].pTextureFilename)
								{

									// テクスチャの読み込み
									D3DXCreateTextureFromFile
									(
										pDevice,
										m_vDataX[nMod].m_pMat[nMat].pTextureFilename,
										&pTexBuffer
									);
								}
								m_vDataX[nMod].m_pTex.emplace_back(pTexBuffer);	// テクスチャデータの追加
							}
						}
					}
				}
			}
		}
	}
	else
	{
		printf("ファイル読み込みに失敗");
	}

	fclose(pFile);	// ファイルを閉じる

	return S_OK;
}

//======================================================
// 終了
//======================================================
void CLoadX::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNum; nCnt++)
	{
		// メッシュの破棄
		if (m_vDataX[nCnt].m_pMesh)
		{
			m_vDataX[nCnt].m_pMesh->Release();
			m_vDataX[nCnt].m_pMesh = nullptr;
		}

		// マテリアルの破棄
		if (m_vDataX[nCnt].m_pBuffMat)
		{
			m_vDataX[nCnt].m_pBuffMat->Release();
			m_vDataX[nCnt].m_pBuffMat = nullptr;
		}

		// テクスチャの破棄
		for (int nTex = 0; nTex < (int)m_vDataX[nCnt].m_nNumMat; nTex++)
		{
			if (m_vDataX[nCnt].m_pTex[nTex])
			{
				m_vDataX[nCnt].m_pTex[nTex]->Release();
				m_vDataX[nCnt].m_pTex[nTex] = nullptr;
			}
		}
	}
}

//======================================================
// Xファイルの取得
//======================================================
CLoadX::XData CLoadX::GetXData(string xType)
{
	// 文字列に合ったint型数値が配列に入る
	return m_vDataX[m_NameX[xType]];
}

