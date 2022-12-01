#include "main.h"
#include "fade.h"
#include "title.h"
#include "input.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_Title] = {};  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;  //���_�o�b�t�@�ւ̃|�C���^
float g_aTexV1[NUM_Title];
Title g_nTitle[NUM_Title];
int nCntT;
bool bFlash;

//==================================================================
//�^�C�g����ʂ̏������̏���
//==================================================================
void InitTitle(void)
{
	int nCntTitle;
	int nCntT = 0;
	bFlash = false;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
		   
	pDevice = GetDevice();     //�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\-PRESS ENTER-.png", &g_pTextureTitle[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_Title,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < NUM_Title; nCntTitle++)
	{
		g_nTitle[nCntTitle].bUse = true;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;   //���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//============================================================0======
//�^�C�g����ʂ̏I������
//==================================================================
void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}

//==================================================================
//�^�C�g����ʂ̍X�V����
//==================================================================
void UpdateTitle(void)
{
	nCntT++;

	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{//����L�[(ENTER�L�[)�������ꂽ
		bFlash = true;
		//���[�h�ݒ�(�Q�[����ʂɈڍs)
		SetFade(MODE_GAME);
	}

	if (nCntT >= 26 && bFlash == false)
	{
		g_nTitle[1].bUse = g_nTitle[1].bUse ? false : true;
		nCntT = 0;
	}
	else if (nCntT >= 6 && bFlash == true)
	{
		g_nTitle[1].bUse = g_nTitle[1].bUse ? false : true;
		nCntT = 0;
	}
}

//==================================================================
//�^�C�g����ʂ̕`�揈��
//==================================================================
void DrawTitle(void)
{
	int nCntTitle;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (nCntTitle = 0; nCntTitle < NUM_Title; nCntTitle++)
	{
		if (g_nTitle[nCntTitle].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 4);
		}
	}
}