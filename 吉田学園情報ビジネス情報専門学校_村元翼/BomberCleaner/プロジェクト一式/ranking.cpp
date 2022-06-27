//================================================
// ランキング処理
// Author:村元翼
//================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "renderer.h"
#include "score.h"
#include "manager.h"
#include "ranking.h"
#include "playdata.h"
#include "number.h"

//================================================
// 静的メンバ変数宣言
//================================================
CScore *CRanking::m_apScore[MAX_RANKING] = {};

//================================================
// コンストラクタ
//================================================
CRanking::CRanking()
{
	// 各種変数初期化
	for (int nCnt = 0; nCnt < MAX_SCORE_DATA; nCnt++)
	{
		m_nScore[nCnt] = 0;				// スコアの初期化
	}
	m_nNowScore = 0;					// 現在のスコア
	m_col = { 0.0f,0.0f,0.0f,0.0f };	// カラー

}

//================================================
// デストラクタ
//================================================
CRanking::~CRanking()
{

}

//================================================
// 初期化処理
//================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// スコアの生成
		m_apScore[nCnt] = CScore::Create
		(
			D3DXVECTOR3
			(
				pos.x,
				pos.y + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)) * nCnt + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)),
				0.0f
			),
			size
		);

		// ランク生成
		m_apScene2D[nCnt] = CScene2D::Create
		(
			D3DXVECTOR3
			(
				RANKING_RANK_POLYGON_POS_X,
				pos.y + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)) * nCnt + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)),
				0.0f
			),
			RANKING_RANK_POLYGON_SIZE
		);

		// 順位テクスチャ
		switch (nCnt)
		{
		case 0:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_1ST"));
			break;
		case 1:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_2ND"));
			break;
		case 2:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_3RD"));
			break;
		case 3:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_4TH"));
			break;
		case 4:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_5TH"));
			break;
		}
	}

	// ロード処理
	FILE *pFile;
	if (pFile = fopen(RANKING_TEXT, "r"))	// ファイルを開く
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fscanf(pFile, "%d", &m_nScore[nCnt]);
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}
	fclose(pFile);	// ファイルを閉じる

	// プレイデータの取得
	CPlayData *pPlayData = nullptr;
	pPlayData = CManager::GetInstance()->GetPlayData();

	// プレイデータに保存してある今回のスコアを変数に保存
	m_nScore[MAX_SCORE_DATA - 1] = pPlayData->GetScore();
	m_nNowScore = pPlayData->GetScore();

	// ソート
	for (int nCnt = 0; nCnt < MAX_SCORE_DATA; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < MAX_SCORE_DATA - 1; nCnt2++)
		{
			// 昇順で並べる
			if ((m_nScore[nCnt2] < m_nScore[nCnt2 + 1] && m_nScore[nCnt2 + 1] != 0) || m_nScore[nCnt2] == 0)
			{
				int nScore = m_nScore[nCnt2];
				m_nScore[nCnt2] = m_nScore[nCnt2 + 1];
				m_nScore[nCnt2 + 1] = nScore;
			}
		}
	}

	// スコアの設定
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_apScore[nCnt]->SetScore(m_nScore[nCnt]);
	}

	// セーブ処理
	if (pFile = fopen(RANKING_TEXT, "w"))	// ファイルを開く
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fprintf(pFile, "%d\n", m_apScore[nCnt]->GetScore());
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}

	fclose(pFile);	// ファイルを閉じる

	//カラーの設定
	m_col = { 255.0f ,0.0f,0.0f,255.0f };

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CRanking::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// シーン2Dの破棄
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = nullptr;
		}

		// スコアの破棄
		if (m_apScore[nCnt])
		{
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = nullptr;
		}
	}

	// オブジェクトの破棄
	Release();
}

//================================================
// 更新処理
//================================================
void CRanking::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// スコアの位置取得処理
		D3DXVECTOR3 posScore = m_apScore[nCnt]->GetPos();

		if (posScore.x > SCREEN_WIDTH / 2.0f)
		{
			if (nCnt > 0)
			{
				// スコアの位置取得処理
				D3DXVECTOR3 posScorePrev = m_apScore[nCnt - 1]->GetPos();
				if (posScorePrev.x <= SCREEN_WIDTH / 2.0f)
				{
					posScore.x += RANKING_MOVE_X;
				}
			}
			else
			{
				posScore.x += RANKING_MOVE_X;
			}
			if (posScore.x <= SCREEN_WIDTH / 2.0f)
			{
				posScore.x = SCREEN_WIDTH / 2.0f;
			}

			// 位置設定
			//m_apScore[nCnt]->SetScorePos(posScore);
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_nNowScore != 0 && m_nScore[nCnt] == m_nNowScore)
		{
			//----------------------------------
			// 点滅処理
			//----------------------------------
			m_col.a -= 255;
			if (m_col.r == 255)
			{
				m_col.r = 0;
				m_col.g = 255;
				m_col.b = 0;
			}
			else if (m_col.g == 255)
			{
				m_col.r = 0;
				m_col.g = 0;
				m_col.b = 255;
			}
			else if (m_col.b == 255)
			{
				m_col.r = 255;
				m_col.g = 0;
				m_col.b = 0;
			}

			if (m_col.a < 0)
			{
				m_col.a = 255;
			}

			for (int nCntNum = 0; nCntNum < MAX_SCORE; nCntNum++)
			{
				// ナンバーを取得してカラーを設定
				m_apScore[nCnt]->GetNumber(nCntNum)->SetCol(m_col);
			}
			break;
		}
	}
}

//================================================
// 描画処理
//================================================
void CRanking::Draw(void)
{

}

//================================================
// 生成処理
//================================================
CRanking* CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンスの生成
	CRanking *pRanking = nullptr;
	if (!pRanking)
	{
		pRanking = new CRanking;
		if (pRanking)
		{
			pRanking->Init(pos, size);	// 初期化
		}
	}
	return pRanking;
}

//================================================
// ランキング削除
//================================================
void CRanking::DeleteRanking(void)
{
	// セーブ処理
	FILE *pFile;
	if (pFile = fopen(RANKING_TEXT, "w"))	// ファイルを開く
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			m_apScore[nCnt]->SetScore(0);							// スコアを0にする
			fprintf(pFile, "%d\n", m_apScore[nCnt]->GetScore());	// 0にしたスコアを保存
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}

	fclose(pFile);		// ファイルを閉じる
}