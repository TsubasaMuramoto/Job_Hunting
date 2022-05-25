//======================================================
// Xファイルの読み込み
// Author:村元翼
//======================================================
#ifndef _LOADX_H_
#define _LOADX_H_
#include "main.h"

//======================================================
// クラス生成
//======================================================
class CLoadX
{
public:
	typedef struct
	{
		LPD3DXMESH					m_pMesh;		// メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER				m_pBuffMat;		// マテリアル（材質情報）へのポインタ
		DWORD						m_nNumMat;		// マテリアルの数
		D3DXMATERIAL				*m_pMat;		// マテリアルへのポインタ
		vector<LPDIRECT3DTEXTURE9>	m_pTex;			// テクスチャ
		string						m_XFilePas;		// ファイルパス

	}XData;

	//-------------------------------
	// コンストラクタ・デストラクタ
	//-------------------------------
	CLoadX();
	~CLoadX();

	//-------------------------------
	// メソッド
	//-------------------------------
	HRESULT Init(void);
	void Uninit(void);

	//-------------------------------
	// Getter
	//-------------------------------
	static XData GetXData(string xName);
	static XData GetXData(int nNum)	{ return m_vDataX[nNum]; }	// Xファイルデータ取得
	static int GetNumAll()			{ return m_nNum; }			// Xファイル総数取得
	static int GetNum(string xName)	{ return m_NameX[xName]; }	// Xファイル指定番号取得

private:
	static vector<XData>		m_vDataX;		// Xファイルデータ構造体
	static map<string, int>		m_NameX;		// Xファイルの種類
	static int m_nNum;
};
#endif _LOADX_H_
