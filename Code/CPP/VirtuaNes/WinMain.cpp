#define	INITGUID
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <crtdbg.h>

#include <string>
using namespace std;

#include "DebugOut.h"
#include "VirtuaNESres.h"

#include "App.h"
#include "Registry.h"
#include "Pathlib.h"
#include "MMTimer.h"

#include "Wnd.h"
#include "WndHook.h"
#include "MainFrame.h"
#include "Plugin.h"
#include "Config.h"
#include "Recent.h"

#include "DirectDraw.h"
#include "DirectSound.h"
#include "DirectInput.h"

INT WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow )
{
#if	_DEBUG
	// メモリリーク検出
	int	Flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	Flag |= _CRTDBG_LEAK_CHECK_DF;
	Flag &= ~_CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag( Flag );
#endif
	// メインフレームウインドウオブジェクト
	CMainFrame	MainFrame;

	// Mutex
	HANDLE	hMutex = NULL;

	// アプリケーションインスタンス等の設定
	TCHAR	szPath[MAX_PATH];
	GetModuleFileName( hInstance, szPath, sizeof(szPath) );
	wstring	ModulePath = CPathlib::SplitPath( szPath );
	CApp::SetModulePath( ModulePath.c_str() );
	DEBUGOUT( L"Module Path:\"%s\"\n",ModulePath.c_str() );
	CApp::SetInstance( hInstance );
	CApp::SetPrevInstance( hPrevInstance );
	CApp::SetCmdLine( lpCmdLine );
	CApp::SetCmdShow( nCmdShow );

//DEBUGOUT( "ThreadID:%08X\n", ::GetCurrentThreadId() );

//	CRegistry::SetRegistryKey( "Emulators\\VirtuaNES" );
	CRegistry::SetRegistryKey( L"VirtuaNES.ini" );

	if( !CPlugin::FindPlugin( CApp::GetModulePath() ) ) {
//		::MessageBox( NULL, L"语言插件未找到.", L"VirtuaNES", MB_ICONERROR|MB_OK );
//		goto	_Error_Exit;
	}
	DEBUGOUT( L"Plugin Path:\"%s\"\n", CPlugin::GetPluginPath() );
	DEBUGOUT( L"Language   :\"%s\"\n", CPlugin::GetPluginLanguage() );
	DEBUGOUT( L"LCID       :\"%d\" \"0x%04X\"\n", CPlugin::GetPluginLocaleID(), CPlugin::GetPluginLocaleID() );

	HINSTANCE hPlugin;
	if( !(hPlugin = CPlugin::LoadPlugin()) ) {
//		::MessageBox( NULL, L"语言插件载入失败.", L"VirtuaNES", MB_ICONERROR|MB_OK );
//		goto	_Error_Exit;
	}
	else
	{
		hPlugin=hInstance;
	}
	CApp::SetPlugin( hPlugin );

	::InitCommonControls();

	// 設定のロード
	CRegistry::SetRegistryKey( L"VirtuaNES.ini" );
	Config.Load();
	CRecent::Load();

	// 二重起動の防止
	hMutex = ::CreateMutex( NULL, FALSE, VIRTUANES_MUTEX );
	if( ::GetLastError() == ERROR_ALREADY_EXISTS ) {
		::CloseHandle( hMutex );
		if( Config.general.bDoubleExecute ) {
			HWND	hWnd = ::FindWindow( VIRTUANES_WNDCLASS, NULL );
//			HWND	hWnd = ::FindWindow( VIRTUANES_WNDCLASS, VIRTUANES_CAPTION );

			TCHAR	szTitle[256];
			::GetWindowText( hWnd, szTitle, sizeof(szTitle)-1 );

			// タイトルバーが同じかどうかチェック
			if( ::_tcsncmp( szTitle, VIRTUANES_CAPTION, ::_tcslen(VIRTUANES_CAPTION) ) == 0 ) {
				// 起動していた方をフォアグラウンドにする
				::SetForegroundWindow( hWnd );

				// コマンドライン引数があるなら動作中のVirtuaNESのウインドウにファイル名
				// メッセージを送りつけてそちらで動作させる
				// (当然の様に対応バージョンでないとダメ)
				if( ::_tcslen( lpCmdLine ) > 0 ) {
					TCHAR	szCmdLine[_MAX_PATH];
					::_tcscpy( szCmdLine, lpCmdLine );
					::PathUnquoteSpaces( szCmdLine );

					COPYDATASTRUCT	cds;
					cds.dwData = 0;
					cds.lpData = (void*)szCmdLine;
					cds.cbData = ::_tcslen(szCmdLine)+1; //  終端のNULLも送る
					//  文字列送信
					::SendMessage( hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds );
				}

				// 終了
				goto	_DoubleExecute_Exit;
			}
		}
	}

	if( !MainFrame.Create(NULL) )
		goto	_Error_Exit;
	DEBUGOUT( L"CreateWindow 成功.\n" );

	// メインウインドウの表示
	::ShowWindow( CApp::GetHWnd(), CApp::GetCmdShow() );
	::UpdateWindow( CApp::GetHWnd() );

	// フック
	CWndHook::Initialize();

	// ランチャー同時起動
	if( Config.general.bStartupLauncher ) {
		::PostMessage( CApp::GetHWnd(), WM_COMMAND, ID_LAUNCHER, 0 );
	}

	// コマンドライン
	if( ::_tcslen( lpCmdLine ) > 0 ) {
		LPTSTR	pCmd = lpCmdLine;
		if( lpCmdLine[0] == L'"' ) {	// Shell execute!!
			lpCmdLine++;
			if( lpCmdLine[::_tcslen( lpCmdLine )-1] == _T('"') ) {
				lpCmdLine[::_tcslen( lpCmdLine )-1] = _T('\0');
			}
		}
	}

	if( ::_tcslen( (LPTSTR)lpCmdLine ) > 0 ) {
		::PostMessage( CApp::GetHWnd(), WM_VNS_COMMANDLINE, 0, (LPARAM)lpCmdLine );
	}
	else
	{
/*
		//加载自己为一个ROM
		TCHAR SelfPath[MAX_PATH];
		GetModuleFileName(NULL,SelfPath,MAX_PATH);
		::PostMessage( CApp::GetHWnd(), WM_VNS_COMMANDLINE, 0, (LPARAM)SelfPath );
*/
	}

	MSG	msg;
	BOOL	bRet;
	while( (bRet = ::GetMessage( &msg, NULL, 0, 0 )) != 0 ) {
		// エラー？
		if( bRet == -1 )
			break;
		// メインウインドウのメッセージフィルタリング
		if( CApp::GetHWnd() == msg.hwnd ) {
			CWnd* pWnd = (CWnd*)::GetWindowLong( msg.hwnd, GWL_USERDATA );
			if( pWnd ) {
				if( pWnd->PreTranslateMessage( &msg ) )
					continue;
			}
		}
		if( CWndList::IsDialogMessage( &msg ) )
			continue;
		::TranslateMessage( &msg );
		::DispatchMessage( &msg );
	}
	// 释放 HOOK
	CWndHook::Release();

	// 保存设置
	CRegistry::SetRegistryKey( L"VirtuaNES.ini" );
	Config.Save();
	CRecent::Save();

	// 释放 Direct X
	DirectDraw.ReleaseDDraw();
	DirectSound.ReleaseDSound();
	DirectInput.ReleaseDInput();

	if( hMutex )
		::ReleaseMutex( hMutex );
	CLOSEHANDLE( hMutex );

_DoubleExecute_Exit:
	::FreeLibrary( CApp::GetPlugin() );

	return	msg.wParam;

_Error_Exit:
	// 释放 DirectX
	DirectDraw.ReleaseDDraw();
	DirectSound.ReleaseDSound();
	DirectInput.ReleaseDInput();

	if( CApp::GetPlugin() ) {
		::FreeLibrary( CApp::GetPlugin() );
	}

	return	-1;
}

