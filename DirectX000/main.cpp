#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "clear.h"
#include "fade.h"
#include "sound.h"

//マクロ定義
#define CLASS_NAME "windowsClass"        //ウインドウクラスの名前
#define WINDOW_NAME "Fish Hunting"       //ウインドウの名前
#define SCREEN_WIDTH (1280)              //ウインドウの幅
#define SCREEN_HEIGHT (720)              //ウインドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;              //Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9  g_pD3DDevice = NULL; //Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;  //フォントへのポインタ
int g_nCountFPS = 0;  //FPSカウンタ
bool g_bDispDebug = false;  //デバッグ表示のON/OFF
MODE g_mode = MODE_TITLE;  //現在のモード
bool g_bPause = false;  //ポーズ状態のON/OFF

//=======================================================================
//メイン関数
//=======================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;   //現在時刻
	DWORD dwExecLastTime;  //現在時刻
	DWORD dwFrameCount;    //フレームカウント
	DWORD dwFPSLastTime;           //最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),            //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                    //ウインドウのスタイル
		WindowProc,                    //ウインドウのプロシージャ
		0,                             //0にする(通常は使用しない)
		0,                             //0にする(通常は使用しない)
		hInstance,                     //インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),    //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),    //クライアント領域の背景色
		NULL,                          //メニューバー
		CLASS_NAME,                    //ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION) //ファイルのアイコン
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };   //画面サイズの構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, (TRUE))))
	{//初期化処理が失敗した場合
		return-1;
	}
	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;  //初期化する
	dwExecLastTime = timeGetTime();  //現在時刻を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//DirectXの処理
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;  //FPSを測定した時刻を保存
				dwFrameCount = 0;  //フレームカウントをクリア
			}

			dwCurrentTime = timeGetTime();  //現在時刻を取得

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;  //処理開始の時刻を保存
				dwFrameCount++;  //フレームカウントを加算

				//更新処理				 
				Update();

				//描画処理
				Draw();
			}
		}
	}

	//終了処理
	Uninit();

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam)
{
	int nID;
	
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する
				DestroyWindow(hWnd);
			}
			break;

		case WM_CLOSE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;  //0を返さないと終了してしまう
			}
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}

//==================================================================
//初期化処理
//==================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;        //ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp; //プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));                         //パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;                      //画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                    //画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;                     //バックバッファの形式
	d3dpp.BackBufferCount = 1;                                 //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;				   //ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;					   //デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				   //デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;								   //ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//リフレッシュルート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;  //インターバル

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DDEVTYPE_REF, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice,18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	InitSound(hWnd);
	PlaySound(SOUND_LABEL_BGM000);

	//フェードの設定
	InitFade(g_mode);

	return S_OK;
}

//==================================================================
//終了処理
//==================================================================
void Uninit(void)
{
	//タイトル画面の終了処理
	UninitTitle();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//リザルト画面の終了処理
	UninitClear();

	//フェードの終了処理
	UninitFade();

	//分解能を戻す
	timeEndPeriod(1);

	//キーボードの終了処理
	UninitKeyboard();

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	StopSound();
	UninitSound();
}

//==================================================================
//更新処理
//==================================================================
void Update(void)
{
	UpdateKeyboard();

	if (GetKeyboardTrigger(DIK_P) == true)
	{//Pキーが押された
		g_bPause ^= 1;
	}
	if (g_bPause == false)
	{
		switch (g_mode)
		{
		case MODE_TITLE:   //タイトル画面
			UpdateTitle();
			break;

		case MODE_GAME:    //ゲーム画面
			UpdateGame();
			break;

		case MODE_RESULT:  //リザルト画面
			UpdateResult();
			break;

		case MODE_CLEAR:  //リザルト画面
			UpdateClear();
			break;
		}
		//フェードの更新処理
		UpdateFade();
	}
}

//==================================================================
//描画処理
//==================================================================
void Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合
		//FPSの表示
		DrawFPS();
		
		switch (g_mode)
		{
		case MODE_TITLE:   //タイトル画面
			DrawTitle();
			break;

		case MODE_GAME:    //ゲーム画面
			DrawGame();
			break;

		case MODE_RESULT:  //リザルト画面
			DrawResult();
			break;

		case MODE_CLEAR:  //リザルト画面
			DrawClear();
			break;
		}

		//フェードの描画処理
		DrawFade();

#ifdef _DEBUG
		//FPSの表示
		DrawFPS();
#endif
		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==================================================================
//FPS描画処理
//==================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//==================================================================
//モードの設定
//==================================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:   //タイトル画面
		UninitTitle();
		break;

	case MODE_GAME:    //ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:  //リザルト画面
		UninitResult();
		break;

	case MODE_CLEAR:  //リザルト画面
		UninitClear();
		break;
	}

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:   //タイトル画面
		InitTitle();
		break;

	case MODE_GAME:    //ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:  //リザルト画面
		InitResult();
		break;

	case MODE_CLEAR:  //リザルト画面
		InitClear();
		break;
	}
	g_mode = mode;  //現在の画面(モード)を切り替える
}

//==================================================================
//モードの取得
//==================================================================
MODE GetMode(void)
{
	return g_mode;
}

//==================================================================
//デバイスの取得
//==================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}