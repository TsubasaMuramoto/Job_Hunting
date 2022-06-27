//--------------------------------------------------------------
// スコアの作成
// Author : 村元翼
//--------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_
#include "scene.h"

//--------------------------------------------------------------
// 前方宣言
//--------------------------------------------------------------
class CNumber;

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define MAX_SCORE (6)			// 最大描画枚数
#define ADD_SCORE (10)			// 加算スコア

//---------------------------------------------------------------
// スコアクラス(シーンの派生)
//---------------------------------------------------------------
class CScore : public CScene
{
public:
	CScore(OBJTYPE nPriority = OBJTYPE_NUMBER);			// コンストラクタ
	~CScore();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	static CScore	*Create(D3DXVECTOR3 pos, D3DXVECTOR3 fsize);			// スコアの生成
	void			AddScore(int AddScore) { m_nScore += AddScore; };		// スコアの加算

	//---------------------------------
	// 設定・取得関数
	//---------------------------------
	void			SetScore(void);											// スコアの描画設定
	void			SetScore(int nScore);									// スコアの設定
	void			SetScorePos(D3DXVECTOR3 pos);							// スコア位置設定
	CNumber			*GetNumber(int nNumber) { return m_apNumber[nNumber]; }	// ナンバー取得
	int				GetScore(void)			{ return m_nScore; }			// 現在スコア取得

private:
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3 m_size;												// サイズ
	CNumber *m_apNumber[MAX_SCORE];									// スコアのポリゴン
	int m_nAddScore;												// スコア加算用変数
	int m_nAll;														// スコアポリゴン数
	int m_nScore;													// 現在のスコア
};
#endif
