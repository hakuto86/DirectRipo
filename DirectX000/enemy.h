#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

//マクロ定義
#define MAX_ENEMY (128)  //敵の最大数

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;    //位置
	D3DXVECTOR3 move;   //移動量
	int nType;          //種類
	int nScore;         //スコア
	int nSpeed;         //速度
	int m_pattern;      //移動パターン
	int nLife;      	//体力
	int nDamage;      	//弾のダメージ
	int B_Type;         //弾の種類
	int B_pattern;      //弾のパターン
	int A_Time;         //出現時間
	bool nStop;          //停止時間
	bool Reversal;       //戻る時間
	int nCntStop;       //カウント
	ENEMYSTATE state;   //状態
	int nCounterState;  //状態管理カウンター
	bool bUse;          //使用しているかどうか
}Enemy;

//マクロ定義
#define ENEMY_SIZE_WIDTH (60.0f)  //敵の横のサイズ
#define ENEMY_SIZE_HEIGHT (60.0f)//敵の縦のサイズ

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void LoadEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nCntEnemy);
Enemy *GetEnemy(void);
void HitEnemyPlayer(Enemy *pEnemy);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy(void);

#endif