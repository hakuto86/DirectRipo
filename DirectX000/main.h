#ifndef _MAIN_H_  //���̃}�N����`������ĂȂ�������
#define _MAIN_H_  //2�A�C���N���[�h�̃}�N�����`����

#include <windows.h>
#include "d3dx9.h"  //�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)  //�r���h���̌x���Ώ��p�}�N��
#include  "dinput.h"  //���͏����ɕK�v
#include  "xaudio2.h"  //�T�E���h�����ɕK�v

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")  //�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib") //[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib") //�V�X�e��
#pragma comment(lib,"dinput8.lib") //���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH (1280)              //�E�C���h�E�̕�
#define SCREEN_HEIGHT (720)              //�E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;  //���_���W
	float rhw;   //���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;  //���_�J���[
	D3DXVECTOR2 tex; //�e�N�X�`�����W
}VERTEX_2D;

//���(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,  //�^�C�g�����
	MODE_GAME,       //�Q�[�����
	MODE_RESULT,     //���U���g���
	MODE_CLEAR,     //���U���g���
	MODE_MAX
}MODE;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif