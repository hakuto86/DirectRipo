#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"

//マクロ定義
#define MAX_Bullet (128)  //弾の最大数
#define MAX_RND (40)  //乱数のふり幅
#define MAX_RND2 (20)  //乱数のふり幅の半分

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nLife;        //寿命
	BULLETTYPE type;  //種類
	bool bUse;        //使用しているかどうか
} Bullet;

//プロトタイプ宣言
void CollisionEnemy(Bullet *pBullet);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;  //頂点バッファへのポインタ
Bullet g_aBullet[MAX_Bullet];

//==================================================================
//弾初期化の処理
//==================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\spear.png", &g_pTextureBullet);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].type;
		g_aBullet[nCntBullet].bUse = false;  //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Bullet,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH, -BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH, -BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH, +BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH, +BULLET_SIZE_HEIGHT, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//==================================================================
//弾の終了処理
//==================================================================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//==================================================================
//弾の更新処理
//==================================================================
void UpdateBullet(void)
{
	int nCntBullet;

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++, pVtx += 4)
	{
		if (g_aBullet[nCntBullet]. bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				CollisionEnemy(&g_aBullet[nCntBullet]);
			}

			//位置を更新
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);

			SetEffect(D3DXVECTOR3((g_aBullet[nCntBullet].pos.x - 60.0f) + (-MAX_RND2 + (rand() % MAX_RND)), (g_aBullet[nCntBullet].pos.y - 5.0f) + (-MAX_RND2 + rand() % MAX_RND), g_aBullet[nCntBullet].pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 8, 20);

			//画面外に出た
			if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.x <= 0 || g_aBullet[nCntBullet].pos.y <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;  //使用していない状態にする
			}
			
		/*	g_aBullet[nCntBullet].nLife--;*/

			//寿命が尽きた
			if (g_aBullet[nCntBullet].nLife == 0)
			{
				SetExplosion(/*g_posEnemy*/g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;  //使用していない状態にする
			}
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==================================================================
//弾の描画処理
//==================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntBullet;

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//==================================================================
//弾の設定処理
//==================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	int nCntBullet;

	//頂点バッファの生成
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++ ,pVtx += 4)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない

			g_aBullet[nCntBullet].pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].bUse = true;  //使用している状態にする

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==================================================================
//敵の当たり判定の処理
//==================================================================
void CollisionEnemy(Bullet *pBullet)
{
	//弾が使用されている
	Enemy *pEnemy;  //敵の情報へのポインタ

	int nCntEnemy;

	//敵の取得
	pEnemy = GetEnemy();  //敵の情報の先頭アドレスが代入される

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている
			if (pEnemy->nType == 8)
			{
				if (pBullet->pos.x >= pEnemy->pos.x - 100.0f - BULLET_SIZE_WIDTH *2
					&& pBullet->pos.x <= pEnemy->pos.x + 100.0f + BULLET_SIZE_WIDTH*2
					&& pBullet->pos.y >= pEnemy->pos.y - 30.0f - BULLET_SIZE_HEIGHT*2
					&& pBullet->pos.y <= pEnemy->pos.y + 30.0f + BULLET_SIZE_HEIGHT*2)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					pBullet->bUse = false;  //使用していない状態にする

					//爆発の設定
					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			else if (pEnemy->nType == 7)
			{
				if (pBullet->pos.x >= pEnemy->pos.x - 90.0f - BULLET_SIZE_WIDTH * 2
					&& pBullet->pos.x <= pEnemy->pos.x + 90.0f + BULLET_SIZE_WIDTH * 2
					&& pBullet->pos.y >= pEnemy->pos.y - 30.0f - BULLET_SIZE_HEIGHT * 2
					&& pBullet->pos.y <= pEnemy->pos.y + 30.0f + BULLET_SIZE_HEIGHT * 2)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					pBullet->bUse = false;  //使用していない状態にする

											//爆発の設定
					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			else
			{
				if (pBullet->pos.x >= pEnemy->pos.x - 30.0f - BULLET_SIZE_WIDTH / 2
					&& pBullet->pos.x <= pEnemy->pos.x + 30.0f + BULLET_SIZE_WIDTH / 2
					&& pBullet->pos.y >= pEnemy->pos.y - 30.0f - BULLET_SIZE_HEIGHT / 2
					&& pBullet->pos.y <= pEnemy->pos.y + 30.0f + BULLET_SIZE_HEIGHT / 2)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					pBullet->bUse = false;  //使用していない状態にする

											//爆発の設定
					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}
	}
}