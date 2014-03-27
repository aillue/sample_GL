// sample_GL.cpp : アプリケーションのエントリ ポイントを定義します。
// 参考：http://marupeke296.com/OGL_No1_Install.html
#include "stdafx.h"
#include "Resource.h"

#pragma comment(lib, "OpenGL32.lib")

#define MAX_LOADSTRING 100

class MyOpenGL
{
private:
	HWND hWnd_;
	HDC  dc_;
	HGLRC glRC_;

public:
	enum RESULT
	{
		OK,
		NO_PIXEL_FORMAT,
		INVALID_PIXEL_FORMAT,
		NO_DEVICE_CONTEXT,
	};

	MyOpenGL() : hWnd_( 0 ) {}
	~MyOpenGL()
	{
		Finalize();
	}

	RESULT Initialize( HWND hWnd )
	{
		hWnd_ = hWnd;
		dc_ = GetDC( hWnd );

		// OpenGL初期化
		// ピクセルフォーマット初期化
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof( PIXELFORMATDESCRIPTOR ),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
			PFD_TYPE_RGBA, //The kind of framebuffer. RGBA or palette.
			32, //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24, //Number of bits for the depthbuffer
			8, //Number of bits for the stencilbuffer
			0, //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		int format = ChoosePixelFormat( dc_, &pfd );
		if ( format == 0 )
		{
			return NO_PIXEL_FORMAT;
		}

		// ピクセルフォーマットをデバイスコンテキストに設定
		if ( !SetPixelFormat( dc_, format, &pfd ) )
		{
			return INVALID_PIXEL_FORMAT;
		}

		// OpenGL contextを作成
		glRC_ = wglCreateContext( dc_ );

		if ( !wglMakeCurrent( dc_, glRC_ ) )
		{
			return NO_DEVICE_CONTEXT;
		}

		return OK;
	}

	void Finalize()
	{
		// カレントコンテキストを無効にする
		wglMakeCurrent( NULL, NULL );

		// カレントコンテキストを削除
		wglDeleteContext( glRC_ );

		// デバイスコンテキスト解放
		ReleaseDC( hWnd_, GetDC( hWnd_ ) );
	}

	void Begin()
	{
		wglMakeCurrent( dc_, glRC_ );
	}
	void End()
	{
		glFlush();
		SwapBuffers( dc_ );
		wglMakeCurrent( NULL, NULL );
	}
};

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
HGLRC glRC;
MyOpenGL *my_gl = NULL;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass( HINSTANCE hInstance );
BOOL				InitInstance( HINSTANCE, int, MyOpenGL *pGL );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

int APIENTRY _tWinMain( HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( hInstance, IDC_SAMPLE_GL, szWindowClass, MAX_LOADSTRING );
	MyRegisterClass( hInstance );

	my_gl = new MyOpenGL();

	// アプリケーションの初期化を実行します:
	if ( !InitInstance ( hInstance, nCmdShow, my_gl ) )
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SAMPLE_GL ) );

	// メイン メッセージ ループ:
	do
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			my_gl->Begin();

			glClearColor( 0.0f, 0.5f, 1.0f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT );

			glRectf( -0.5f, -0.5f, 0.5f, 0.5f );

			my_gl->End();
		}
	}
	while( msg.message != WM_QUIT );

	// 後処理

	delete my_gl;

	return ( int ) msg.wParam;
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
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SAMPLE_GL ) );
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= ( HBRUSH )( COLOR_WINDOW+1 );
	wcex.lpszMenuName	= MAKEINTRESOURCE( IDC_SAMPLE_GL );
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassEx( &wcex );
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
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow, MyOpenGL *pGL )
{
	HWND hWnd;

	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	                     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL );

	if ( !hWnd )
	{
		return FALSE;
	}

	if( pGL->Initialize( hWnd ) != MyOpenGL::OK )
	{
		return FALSE;
	}

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

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
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( message )
	{
	case WM_COMMAND:
		wmId    = LOWORD( wParam );
		wmEvent = HIWORD( wParam );
		// 選択されたメニューの解析:
		switch ( wmId )
		{
		case IDM_ABOUT:
			DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
			break;
		case IDM_EXIT:
			DestroyWindow( hWnd );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		// TODO: 描画コードをここに追加してください...
		EndPaint( hWnd, &ps );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( message )
	{
	case WM_INITDIALOG:
		return ( INT_PTR )TRUE;

	case WM_COMMAND:
		if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
		{
			EndDialog( hDlg, LOWORD( wParam ) );
			return ( INT_PTR )TRUE;
		}
		break;
	}
	return ( INT_PTR )FALSE;
}

