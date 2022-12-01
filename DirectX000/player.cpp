#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "enemy.h"
#include "hp.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;  //�e�N�X�`���̃|�C���^
//VERTEX_2D g_aVertex[4];  //���_�����i�[
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;  //���_�o�b�t�@�ւ̃|�C���^
int g_nCounterAnimPlayer;  //�A�j���[�V�����J�E���^�[
int g_nPatternAnimPlayer;  //�A�j���[�V�����p�^�[��No.
float g_fLengthPlayer;    //�Ίp���̒���
float g_fAnglePlayer;     //�Ίp���̊p�x
Player g_player;

Player *GetPlayer(void)
{
	return &g_player;
}

//==================================================================
//�v���C���[�������̏���
//==================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	g_nCounterAnimPlayer = 0;  //�J�E���^�[������������
	g_nPatternAnimPlayer = 0;  //�p�^�[��No.������������
	g_player.pos = D3DXVECTOR3(500.0f,500.0f,0.0f); //�v���C���[�̏����ʒu
	g_player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);   //�v���C���[�̏����ړ���
	g_player.nLife = 4;                            //�v���C���[�̗̑�
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //����������������(Z�l���g�p)
	g_fLengthPlayer = sqrtf(PLAYER_SIZE_WIDTH * PLAYER_SIZE_WIDTH + PLAYER_SIZE_HEIGHT * PLAYER_SIZE_HEIGHT)* 0.5f;  //�Ίp���̒������Z�o����
	g_fAnglePlayer = atan2f(140.0f, 200.0f);  //�Ίp���̊p�x���Z�o����
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.nCounterState = 0;
	g_player.bDisp = true;
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player.png", &g_pTexturePlayer);
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, -PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+PLAYER_SIZE_WIDTH, -PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, +PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+PLAYER_SIZE_WIDTH, +PLAYER_SIZE_HEIGHT, 0.0f);

	////��]�̏���
	//pVtx[0].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[0].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[0].pos.z = 0.0f;

	//pVtx[1].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[1].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[1].pos.z = 0.0f;

	//pVtx[2].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[2].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[2].pos.z = 0.0f;

	//pVtx[3].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[3].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)) / PATTERN_WIDTH,
		(float)(g_nPatternAnimPlayer / PATTERN_WIDTH) / PATTERN_HEIGHT);
	pVtx[1].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH) + 1) / PATTERN_WIDTH,
		(float)(g_nPatternAnimPlayer / PATTERN_WIDTH) / PATTERN_HEIGHT);
	pVtx[2].tex = D3DXVECTOR2((float)(g_nPatternAnimPlayer % PATTERN_WIDTH) / PATTERN_WIDTH,
		((float)(g_nPatternAnimPlayer / PATTERN_WIDTH) + 1) / PATTERN_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH) + 1) / PATTERN_WIDTH,
		((float)(g_nPatternAnimPlayer / PATTERN_WIDTH) + 1) / PATTERN_HEIGHT);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	AddHp(g_player.nLife);
}

//==================================================================
//�v���C���[�̏I������
//==================================================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

#if 1
//==================================================================
//�v���C���[�̍X�V����
//==================================================================
void UpdatePlayer(void)
{
	//���_�o�b�t�@�̐���
	VERTEX_2D *pVtx;

	if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[�������ꂽ

		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * MOVE;
		}
		else
		{
			g_player.move.x -= MOVE;
		}
	}

	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[�������ꂽ
		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ
			g_player.move.x += sinf( D3DX_PI * 0.75f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
			g_player.move.x += sinf( D3DX_PI * 0.25f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * MOVE;
		}
		else
		{
			g_player.move.x += MOVE;
		}
	}

	else if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[�������ꂽ
		g_player.move.y -= MOVE;
	}
	
	else if (GetKeyboardPress(DIK_S) == true)
	{//S�L�[�������ꂽ
		g_player.move.y += MOVE;
	}	 

	//��ʊO�ɏo��
	if (g_player.pos.x >= SCREEN_WIDTH - 100)
	{
		g_player.pos.x = SCREEN_WIDTH - 100;
	}

	if (g_player.pos.y >= SCREEN_HEIGHT -30)
	{
		g_player.pos.y = SCREEN_HEIGHT -30;
	}

	if (g_player.pos.x <= 0 + 150)
	{
		g_player.pos.x = 0 +150;
	}

	if (g_player.pos.y <= 0 +65)
	{
		g_player.pos.y = 0 +65;
	}

	//�ʒu���X�V
	g_player.pos.x +=g_player.move.x;
	g_player.pos.y +=g_player.move.y;

	//�ړ��ʂ��X�V(����������)
	g_player.move.x += (0.0f - g_player.move.x) * 0.1f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.1f;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_player.pos.x - PLAYER_SIZE_WIDTH;
	pVtx[0].pos.y = g_player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[1].pos.x = g_player.pos.x + PLAYER_SIZE_WIDTH;
	pVtx[1].pos.y = g_player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[2].pos.x = g_player.pos.x - PLAYER_SIZE_WIDTH;
	pVtx[2].pos.y = g_player.pos.y + PLAYER_SIZE_HEIGHT;
	pVtx[3].pos.x = g_player.pos.x + PLAYER_SIZE_WIDTH;
	pVtx[3].pos.y = g_player.pos.y + PLAYER_SIZE_HEIGHT;

	g_nCounterAnimPlayer++; //�J�E���^�[�����Z

	if ((g_nCounterAnimPlayer % 10) == 0)
	{//��莞�Ԍo��
		//�p�^�[��No.���X�V����
		g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % 10;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH))/ PATTERN_WIDTH,
			(float)(g_nPatternAnimPlayer / PATTERN_WIDTH)/ PATTERN_HEIGHT);
		pVtx[1].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)+1)/ PATTERN_WIDTH,
			(float)(g_nPatternAnimPlayer / PATTERN_WIDTH)/ PATTERN_HEIGHT);
		pVtx[2].tex = D3DXVECTOR2((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)/ PATTERN_WIDTH,
			((float)(g_nPatternAnimPlayer / PATTERN_WIDTH)+1)/ PATTERN_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)+1)/ PATTERN_WIDTH,
			((float)(g_nPatternAnimPlayer / PATTERN_WIDTH)+1)/ PATTERN_HEIGHT);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPlayer->Unlock();
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACE�L�[�������ꂽ
		 //�e�̐ݒ�
			SetBullet(D3DXVECTOR3(g_player.pos.x + 180.0f, g_player.pos.y - 20.0f, g_player.pos.z), D3DXVECTOR3(BULLET_LIGHT, 0.0f, 0.0f), 100, BULLETTYPE_PLAYER);
		}
		break;

	case PLAYERSTATE_DAMAGE:

		g_player.nCounterState--;

		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (g_player.nCounterState % 5 == 0)
		{
			g_player.bDisp = g_player.bDisp ? false : true;
			
			if (g_player.nCounterState <= 0)
			{
				g_player.bDisp = true;
			}
		}
		break;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}
#endif

//==================================================================
//�v���C���[�̕`�揈��
//==================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	    //�f�o�C�X�̎擾
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	

	//�v���C���[�̕`��
	if (g_player.bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==================================================================
//�v���C���[�̃q�b�g����
//==================================================================
void HitPlayer(int nDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL && g_player.nCounterState == 0)
	{
		g_player.nLife -= nDamage;
		AddHp(-1);
	}

	if (g_player.nLife <= 0)
	{		
		//�����̐ݒ�
		SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		g_player.state = PLAYERSTATE_DEATH;

		//�Q�[���̏�Ԃ̐ݒ�
		SetGameState(GAMESTATE_END, 60);
		g_player.bDisp = false;
	}

	else
	{
		VERTEX_2D * pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPlayer->Unlock();

		if (g_player.state == PLAYERSTATE_NORMAL && g_player.nCounterState == 0)
		{
			g_player.state = PLAYERSTATE_DAMAGE;
			g_player.nCounterState = 100;
		}
	}
}