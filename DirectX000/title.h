#ifndef _TITLE_H_  //���̃}�N����`������ĂȂ�������
#define _TITLE_H_  //2�A�C���N���[�h�̃}�N�����`����
#include "main.h"

//�}�N����`
#define NUM_Title (2)  //�^�C�g���̐�

typedef struct
{
	bool bUse;          //�g�p���Ă��邩�ǂ���
}Title;

//�v���g�^�C�v�錾
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif