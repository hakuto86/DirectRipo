#ifndef _INPUT_H_  //���̃}�N����`������ĂȂ�������
#define _INPUT_H_  //2�A�C���N���[�h�̃}�N�����`����
#include "main.h"

//�v���g�^�C�v�錾
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int Key);
bool GetKeyboardTrigger(int nKey);

#endif


