// Hanafuda.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Hanafuda.h"
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

#define MUTEX_NAME TEXT("Hanafuda Mutex")

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	HANDLE hMutex;

	//多重起動防止
	hMutex = CreateMutex(NULL,TRUE,MUTEX_NAME);
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		return FALSE;
	}

	//GDI+
	GdiplusStartupInput gdiSI;
    ULONG_PTR gdiToken;

	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartup(&gdiToken, &gdiSI, NULL);

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HANAFUDA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HANAFUDA));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	//GDI+終了
	GdiplusShutdown(gdiToken);

	//Mutexの解放
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HANAFUDA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.hbrBackground	= (HBRUSH)CreateSolidBrush(RGB(0, 0xff, 0xff)); 
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HANAFUDA);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	Mediator* md = Mediator::GetInstance();
	HWND hWnd;

	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   int width = md->WND_WIDTH
//  + GetSystemMetrics(SM_CXFIXEDFRAME) // タイトルバー＆固定サイズウィンドウのフレームの幅
//  + GetSystemMetrics(SM_CXFRAME)//ウィンドウの枠の幅
  + GetSystemMetrics(SM_CXEDGE)        // 横の縁の幅(立体)
  + GetSystemMetrics(SM_CXBORDER);      // 横の縁の幅(非立体)
   int height = md->WND_HEIGHT
//  + GetSystemMetrics(SM_CYFIXEDFRAME)  // タイトルバー＆固定サイズウィンドウのフレームの高さ
//  + GetSystemMetrics(SM_CYFRAME)//ウィンドウの枠の高さ
  + GetSystemMetrics(SM_CYEDGE)        // 縦の縁の高さ(立体)
  + GetSystemMetrics(SM_CYBORDER)      // 縦の縁の高さ(非立体)
  + GetSystemMetrics(SM_CYCAPTION)	// タイトルバーの高さ
  + GetSystemMetrics(SM_CYMENU);   //メニューバーの高さ
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
	   CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mediator* md = Mediator::GetInstance();
	int wmId, wmEvent;
	static BOOL bHook = FALSE;
/*	PAINTSTRUCT ps;
	HDC hdc;*/

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_NEWGAME://新しいゲーム
			md->Reset();
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//初期化
		md->Init(hWnd);
		break;
	case WM_MOUSEMOVE:
		md->MouseMove(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		md->MouseClick(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		md->Paint();
//		EndPaint(hWnd, &ps);
		break;
	case WM_SOUND://効果音
		md->Sound(wParam);
		break;
	case WM_SETCURSOR://カーソル変更
		if(md->GetROID()) SetCursor(LoadCursor(NULL,IDC_HAND));
		else SetCursor(LoadCursor(NULL,IDC_ARROW));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
