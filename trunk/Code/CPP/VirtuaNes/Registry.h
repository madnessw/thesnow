//
// レジストリサポートクラス
//
#ifndef	__CREGISTRY_INCLUDED__
#define	__CREGISTRY_INCLUDED__

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <TCHAR.h>

#include <string>
using namespace std;

class	CRegistry
{
public:
	static	void	SetRegistryKey( LPCWSTR	lpszKey );

	static	UINT	GetProfileInt   ( LPCWSTR lpszSection, LPCWSTR lpszEntry, INT nDefault );
	static	BOOL	GetProfileString( LPCWSTR lpszSection, LPCWSTR lpszEntry, LPVOID lpData, UINT nBytes );
	static	BOOL	GetProfileBinary( LPCWSTR lpszSection, LPCWSTR lpszEntry, LPVOID lpData, UINT nBytes );

	static	BOOL	WriteProfileInt( LPCWSTR lpszSection, LPCWSTR lpszEntry, INT nValue );
	static	BOOL	WriteProfileString( LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszValue );
	static	BOOL	WriteProfileBinary( LPCWSTR lpszSection, LPCWSTR lpszEntry, LPVOID pData, UINT nBytes );

protected:
	static	WCHAR	m_szRegistryKey[MAX_PATH];

	static	HKEY	GetRegistryKey();
	static	HKEY	GetSectionKey( LPCTSTR lpszSection );

private:
};

#endif	// !__CREGISTRY_INCLUDED__
