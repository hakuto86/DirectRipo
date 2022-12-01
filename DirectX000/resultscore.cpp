#include "main.h"
#include "score.h"
#include "enemy.h"
#include "resultscore.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;  //頂点バッファへのポインタ
D3DXVECTOR3 g_posResultScore;   //スコアの位置
int g_nResultScore;  //スコアの値
int g_aRankingScore[NUM_RANKING] = { 100,200,300,400,500 };  //ランキング

//==================================================================
//スコアの初期化の処理
//==================================================================
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	   //デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureResultScore);

	g_posResultScore = D3DXVECTOR3(1100, 50, 0.0f);  //位置を初期化する
	g_nResultScore = 0;  //値を初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE * NUM_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPlace;
	int nCntRnking;

	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(400.0f, 520.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(500.0f, 520.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 540.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(500.0f, 540.0f, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//==================================================================
//スコアの終了処理
//==================================================================
void UninitResultScore(void)
{
	int nCntRnking;

	//テクスチャの破棄
	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		if (g_pTextureResultScore != NULL)
		{
			g_pTextureResultScore->Release();
			g_pTextureResultScore = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//==================================================================
//スコアの更新処理
//==================================================================
void UpdateResultScore(void)
{
	int nCntPlace;
	int nCntRnking;

	int aTexU[NUM_PLACE];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		aTexU[0] = g_aRankingScore[nCntRnking] % 100000000 / 10000000;
		aTexU[1] = g_aRankingScore[nCntRnking] % 10000000 / 1000000;
		aTexU[2] = g_aRankingScore[nCntRnking] % 1000000 / 100000;
		aTexU[3] = g_aRankingScore[nCntRnking] % 100000 / 10000;
		aTexU[4] = g_aRankingScore[nCntRnking] % 10000 / 1000;
		aTexU[5] = g_aRankingScore[nCntRnking] % 1000 / 100;
		aTexU[6] = g_aRankingScore[nCntRnking] % 100 / 10;
		aTexU[7] = g_aRankingScore[nCntRnking] % 10 / 1;

		for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(450.0f + (nCntPlace * 40), 420.0f + (nCntRnking * 50), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(495.0f + (nCntPlace * 40), 420.0f + (nCntRnking * 50), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f + (nCntPlace * 40), 470.0f + (nCntRnking * 50), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(495.0f + (nCntPlace * 40), 470.0f + (nCntRnking * 50), 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 1.0f);
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//==================================================================
//スコアの描画処理
//==================================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntPlace;
	int nCntRnking;
	int nPoligon = 0;

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResultScore);

		for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, nPoligon++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPoligon * 4, 2);
		}
	}
}

//==================================================================
//スコアの設定処理
//==================================================================
void SetResultScore(int nScore)
{
	g_nResultScore = nScore;
}

////==================================================================
////スコアの加算処理
////==================================================================
//void AddResultScore(int nValue)
//{
//	int aTexU[NUM_PLACE];
//	int nCntPlace;;
//	VERTEX_2D * pVtx;
//
//	g_nResultScore += nValue;
//
//	aTexU[0] = g_nResultScore % 100000000 / 10000000;
//	aTexU[1] = g_nResultScore % 10000000 / 1000000;
//	aTexU[2] = g_nResultScore % 1000000 / 100000;
//	aTexU[3] = g_nResultScore % 100000 / 10000;
//	aTexU[4] = g_nResultScore % 10000 / 1000;
//	aTexU[5] = g_nResultScore % 1000 / 100;
//	aTexU[6] = g_nResultScore % 100 / 10;
//	aTexU[7] = g_nResultScore % 10 / 1;
//
//	//頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
//	{
//		//テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 1.0f);
//	}
//	//頂点バッファをアンロックする
//	g_pVtxBuffResultScore->Unlock();
//}

//==================================================================
//ランキングの処理
//==================================================================
void RankingResultScore(void)
{
	int nCount;
	int nCount2;
	int nTemp;

	//降順の処理
	for (nCount2 = 0; nCount2 < NUM_RANKING - 1; nCount2++)
	{
		for (nCount = (nCount2 + 1); nCount < NUM_RANKING; nCount++)
		{
			if (g_aRankingScore[nCount2] < g_aRankingScore[nCount])
			{
				nTemp = g_aRankingScore[nCount2];                 //元データを格納
				g_aRankingScore[nCount2] = g_aRankingScore[nCount];//元データに参照しているデータを代入
				g_aRankingScore[nCount] = nTemp;                  //格納したデータを参照データに代入
			}
		}
	}

	if (g_nResultScore >= g_aRankingScore[NUM_RANKING - 1])
	{
		g_aRankingScore[NUM_RANKING - 1] = g_nResultScore;
	}

	//降順の処理
	for (nCount2 = 0; nCount2 < NUM_RANKING - 1; nCount2++)
	{
		for (nCount = (nCount2 + 1); nCount < NUM_RANKING; nCount++)
		{
			if (g_aRankingScore[nCount2] < g_aRankingScore[nCount])
			{
				nTemp = g_aRankingScore[nCount2];                 //元データを格納
				g_aRankingScore[nCount2] = g_aRankingScore[nCount];//元データに参照しているデータを代入
				g_aRankingScore[nCount] = nTemp;                  //格納したデータを参照データに代入
			}
		}
	}
}