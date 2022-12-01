#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"
#include "enemy_bullet.h"

//マクロ定義
#define MAX_Bullet (128)  //弾の最大数
#define NUM_Bullet (6)  //敵の弾の種類
#define MAX_RND (40)  //乱数のふり幅
#define MAX_RND2 (20)  //乱数のふり幅の半分

//敵の弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	D3DXVECTOR3 h_move; //ホーミングの移動量
	int h_Time;         //ホーミングする時間
	int nLife;        //寿命
	ENEMYBULLETTYPE type;  //種類
	int sType;
	bool bUse;        //使用しているかどうか
} E_Bullet;

//プロトタイプ宣言
void CollisionPlayer(E_Bullet *pBullet);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureE_Bullet[NUM_Bullet] = {};  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxE_BuffBullet = NULL;  //頂点バッファへのポインタ
E_Bullet g_aE_Bullet[MAX_Bullet];

//==================================================================
//敵の弾の初期化の処理
//==================================================================
void InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ebi.png", &g_pTextureE_Bullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\isome.png", &g_pTextureE_Bullet[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\iwasi.png", &g_pTextureE_Bullet[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rua-.png", &g_pTextureE_Bullet[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\toge.png", &g_pTextureE_Bullet[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sumi.png", &g_pTextureE_Bullet[5]);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		g_aE_Bullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aE_Bullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aE_Bullet[nCntBullet].h_Time = 0;
		g_aE_Bullet[nCntBullet].nLife = 100;
		g_aE_Bullet[nCntBullet].type;
		g_aE_Bullet[nCntBullet].sType = 0;
		g_aE_Bullet[nCntBullet].bUse = false;  //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Bullet,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxE_BuffBullet,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxE_BuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH, -E_BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH, -E_BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH, +E_BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH, +E_BULLET_SIZE_HEIGHT, 0.0f);

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
	g_pVtxE_BuffBullet->Unlock();
}

//==================================================================
//敵の弾の終了処理
//==================================================================
void UninitEnemyBullet(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_Bullet; nCnt++)
	{
		if (g_pTextureE_Bullet[nCnt] != NULL)
		{
			g_pTextureE_Bullet[nCnt]->Release();
			g_pTextureE_Bullet[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxE_BuffBullet != NULL)
	{
		g_pVtxE_BuffBullet->Release();
		g_pVtxE_BuffBullet = NULL;
	}
}

//==================================================================
//敵の弾の更新処理
//==================================================================
void UpdateEnemyBullet(void)
{
	int nCntBullet;

	VERTEX_2D * pVtx;

	Player*player = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxE_BuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++, pVtx += 4)
	{
		if (g_aE_Bullet[nCntBullet].bUse == true)
		{
			switch (g_aE_Bullet[nCntBullet].type)
			{
			case ENEMYBULLETTYPE_STRAIGHT:
				//位置を更新
				g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
				g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				break;
			case ENEMYBULLETTYPE_ROCKON:
				if (g_aE_Bullet[nCntBullet].h_Time >= 59)
				{
					g_aE_Bullet[nCntBullet].h_move = player->pos - g_aE_Bullet[nCntBullet].pos;
					D3DXVec3Normalize(&g_aE_Bullet[nCntBullet].h_move, &g_aE_Bullet[nCntBullet].h_move);
					g_aE_Bullet[nCntBullet].h_move = g_aE_Bullet[nCntBullet].h_move * 6.0f;
					g_aE_Bullet[nCntBullet].pos += g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].move = g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].h_Time--;
				}
				else
				{
					g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
					g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				}
				break;
			case ENEMYBULLETTYPE_HOMING:
				if (g_aE_Bullet[nCntBullet].h_Time >= 30)
				{
					g_aE_Bullet[nCntBullet].h_move = player->pos - g_aE_Bullet[nCntBullet].pos;
					D3DXVec3Normalize(&g_aE_Bullet[nCntBullet].h_move, &g_aE_Bullet[nCntBullet].h_move);
					g_aE_Bullet[nCntBullet].h_move = g_aE_Bullet[nCntBullet].h_move * 4.0f;
					g_aE_Bullet[nCntBullet].pos += g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].move = g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].h_Time--;
				}
				else
				{
					g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
					g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				}
				break;
			case ENEMYBULLETTYPE_BOSS:
				if (g_aE_Bullet[nCntBullet].h_Time >= 20)
				{
					g_aE_Bullet[nCntBullet].h_move = player->pos - g_aE_Bullet[nCntBullet].pos;
					D3DXVec3Normalize(&g_aE_Bullet[nCntBullet].h_move, &g_aE_Bullet[nCntBullet].h_move);
					g_aE_Bullet[nCntBullet].h_move = g_aE_Bullet[nCntBullet].h_move * 5.0f;
					g_aE_Bullet[nCntBullet].pos += g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].move = g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].h_Time--;
				}
				else
				{
					g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
					g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				}
			}

			CollisionPlayer(&g_aE_Bullet[nCntBullet]);

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);

			SetEffect(D3DXVECTOR3((
				g_aE_Bullet[nCntBullet].pos.x + 20.0f) + (-20 + (rand() % MAX_RND)),
				g_aE_Bullet[nCntBullet].pos.y + (-20 + rand() % MAX_RND),
				g_aE_Bullet[nCntBullet].pos.z), 
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10, 10);

			//画面外に出た
			if (g_aE_Bullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aE_Bullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aE_Bullet[nCntBullet].pos.x <= 0 || g_aE_Bullet[nCntBullet].pos.y <= 0)
			{
				g_aE_Bullet[nCntBullet].bUse = false;  //使用していない状態にする
			}

			/*	g_aBullet[nCntBullet].nLife--;*/

			//寿命が尽きた
			if (g_aE_Bullet[nCntBullet].nLife == 0)
			{
				SetExplosion(/*g_posEnemy*/g_aE_Bullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aE_Bullet[nCntBullet].bUse = false;  //使用していない状態にする
			}
		}
	}
	//頂点バッファをアンロックする
	g_pVtxE_BuffBullet->Unlock();
}

//==================================================================
//敵の弾の描画処理
//==================================================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntBullet;

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxE_BuffBullet, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		if (g_aE_Bullet[nCntBullet].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureE_Bullet[g_aE_Bullet[nCntBullet].sType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//==================================================================
//敵の弾の設定処理
//==================================================================
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, ENEMYBULLETTYPE type,int sType)
{
	int nCntBullet;

	//頂点バッファの生成
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxE_BuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++, pVtx += 4)
	{
		if (g_aE_Bullet[nCntBullet].bUse == false)
		{//弾が使用されていない

			g_aE_Bullet[nCntBullet].pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);

			g_aE_Bullet[nCntBullet].move = move;
			g_aE_Bullet[nCntBullet].type = type;
			g_aE_Bullet[nCntBullet].sType = sType;
			if (g_aE_Bullet[nCntBullet].type == 1)
			{
				g_aE_Bullet[nCntBullet].h_Time = 60;
			}
			else if (g_aE_Bullet[nCntBullet].type == 2)
			{
				g_aE_Bullet[nCntBullet].h_Time = 60;
			}
			g_aE_Bullet[nCntBullet].nLife = 100;
			g_aE_Bullet[nCntBullet].bUse = true;  //使用している状態にする

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxE_BuffBullet->Unlock();
}

//==================================================================
//プレイヤーの当たり判定の処理
//==================================================================
void CollisionPlayer(E_Bullet *pBullet)
{
	//弾が使用されている
	Player *pPlayer;  //プレイヤーの情報へのポインタ

	//敵の取得
	pPlayer = GetPlayer();  //プレイヤーの情報の先頭アドレスが代入される

	if (pPlayer->bDisp == true)
	{//敵が使用されている
		if (pBullet->pos.x <= pPlayer->pos.x + 50.0f + E_BULLET_SIZE_WIDTH / 2
			&& pBullet->pos.x >= pPlayer->pos.x - 50.0f - E_BULLET_SIZE_WIDTH / 2
			&& pBullet->pos.y >= pPlayer->pos.y - 30.0f - E_BULLET_SIZE_HEIGHT / 2
			&& pBullet->pos.y <= pPlayer->pos.y + 30.0f + E_BULLET_SIZE_HEIGHT / 2)
		{
				//プレイヤーのヒット処理
				HitPlayer(1);

			pBullet->bUse = false;  //使用していない状態にする

			//爆発の設定
			SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}