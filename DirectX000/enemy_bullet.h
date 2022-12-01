#ifndef _E_BULLET_H_
#define _E_BULLET_H_
#include "main.h"

//マクロ定義
#define E_BULLET_SIZE_WIDTH (40.0f)  //弾の横のサイズ
#define E_BULLET_SIZE_HEIGHT (30.0f)//弾の縦のサイズ

//弾の種類
typedef enum
{
	ENEMYBULLETTYPE_STRAIGHT = 0,  //真っ直ぐ撃つ
	ENEMYBULLETTYPE_ROCKON,  //狙って撃つ
	ENEMYBULLETTYPE_HOMING,  //ホーミング弾
	ENEMYBULLETTYPE_BOSS,
	ENEMYBULLETTYPE_MAX,
}ENEMYBULLETTYPE;

//プロトタイプ宣言
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, ENEMYBULLETTYPE type,int sType);

#endif