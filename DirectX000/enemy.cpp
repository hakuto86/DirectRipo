#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "player.h"
#include "score.h"
#include "enemy_bullet.h"
#include "sound.h"

//マクロ定義
#define NUM_ENEMY (9)  //敵の種類

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;  //頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY];
D3DXVECTOR3 g_posEnemy;  //位置
D3DXVECTOR3 g_moveEnemy; //移動量
int g_nNumEnemy;   //敵の総数
int g_nTime;   //時間
int g_nCounterAnimEnemy;  //アニメーションカウンター

//==================================================================
//敵の初期化の処理
//==================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntEnemy;

	g_nTime = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\small fish.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\soi.png", &g_pTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\saba.png", &g_pTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\octpus.png", &g_pTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\hirame.png", &g_pTextureEnemy[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ainame.png", &g_pTextureEnemy[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ei.png", &g_pTextureEnemy[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ryuuguunotukai.png", &g_pTextureEnemy[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shark.png", &g_pTextureEnemy[8]);

	//敵の情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;         //種類
		g_aEnemy[nCntEnemy].bUse = false;  //使用していない状態にする
		g_aEnemy[nCntEnemy].nStop = true;
		g_aEnemy[nCntEnemy].Reversal = true;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-ENEMY_SIZE_WIDTH, -ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+ENEMY_SIZE_WIDTH, -ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMY_SIZE_WIDTH, +ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+ENEMY_SIZE_WIDTH, +ENEMY_SIZE_HEIGHT, 0.0f);

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
	g_pVtxBuffEnemy->Unlock();

	g_nNumEnemy = 0;   //敵の総数をクリア

	LoadEnemy();
}

//==================================================================
//敵の終了処理
//==================================================================
void UninitEnemy(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++) 
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{
			g_pTextureEnemy[nCnt]->Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//==================================================================
//敵の更新処理
//==================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D * pVtx;

	g_nTime++;  //時間を進める

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_nCounterAnimEnemy++; //カウンターを加算

	//敵の出現時間
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		HitEnemyPlayer(&g_aEnemy[nCntEnemy]);

		if (g_aEnemy[nCntEnemy].A_Time > g_nTime)
		{
			g_aEnemy[nCntEnemy].bUse = false;
		}

		//A_Timeがg_nTime(PC内の時間)になった時出現する
		else if (g_aEnemy[nCntEnemy].A_Time == g_nTime)
		{
			g_aEnemy[nCntEnemy].bUse = true;
		}

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].m_pattern)
			{
			case 0:
				if (g_aEnemy[nCntEnemy].A_Time <= g_nTime)
				{
					g_aEnemy[nCntEnemy].pos.x -= g_aEnemy[nCntEnemy].nSpeed;
				}
				break;

			case 1:
				if (g_aEnemy[nCntEnemy].A_Time <= g_nTime)
				{
					if (g_aEnemy[nCntEnemy].nStop == false)
					{
						if (g_aEnemy[nCntEnemy].Reversal == false)
						{
							g_aEnemy[nCntEnemy].pos.y -= 2;
							if (g_aEnemy[nCntEnemy].pos.y < 150)
							{
								g_aEnemy[nCntEnemy].pos.y = 152;
								g_aEnemy[nCntEnemy].nStop = true;
							}
						}
						else if (g_aEnemy[nCntEnemy].Reversal == true)
						{
							g_aEnemy[nCntEnemy].pos.y += 2;
							if (g_aEnemy[nCntEnemy].pos.y > 550)
							{
								g_aEnemy[nCntEnemy].pos.y = 548;
								g_aEnemy[nCntEnemy].nStop = true;
							}
						}
					}
					else if (g_aEnemy[nCntEnemy].nStop == true)
					{
						g_aEnemy[nCntEnemy].nCntStop++;
						if (g_aEnemy[nCntEnemy].nCntStop == 20)
						{
							if (g_aEnemy[nCntEnemy].Reversal == false)
							{
								g_aEnemy[nCntEnemy].Reversal = true;
							}
							else if (g_aEnemy[nCntEnemy].Reversal == true)
							{
								g_aEnemy[nCntEnemy].Reversal = false;
							}
							g_aEnemy[nCntEnemy].nStop = false;
							g_aEnemy[nCntEnemy].nCntStop = 0;
						}
					}
				}
				break;
			}

			if (g_aEnemy[nCntEnemy].nType == 7)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT * 3, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT * 3, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT * 3, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT * 3, 0.0f);
			}
			else if (g_aEnemy[nCntEnemy].nType == 8)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT*5, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT*5, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT*5, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT*5, 0.0f);
			}
			else
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);
			}

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:

				//画面外に出た
				if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH + 100 || g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT +100 || g_aEnemy[nCntEnemy].pos.x <= -100 || g_aEnemy[nCntEnemy].pos.y <= -100)
				{
					g_aEnemy[nCntEnemy].bUse = false;  //使用していない状態にする
				}

				break;

			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_nCounterAnimEnemy++; //カウンターを加算

					//if ((g_nCounterAnimEnemy % 300) == 0)
					//{//一定時間経過
					//		//弾の設定
					//		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(BULLET_LEFT, 0.0f, 0.0f), 100, BULLETTYPE_ENEMY);
					//}

					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				//画面外に出た
				if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.x < 0 || g_aEnemy[nCntEnemy].pos.y < 0)
				{
					g_aEnemy[nCntEnemy].bUse = false;  //使用していない状態にする
				}

				break;
			}
			switch (g_aEnemy[nCntEnemy].B_pattern)
			{
			case ENEMYBULLETTYPE_STRAIGHT:

				if ((g_nCounterAnimEnemy % 100) == 0)
				{//一定時間経過
				 //弾の設定
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-7.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT,g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			case ENEMYBULLETTYPE_ROCKON:
				if ((g_nCounterAnimEnemy % 120) == 0)
				{//一定時間経過
				 //弾の設定
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_ROCKON, g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			case ENEMYBULLETTYPE_HOMING:
				if ((g_nCounterAnimEnemy % 140) == 0)
				{//一定時間経過
				 //弾の設定
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_HOMING, g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			case ENEMYBULLETTYPE_BOSS:
				if ((g_nCounterAnimEnemy % 80) == 0)
				{//一定時間経過
				 //弾の設定
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-7.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * 0.3f) * (-10.0f), (D3DX_PI * 0.3f), 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * 0.1f) * (-10.0f), (D3DX_PI * 0.1f), 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * 0.1f) * (-10.0f), (D3DX_PI * 0.1f), 0.0f), 100, ENEMYBULLETTYPE_BOSS, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_ROCKON, g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==================================================================
//敵の情報をファイルから読み込む
//==================================================================
void LoadEnemy(void)
{
	int c = 0;       //現在の文字を読み込む
	int column = 1;  //列数を数える
	int line = 0;    //行数を数える
	int nCntEnemy = 0; //敵のカウント

	char aData[100];
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/enemy.csv", "r");

	while (fgetc(pFile) != '\n');

	while (c != EOF)
	{
		while (1)
		{
			c = fgetc(pFile);

			if (c == EOF)
				break;

			if (c != ',' && c != '\n')
				strcat(aData, (const char*)&c);

			else
				break;
		}

		switch (column)
		{
		case 1:g_aEnemy[line].nType = atoi(aData); break;
		case 2:g_aEnemy[line].pos.x = (float)(atoi(aData)); break;
		case 3:g_aEnemy[line].pos.y = (float)(atoi(aData)); break;
		case 4:g_aEnemy[line].nSpeed = atoi(aData); break;
		case 5:g_aEnemy[line].m_pattern = atoi(aData); break;
		case 6:g_aEnemy[line].B_Type = atoi(aData); break;
		case 7:g_aEnemy[line].B_pattern = atoi(aData); break;
		case 8:g_aEnemy[line].A_Time = atoi(aData); break;
		case 9:g_aEnemy[line].nLife = atoi(aData); break;
		case 10:g_aEnemy[line].nDamage = atoi(aData); break;
		}

		/*g_aEnemy[line].bUse = true;*/

		memset(aData, 0, sizeof(aData));

		column++;

		SetEnemy(g_aEnemy[nCntEnemy].pos, line);

		if (c == '\n')
		{
			column = 1;
			line++;
		}
	}
	fclose(pFile);
}
//==================================================================
//敵の描画処理
//==================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
	int nCntEnemy;

	pDevice = GetDevice();	    //デバイスの取得

								//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//==================================================================
//敵とプレイヤーの当たり判定の処理
//==================================================================
void HitEnemyPlayer(Enemy *pEnemy)
{
	int nCntEnemy;

	Player *pPlayer = GetPlayer(); //プレイヤーの情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pPlayer->state == PLAYERSTATE_NORMAL)
		{//敵が使用されている
			if (pEnemy->pos.x >= pPlayer->pos.x - PLAYER_SIZE_WIDTH / 2
				&& pEnemy->pos.x <= pPlayer->pos.x + PLAYER_SIZE_WIDTH / 2
				&& pEnemy->pos.y >= pPlayer->pos.y - PLAYER_SIZE_HEIGHT / 2
				&& pEnemy->pos.y <= pPlayer->pos.y + PLAYER_SIZE_HEIGHT / 2)
			{//敵とプレイヤーが当たった
				HitPlayer(1);
			}
		}
	}
}

//==================================================================
//敵のヒット処理
//==================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntEnemy * 4;

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	PlaySound(SOUND_LABEL_SE_HIT);

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//爆発の設定
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		g_aEnemy[nCntEnemy].bUse = false;

      	AddScore(g_aEnemy[nCntEnemy].nScore);
		g_nNumEnemy--;    //敵の総数を減らす
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

	if (g_aEnemy[78].bUse == false && g_aEnemy[78].nLife <= 0)
	{//敵が使用されていない
			//ゲームの状態の設定
			SetGameState(GAMESTATE_CLEAR, 60);
	}
}

//==================================================================
//敵の総数の取得
//==================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}

//==================================================================
//敵の設定処理
//==================================================================
void SetEnemy(D3DXVECTOR3 pos,int nCntEnemy)
{
	//頂点バッファの生成
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aEnemy[nCntEnemy].bUse == false)
	{//敵が使用されていない
		if (g_aEnemy[nCntEnemy].nType == 0)
		{
			g_aEnemy[nCntEnemy].nScore = 100;
		}
		else if (g_aEnemy[nCntEnemy].nType == 1)
		{
			g_aEnemy[nCntEnemy].nScore = 200;
		}
		else if (g_aEnemy[nCntEnemy].nType == 2)
		{
			g_aEnemy[nCntEnemy].nScore = 300;
		}
		else if (g_aEnemy[nCntEnemy].nType == 3)
		{
			g_aEnemy[nCntEnemy].nScore = 1500;
		}
		else if (g_aEnemy[nCntEnemy].nType == 4)
		{
			g_aEnemy[nCntEnemy].nScore = 500;
		}
		else if (g_aEnemy[nCntEnemy].nType == 5)
		{
			g_aEnemy[nCntEnemy].nScore = 400;
		}
		else if (g_aEnemy[nCntEnemy].nType == 6)
		{
			g_aEnemy[nCntEnemy].nScore = 700;
		}
		else if (g_aEnemy[nCntEnemy].nType == 7)
		{
			g_aEnemy[nCntEnemy].nScore = 3000;
		}
		else if (g_aEnemy[nCntEnemy].nType == 8)
		{
			g_aEnemy[nCntEnemy].nScore = 5000;
		}
		g_aEnemy[nCntEnemy].pos = pos;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);

		g_aEnemy[nCntEnemy].bUse = true;  //使用している状態にする

		g_nNumEnemy++;   //敵の総数を加算する
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==================================================================
//敵の取得
//==================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}
