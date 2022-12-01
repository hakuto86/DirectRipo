#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "score.h"
#include "resultscore.h"
#include "fade.h"
#include "Effect.h"
#include "enemy_bullet.h"
#include "hp.h"
#include "hp_mark.h"

//�O���[�o���ϐ�
GAMESTATE g_gameState = GAMESTATE_NONE;   //�Q�[���̏��
int g_nCounterGameState = 0;              //��ԊǗ��J�E���^�[
/*bool g_bPause = false; */                   //�|�[�Y��Ԃ�ON/OFF

//==================================================================
//�Q�[����ʂ̏���������
//==================================================================
void InitGame(void)
{
	//�w�i�̏���������
	InitBg();
	//�X�R�A�̏���������
	InitScore();
	//�̗͂̏���������
	InitHp();
	//�̗͂̏���������
	InitHpMark();
	//�v���C���[�̏���������
	InitPlayer();
	//�e�̏���������
	InitBullet();
	//�����̏���������
	InitExplosion();
	//�G�t�F�N�g�̏���������
	InitEffect();
	//�G�̏���������
	InitEnemy();
	//�G�̒e�̏���������
	InitEnemyBullet();

	g_gameState = GAMESTATE_NORMAL;   //�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;
}

//==================================================================
//�Q�[����ʂ̏I������
//==================================================================
void UninitGame(void)
{
	//�w�i�̏I������
	UninitBg();
	//�X�R�A�̏I������
	UninitScore();
	//�̗͂̏I������
	UninitHp();
	//�̗͂̏I������
	UninitHpMark();
	//�v���C���[�̏I������
	UninitPlayer();
	//�e�̏I������
	UninitBullet();
	//�����̏I������
	UninitExplosion();
	//�G�t�F�N�g�̏I������
	UninitEffect();
	//�G�̏I������
	UninitEnemy();
	//�G�̒e�̏I������
	UninitEnemyBullet();

	SetResultScore(*GetScore());

	// �����L���O�\�[�g
	RankingResultScore();
}

//==================================================================
//�Q�[����ʂ̍X�V����
//==================================================================
void UpdateGame(void)
{
	//�w�i�̍X�V����
	UpdateBg();
	//�X�R�A�̍X�V����
	UpdateScore();
	//�̗͂̍X�V����
	UpdateHp();
	//�̗͂̍X�V����
	UpdateHpMark();
	//�v���C���[�̍X�V����
	UpdatePlayer();
	//�e�̍X�V����
	UpdateBullet();
	//�����̍X�V����
	UpdateExplosion();
	//�G�t�F�N�g�̍X�V����
	UpdateEffect();
	//�G�̍X�V����
	UpdateEnemy();
	//�G�̒e�̍X�V����
	UpdateEnemyBullet();
	SetScore();

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:  //�ʏ���
		break;

	case GAMESTATE_END:     //�I�����
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;  //�������Ă��Ȃ���Ԃɐݒ�

			//���[�h�ݒ�(���U���g��ʂɈڍs)
			SetFade(MODE_RESULT);
			RankingResultScore();
		}
		break;
	case GAMESTATE_CLEAR:     //�I�����
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;  //�������Ă��Ȃ���Ԃɐݒ�

										   //���[�h�ݒ�(���U���g��ʂɈڍs)
			SetFade(MODE_CLEAR);
			RankingResultScore();
		}
		break;
	}
}

//==================================================================
//�Q�[����ʂ̕`�揈��
//==================================================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBg();
	//�X�R�A�̕`�揈��
	DrawScore();
	//�̗͂̕`�揈��
	DrawHp();
	//�̗͂̕`�揈��
	DrawHpMark();
	//�v���C���[�̕`�揈��
	DrawPlayer();
	//�e�̕`�揈��
	DrawBullet();
	//�����̕`�揈��
	DrawExplosion();
	//�w�i�̕`�揈��
	DrawEffect();
	//�G�̕`�揈��
	DrawEnemy();
	//�G�̒e�̕`�揈��
	DrawEnemyBullet();
}

//==================================================================
//�Q�[���̏�Ԃ̐ݒ�
//==================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//==================================================================
//�Q�[���̏�Ԃ̎擾
//==================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}