#include "main.h"
#include "hp.h"
#include "enemy.h"
#include "player.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureHp = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;  //頂点バッファへのポインタ
D3DXVECTOR3 g_posHp;   //スコアの位置
int g_nHp;  //スコアの値

//==================================================================
//スコアの初期化の処理
//==================================================================
void InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	   //デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureHp);

	g_posHp = D3DXVECTOR3(1100, 50, 0.0f);  //位置を初期化する
	g_nHp = 0;  //値を初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_HP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHp,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	int nCntScore;

	for (nCntScore = 0; nCntScore < NUM_HP; nCntScore++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(150.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(200.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(150.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200.0f, 40.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 100, 100, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}

//==================================================================
//スコアの終了処理
//==================================================================
void UninitHp(void)
{
	//テクスチャの破棄
	if (g_pTextureHp != NULL)
	{
		g_pTextureHp->Release();
		g_pTextureHp = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
}

//==================================================================
//スコアの更新処理
//==================================================================
void UpdateHp(void)
{
	int nCntHp;

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(400.0f + (nCntHp * 40), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(445.0f + (nCntHp * 40), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(400.0f + (nCntHp * 40), 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(445.0f + (nCntHp * 40), 70.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}

//==================================================================
//スコアの描画処理
//==================================================================
void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntHp;

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHp);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHp * 4, 2);
	}
}

//==================================================================
//スコアの設定処理
//==================================================================
void SetHp(void)
{
	int aTexU[NUM_HP];
	int nCntHp;
	VERTEX_2D * pVtx;

	aTexU[0] = g_nHp % 100 / 10;
	aTexU[1] = g_nHp % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}

//==================================================================
//スコアの加算処理
//==================================================================
void AddHp(int nValue)
{
	int aTexU[NUM_HP];
	int nCntHp;

	VERTEX_2D * pVtx;

	g_nHp += nValue;

	aTexU[0] = g_nHp % 100 / 10;
	aTexU[1] = g_nHp % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}