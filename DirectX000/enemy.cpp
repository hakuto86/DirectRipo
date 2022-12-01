#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "player.h"
#include "score.h"
#include "enemy_bullet.h"
#include "sound.h"

//�}�N����`
#define NUM_ENEMY (9)  //�G�̎��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;  //���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];
D3DXVECTOR3 g_posEnemy;  //�ʒu
D3DXVECTOR3 g_moveEnemy; //�ړ���
int g_nNumEnemy;   //�G�̑���
int g_nTime;   //����
int g_nCounterAnimEnemy;  //�A�j���[�V�����J�E���^�[

//==================================================================
//�G�̏������̏���
//==================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntEnemy;

	g_nTime = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\small fish.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\soi.png", &g_pTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\saba.png", &g_pTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\octpus.png", &g_pTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\hirame.png", &g_pTextureEnemy[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ainame.png", &g_pTextureEnemy[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ei.png", &g_pTextureEnemy[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ryuuguunotukai.png", &g_pTextureEnemy[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shark.png", &g_pTextureEnemy[8]);

	//�G�̏��̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;         //���
		g_aEnemy[nCntEnemy].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
		g_aEnemy[nCntEnemy].nStop = true;
		g_aEnemy[nCntEnemy].Reversal = true;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-ENEMY_SIZE_WIDTH, -ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+ENEMY_SIZE_WIDTH, -ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMY_SIZE_WIDTH, +ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+ENEMY_SIZE_WIDTH, +ENEMY_SIZE_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	g_nNumEnemy = 0;   //�G�̑������N���A

	LoadEnemy();
}

//==================================================================
//�G�̏I������
//==================================================================
void UninitEnemy(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++) 
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{
			g_pTextureEnemy[nCnt]->Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//==================================================================
//�G�̍X�V����
//==================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D * pVtx;

	g_nTime++;  //���Ԃ�i�߂�

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_nCounterAnimEnemy++; //�J�E���^�[�����Z

	//�G�̏o������
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		HitEnemyPlayer(&g_aEnemy[nCntEnemy]);

		if (g_aEnemy[nCntEnemy].A_Time > g_nTime)
		{
			g_aEnemy[nCntEnemy].bUse = false;
		}

		//A_Time��g_nTime(PC���̎���)�ɂȂ������o������
		else if (g_aEnemy[nCntEnemy].A_Time == g_nTime)
		{
			g_aEnemy[nCntEnemy].bUse = true;
		}

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].m_pattern)
			{
			case 0:
				if (g_aEnemy[nCntEnemy].A_Time <= g_nTime)
				{
					g_aEnemy[nCntEnemy].pos.x -= g_aEnemy[nCntEnemy].nSpeed;
				}
				break;

			case 1:
				if (g_aEnemy[nCntEnemy].A_Time <= g_nTime)
				{
					if (g_aEnemy[nCntEnemy].nStop == false)
					{
						if (g_aEnemy[nCntEnemy].Reversal == false)
						{
							g_aEnemy[nCntEnemy].pos.y -= 2;
							if (g_aEnemy[nCntEnemy].pos.y < 150)
							{
								g_aEnemy[nCntEnemy].pos.y = 152;
								g_aEnemy[nCntEnemy].nStop = true;
							}
						}
						else if (g_aEnemy[nCntEnemy].Reversal == true)
						{
							g_aEnemy[nCntEnemy].pos.y += 2;
							if (g_aEnemy[nCntEnemy].pos.y > 550)
							{
								g_aEnemy[nCntEnemy].pos.y = 548;
								g_aEnemy[nCntEnemy].nStop = true;
							}
						}
					}
					else if (g_aEnemy[nCntEnemy].nStop == true)
					{
						g_aEnemy[nCntEnemy].nCntStop++;
						if (g_aEnemy[nCntEnemy].nCntStop == 20)
						{
							if (g_aEnemy[nCntEnemy].Reversal == false)
							{
								g_aEnemy[nCntEnemy].Reversal = true;
							}
							else if (g_aEnemy[nCntEnemy].Reversal == true)
							{
								g_aEnemy[nCntEnemy].Reversal = false;
							}
							g_aEnemy[nCntEnemy].nStop = false;
							g_aEnemy[nCntEnemy].nCntStop = 0;
						}
					}
				}
				break;
			}

			if (g_aEnemy[nCntEnemy].nType == 7)
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT * 3, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT * 3, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT * 3, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH * 3, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT * 3, 0.0f);
			}
			else if (g_aEnemy[nCntEnemy].nType == 8)
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT*5, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT*5, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT*5, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH*5, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT*5, 0.0f);
			}
			else
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);
			}

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:

				//��ʊO�ɏo��
				if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH + 100 || g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT +100 || g_aEnemy[nCntEnemy].pos.x <= -100 || g_aEnemy[nCntEnemy].pos.y <= -100)
				{
					g_aEnemy[nCntEnemy].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
				}

				break;

			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_nCounterAnimEnemy++; //�J�E���^�[�����Z

					//if ((g_nCounterAnimEnemy % 300) == 0)
					//{//��莞�Ԍo��
					//		//�e�̐ݒ�
					//		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(BULLET_LEFT, 0.0f, 0.0f), 100, BULLETTYPE_ENEMY);
					//}

					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				//��ʊO�ɏo��
				if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.x < 0 || g_aEnemy[nCntEnemy].pos.y < 0)
				{
					g_aEnemy[nCntEnemy].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
				}

				break;
			}
			switch (g_aEnemy[nCntEnemy].B_pattern)
			{
			case ENEMYBULLETTYPE_STRAIGHT:

				if ((g_nCounterAnimEnemy % 100) == 0)
				{//��莞�Ԍo��
				 //�e�̐ݒ�
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-7.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT,g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			case ENEMYBULLETTYPE_ROCKON:
				if ((g_nCounterAnimEnemy % 120) == 0)
				{//��莞�Ԍo��
				 //�e�̐ݒ�
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_ROCKON, g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			case ENEMYBULLETTYPE_HOMING:
				if ((g_nCounterAnimEnemy % 140) == 0)
				{//��莞�Ԍo��
				 //�e�̐ݒ�
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_HOMING, g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			case ENEMYBULLETTYPE_BOSS:
				if ((g_nCounterAnimEnemy % 80) == 0)
				{//��莞�Ԍo��
				 //�e�̐ݒ�
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-7.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * 0.3f) * (-10.0f), (D3DX_PI * 0.3f), 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * 0.1f) * (-10.0f), (D3DX_PI * 0.1f), 0.0f), 100, ENEMYBULLETTYPE_STRAIGHT, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * 0.1f) * (-10.0f), (D3DX_PI * 0.1f), 0.0f), 100, ENEMYBULLETTYPE_BOSS, g_aEnemy[nCntEnemy].B_Type);
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 100, ENEMYBULLETTYPE_ROCKON, g_aEnemy[nCntEnemy].B_Type);
				}
				break;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==================================================================
//�G�̏����t�@�C������ǂݍ���
//==================================================================
void LoadEnemy(void)
{
	int c = 0;       //���݂̕�����ǂݍ���
	int column = 1;  //�񐔂𐔂���
	int line = 0;    //�s���𐔂���
	int nCntEnemy = 0; //�G�̃J�E���g

	char aData[100];
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//�t�@�C�����J��
	pFile = fopen("data/enemy.csv", "r");

	while (fgetc(pFile) != '\n');

	while (c != EOF)
	{
		while (1)
		{
			c = fgetc(pFile);

			if (c == EOF)
				break;

			if (c != ',' && c != '\n')
				strcat(aData, (const char*)&c);

			else
				break;
		}

		switch (column)
		{
		case 1:g_aEnemy[line].nType = atoi(aData); break;
		case 2:g_aEnemy[line].pos.x = (float)(atoi(aData)); break;
		case 3:g_aEnemy[line].pos.y = (float)(atoi(aData)); break;
		case 4:g_aEnemy[line].nSpeed = atoi(aData); break;
		case 5:g_aEnemy[line].m_pattern = atoi(aData); break;
		case 6:g_aEnemy[line].B_Type = atoi(aData); break;
		case 7:g_aEnemy[line].B_pattern = atoi(aData); break;
		case 8:g_aEnemy[line].A_Time = atoi(aData); break;
		case 9:g_aEnemy[line].nLife = atoi(aData); break;
		case 10:g_aEnemy[line].nDamage = atoi(aData); break;
		}

		/*g_aEnemy[line].bUse = true;*/

		memset(aData, 0, sizeof(aData));

		column++;

		SetEnemy(g_aEnemy[nCntEnemy].pos, line);

		if (c == '\n')
		{
			column = 1;
			line++;
		}
	}
	fclose(pFile);
}
//==================================================================
//�G�̕`�揈��
//==================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntEnemy;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

								//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//==================================================================
//�G�ƃv���C���[�̓����蔻��̏���
//==================================================================
void HitEnemyPlayer(Enemy *pEnemy)
{
	int nCntEnemy;

	Player *pPlayer = GetPlayer(); //�v���C���[�̏��ւ̃|�C���^

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pPlayer->state == PLAYERSTATE_NORMAL)
		{//�G���g�p����Ă���
			if (pEnemy->pos.x >= pPlayer->pos.x - PLAYER_SIZE_WIDTH / 2
				&& pEnemy->pos.x <= pPlayer->pos.x + PLAYER_SIZE_WIDTH / 2
				&& pEnemy->pos.y >= pPlayer->pos.y - PLAYER_SIZE_HEIGHT / 2
				&& pEnemy->pos.y <= pPlayer->pos.y + PLAYER_SIZE_HEIGHT / 2)
			{//�G�ƃv���C���[����������
				HitPlayer(1);
			}
		}
	}
}

//==================================================================
//�G�̃q�b�g����
//==================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntEnemy * 4;

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	PlaySound(SOUND_LABEL_SE_HIT);

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//�����̐ݒ�
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		g_aEnemy[nCntEnemy].bUse = false;

      	AddScore(g_aEnemy[nCntEnemy].nScore);
		g_nNumEnemy--;    //�G�̑��������炷
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	if (g_aEnemy[78].bUse == false && g_aEnemy[78].nLife <= 0)
	{//�G���g�p����Ă��Ȃ�
			//�Q�[���̏�Ԃ̐ݒ�
			SetGameState(GAMESTATE_CLEAR, 60);
	}
}

//==================================================================
//�G�̑����̎擾
//==================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}

//==================================================================
//�G�̐ݒ菈��
//==================================================================
void SetEnemy(D3DXVECTOR3 pos,int nCntEnemy)
{
	//���_�o�b�t�@�̐���
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aEnemy[nCntEnemy].bUse == false)
	{//�G���g�p����Ă��Ȃ�
		if (g_aEnemy[nCntEnemy].nType == 0)
		{
			g_aEnemy[nCntEnemy].nScore = 100;
		}
		else if (g_aEnemy[nCntEnemy].nType == 1)
		{
			g_aEnemy[nCntEnemy].nScore = 200;
		}
		else if (g_aEnemy[nCntEnemy].nType == 2)
		{
			g_aEnemy[nCntEnemy].nScore = 300;
		}
		else if (g_aEnemy[nCntEnemy].nType == 3)
		{
			g_aEnemy[nCntEnemy].nScore = 1500;
		}
		else if (g_aEnemy[nCntEnemy].nType == 4)
		{
			g_aEnemy[nCntEnemy].nScore = 500;
		}
		else if (g_aEnemy[nCntEnemy].nType == 5)
		{
			g_aEnemy[nCntEnemy].nScore = 400;
		}
		else if (g_aEnemy[nCntEnemy].nType == 6)
		{
			g_aEnemy[nCntEnemy].nScore = 700;
		}
		else if (g_aEnemy[nCntEnemy].nType == 7)
		{
			g_aEnemy[nCntEnemy].nScore = 3000;
		}
		else if (g_aEnemy[nCntEnemy].nType == 8)
		{
			g_aEnemy[nCntEnemy].nScore = 5000;
		}
		g_aEnemy[nCntEnemy].pos = pos;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE_WIDTH, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE_HEIGHT, 0.0f);

		g_aEnemy[nCntEnemy].bUse = true;  //�g�p���Ă����Ԃɂ���

		g_nNumEnemy++;   //�G�̑��������Z����
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==================================================================
//�G�̎擾
//==================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}
