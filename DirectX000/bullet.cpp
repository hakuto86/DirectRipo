#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"

//�}�N����`
#define MAX_Bullet (128)  //�e�̍ő吔
#define MAX_RND (40)  //�����̂ӂ蕝
#define MAX_RND2 (20)  //�����̂ӂ蕝�̔���

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXVECTOR3 move; //�ړ���
	int nLife;        //����
	BULLETTYPE type;  //���
	bool bUse;        //�g�p���Ă��邩�ǂ���
} Bullet;

//�v���g�^�C�v�錾
void CollisionEnemy(Bullet *pBullet);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;  //���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_Bullet];

//==================================================================
//�e�������̏���
//==================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\spear.png", &g_pTextureBullet);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].type;
		g_aBullet[nCntBullet].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Bullet,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH, -BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH, -BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH, +BULLET_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH, +BULLET_SIZE_HEIGHT, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//==================================================================
//�e�̏I������
//==================================================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//==================================================================
//�e�̍X�V����
//==================================================================
void UpdateBullet(void)
{
	int nCntBullet;

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++, pVtx += 4)
	{
		if (g_aBullet[nCntBullet]. bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				CollisionEnemy(&g_aBullet[nCntBullet]);
			}

			//�ʒu���X�V
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);

			SetEffect(D3DXVECTOR3((g_aBullet[nCntBullet].pos.x - 60.0f) + (-MAX_RND2 + (rand() % MAX_RND)), (g_aBullet[nCntBullet].pos.y - 5.0f) + (-MAX_RND2 + rand() % MAX_RND), g_aBullet[nCntBullet].pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 8, 20);

			//��ʊO�ɏo��
			if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.x <= 0 || g_aBullet[nCntBullet].pos.y <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
			}
			
		/*	g_aBullet[nCntBullet].nLife--;*/

			//�������s����
			if (g_aBullet[nCntBullet].nLife == 0)
			{
				SetExplosion(/*g_posEnemy*/g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//==================================================================
//�e�̕`�揈��
//==================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntBullet;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//==================================================================
//�e�̐ݒ菈��
//==================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	int nCntBullet;

	//���_�o�b�t�@�̐���
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_Bullet; nCntBullet++ ,pVtx += 4)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			g_aBullet[nCntBullet].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BULLET_SIZE_WIDTH + g_aBullet[nCntBullet].pos.x, +BULLET_SIZE_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].bUse = true;  //�g�p���Ă����Ԃɂ���

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//==================================================================
//�G�̓����蔻��̏���
//==================================================================
void CollisionEnemy(Bullet *pBullet)
{
	//�e���g�p����Ă���
	Enemy *pEnemy;  //�G�̏��ւ̃|�C���^

	int nCntEnemy;

	//�G�̎擾
	pEnemy = GetEnemy();  //�G�̏��̐擪�A�h���X����������

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă���
			if (pEnemy->nType == 8)
			{
				if (pBullet->pos.x >= pEnemy->pos.x - 100.0f - BULLET_SIZE_WIDTH *2
					&& pBullet->pos.x <= pEnemy->pos.x + 100.0f + BULLET_SIZE_WIDTH*2
					&& pBullet->pos.y >= pEnemy->pos.y - 30.0f - BULLET_SIZE_HEIGHT*2
					&& pBullet->pos.y <= pEnemy->pos.y + 30.0f + BULLET_SIZE_HEIGHT*2)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					pBullet->bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���

					//�����̐ݒ�
					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			else if (pEnemy->nType == 7)
			{
				if (pBullet->pos.x >= pEnemy->pos.x - 90.0f - BULLET_SIZE_WIDTH * 2
					&& pBullet->pos.x <= pEnemy->pos.x + 90.0f + BULLET_SIZE_WIDTH * 2
					&& pBullet->pos.y >= pEnemy->pos.y - 30.0f - BULLET_SIZE_HEIGHT * 2
					&& pBullet->pos.y <= pEnemy->pos.y + 30.0f + BULLET_SIZE_HEIGHT * 2)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					pBullet->bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���

											//�����̐ݒ�
					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			else
			{
				if (pBullet->pos.x >= pEnemy->pos.x - 30.0f - BULLET_SIZE_WIDTH / 2
					&& pBullet->pos.x <= pEnemy->pos.x + 30.0f + BULLET_SIZE_WIDTH / 2
					&& pBullet->pos.y >= pEnemy->pos.y - 30.0f - BULLET_SIZE_HEIGHT / 2
					&& pBullet->pos.y <= pEnemy->pos.y + 30.0f + BULLET_SIZE_HEIGHT / 2)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					pBullet->bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���

											//�����̐ݒ�
					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}
	}
}