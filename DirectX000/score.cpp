#include "main.h"
#include "score.h"
#include "enemy.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;  //頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;   //スコアの位置
int g_nScore;  //スコアの値

//==================================================================
//スコアの初期化の処理
//==================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

    pDevice = GetDevice();	   //デバイスの取得
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureScore);

	g_posScore = D3DXVECTOR3(1100, 50, 0.0f);  //位置を初期化する
	g_nScore = 0;  //値を初期化する

    //頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCntScore;

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(100.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(100.0f, 40.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 200, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 200, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 200, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 200, 100, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//スコアの終了処理
//==================================================================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//==================================================================
//スコアの更新処理
//==================================================================
void UpdateScore(void)
{
	int nCntScore;

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f + (nCntScore * 40), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(45.0f + (nCntScore * 40), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f + (nCntScore * 40), 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(45.0f + (nCntScore * 40), 70.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//スコアの描画処理
//==================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntScore;

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//==================================================================
//スコアの設定処理
//==================================================================
void SetScore(void)
{
	int aTexU[NUM_PLACE];
	int nCntScore;
	VERTEX_2D * pVtx;

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//スコアの加算処理
//==================================================================
void AddScore(int nValue)
{
	int aTexU[NUM_PLACE];
	int nCntScore;

	VERTEX_2D * pVtx;

	g_nScore += nValue;

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//スコアの取得
//==================================================================
int *GetScore(void)
{
	return &g_nScore;
}