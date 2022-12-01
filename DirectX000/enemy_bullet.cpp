#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"
#include "enemy_bullet.h"

//�}�N����`
#define MAX_Bullet (128)  //�e�̍ő吔
#define NUM_Bullet (6)  //�G�̒e�̎��
#define MAX_RND (40)  //�����̂ӂ蕝
#define MAX_RND2 (20)  //�����̂ӂ蕝�̔���

//�G�̒e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXVECTOR3 move; //�ړ���
	D3DXVECTOR3 h_move; //�z�[�~���O�̈ړ���
	int h_Time;         //�z�[�~���O���鎞��
	int nLife;        //����
	ENEMYBULLETTYPE type;  //���
	int sType;
	bool bUse;        //�g�p���Ă��邩�ǂ���
} E_Bullet;

//�v���g�^�C�v�錾
void CollisionPlayer(E_Bullet *pBullet);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureE_Bullet[NUM_Bullet] = {};  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxE_BuffBullet = NULL;  //���_�o�b�t�@�ւ̃|�C���^
E_Bullet g_aE_Bullet[MAX_Bullet];

//==================================================================
//�G�̒e�̏������̏���
//==================================================================
void InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ebi.png", &g_pTextureE_Bullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\isome.png", &g_pTextureE_Bullet[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\iwasi.png", &g_pTextureE_Bullet[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rua-.png", &g_pTextureE_Bullet[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\toge.png", &g_pTextureE_Bullet[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sumi.png", &g_pTextureE_Bullet[5]);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		g_aE_Bullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aE_Bullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aE_Bullet[nCntBullet].h_Time = 0;
		g_aE_Bullet[nCntBullet].nLife = 100;
		g_aE_Bullet[nCntBullet].type;
		g_aE_Bullet[nCntBullet].sType = 0;
		g_aE_Bullet[nCntBullet].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Bullet,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxE_BuffBullet,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxE_BuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH, -E_BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH, -E_BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH, +E_BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH, +E_BULLET_SIZE_HEIGHT, 0.0f);

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
	g_pVtxE_BuffBullet->Unlock();
}

//==================================================================
//�G�̒e�̏I������
//==================================================================
void UninitEnemyBullet(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_Bullet; nCnt++)
	{
		if (g_pTextureE_Bullet[nCnt] != NULL)
		{
			g_pTextureE_Bullet[nCnt]->Release();
			g_pTextureE_Bullet[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxE_BuffBullet != NULL)
	{
		g_pVtxE_BuffBullet->Release();
		g_pVtxE_BuffBullet = NULL;
	}
}

//==================================================================
//�G�̒e�̍X�V����
//==================================================================
void UpdateEnemyBullet(void)
{
	int nCntBullet;

	VERTEX_2D * pVtx;

	Player*player = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxE_BuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++, pVtx += 4)
	{
		if (g_aE_Bullet[nCntBullet].bUse == true)
		{
			switch (g_aE_Bullet[nCntBullet].type)
			{
			case ENEMYBULLETTYPE_STRAIGHT:
				//�ʒu���X�V
				g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
				g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				break;
			case ENEMYBULLETTYPE_ROCKON:
				if (g_aE_Bullet[nCntBullet].h_Time >= 59)
				{
					g_aE_Bullet[nCntBullet].h_move = player->pos - g_aE_Bullet[nCntBullet].pos;
					D3DXVec3Normalize(&g_aE_Bullet[nCntBullet].h_move, &g_aE_Bullet[nCntBullet].h_move);
					g_aE_Bullet[nCntBullet].h_move = g_aE_Bullet[nCntBullet].h_move * 6.0f;
					g_aE_Bullet[nCntBullet].pos += g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].move = g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].h_Time--;
				}
				else
				{
					g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
					g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				}
				break;
			case ENEMYBULLETTYPE_HOMING:
				if (g_aE_Bullet[nCntBullet].h_Time >= 30)
				{
					g_aE_Bullet[nCntBullet].h_move = player->pos - g_aE_Bullet[nCntBullet].pos;
					D3DXVec3Normalize(&g_aE_Bullet[nCntBullet].h_move, &g_aE_Bullet[nCntBullet].h_move);
					g_aE_Bullet[nCntBullet].h_move = g_aE_Bullet[nCntBullet].h_move * 4.0f;
					g_aE_Bullet[nCntBullet].pos += g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].move = g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].h_Time--;
				}
				else
				{
					g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
					g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				}
				break;
			case ENEMYBULLETTYPE_BOSS:
				if (g_aE_Bullet[nCntBullet].h_Time >= 20)
				{
					g_aE_Bullet[nCntBullet].h_move = player->pos - g_aE_Bullet[nCntBullet].pos;
					D3DXVec3Normalize(&g_aE_Bullet[nCntBullet].h_move, &g_aE_Bullet[nCntBullet].h_move);
					g_aE_Bullet[nCntBullet].h_move = g_aE_Bullet[nCntBullet].h_move * 5.0f;
					g_aE_Bullet[nCntBullet].pos += g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].move = g_aE_Bullet[nCntBullet].h_move;
					g_aE_Bullet[nCntBullet].h_Time--;
				}
				else
				{
					g_aE_Bullet[nCntBullet].pos.x += g_aE_Bullet[nCntBullet].move.x;
					g_aE_Bullet[nCntBullet].pos.y += g_aE_Bullet[nCntBullet].move.y;
				}
			}

			CollisionPlayer(&g_aE_Bullet[nCntBullet]);

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);

			SetEffect(D3DXVECTOR3((
				g_aE_Bullet[nCntBullet].pos.x + 20.0f) + (-20 + (rand() % MAX_RND)),
				g_aE_Bullet[nCntBullet].pos.y + (-20 + rand() % MAX_RND),
				g_aE_Bullet[nCntBullet].pos.z), 
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10, 10);

			//��ʊO�ɏo��
			if (g_aE_Bullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aE_Bullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aE_Bullet[nCntBullet].pos.x <= 0 || g_aE_Bullet[nCntBullet].pos.y <= 0)
			{
				g_aE_Bullet[nCntBullet].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
			}

			/*	g_aBullet[nCntBullet].nLife--;*/

			//�������s����
			if (g_aE_Bullet[nCntBullet].nLife == 0)
			{
				SetExplosion(/*g_posEnemy*/g_aE_Bullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aE_Bullet[nCntBullet].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxE_BuffBullet->Unlock();
}

//==================================================================
//�G�̒e�̕`�揈��
//==================================================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntBullet;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxE_BuffBullet, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		if (g_aE_Bullet[nCntBullet].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureE_Bullet[g_aE_Bullet[nCntBullet].sType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//==================================================================
//�G�̒e�̐ݒ菈��
//==================================================================
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, ENEMYBULLETTYPE type,int sType)
{
	int nCntBullet;

	//���_�o�b�t�@�̐���
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxE_BuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++, pVtx += 4)
	{
		if (g_aE_Bullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			g_aE_Bullet[nCntBullet].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, -E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+E_BULLET_SIZE_WIDTH + g_aE_Bullet[nCntBullet].pos.x, +E_BULLET_SIZE_HEIGHT + g_aE_Bullet[nCntBullet].pos.y, 0.0f);

			g_aE_Bullet[nCntBullet].move = move;
			g_aE_Bullet[nCntBullet].type = type;
			g_aE_Bullet[nCntBullet].sType = sType;
			if (g_aE_Bullet[nCntBullet].type == 1)
			{
				g_aE_Bullet[nCntBullet].h_Time = 60;
			}
			else if (g_aE_Bullet[nCntBullet].type == 2)
			{
				g_aE_Bullet[nCntBullet].h_Time = 60;
			}
			g_aE_Bullet[nCntBullet].nLife = 100;
			g_aE_Bullet[nCntBullet].bUse = true;  //�g�p���Ă����Ԃɂ���

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxE_BuffBullet->Unlock();
}

//==================================================================
//�v���C���[�̓����蔻��̏���
//==================================================================
void CollisionPlayer(E_Bullet *pBullet)
{
	//�e���g�p����Ă���
	Player *pPlayer;  //�v���C���[�̏��ւ̃|�C���^

	//�G�̎擾
	pPlayer = GetPlayer();  //�v���C���[�̏��̐擪�A�h���X����������

	if (pPlayer->bDisp == true)
	{//�G���g�p����Ă���
		if (pBullet->pos.x <= pPlayer->pos.x + 50.0f + E_BULLET_SIZE_WIDTH / 2
			&& pBullet->pos.x >= pPlayer->pos.x - 50.0f - E_BULLET_SIZE_WIDTH / 2
			&& pBullet->pos.y >= pPlayer->pos.y - 30.0f - E_BULLET_SIZE_HEIGHT / 2
			&& pBullet->pos.y <= pPlayer->pos.y + 30.0f + E_BULLET_SIZE_HEIGHT / 2)
		{
				//�v���C���[�̃q�b�g����
				HitPlayer(1);

			pBullet->bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���

			//�����̐ݒ�
			SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}