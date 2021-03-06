// ## MenuContainer.h
// Classic Shell (c) 2009-2011, Ivo Beltchev
// The sources for Classic Shell are distributed under the MIT open source license

// MenuCommands.cpp - handles the commands and actions of CMenuContainer

#include "stdafx.h"
#include "MenuContainer.h"
#include "ClassicStartMenuDLL.h"
#include "Settings.h"
#include "SettingsUI.h"
#include "SettingsUIHelper.h"
#include "Translations.h"
#include "LogManager.h"
#include "FNVHash.h"
#include "ResourceHelper.h"
#include "resource.h"
#include <WtsApi32.h>
#include <PowrProf.h>
#include <algorithm>

static CString g_RenameText;
static const POINT *g_RenamePos;

// Dialog proc for the Rename dialog box
static INT_PTR CALLBACK RenameDlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg==WM_INITDIALOG)
	{
		// translate text
		SetWindowText(hwndDlg,FindTranslation(L"Menu.RenameTitle",L"Rename"));
		SetDlgItemText(hwndDlg,IDC_LABEL,FindTranslation(L"Menu.RenamePrompt",L"&New name:"));
		SetDlgItemText(hwndDlg,IDOK,FindTranslation(L"Menu.RenameOK",L"OK"));
		SetDlgItemText(hwndDlg,IDCANCEL,FindTranslation(L"Menu.RenameCancel",L"Cancel"));
		SetDlgItemText(hwndDlg,IDC_EDITNAME,g_RenameText);
		if (g_RenamePos)
		{
			// position near the item
			SetWindowPos(hwndDlg,NULL,g_RenamePos->x,g_RenamePos->y,0,0,SWP_NOZORDER|SWP_NOSIZE);
			SendMessage(hwndDlg,DM_REPOSITION,0,0);
		}
		return TRUE;
	}
	if (uMsg==WM_COMMAND && wParam==IDOK)
	{
		wchar_t buf[1024];
		GetDlgItemText(hwndDlg,IDC_EDITNAME,buf,_countof(buf));
		g_RenameText=buf;

		EndDialog(hwndDlg,1);
		return TRUE;
	}
	if (uMsg==WM_COMMAND && wParam==IDCANCEL)
	{
		EndDialog(hwndDlg,0);
		return TRUE;
	}
	return FALSE;
}

static void SetShutdownPrivileges( void )
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
	{
		TOKEN_PRIVILEGES tp={1};
		if (LookupPrivilegeValue(NULL,L"SeShutdownPrivilege",&tp.Privileges[0].Luid))
			tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL); 
		CloseHandle(hToken);
	}
}

static void DoSearchSubst( wchar_t *buf, int size, const wchar_t *search )
{
	wchar_t search2[256];
	char utf8[1024];
	WcsToMbs(utf8,_countof(utf8),search,CP_UTF8);
	int len=0;
	for (const char *c=utf8;*c;c++)
	{
		if ((*c>='a' && *c<='z') || (*c>='A' && *c<='Z') || (*c>='0' && *c<='9'))
		{
			search2[len++]=*c;
		}
		else if (len<_countof(search2)-4)
		{
			len+=Sprintf(search2+len,_countof(search2)-len,L"%%%02X",(unsigned char)*c);
		}
		else
			break;
	}
	search2[len]=0;
	DWORD_PTR args[100]={(DWORD_PTR)search,(DWORD_PTR)search2};
	wchar_t *pBuf=buf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_ARGUMENT_ARRAY|FORMAT_MESSAGE_FROM_STRING,buf,0,0,(LPWSTR)&pBuf,0,(va_list*)args);
	Strcpy(buf,size,pBuf);
	LocalFree(pBuf);
}

static void ExecuteCommand( const wchar_t *command )
{
	wchar_t exe[_MAX_PATH];
	const wchar_t *args=SeparateArguments(command,exe);
	ShellExecute(NULL,NULL,exe,args,NULL,SW_SHOWNORMAL);
}

// Dialog proc for the Log Off dialog box
static INT_PTR CALLBACK LogOffDlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg==WM_INITDIALOG)
	{
		// translate text
		SendDlgItemMessage(hwndDlg,IDC_STATICICON1,STM_SETICON,lParam,0);
		SetWindowText(hwndDlg,FindTranslation(L"Menu.LogoffTitle",L"Log Off Windows"));
		SetDlgItemText(hwndDlg,IDC_PROMPT,FindTranslation(L"Menu.LogoffPrompt",L"Are you sure you want to log off?"));
		SetDlgItemText(hwndDlg,IDOK,FindTranslation(L"Menu.LogoffYes",L"&Log Off"));
		SetDlgItemText(hwndDlg,IDCANCEL,FindTranslation(L"Menu.LogoffNo",L"&No"));
		return TRUE;
	}
	if (uMsg==WM_COMMAND && wParam==IDOK)
	{
		EndDialog(hwndDlg,1);
		return TRUE;
	}
	if (uMsg==WM_COMMAND && wParam==IDCANCEL)
	{
		EndDialog(hwndDlg,0);
		return TRUE;
	}
	return FALSE;
}

struct ShortcutParams
{
	wchar_t target[_MAX_PATH+1];
	wchar_t temp[_MAX_PATH];
	wchar_t fname[_MAX_PATH+1];
};

static DWORD WINAPI NewShortcutThread( void *param )
{
	ShortcutParams *pParams=(ShortcutParams*)param;
	HANDLE hFile=CreateFile(pParams->fname,0,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		// run the shortcut wizard
		wchar_t cmdLine[1024];
		Sprintf(cmdLine,_countof(cmdLine),L"rundll32.exe appwiz.cpl,NewLinkHere %s",pParams->fname);

		STARTUPINFO startupInfo={sizeof(startupInfo)};
		PROCESS_INFORMATION processInfo;
		memset(&processInfo,0,sizeof(processInfo));
		wchar_t exe[_MAX_PATH]=L"%windir%\\system32\\rundll32.exe";
		DoEnvironmentSubst(exe,_countof(exe));
		if (CreateProcess(exe,cmdLine,NULL,NULL,FALSE,0,NULL,pParams->temp,&startupInfo,&processInfo))
		{
			CloseHandle(processInfo.hThread);
			WaitForSingleObject(processInfo.hProcess,INFINITE);
			CloseHandle(processInfo.hProcess);

			// see what the file was renamed to
			struct {
				DWORD  FileNameLength;
				wchar_t FileName[_MAX_PATH];
			} nameInfo={0};
			BOOL bInfo=GetFileInformationByHandleEx(hFile,FileNameInfo,&nameInfo,sizeof(nameInfo));
			CloseHandle(hFile);
			if (bInfo)
			{
				// move to the final target folder
				int len=Sprintf(pParams->fname,_countof(pParams->fname)-1,L"%s\\%s",pParams->temp,PathFindFileName(nameInfo.FileName));
				pParams->fname[len+1]=0;
				SHFILEOPSTRUCT shfop={g_OwnerWindow,FO_MOVE,pParams->fname,pParams->target};
				SHFileOperation(&shfop);
			}
		}
		else
			CloseHandle(hFile);
		DeleteFile(pParams->fname);
	}
	delete pParams;
	return 0;
}

static DWORD WINAPI SleepThread( void *param )
{
	SetSuspendState((BOOL)param,FALSE,FALSE);
	return 0;
}

// This function "activates" an item. The item can be activated in multiple ways:
// ACTIVATE_SELECT - select the item, make sure it is visible
// ACTIVATE_OPEN - if the item is a submenu, it is opened. otherwise the item is just selected (but all submenus are closed first)
// ACTIVATE_OPEN_KBD - same as above, but when done with a keyboard
// ACTIVATE_OPEN_SEARCH - opens the search results submenu
// ACTIVATE_EXECUTE - executes the item. it can be a shell item or a command item
// ACTIVATE_MENU - shows the context menu for the item
void CMenuContainer::ActivateItem( int index, TActivateType type, const POINT *pPt )
{
	LOG_MENU(LOG_EXECUTE,L"Activate Item, ptr=%p, index=%d, type=%d",this,index,type);
	if (index<0)
	{
		if (type==ACTIVATE_SELECT)
		{
			if (!(m_Options&CONTAINER_SEARCH))
				SetFocus();
			SetHotItem(-1);
		}
		return;
	}

	MenuItem &item=m_Items[index];

	if (type==ACTIVATE_SELECT)
	{
		// set the hot item
		if (item.id==MENU_SEARCH_BOX)
		{
			m_SearchBox.SetFocus();
			SetHotItem(-1);
		}
		else
		{
			if (!(m_Options&CONTAINER_SEARCH))
				SetFocus();
			SetHotItem(index);
		}
		if (m_ScrollHeight>0 && index<m_ScrollCount)
		{
			// scroll the pager to make this item visible
			RECT rc=item.itemRect;
			OffsetRect(&rc,0,-m_rContent.top);
			int pos=m_ScrollOffset;
			int total=m_Items[m_ScrollCount-1].itemRect.bottom-m_rContent.top-m_ScrollHeight;
			if (rc.top<m_ScrollOffset)
				pos=rc.top;
			else if (rc.bottom+m_ScrollButtonSize>m_ScrollOffset+m_ScrollHeight)
				pos=rc.bottom+m_ScrollButtonSize-m_ScrollHeight;
			if (pos<0) pos=0;
			if (pos>total) pos=total;
			if (m_ScrollOffset!=pos)
			{
				m_ScrollOffset=pos;
				UpdateScroll();
				Invalidate();
			}
		}
		return;
	}

	if (type==ACTIVATE_OPEN || type==ACTIVATE_OPEN_KBD || type==ACTIVATE_OPEN_SEARCH)
	{
		if (item.id==MENU_SEARCH_BOX && type!=ACTIVATE_OPEN_SEARCH)
			return;
		m_HotPos=GetMessagePos();
		if (!item.bFolder && item.id!=MENU_SEARCH_BOX)
		{
			SetActiveWindow();
			// destroy old submenus
			for (int i=(int)s_Menus.size()-1;s_Menus[i]!=this;i--)
				if (!s_Menus[i]->m_bDestroyed)
					s_Menus[i]->DestroyWindow();

			// just select the item
			ActivateItem(index,ACTIVATE_SELECT,NULL);
			return;
		}

		// open a submenu - create a new menu object
		const StdMenuItem *pSubMenu=item.pStdItem?item.pStdItem->submenu:NULL;
		bool bOpenUp=false;

		int options=(type==ACTIVATE_OPEN_SEARCH)?CONTAINER_DRAG|CONTAINER_SEARCH:CONTAINER_DRAG|CONTAINER_DROP;
		if (item.id==MENU_CONTROLPANEL)
			options|=CONTAINER_CONTROLPANEL;
		if (item.id==MENU_DOCUMENTS)
			options|=CONTAINER_DOCUMENTS;
		if (item.bPrograms)
			options|=CONTAINER_PROGRAMS;
		if (item.bLink || (m_Options&CONTAINER_LINK))
			options|=CONTAINER_LINK;
		if ((m_Options&CONTAINER_TRACK) || (item.id==MENU_PROGRAMS))
			options|=CONTAINER_TRACK;

		if (item.id==MENU_RECENT_ITEMS)
			options|=CONTAINER_RECENT;

		if (m_Options&CONTAINER_OPENUP_REC)
		{
			options|=CONTAINER_OPENUP_REC;
			bOpenUp=true;
		}
		if (m_Options&CONTAINER_SORTZA_REC)
			options|=CONTAINER_SORTZA|CONTAINER_SORTZA_REC;

		if (item.pStdItem)
		{
			if (item.pStdItem->settings&StdMenuItem::MENU_OPENUP)
				bOpenUp=true;
			if (item.pStdItem->settings&StdMenuItem::MENU_OPENUP_REC)
				options|=CONTAINER_OPENUP_REC;
			if (item.pStdItem->settings&StdMenuItem::MENU_SORTZA)
				options|=CONTAINER_SORTZA;
			if (item.pStdItem->settings&StdMenuItem::MENU_SORTZA_REC)
				options|=CONTAINER_SORTZA_REC;
			if (item.pStdItem->settings&StdMenuItem::MENU_SORTONCE)
				options|=CONTAINER_SORTONCE;
			if (item.pStdItem->settings&StdMenuItem::MENU_ITEMS_FIRST)
				options|=CONTAINER_ITEMS_FIRST;
			if (item.pStdItem->settings&StdMenuItem::MENU_TRACK)
				options|=CONTAINER_TRACK;
			if (item.pStdItem->settings&StdMenuItem::MENU_NOTRACK)
				options&=~CONTAINER_TRACK;
			if (item.pStdItem->settings&StdMenuItem::MENU_MULTICOLUMN)
				options|=CONTAINER_MULTICOL_REC;
			if (item.pStdItem->settings&StdMenuItem::MENU_NOEXTENSIONS)
				options|=CONTAINER_NOEXTENSIONS;
		}

		if (item.pItem1 && s_pKnownFolders)
		{
			PIDLIST_ABSOLUTE pidl=item.pItem1;
			if (item.bLink)
			{
				// resolve link
				CComPtr<IShellFolder> pFolder;
				PCUITEMID_CHILD child;
				SHBindToParent(item.pItem1,IID_IShellFolder,(void**)&pFolder,&child);
				CComPtr<IShellLink> pLink;
				if (pFolder)
					pFolder->GetUIObjectOf(g_OwnerWindow,1,&child,IID_IShellLink,NULL,(void**)&pLink);
				if (pLink)
					pLink->GetIDList(&pidl);
			}
			CComPtr<IKnownFolder> pFolder;
			s_pKnownFolders->FindFolderFromIDList(pidl,&pFolder);
			if (pidl && pidl!=item.pItem1)
				ILFree(pidl);
			if (pFolder)
			{
				KNOWNFOLDERID id;
				if (SUCCEEDED(pFolder->GetId(&id)))
				{
					for (int i=0;g_SpecialFolders[i].folder;i++)
						if (*g_SpecialFolders[i].folder==id)
						{
							if (g_SpecialFolders[i].settings&SpecialFolder::FOLDER_NOSUBFOLDERS)
								options|=CONTAINER_NOSUBFOLDERS;
							if (g_SpecialFolders[i].settings&SpecialFolder::FOLDER_NONEWFOLDER)
								options|=CONTAINER_NONEWFOLDER;
							if (g_SpecialFolders[i].settings&SpecialFolder::FOLDER_NODROP)
								options&=~CONTAINER_DROP;
							break;
						}
				}
			}
		}

		if (m_Options&CONTAINER_NOEXTENSIONS)
			options|=CONTAINER_NOEXTENSIONS;

		if (options&CONTAINER_LINK)
			options&=~(CONTAINER_MULTICOLUMN|CONTAINER_MULTICOL_REC);
		else
		{
			if (item.id==MENU_PROGRAMS || (m_Options&CONTAINER_MULTICOL_REC))
				options|=CONTAINER_MULTICOL_REC;
			if ((options&CONTAINER_MULTICOL_REC) && GetKeyState(VK_SHIFT)>=0)
				options|=CONTAINER_MULTICOLUMN;
		}
		CMenuContainer *pMenu=new CMenuContainer(this,index,options,pSubMenu,item.pItem1,item.pItem2,m_RegName+L"\\"+item.name);
		if (type==ACTIVATE_OPEN_SEARCH)
		{
			wchar_t text[256];
			m_SearchBox.GetWindowText(text,_countof(text));
			wchar_t *pText=text;
			while (*pText==' ' || *pText=='\t')
				pText++;
			int len=Strlen(pText);
			while (len>0 && (pText[len-1]==' ' || pText[len-1]=='\t'))
				len--;
			pText[len]=0;
			CharUpper(pText);
			pMenu->InitItems(m_SearchItems,pText);
		}
		else
			pMenu->InitItems();

		RECT itemRect;
		GetItemRect(index,itemRect);
		ClientToScreen(&itemRect);
		RECT border={-s_Skin.Submenu_padding.left+s_Skin.Submenu_offset,-s_Skin.Submenu_padding.top,s_Skin.Submenu_padding.right-s_Skin.Submenu_offset,s_Skin.Submenu_padding.bottom};
		if (s_bRTL)
		{
			// swap and change signs
			int q=border.left; border.left=-border.right; border.right=-q;
			// rc.left and rc.right coming from GetItemRect are swapped
			q=itemRect.left; itemRect.left=itemRect.right; itemRect.right=q;
		}
		AdjustWindowRect(&border,s_SubmenuStyle,FALSE);

		if (m_bSubMenu)
			pMenu->m_MaxWidth=s_MainRect.right-s_MainRect.left;
		else if (s_bExpandRight)
			pMenu->m_MaxWidth=s_MainRect.right-itemRect.right-border.left;
		else
			pMenu->m_MaxWidth=itemRect.left+border.right-s_MainRect.left;

		DWORD animFlags=AW_ACTIVATE;
		{
			bool bDef;
			int anim=GetSettingInt(L"SubMenuAnimation",bDef);
			if (bDef)
			{
				DWORD fade;
				SystemParametersInfo(SPI_GETMENUFADE,NULL,&fade,0);
				animFlags=AW_ACTIVATE|(fade?AW_BLEND:AW_SLIDE);
			}
			if (anim==3) animFlags=AW_ACTIVATE|((rand()<RAND_MAX/2)?AW_BLEND:AW_SLIDE);
			else if (anim==1) animFlags=AW_ACTIVATE|AW_BLEND;
			else if (anim==2) animFlags=AW_ACTIVATE|AW_SLIDE;
		}

		BOOL animate;
		if ((animFlags&(AW_BLEND|AW_SLIDE))==0 || m_Submenu>=0)
			animate=FALSE;
		else
			SystemParametersInfo(SPI_GETMENUANIMATION,NULL,&animate,0);

		// destroy old submenus
		SetActiveWindow();
		for (int i=(int)s_Menus.size()-2;s_Menus[i]!=this;i--)
			if (!s_Menus[i]->m_bDestroyed)
				s_Menus[i]->DestroyWindow();

		// open submenu
		HWND parent=GetParent();
		pMenu->Create(parent,NULL,NULL,s_SubmenuStyle,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|(s_bRTL?WS_EX_LAYOUTRTL:0));
		if (!parent)
		{
			// place sub-menus in front of the taskbar
			if (type==ACTIVATE_OPEN_SEARCH)
				pMenu->SetWindowPos(g_TaskBar,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
			else
				pMenu->SetWindowPos(g_TaskBar,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		}
		RECT rc2;
		pMenu->GetWindowRect(&rc2);

		// position new menu
		int w=rc2.right-rc2.left;
		int h=rc2.bottom-rc2.top;

		if (s_bExpandRight)
		{
			if (itemRect.right+border.left+w<=s_MainRect.right)
			{
				// right
				rc2.left=itemRect.right+border.left;
				rc2.right=rc2.left+w;
				animFlags|=AW_HOR_POSITIVE;
				pMenu->m_Options|=CONTAINER_LEFT;
			}
			else if (itemRect.left+border.right-w>=s_MainRect.left)
			{
				// left
				rc2.right=itemRect.left+border.right;
				rc2.left=rc2.right-w;
				animFlags|=AW_HOR_NEGATIVE;
			}
			else
			{
				// right again
				rc2.right=s_MainRect.right;
				rc2.left=rc2.right-w;
				if (!s_bRTL)
				{
					int minx=m_bSubMenu?s_MainRect.left:(itemRect.right+border.left);
					if (rc2.left<minx)
					{
						rc2.left=minx;
						rc2.right=minx+w;
					}
				}
				animFlags|=AW_HOR_POSITIVE;
				pMenu->m_Options|=CONTAINER_LEFT;
			}
		}
		else
		{
			if (itemRect.left+border.right-w>=s_MainRect.left)
			{
				// left
				rc2.right=itemRect.left+border.right;
				rc2.left=rc2.right-w;
				animFlags|=AW_HOR_NEGATIVE;
			}
			else if (itemRect.right+border.left+w<=s_MainRect.right)
			{
				// right
				rc2.left=itemRect.right+border.left;
				rc2.right=rc2.left+w;
				animFlags|=AW_HOR_POSITIVE;
				pMenu->m_Options|=CONTAINER_LEFT;
			}
			else
			{
				// left again
				rc2.left=s_MainRect.left;
				rc2.right=rc2.left+w;
				if (s_bRTL)
				{
					int maxx=m_bSubMenu?s_MainRect.right:(itemRect.left+border.right);
					if (rc2.right>maxx)
					{
						rc2.left=maxx-w;
						rc2.right=maxx;
					}
				}
				animFlags|=AW_HOR_NEGATIVE;
			}
		}

		if (s_bRTL)
			animFlags^=(AW_HOR_POSITIVE|AW_HOR_NEGATIVE); // RTL flips the animation

		int padTop=0, padBottom=0;
		if (pMenu->m_ScrollHeight)
		{
			RECT rcc=pMenu->m_rContent;
			pMenu->ClientToScreen(&rcc);
			padTop=rcc.top-rc2.top;
			padBottom=rc2.bottom-rcc.bottom;
		}

		if (bOpenUp)
		{
			if (itemRect.bottom+border.bottom-h>=s_MainRect.top)
			{
				// up
				rc2.bottom=itemRect.bottom+border.bottom;
				rc2.top=rc2.bottom-h;
			}
			else if (itemRect.top+border.top+h<=s_MainRect.bottom)
			{
				// down
				rc2.top=itemRect.top+border.top;
				rc2.bottom=rc2.top+h;
				pMenu->m_Options|=CONTAINER_TOP;
			}
			else
			{
				// up again
				rc2.top=s_MainRect.top-padTop;
				rc2.bottom=rc2.top+h;
			}
		}
		else
		{
			if (itemRect.top+border.top+h<=s_MainRect.bottom)
			{
				// down
				rc2.top=itemRect.top+border.top;
				rc2.bottom=rc2.top+h;
				pMenu->m_Options|=CONTAINER_TOP;
			}
			else if (itemRect.bottom+border.bottom-h>=s_MainRect.top)
			{
				// up
				rc2.bottom=itemRect.bottom+border.bottom;
				rc2.top=rc2.bottom-h;
			}
			else
			{
				// down again
				rc2.bottom=s_MainRect.bottom+padBottom;
				rc2.top=rc2.bottom-h;
				pMenu->m_Options|=CONTAINER_TOP;
			}
		}

		m_Submenu=index;
		InvalidateItem(index);
		if (type!=ACTIVATE_OPEN_SEARCH)
			SetHotItem(index);
		UpdateWindow();

		if (type==ACTIVATE_OPEN_SEARCH)
			pMenu->SetWindowPos(HWND_TOPMOST,&rc2,SWP_SHOWWINDOW|SWP_NOACTIVATE);
		else if (animate)
		{
			pMenu->SetWindowPos(HWND_TOPMOST,&rc2,0);
			int speed=GetSettingInt(L"SubMenuAnimationSpeed");
			if (speed<=0) speed=MENU_ANIM_SPEED_SUBMENU;
			else if (speed>=10000) speed=10000;
			if (!AnimateWindow(pMenu->m_hWnd,speed,animFlags))
			{
				if (pMenu->m_SearchBox.m_hWnd)
					pMenu->m_SearchBox.ShowWindow(SW_SHOW);
				pMenu->ShowWindow(SW_SHOW); // show the menu anyway if AnimateWindow fails
			}
			else
			{
				if (pMenu->m_SearchBox.m_hWnd)
					pMenu->m_SearchBox.ShowWindow(SW_SHOW);
			}
		}
		else
		{
			if (pMenu->m_SearchBox.m_hWnd)
				pMenu->m_SearchBox.ShowWindow(SW_SHOW);
			pMenu->SetWindowPos(HWND_TOPMOST,&rc2,SWP_SHOWWINDOW);
		}
		if (type!=ACTIVATE_OPEN_SEARCH)
			pMenu->SetFocus();
		if (type==ACTIVATE_OPEN_KBD || (type==ACTIVATE_OPEN_SEARCH && m_ResultsHash!=1))
			pMenu->SetHotItem(0);
		else
			pMenu->SetHotItem(-1);
		if (s_Tooltip.m_hWnd)
			s_Tooltip.SetWindowPos(HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
		return;
	}

	bool bShift=GetKeyState(VK_SHIFT)<0;
	bool bCtrl=GetKeyState(VK_CONTROL)<0;
	bool bKeepOpen=(type==ACTIVATE_EXECUTE) && bShift && !bCtrl;

	if (type==ACTIVATE_EXECUTE)
	{
		if (item.id==MENU_EMPTY || item.id==MENU_EMPTY_TOP) return;
		if (item.id==MENU_SEARCH_BOX)
		{
			// the search button was pressed
			m_SearchBox.SetFocus();
			for (std::vector<CMenuContainer*>::reverse_iterator it=s_Menus.rbegin();*it!=this;++it)
				if (!(*it)->m_bDestroyed)
					(*it)->PostMessage(WM_CLOSE);
			if (m_SearchState==SEARCH_MORE)
				UpdateSearchResults(true);
			else
				m_SearchBox.SetWindowText(L"");
			return;
		}

		if (!item.pItem1)
		{
			if (item.id<MENU_PROGRAMS) return; // non-executable item
			if (item.bFolder && item.pStdItem && item.pStdItem->submenu && !item.pStdItem->command)
				return; // non-executable item
		}

		// when executing an item close the whole menu
		if (!bKeepOpen)
		{
			if (g_TopMenu && s_bAllPrograms)
			{
				// send, don't post. the top menu must be closed immediately. otherwise its closing may interfere with launching the command
				::SendMessage(g_TopMenu,WM_CLOSE,0,0);
			}
			else
			{
				for (std::vector<CMenuContainer*>::reverse_iterator it=s_Menus.rbegin();it!=s_Menus.rend();++it)
					if (!(*it)->m_bDestroyed)
						(*it)->PostMessage(WM_CLOSE);
			}
		}
	}

	if (type==ACTIVATE_MENU)
	{
		// when showing the context menu close all submenus
		if (!(m_Options&CONTAINER_SEARCH))
			SetActiveWindow();
		for (int i=(int)s_Menus.size()-1;s_Menus[i]!=this;i--)
			if (!s_Menus[i]->m_bDestroyed)
				s_Menus[i]->DestroyWindow();
	}

	SetHotItem(index);

	if ((!item.pItem1 || (type==ACTIVATE_EXECUTE && item.pStdItem && item.pStdItem->command && *item.pStdItem->command)) && !((item.id==MENU_EMPTY || item.id==MENU_EMPTY_TOP) && type==ACTIVATE_MENU))
	{
		// regular command item
		if (type!=ACTIVATE_EXECUTE) return;

		CString search;
		for (CMenuContainer *pSearchMenu=this;pSearchMenu;pSearchMenu=pSearchMenu->m_pParent)
			if (pSearchMenu->m_SearchBox.m_hWnd)
			{
				pSearchMenu->m_SearchBox.GetWindowText(search);
				break;
			}
		if (bKeepOpen)
			LockSetForegroundWindow(LSFW_LOCK);
		else
		{
			LockSetForegroundWindow(LSFW_UNLOCK);
			if (item.id!=MENU_SLEEP && item.id!=MENU_HIBERNATE)
				FadeOutItem(index);
			// flush all messages to close the menus
			// m_hWnd is not valid after this point
			MSG msg;
			while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		PlayMenuSound(SOUND_COMMAND);

		// special handling for command items
		CComPtr<IShellDispatch2> pShellDisp;
		switch (item.id)
		{
		case MENU_TASKBAR: // show taskbar properties
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->TrayProperties();
			break;
		case MENU_CLASSIC_SETTINGS: // show our settings
#ifdef ALLOW_DEACTIVATE
			EditSettings(false);
#else
			EditSettings(true);
#endif
			break;
		case MENU_FEATURES:
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->ControlPanelItem(CComBSTR(L"appwiz.cpl"));
			break;
		case MENU_SECURITY:
			{
				CComPtr<IShellDispatch4> pShellDisp4;
				if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch4,(void**)&pShellDisp4)))
					pShellDisp4->WindowsSecurity();
			}
			break;
		case MENU_SEARCH_FILES: // show the search UI
			{
				CString command=GetSettingString(L"SearchFilesCommand");
				if (command.IsEmpty())
				{
					if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
						pShellDisp->FindFiles();
				}
				else
				{
					wchar_t buf[1024];
					Strcpy(buf,_countof(buf),command);
					DoEnvironmentSubst(buf,_countof(buf));
					ExecuteCommand(buf);
				}
			}
			break;
		case MENU_SEARCH_PRINTER: // search for network printers
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->FindPrinter(CComBSTR(L""),CComBSTR(L""),CComBSTR(L""));
			break;
		case MENU_SEARCH_COMPUTERS: // search for computers
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->FindComputer();
			break;
		case MENU_SEARCH_PEOPLE: // search for people using Windows Mail
			{
				SHELLEXECUTEINFO execute={sizeof(execute),SEE_MASK_DOENVSUBST,NULL,L"open"};
				execute.lpFile=L"%ProgramFiles%\\Windows Mail\\wab.exe";
				execute.lpParameters=L"/find";
				execute.lpDirectory=L"%ProgramFiles%\\Windows Mail";
				execute.nShow=SW_SHOWNORMAL;
				ShellExecuteEx(&execute);
			}
			break;
		case MENU_HELP: // show Windows help
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->Help();
			break;
		case MENU_RUN: // show the Run box
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->FileRun();
			break;
		case MENU_LOGOFF: // log off
			{
				if (!GetSettingBool(L"ConfirmLogOff"))
					ExitWindowsEx(EWX_LOGOFF,0);
				else
				{
					HMODULE hShell32=GetModuleHandle(L"Shell32.dll");
					HICON icon=LoadIcon(hShell32,MAKEINTRESOURCE(45));
					INT_PTR res=DialogBoxParam(g_Instance,MAKEINTRESOURCE(s_bRTL?IDD_LOGOFFR:IDD_LOGOFF),NULL,LogOffDlgProc,(LPARAM)icon);
					DestroyIcon(icon);
					if (res)
						ExitWindowsEx(EWX_LOGOFF,0);
				}
			}
			break;
		case MENU_RESTART: // restart
			SetShutdownPrivileges();
			ExitWindowsEx(EWX_REBOOT,0);
			break;
		case MENU_SWITCHUSER: // switch_user
			WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE,WTS_CURRENT_SESSION,FALSE); // same as "disconnect"
			break;
		case MENU_LOCK: // lock
			LockWorkStation();
			break;
		case MENU_SHUTDOWN: // shutdown, don't ask
			SetShutdownPrivileges();
			ExitWindowsEx(EWX_SHUTDOWN,0);
			break;
		case MENU_SLEEP:
			CreateThread(NULL,0,SleepThread,(void*)FALSE,0,NULL);
			break;
		case MENU_HIBERNATE:
			CreateThread(NULL,0,SleepThread,(void*)TRUE,0,NULL);
			break;
		case MENU_DISCONNECT: // disconnect the current Terminal Services session (remote desktop)
			WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE,WTS_CURRENT_SESSION,FALSE);
			break;
		case MENU_UNDOCK: // undock the PC
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->EjectPC();
			break;
		case MENU_SHUTDOWN_BOX: // shutdown - ask to shutdown, log off, sleep, etc
			if (SUCCEEDED(CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IShellDispatch2,(void**)&pShellDisp)))
				pShellDisp->ShutdownWindows();
			break;
		default:
			if (item.pStdItem && item.pStdItem->command && *item.pStdItem->command)
			{
				wchar_t buf[1024];
				Strcpy(buf,_countof(buf),item.pStdItem->command);
				DoEnvironmentSubst(buf,_countof(buf));
				if (!search.IsEmpty() && (wcswcs(buf,L"%1") || wcswcs(buf,L"%2")))
					DoSearchSubst(buf,_countof(buf),search);
				ExecuteCommand(buf);
				return;
			}
		}
		return;
	}

	// create a context menu for the selected item. the context menu can be shown (ACTIVATE_MENU) or its default
	// item can be executed automatically (ACTIVATE_EXECUTE)
	CComPtr<IContextMenu> pMenu;
	HMENU menu=CreatePopupMenu();

	CComPtr<IShellFolder> pFolder;
	PCUITEMID_CHILD pidl;

	if (item.id==MENU_EMPTY || item.id==MENU_EMPTY_TOP)
	{
		s_pDesktop->BindToObject(m_Path1a[0],NULL,IID_IShellFolder,(void**)&pFolder);
	}
	else
	{
		SHBindToParent(item.pItem1,IID_IShellFolder,(void**)&pFolder,&pidl);
		if (FAILED(pFolder->GetUIObjectOf(g_OwnerWindow,1,&pidl,IID_IContextMenu,NULL,(void**)&pMenu)))
		{
			DestroyMenu(menu);
			return;
		}

		UINT flags=CMF_DEFAULTONLY;
		if (type==ACTIVATE_MENU)
		{
			flags=CMF_NORMAL|CMF_CANRENAME;
			if (bShift) flags|=CMF_EXTENDEDVERBS;
		}
		if (type==ACTIVATE_EXECUTE && bShift && bCtrl)
			flags|=CMF_EXTENDEDVERBS;
		if (FAILED(pMenu->QueryContextMenu(menu,0,CMD_LAST,32767,flags)))
		{
			DestroyMenu(menu);
			return;
		}

		if (item.bFolder && item.pItem2)
		{
			// context menu for a double folder - remove most commands, add Open All Users
			int n=GetMenuItemCount(menu);
			for (int i=0;i<n;i++)
			{
				int id=GetMenuItemID(menu,i);
				if (id>0)
				{
					char command[256];
					if (FAILED(pMenu->GetCommandString(id-CMD_LAST,GCS_VERBA,NULL,command,_countof(command))))
						command[0]=0;
					if (_stricmp(command,"open")==0)
					{
						if (!s_bNoCommonFolders)
							InsertMenu(menu,i+1,MF_BYPOSITION|MF_STRING,CMD_OPEN_ALL,FindTranslation(L"Menu.OpenAll",L"O&pen All Users"));
						InsertMenu(menu,i+2,MF_BYPOSITION|MF_SEPARATOR,0,0);
						i+=2;
						n+=2;
						continue;
					}
					else if (_stricmp(command,"rename")==0 || _stricmp(command,"delete")==0)
					{
						if (item.id!=MENU_PROGRAMS) continue;
					}
					else if (_stricmp(command,"properties")==0)
						continue;
				}
				DeleteMenu(menu,i,MF_BYPOSITION);
				i--;
				n--;
			}
		}
		else if (type==ACTIVATE_MENU && item.id==MENU_RECENT)
		{
			// context menu for a recent item - leave just open and runas
			int n=GetMenuItemCount(menu);
			for (int i=0;i<n;i++)
			{
				int id=GetMenuItemID(menu,i);
				if (id>0)
				{
					char command[256];
					if (FAILED(pMenu->GetCommandString(id-CMD_LAST,GCS_VERBA,NULL,command,_countof(command))))
						command[0]=0;
					if (_stricmp(command,"open")==0 || _stricmp(command,"runas")==0 || _stricmp(command,"properties")==0 || _stricmp(command,"opencontaining")==0)
						continue;
				}
				DeleteMenu(menu,i,MF_BYPOSITION);
				i--;
				n--;
			}
			if (n>0)
				AppendMenu(menu,MF_SEPARATOR,0,0);
			AppendMenu(menu,MF_STRING,CMD_DELETEMRU,FindTranslation(L"Menu.RemoveList",L"Remove &from this list"));
		}
		else if (type==ACTIVATE_MENU && (m_Options&CONTAINER_SEARCH))
		{
			// context menu for a search item - remove delete, rename and link
			int n=GetMenuItemCount(menu);
			for (int i=0;i<n;i++)
			{
				int id=GetMenuItemID(menu,i);
				if (id>0)
				{
					char command[256];
					if (FAILED(pMenu->GetCommandString(id-CMD_LAST,GCS_VERBA,NULL,command,_countof(command))))
						command[0]=0;
					if (_stricmp(command,"delete")!=0 && _stricmp(command,"rename")!=0 && _stricmp(command,"link")!=0)
						continue;
				}
				DeleteMenu(menu,i,MF_BYPOSITION);
				i--;
				n--;
			}
		}
		// remove multiple separators
		{
			bool bSeparator=false;
			int n=GetMenuItemCount(menu);
			for (int i=0;i<n;i++)
			{
				MENUITEMINFO info={sizeof(info),MIIM_FTYPE};
				if (GetMenuItemInfo(menu,i,TRUE,&info))
				{
					if (info.fType==MFT_SEPARATOR && bSeparator)
					{
						DeleteMenu(menu,i,MF_BYPOSITION);
						i--;
						n--;
					}
					bSeparator=(info.fType==MFT_SEPARATOR);
				}
			}
		}
	}

	int res=0;
	if (type==ACTIVATE_EXECUTE)
	{
		// just pick the default item
		res=GetMenuDefaultItem(menu,FALSE,0);
		if (bShift && bCtrl)
		{
			// find the runas verb if available
			char command[256];
			int n=GetMenuItemCount(menu);
			for (int i=0;i<n;i++)
			{
				int id=GetMenuItemID(menu,i);
				if (id>=CMD_LAST && SUCCEEDED(pMenu->GetCommandString(id-CMD_LAST,GCS_VERBA,NULL,command,_countof(command))))
				{
					if (_stricmp(command,"runas")==0)
					{
						res=id;
						break;
					}
				}
			}
		}
		if (res<0) res=0;
	}
	else
	{
		// show the context menu
		m_pMenu2=pMenu;
		m_pMenu3=pMenu;
		HBITMAP shellBmp=NULL;
		HBITMAP newFolderBmp=NULL;
		HBITMAP newShortcutBmp=NULL;
		if ((item.id==MENU_NO || item.id==MENU_EMPTY) && (m_Options&CONTAINER_DROP))// clicked on a movable item
		{
			AppendMenu(menu,MF_SEPARATOR,0,0);
			HMENU menu2=menu;
			if (GetSettingBool(L"CascadingMenu"))
				menu2=CreatePopupMenu();
			bool bSort=false, bNew=false;
			int n=0;
			for (std::vector<MenuItem>::const_iterator it=m_Items.begin();it!=m_Items.end();++it)
				if (it->id==MENU_NO)
					n++;
			if (n>1)
				bSort=true; // more than 1 movable items
			wchar_t path[_MAX_PATH];
			if (pFolder && !(m_Options&CONTAINER_NONEWFOLDER) && GetSettingBool(L"ShowNewFolder") && SHGetPathFromIDList(m_Path1a[item.priority==2?1:0],path))
				bNew=true;

			if (bSort)
				AppendMenu(menu2,MF_STRING,CMD_SORT,FindTranslation(L"Menu.SortByName",L"Sort &by Name"));

			AppendMenu(menu2,MF_STRING,CMD_AUTOSORT,FindTranslation(L"Menu.AutoArrange",L"&Auto Arrange"));
			if (m_Options&CONTAINER_AUTOSORT)
			{
				EnableMenuItem(menu2,CMD_SORT,MF_BYCOMMAND|MF_GRAYED);
				CheckMenuItem(menu2,CMD_AUTOSORT,MF_BYCOMMAND|MF_CHECKED);
			}

			if (bNew)
			{
				AppendMenu(menu2,MF_STRING,CMD_NEWFOLDER,FindTranslation(L"Menu.NewFolder",L"New Folder"));
				AppendMenu(menu2,MF_STRING,CMD_NEWSHORTCUT,FindTranslation(L"Menu.NewShortcut",L"New Shortcut"));
			}

			if (bNew || menu!=menu2)
			{
				int size=16;
				if (bNew)
				{
					HMODULE hShell32=GetModuleHandle(L"shell32.dll");
					HICON hIcon=(HICON)LoadImage(hShell32,MAKEINTRESOURCE(319),IMAGE_ICON,size,size,LR_DEFAULTCOLOR);
					if (hIcon)
					{
						newFolderBmp=BitmapFromIcon(hIcon,size,NULL,true);
						MENUITEMINFO mii={sizeof(mii)};
						mii.fMask=MIIM_BITMAP;
						mii.hbmpItem=newFolderBmp;
						SetMenuItemInfo(menu2,CMD_NEWFOLDER,FALSE,&mii);
					}
					hIcon=(HICON)LoadImage(hShell32,MAKEINTRESOURCE(16769),IMAGE_ICON,size,size,LR_DEFAULTCOLOR);
					if (hIcon)
					{
						newShortcutBmp=BitmapFromIcon(hIcon,size,NULL,true);
						MENUITEMINFO mii={sizeof(mii)};
						mii.fMask=MIIM_BITMAP;
						mii.hbmpItem=newShortcutBmp;
						SetMenuItemInfo(menu2,CMD_NEWSHORTCUT,FALSE,&mii);
					}
				}
				if (menu!=menu2)
				{
					int idx=GetMenuItemCount(menu);
					AppendMenu(menu,MF_POPUP,(UINT_PTR)menu2,FindTranslation(L"Menu.Organize",L"Organize Start menu"));
					HICON hIcon=(HICON)LoadImage(g_Instance,MAKEINTRESOURCE(IDI_APPICON),IMAGE_ICON,size,size,LR_DEFAULTCOLOR);
					if (hIcon)
					{
						shellBmp=BitmapFromIcon(hIcon,size,NULL,true);
						MENUITEMINFO mii={sizeof(mii)};
						mii.fMask=MIIM_BITMAP;
						mii.hbmpItem=shellBmp;
						SetMenuItemInfo(menu,idx,TRUE,&mii);
					}
				}
			}
		}

		TPMPARAMS params={sizeof(params)}, *pParams=NULL;
		POINT pt2=*pPt;
		if (pt2.x==-1 && pt2.y==-1)
		{
			GetItemRect(index,params.rcExclude);
			ClientToScreen(&params.rcExclude);
			pt2.x=params.rcExclude.left;
			pt2.y=params.rcExclude.top;
			pParams=&params;
		}
		m_ContextItem=index;
		InvalidateItem(index);
		KillTimer(TIMER_HOVER);
		res=TrackPopupMenuEx(menu,TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_VERTICAL|(IsLanguageRTL()?TPM_LAYOUTRTL:0),pt2.x,pt2.y,m_hWnd,pParams);
		m_ContextItem=-1;
		if (m_HotItem<0) SetHotItem(index);
		if (m_pMenu2) m_pMenu2.Release();
		if (m_pMenu3) m_pMenu3.Release();
		if (newFolderBmp) DeleteObject(newFolderBmp);
		if (newShortcutBmp) DeleteObject(newShortcutBmp);
		if (shellBmp) DeleteObject(shellBmp);
	}

	if (type==ACTIVATE_EXECUTE)
	{
		if (bKeepOpen)
			LockSetForegroundWindow(LSFW_LOCK);
		else
		{
			LockSetForegroundWindow(LSFW_UNLOCK);
			FadeOutItem(index);
		}
		PlayMenuSound(SOUND_COMMAND);
	}

	// handle our standard commands
	if (res==CMD_OPEN_ALL)
	{
		SHELLEXECUTEINFO execute={sizeof(execute),SEE_MASK_IDLIST|SEE_MASK_INVOKEIDLIST};
		execute.lpVerb=L"open";
		execute.lpIDList=item.pItem2;
		execute.nShow=SW_SHOWNORMAL;
		ShellExecuteEx(&execute);
	}

	if (res==CMD_SORT)
	{
		std::vector<SortMenuItem> items;
		for (std::vector<MenuItem>::const_iterator it=m_Items.begin();it!=m_Items.end();++it)
			if (it->id==MENU_NO)
			{
				SortMenuItem item={it->name,it->nameHash,it->bFolder};
				items.push_back(item);
			}
			std::sort(items.begin(),items.end());
			if (m_Options&CONTAINER_SORTZA)
				std::reverse(items.begin(),items.end());
			SaveItemOrder(items);
			PostRefreshMessage();
	}

	if (res==CMD_AUTOSORT)
	{
		CRegKey regOrder;
		if (regOrder.Open(HKEY_CURRENT_USER,m_RegName)!=ERROR_SUCCESS)
			regOrder.Create(HKEY_CURRENT_USER,m_RegName);
		if (m_Options&CONTAINER_AUTOSORT)
			regOrder.SetBinaryValue(L"Order",NULL,0);
		else
		{
			DWORD cAuto='AUTO';
			regOrder.SetBinaryValue(L"Order",&cAuto,4);
		}
		PostRefreshMessage();
	}

	if (res==CMD_NEWFOLDER)
	{
		s_pDragSource=this; // HACK - prevent the menu from closing
		g_RenameText=item.name;
		g_RenamePos=pPt;
		bool bAllPrograms=s_bAllPrograms;
		for (std::vector<CMenuContainer*>::iterator it=s_Menus.begin();it!=s_Menus.end();++it)
			(*it)->EnableWindow(FALSE); // disable all menus
		if (bAllPrograms) ::EnableWindow(g_TopMenu,FALSE);

		CComPtr<IContextMenu> pMenu2;
		HMENU menu2=CreatePopupMenu();

		std::vector<unsigned int> items;
		{
			CComPtr<IEnumIDList> pEnum;
			if (pFolder->EnumObjects(NULL,SHCONTF_FOLDERS,&pEnum)!=S_OK) pEnum=NULL;

			PITEMID_CHILD pidl;
			while (pEnum && pEnum->Next(1,&pidl,NULL)==S_OK)
			{
				STRRET str;
				if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_INFOLDER|SHGDN_FORPARSING,&str)))
				{
					wchar_t *name;
					StrRetToStr(&str,pidl,&name);
					items.push_back(CalcFNVHash(name));
					CoTaskMemFree(name);
				}
				ILFree(pidl);
			}
		}

		if (SUCCEEDED(pFolder->CreateViewObject(g_OwnerWindow,IID_IContextMenu,(void**)&pMenu2)))
		{
			if (SUCCEEDED(pMenu2->QueryContextMenu(menu2,0,1,32767,CMF_NORMAL)))
			{
				CMINVOKECOMMANDINFOEX info={sizeof(info),CMIC_MASK_UNICODE};
				info.lpVerb="NewFolder";
				info.lpVerbW=L"NewFolder";
				info.nShow=SW_SHOWNORMAL;
				info.fMask|=CMIC_MASK_NOASYNC;
				info.hwnd=g_OwnerWindow;
				pMenu2->InvokeCommand((CMINVOKECOMMANDINFO*)&info);
			}
		}
		DestroyMenu(menu2);

		PITEMID_CHILD newPidl=NULL;
		unsigned int newHash=0;
		{
			CComPtr<IEnumIDList> pEnum;
			if (pFolder->EnumObjects(NULL,SHCONTF_FOLDERS,&pEnum)!=S_OK) pEnum=NULL;

			PITEMID_CHILD pidl;
			while (pEnum && pEnum->Next(1,&pidl,NULL)==S_OK)
			{
				STRRET str;
				if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_INFOLDER|SHGDN_FORPARSING,&str)))
				{
					wchar_t *name;
					StrRetToStr(&str,pidl,&name);
					unsigned int hash=CalcFNVHash(name);
					if (std::find(items.begin(),items.end(),hash)==items.end())
					{
						if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_INFOLDER|SHGDN_FOREDITING,&str)))
						{
							wchar_t *name2;
							StrRetToStr(&str,pidl,&name2);
							g_RenameText=name2;
							CoTaskMemFree(name2);
							StrRetToStr(&str,pidl,&name2);
						}
						else
							g_RenameText=name;
						CharUpper(name);
						newHash=CalcFNVHash(name,item.priority==2?CalcFNVHash(L"\\"):FNV_HASH0);
						CoTaskMemFree(name);
						newPidl=pidl;
						break;
					}
					CoTaskMemFree(name);
				}
				ILFree(pidl);
			}
		}

		PostRefreshMessage();
		PostMessage(MCM_SETCONTEXTITEM,newHash);
		// show the Rename dialog box
		if (newPidl && DialogBox(g_Instance,MAKEINTRESOURCE(s_bRTL?IDD_RENAMER:IDD_RENAME),g_OwnerWindow,RenameDlgProc))
		{
			pFolder->SetNameOf(g_OwnerWindow,newPidl,g_RenameText,SHGDN_INFOLDER|SHGDN_FOREDITING,NULL);
			PostRefreshMessage();
		}
		for (std::vector<CMenuContainer*>::iterator it=s_Menus.begin();it!=s_Menus.end();++it)
			(*it)->EnableWindow(TRUE); // enable all menus
		if (bAllPrograms) ::EnableWindow(g_TopMenu,TRUE);
		SetForegroundWindow(m_hWnd);
		SetActiveWindow();
		SetFocus();
		s_pDragSource=NULL;
		m_ContextItem=-1;
		if (m_HotItem<0) SetHotItem(index);
		Invalidate();
		DestroyMenu(menu);
		return;
	}

	if (res==CMD_NEWSHORTCUT)
	{
		wchar_t target[_MAX_PATH+1];
		SHGetPathFromIDList(m_Path1a[0],target);
		target[Strlen(target)+1]=0;
		wchar_t fname[_MAX_PATH+1];

		// first try in the original folder
		PathMakeUniqueName(fname,_countof(fname)-1,L"scut.lnk",L"New Shortcut.lnk",target);
		HANDLE hFile=CreateFile(fname,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);

			// just run the shortcut wizard
			wchar_t cmdLine[1024];
			Sprintf(cmdLine,_countof(cmdLine),L"rundll32.exe appwiz.cpl,NewLinkHere %s",fname);

			STARTUPINFO startupInfo={sizeof(startupInfo)};
			PROCESS_INFORMATION processInfo;
			memset(&processInfo,0,sizeof(processInfo));
			wchar_t exe[_MAX_PATH]=L"%windir%\\system32\\rundll32.exe";
			DoEnvironmentSubst(exe,_countof(exe));
			if (CreateProcess(exe,cmdLine,NULL,NULL,FALSE,0,NULL,target,&startupInfo,&processInfo))
			{
				CloseHandle(processInfo.hThread);
				CloseHandle(processInfo.hProcess);
			}
		}
		else if (GetLastError()==ERROR_ACCESS_DENIED)
		{
			// there was a problem, most likely UAC didn't let us create a folder

			// create a temp folder just for us
			wchar_t temp[_MAX_PATH];
			GetTempPath(_countof(temp),temp);
			Strcat(temp,_countof(temp),L"ClassicShell");
			CreateDirectory(temp,NULL);

			// make a unique link file and keep a handle to the file
			PathMakeUniqueName(fname,_countof(fname)-1,L"scut.lnk",L"New Shortcut.lnk",temp);

			HANDLE hFile=CreateFile(fname,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(hFile);
				// wait for the wizard to finish in a separate thread and close the menu
				// otherwise it appears behind the menu
				ShortcutParams *pParams=new ShortcutParams;
				memcpy(pParams->target,target,sizeof(target));
				memcpy(pParams->temp,temp,sizeof(temp));
				memcpy(pParams->fname,fname,sizeof(fname));
				CreateThread(NULL,0,NewShortcutThread,pParams,0,NULL);
			}
		}
	}

	if (res==CMD_DELETEMRU && item.id==MENU_RECENT && s_bRecentItems)
	{
		STRRET str;
		if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_FORPARSING,&str)))
		{
			wchar_t *name;
			StrRetToStr(&str,pidl,&name);
			DeleteMRUShortcut(name);
			CoTaskMemFree(name);
		}
		PostRefreshMessage();
		return;
	}

	// handle the shell commands
	if (res>=CMD_LAST)
	{
		// handle special verbs
		char command[256];
		if (FAILED(pMenu->GetCommandString(res-CMD_LAST,GCS_VERBA,NULL,command,_countof(command))))
			command[0]=0;
		if (_stricmp(command,"rename")==0)
		{
			// show the Rename dialog box

			s_pDragSource=this; // HACK - prevent the menu from closing
			STRRET str;
			if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_FOREDITING,&str)))
			{
				wchar_t *name;
				StrRetToStr(&str,pidl,&name);
				g_RenameText=name;
				CoTaskMemFree(name);
			}
			else
				g_RenameText=item.name;
			g_RenamePos=pPt;
			for (std::vector<CMenuContainer*>::iterator it=s_Menus.begin();it!=s_Menus.end();++it)
				(*it)->EnableWindow(FALSE); // disable all menus
			bool bAllPrograms=s_bAllPrograms;
			if (bAllPrograms) ::EnableWindow(g_TopMenu,FALSE);

			m_ContextItem=index;
			InvalidateItem(index);
			bool bRenamed=DialogBox(g_Instance,MAKEINTRESOURCE(s_bRTL?IDD_RENAMER:IDD_RENAME),g_OwnerWindow,RenameDlgProc)!=0;
			m_ContextItem=-1;
			if (m_HotItem<0) SetHotItem(index);

			if (bRenamed)
			{
				// perform the rename operation
				PITEMID_CHILD newPidl;
				if (SUCCEEDED(pFolder->SetNameOf(g_OwnerWindow,pidl,g_RenameText,SHGDN_INFOLDER|SHGDN_FOREDITING,&newPidl)))
				{
					STRRET str;
					if (SUCCEEDED(pFolder->GetDisplayNameOf(newPidl,SHGDN_INFOLDER|SHGDN_FORPARSING,&str)))
					{
						wchar_t *name;
						StrRetToStr(&str,newPidl,&name);
						CharUpper(name);
						item.name=g_RenameText;
						item.nameHash=CalcFNVHash(name);
						CoTaskMemFree(name);

						if (!(m_Options&CONTAINER_AUTOSORT))
						{
							std::vector<SortMenuItem> items;
							for (std::vector<MenuItem>::const_iterator it=m_Items.begin();it!=m_Items.end();++it)
								if (it->id==MENU_NO)
								{
									SortMenuItem item={it->name,it->nameHash,it->bFolder};
									items.push_back(item);
								}
								SaveItemOrder(items);
						}
					}
					ILFree(newPidl);
				}
				PostRefreshMessage();
			}
			for (std::vector<CMenuContainer*>::iterator it=s_Menus.begin();it!=s_Menus.end();++it)
				(*it)->EnableWindow(TRUE); // enable all menus
			if (bAllPrograms) ::EnableWindow(g_TopMenu,TRUE);
			SetForegroundWindow(m_hWnd);
			SetActiveWindow();
			SetFocus();
			s_pDragSource=NULL;
			DestroyMenu(menu);
			return;
		}

		bool bRefresh=(_stricmp(command,"delete")==0 || _stricmp(command,"link")==0);

		CMINVOKECOMMANDINFOEX info={sizeof(info),CMIC_MASK_UNICODE};
		info.lpVerb=MAKEINTRESOURCEA(res-CMD_LAST);
		info.lpVerbW=MAKEINTRESOURCEW(res-CMD_LAST);
		info.nShow=SW_SHOWNORMAL;
		if (pPt)
		{
			info.fMask|=CMIC_MASK_PTINVOKE;
			info.ptInvoke=*pPt;
		}
		if (type==ACTIVATE_MENU)
		{
			if (GetKeyState(VK_CONTROL)<0) info.fMask|=CMIC_MASK_CONTROL_DOWN;
			if (GetKeyState(VK_SHIFT)<0) info.fMask|=CMIC_MASK_SHIFT_DOWN;
		}

		if (bRefresh)
			info.fMask|=CMIC_MASK_NOASYNC; // wait for delete/link commands to finish so we can refresh the menu

		s_pDragSource=this; // prevent the menu from closing. the command may need a HWND to show its UI
		for (std::vector<CMenuContainer*>::iterator it=s_Menus.begin();it!=s_Menus.end();++it)
			(*it)->EnableWindow(FALSE); // disable all menus
		bool bAllPrograms=s_bAllPrograms;
		if (bAllPrograms) ::EnableWindow(g_TopMenu,FALSE);
		info.hwnd=g_OwnerWindow;

		RECT rc;
		GetWindowRect(&rc);
		::SetForegroundWindow(g_OwnerWindow);
		::SetWindowPos(g_OwnerWindow,HWND_TOPMOST,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,0);
		HRESULT hr=pMenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
		if (type==ACTIVATE_EXECUTE && SUCCEEDED(hr))
		{
			if ((item.id==MENU_RECENT || (m_Options&CONTAINER_TRACK)) && s_bRecentItems)
			{
				// update MRU list
				STRRET str;
				if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_FORPARSING,&str)))
				{
					wchar_t *name;
					StrRetToStr(&str,pidl,&name);
					AddMRUShortcut(name);
					CoTaskMemFree(name);
				}
			}
			if (!(m_Options&CONTAINER_LINK))
			{
				// update item ranks
				STRRET str;
				if (SUCCEEDED(pFolder->GetDisplayNameOf(pidl,SHGDN_INFOLDER|SHGDN_NORMAL,&str)))
				{
					wchar_t *name;
					StrRetToStr(&str,pidl,&name);
					CharUpper(name);
					AddItemRank(CalcFNVHash(name));
					CoTaskMemFree(name);
				}
			}
		}
		for (std::vector<CMenuContainer*>::iterator it=s_Menus.begin();it!=s_Menus.end();++it)
			if (!(*it)->m_bDestroyed)
				(*it)->EnableWindow(TRUE); // enable all menus
		if (bAllPrograms) ::EnableWindow(g_TopMenu,TRUE);
		if (bRefresh && !m_bDestroyed)
		{
			SetForegroundWindow(m_hWnd);
			SetActiveWindow();
			SetFocus();
		}
		s_pDragSource=NULL;

		if (!bKeepOpen && GetActiveWindow()!=m_hWnd)
		{
			// if after all the window is not active, then another application was launched - close all menus
			for (std::vector<CMenuContainer*>::reverse_iterator it=s_Menus.rbegin();it!=s_Menus.rend();++it)
				if (!(*it)->m_bDestroyed)
					(*it)->PostMessage(WM_CLOSE);
			if (g_TopMenu && s_bAllPrograms) ::PostMessage(g_TopMenu,WM_CLOSE,0,0);
		}

		if (bRefresh)
			PostRefreshMessage(); // refresh the menu after an item was deleted or created
	}
	DestroyMenu(menu);
}

void CMenuContainer::RunUserCommand( bool bPicture )
{
	wchar_t buf[1024];
	Strcpy(buf,_countof(buf),GetSettingString(bPicture?L"UserPictureCommand":L"UserNameCommand"));
	if (buf[0])
	{
		DoEnvironmentSubst(buf,_countof(buf));
		ExecuteCommand(buf);
	}
}

static DWORD WINAPI FaderThreadProc( void *param )
{
	((CMenuFader*)param)->Create();
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void CMenuContainer::FadeOutItem( int index )
{
	int speed=GetSettingInt(L"MenuFadeSpeed");
	if (speed<=0) return;

	const MenuItem &item=m_Items[index];
	RECT rc;
	GetItemRect(index,rc);

	BITMAPINFO dib={sizeof(dib)};
	dib.bmiHeader.biWidth=rc.right-rc.left;
	dib.bmiHeader.biHeight=rc.top-rc.bottom;
	dib.bmiHeader.biPlanes=1;
	dib.bmiHeader.biBitCount=32;
	dib.bmiHeader.biCompression=BI_RGB;

	HDC hdc=CreateCompatibleDC(NULL);
	if (s_bRTL) SetLayout(hdc,LAYOUT_RTL);
	unsigned int *bits;
	HBITMAP bmp=CreateDIBSection(hdc,&dib,DIB_RGB_COLORS,(void**)&bits,NULL,0);
	HBITMAP bmp0=(HBITMAP)SelectObject(hdc,bmp);
	SetViewportOrgEx(hdc,-rc.left,-rc.top,NULL);

	// create a region from the opaque pixels of the selection bitmap
	int *slicesX, *slicesY;
	HBITMAP bmpSel=NULL;
	bool b32=false;
	if (m_bSubMenu)
	{
		// sub-menu
		if (!s_Skin.Submenu_selectionColor)
		{
			bmpSel=s_Skin.Submenu_selection.bmp;
			b32=s_Skin.Submenu_selection32;
			slicesX=s_Skin.Submenu_selection_slices_X;
			slicesY=s_Skin.Submenu_selection_slices_Y;
		}
	}
	else
	{
		// main menu
		if (!s_Skin.Main_selectionColor)
		{
			bmpSel=s_Skin.Main_selection.bmp;
			b32=s_Skin.Main_selection32;
			slicesX=s_Skin.Main_selection_slices_X;
			slicesY=s_Skin.Main_selection_slices_Y;
		}
	}
	HRGN region=NULL;
	if (bmpSel && b32)
	{
		HDC hdc2=CreateCompatibleDC(hdc);
		SetLayout(hdc2,0);
		HBITMAP bmp02=(HBITMAP)SelectObject(hdc2,bmpSel);
		FillRect(hdc,&rc,(HBRUSH)GetStockObject(WHITE_BRUSH));
		RECT rSrc={0,0,slicesX[0]+slicesX[1]+slicesX[2],slicesY[0]+slicesY[1]+slicesY[2]};
		RECT rMargins={slicesX[0],slicesY[0],slicesX[2],slicesY[2]};
		int w=dib.bmiHeader.biWidth;
		int h=-dib.bmiHeader.biHeight;
		if (rMargins.left>w) rMargins.left=w;
		if (rMargins.right>w) rMargins.right=w;
		if (rMargins.top>h) rMargins.top=h;
		if (rMargins.bottom>h) rMargins.bottom=h;
		MarginsBlit(hdc2,hdc,rSrc,rc,rMargins,false);
		SelectObject(hdc2,bmp02);
		DeleteDC(hdc2);
		SelectObject(hdc,bmp0);

		for (int y=0;y<h;y++)
		{
			int minx=-1, maxx=-1;
			int yw=y*w;
			for (int x=0;x<w;x++)
			{
				if ((bits[yw+x]>>24)>=32)
				{
					if (minx==-1) minx=x; // first non-transparent pixel
					if (maxx<x) maxx=x; // last non-transparent pixel
				}
			}
			if (minx>=0)
			{
				maxx++;
				HRGN r=CreateRectRgn(minx,y,maxx,y+1);
				if (!region)
					region=r;
				else
				{
					CombineRgn(region,region,r,RGN_OR);
					DeleteObject(r);
				}
			}
		}

		SelectObject(hdc,bmp);
	}

	DrawBackground(hdc,rc);

	SelectObject(hdc,bmp0);
	DeleteDC(hdc);

	ClientToScreen(&rc);
	CMenuFader *pFader=new CMenuFader(bmp,region,speed,rc);
	CreateThread(NULL,0,FaderThreadProc,pFader,0,NULL);
}

///////////////////////////////////////////////////////////////////////////////

CMenuFader::CMenuFader( HBITMAP bmp, HRGN region, int duration, RECT &rect )
{
	m_Bitmap=bmp;
	m_Region=region;
	m_Duration=duration;
	m_Rect=rect;
	s_Faders.push_back(this);
}

CMenuFader::~CMenuFader( void )
{
	if (m_Bitmap) DeleteObject(m_Bitmap);
	if (m_Region) DeleteObject(m_Region);
	s_Faders.erase(std::find(s_Faders.begin(),s_Faders.end(),this));
}

void CMenuFader::Create( void )
{
	bool bRtl=false;
	if (m_Rect.left>m_Rect.right)
	{
		bRtl=true;
		int q=m_Rect.left; m_Rect.left=m_Rect.right; m_Rect.right=q;
	}
	CWindowImpl<CMenuFader>::Create(NULL,&m_Rect,NULL,WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED|(bRtl?WS_EX_LAYOUTRTL:0));
	ShowWindow(SW_SHOWNOACTIVATE);
	if (m_Region)
	{
		SetWindowRgn(m_Region);
		m_Region=NULL;
	}
	SetTimer(1,20);
	m_Time0=0;
	m_LastTime=0;
	PostMessage(WM_TIMER,0,0);
	SetLayeredWindowAttributes(m_hWnd,0,255,LWA_ALPHA);
}

LRESULT CMenuFader::OnEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	RECT rc;
	GetClientRect(&rc);
	HDC hdc=(HDC)wParam;

	// draw the background
	HDC hdc2=CreateCompatibleDC(hdc);
	HGDIOBJ bmp0=SelectObject(hdc2,m_Bitmap);
	BitBlt(hdc,0,0,rc.right,rc.bottom,hdc2,0,0,SRCCOPY);
	SelectObject(hdc2,bmp0);
	DeleteDC(hdc2);
	return 1;
}

LRESULT CMenuFader::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_Time0==0)
		m_Time0=GetMessageTime();
	int t=GetMessageTime()-m_Time0;
	const int MAX_DELTA=80; // allow at most 80ms between redraws. if more, slow down time
	if (t>MAX_DELTA+m_LastTime)
	{
		m_Time0+=t-MAX_DELTA-m_LastTime;
		t=MAX_DELTA+m_LastTime;
	}
	m_LastTime=t;
	if (t<m_Duration)
	{
		SetLayeredWindowAttributes(m_hWnd,0,(m_Duration-t)*255/m_Duration,LWA_ALPHA);
		RedrawWindow();
	}
	else
	{
		KillTimer(1);
		PostMessage(WM_CLOSE);
	}
	return 0;
}

void CMenuFader::ClearAll( void )
{
	while (!s_Faders.empty())
		s_Faders[0]->SendMessage(WM_CLOSE);
}
