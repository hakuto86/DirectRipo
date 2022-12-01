#ifndef _HP_H_
#define _HP_H_
#include "main.h"

//マクロ定義
#define HP_SIZE_WIDTH (30.0f)
#define HP_SIZE_HEIGHT (80.0f)
#define NUM_HP  (2)  //HPの桁数

//プロトタイプ宣言
void InitHp(void);
void UninitHp(void);
void UpdateHp(void);
void DrawHp(void);
void SetHp(void);
void AddHp(int nValue);

#endif