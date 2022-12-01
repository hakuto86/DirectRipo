#include "main.h"
#include "Effect.h"
#include "Bullet.h"
#include "explosion.h"
#include "enemy.h"

//マクロ定義
#define MAX_Effect (4096)  //エフェクトの最大数

//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXCOLOR col;    //移動量
	float fRadius;    //半径(大きさ)
	int nLife;        //寿命(表示時間)
	bool bUse;        //使用しているかどうか
} Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;  //頂点バッファへのポインタ
Effect g_aEffect[MAX_Effect];

//==================================================================
//エフェクトの初期化の処理
//==================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bubble.jpg", &g_pTextureEffect);

	//弾の情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aEffect[nCntEffect].nLife = 100;
		g_aEffect[nCntEffect].bUse = false;  //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Effect,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-EFFECT_SIZE_WIDTH, -EFFECT_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+EFFECT_SIZE_WIDTH, -EFFECT_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EFFECT_SIZE_WIDTH, +EFFECT_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+EFFECT_SIZE_WIDTH, +EFFECT_SIZE_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//==================================================================
//エフェクトの終了処理
//==================================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==================================================================
//エフェクトの更新処理
//==================================================================
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_2D * pVtx;
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
		  
	pDevice = GetDevice(); //デバイスの取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, -g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, -g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, +g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, +g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);

			//画面外に出た
			if (g_aEffect[nCntEffect].pos.x >= SCREEN_WIDTH || g_aEffect[nCntEffect].pos.y >= SCREEN_HEIGHT || g_aEffect[nCntEffect].pos.x <= 0 || g_aEffect[nCntEffect].pos.y <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;  //使用していない状態にする
			}

			g_aEffect[nCntEffect].nLife--;

			//寿命が尽きた
			if (g_aEffect[nCntEffect].nLife == 0)
			{
				g_aEffect[nCntEffect].bUse = false;  //使用していない状態にする
			}
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//==================================================================
//エフェクトの描画処理
//==================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntEffect;

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================
//エフェクトの設定処理
//==================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++, pVtx+=4)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//弾が使用されていない

			g_aEffect[nCntEffect].pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fRadius + g_aEffect[nCntEffect].pos.x, -fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+fRadius + g_aEffect[nCntEffect].pos.x, -fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fRadius + g_aEffect[nCntEffect].pos.x, +fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+fRadius + g_aEffect[nCntEffect].pos.x, +fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			
			g_aEffect[nCntEffect].col = col;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}