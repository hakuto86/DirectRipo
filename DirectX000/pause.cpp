//#include "main.h"
//#include "pause.h"
//#include "fade.h"
//#include "input.h"
//
////�O���[�o���ϐ�
//LPDIRECT3DTEXTURE9 g_apTexturePause[3] = {};  //�e�N�X�`���̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;  //���_�o�b�t�@�ւ̃|�C���^
//PAUSE_MENU g_pauseMenu;
//
////==================================================================
////�|�[�Y�̏������̏���
////==================================================================
//void InitPause(void)
//{
//	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
//
//	pDevice = GetDevice();     //�f�o�C�X�̎擾
//
//	//�e�N�X�`���̓ǂݍ���
//	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title.png", &g_pTexturePause);
//
//	//���_�o�b�t�@�̐���
//	pDevice->CreateVertexBuffer(
//		sizeof(VERTEX_2D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffPause,
//		NULL);
//	VERTEX_2D * pVtx;
//
//	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
//
//	//���_���W�̐ݒ�
//	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
//	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
//	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//	//rhw�̐ݒ�
//	pVtx[0].rhw = 1.0f;
//	pVtx[1].rhw = 1.0f;
//	pVtx[2].rhw = 1.0f;
//	pVtx[3].rhw = 1.0f;
//
//	//���_�J���[�̐ݒ�
//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//	//�e�N�X�`�����W�̐ݒ�
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	//���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffPause->Unlock();
//}
//
////==================================================================
////�|�[�Y�̏I������
////==================================================================
//void UninitPause(void)
//{
//	//�e�N�X�`���̔j��
//	if (g_pTexturePause != NULL)
//	{
//		g_pTexturePause->Release();
//		g_pTexturePause = NULL;
//	}
//
//	//���_�o�b�t�@�̔j��
//	if (g_pVtxBuffPause != NULL)
//	{
//		g_pVtxBuffPause->Release();
//		g_pVtxBuffPause = NULL;
//	}
//}
//
////==================================================================
////�|�[�Y�̍X�V����
////==================================================================
//void UpdateTitle(void)
//{
//	if (GetKeyboardTrigger(DIK_RETURN) == true)
//	{//����L�[(ENTER�L�[)�������ꂽ
//	 //���[�h�ݒ�(�Q�[����ʂɈڍs)
//		SetFade(MODE_GAME);
//	}
//}
//
////==================================================================
////�|�[�Y�̕`�揈��
////==================================================================
//void DrawTitle(void)
//{
//	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
//
//	pDevice = GetDevice();	    //�f�o�C�X�̎擾
//
//								//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));
//
//	//���_�̃t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	//�e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, g_pTextureTitle);
//
//	//�|���S���̕`��
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//}