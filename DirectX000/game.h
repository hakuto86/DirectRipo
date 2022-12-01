#ifndef _GAME_H_  //���̃}�N����`������ĂȂ�������
#define _GAME_H_  //2�A�C���N���[�h�̃}�N�����`����
#include "main.h"

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,   //�������Ă��Ȃ����
	GAMESTATE_NORMAL,     //�ʏ���
	GAMESTATE_END,        //�I�����
	GAMESTATE_CLEAR,        //�I�����
	GAMESTATE_MAX
}GAMESTATE;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);

#endif
