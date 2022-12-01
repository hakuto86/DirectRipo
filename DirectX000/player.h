#ifndef _PLAYER_H_
#define _PLAYER_H_

//�}�N����`
#define PATTERN_WIDTH (1)
#define PATTERN_HEIGHT (3)
#define MOVE (0.8f)  //�ړ���
#define BULLET_UP (-10.0f)  //�ړ���
#define BULLET_DIA (10.0f)  //�ړ���
#define BULLET_LIGHT (10.0f)  //�ړ���
#define BULLET_LEFT (-10.0f)  //�ړ���
#define PLAYER_SIZE_WIDTH (130.0f)  //�v���C���[�̉��̃T�C�Y
#define PLAYER_SIZE_HEIGHT (80.0f)//�v���C���[�̏c�̃T�C�Y

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//�o�����(�_��)
	PLAYERSTATE_NORMAL,			//�ʏ���
	PLAYERSTATE_DAMAGE,			//�_���[�W���
	PLAYERSTATE_WAIT,			//�o���҂����(�\���Ȃ�)
	PLAYERSTATE_DEATH,			//���S���
	PLAYESTATE_MAX
}PLAYERSTATE;

//�v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;   //�ʒu
	D3DXVECTOR3 rot;   //����
	D3DXVECTOR3 move;  //�ړ���
	int nLife;         //�̗�
	PLAYERSTATE state; //�v���C���[�̏��
	int nCounterState; //��ԊǗ��J�E���^�[
	bool bDisp;        //�\�����邩���Ȃ���
	//bool bUse;         //�g�p����Ă��邩�ǂ���
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);

#endif

