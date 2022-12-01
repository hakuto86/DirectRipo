#ifndef _INPUT_H_  //このマクロ定義がされてなかったら
#define _INPUT_H_  //2連インクルードのマクロを定義する
#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int Key);
bool GetKeyboardTrigger(int nKey);

#endif


