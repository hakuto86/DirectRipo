#ifndef _TITLE_H_  //このマクロ定義がされてなかったら
#define _TITLE_H_  //2連インクルードのマクロを定義する
#include "main.h"

//マクロ定義
#define NUM_Title (2)  //タイトルの数

typedef struct
{
	bool bUse;          //使用しているかどうか
}Title;

//プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif