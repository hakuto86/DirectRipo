#ifndef _HP_MARK_H_
#define  _HP_MARK_H_
#include "main.h"

//マクロ定義
#define HP_MARK_SIZE_WIDTH (30.0f)
#define HP_MARK_SIZE_HEIGHT (80.0f)

//プロトタイプ宣言
void InitHpMark(void);
void UninitHpMark(void);
void UpdateHpMark(void);
void DrawHpMark(void);

#endif