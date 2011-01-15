﻿//////////////////////////////////////////////////////
//
// Projet TFTPD32.  March 2007 Ph.jounin
// File gui_mai.c:  The MAIN window management
//
//
//////////////////////////////////////////////////////


#include "headers.h"

// as global variables, we have the settings both general and DHCP)
struct S_DHCP_Param  sGuiParamDHCP;
struct S_Tftpd32Settings sGuiSettings;
HWND      hDbgMainWnd ;
struct hostent  HostEntry;  // seel tftp_ip.c

static SOCKET    sService;

char szConsoleHost[32] = "127.0.0.1";


static BOOL g_hourglass = FALSE;
void SetHourglass(BOOL hourglass)
{
	g_hourglass = hourglass;
}
BOOL GetHourglass (void) { return g_hourglass; }

			  
// id of system Menu (SYSMENU)
enum { IDM_TFTP_HIDE = 0x1000, IDM_TFTP_EXPLORER, IDM_STOP_SERVICES, IDM_START_SERVICES, IDM_RESTART_SERVICES };     // should be under 0xF000



///////////////////////////////
// verify that all transfer are terminated
// by listing list view
BOOL GuiIsActiveTransfer ( HWND hMainWnd )
{
return ListView_GetItemCount ( GetDlgItem (hMainWnd, IDC_LV_TFTP) ) > 0 ;
} // GuiIsActiveTransfer


///////////////////////////////
// return selected directory in combo box
char *GetActiveDirectory (char *szActiveDirectory, int nSize)
{
int n;
HWND hCBWnd = GetDlgItem (hDbgMainWnd, IDC_CB_DIR);
  szActiveDirectory[0]=0;
  n = ComboBox_GetCurSel (hCBWnd);
   if (     n!=CB_ERR  
        &&  ComboBox_GetLBTextLen (hCBWnd, n) < nSize -1  )
   {
      int len = ComboBox_GetLBText (hCBWnd, n, szActiveDirectory);
      if (szActiveDirectory [len-1] != '\\')  szActiveDirectory[len]='\\', szActiveDirectory[len+1]=0;
      return szActiveDirectory;
   }
return NULL;
} // GetActiveDirectory


//////////////////////////////////////////////////////
//
//  GUI management
//
//////////////////////////////////////////////////////

/* ------------------------------------------------- */
/* The control of the edit box IDC_CB_DIR            */
/* just intercept ESC & ENTER Key from edit box ctrl */
/* ------------------------------------------------- */
static WNDPROC lpfnCBDirEditWndProc; // original wndproc for the combo box 
LRESULT CALLBACK CBDirSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{ 
HWND hwndMain = GetParent (GetParent (hwnd));
    switch (msg) 
    { 
        case WM_KEYDOWN: 
            break;  
        case WM_KEYUP: 
        case WM_CHAR: 
            switch (wParam) 
            { 
				// send message to main evant loop
                case VK_TAB: 
                case VK_ESCAPE: SendMessage(hwndMain, WM_ESC_EDITBOX, 0, (LPARAM) hwnd); 
					            return 0;
                case VK_RETURN: SendMessage(hwndMain, WM_ENTER_EDITBOX, 0, (LPARAM) hwnd); 
                                return 0; 
            } 
    } 
    // Call the original window procedure for default processing. 
    return CallWindowProc(lpfnCBDirEditWndProc, hwnd, msg, wParam, lParam); 
}  // CBDirSubClassProc


/* ------------------------------------------------- */
/* Display the IP addresses into a Combo BOX         */
/* ------------------------------------------------- */

int ChangeIPAddress (HWND hWnd, int nb_addr, struct S_IPAddressEntry *ent)
{
HWND hCBWnd = GetDlgItem (hWnd, IDC_CB_IP);
int Ark;

   ComboBox_ResetContent (hCBWnd);
   for ( Ark=0 ; Ark<nb_addr ; Ark++ )
   {
       ComboBox_AddString (hCBWnd, inet_ntoa (ent[Ark].address));
   }
   SetDlgItemText (hWnd, IDC_TXT_ADDRESS, Ark>=2 ? "单服务器接口" : "多服务器接口" );
   // Settings button can be safely allowed
   Button_Enable (GetDlgItem (hWnd, IDC_SETTINGS_BUTTON), TRUE);
   // Display first address
   ComboBox_SetCurSel (hCBWnd, 0);

return Ark;
} // ChangeIPAddress


static int SubClassCombo (HWND hMainWnd, int nId, WNDPROC cbk)
{
HWND hWndEdit, hWndCBBox;
POINT pt={5,5};
    // subclass the edit box part of the combo box IDC_CB_DIR
    // code copied from MSDN "Creating a Combo Box Toolbar"
	hWndCBBox = GetDlgItem (hMainWnd, nId);
	hWndEdit = ChildWindowFromPoint(hWndCBBox, pt);
    // lpfnCBDirEditWndProc = (WNDPROC) SetWindowLong (hWndEdit, GWL_WNDPROC, (DWORD) cbk); 
    lpfnCBDirEditWndProc = (WNDPROC) SetWindowLongPtr (hWndEdit, GWLP_WNDPROC,(LONG_PTR) cbk); 
return 0;
} // SubClassCombo



/* ------------------------------------------------- */
/* VM_COMMAND                                        */
/* ------------------------------------------------- */
static int Handle_VM_Command (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
int         wItem = (int) LOWORD (wParam);
HWND        hSSWindow;

   // Our "manual" subclassing :
   // If the button belongs to a sub window, we forward the message
   // hSSWindow =  (HWND) GetWindowLong (GetDlgItem (hWnd, wItem), GWL_USERDATA);
   hSSWindow =  (HWND) GetWindowLongPtr (GetDlgItem (hWnd, wItem), (LONG_PTR)GWLP_USERDATA);
   if (hSSWindow!=NULL)
   {
       PostMessage (hSSWindow, WM_COMMAND, wParam, lParam);
       return FALSE;
   }

   // otherwise we have to deal with the message
   switch ( wItem )
   {
       case IDC_CB_IP :
           if (HIWORD(wParam) == CBN_SELCHANGE)
           {char szIP[20];
            HWND hCBWnd = (HWND) lParam;
           int n = ComboBox_GetCurSel (hCBWnd); 
            if (ComboBox_GetLBTextLen (hCBWnd, n) < sizeof szIP)
             {
                ComboBox_GetLBText (hCBWnd, n, szIP);
                CopyTextToClipboard (szIP);
              }
           }
          break;

       case IDC_ABOUT_BUTTON :
           OpenNewDialogBox (hWnd,
                             IDD_DIALOG_ABOUT,
                             (DLGPROC) AboutProc, 
                             0, 
                             NULL );
           break;

       case IDC_SHDIR_BUTTON :
           // Ask directory content to the service
		   // dialog window will be displayed once the 
		   // message C_REPLY_DIRECTORY_CONTENT is rcvd
           Gui_RequestListDirectory (sService);
           break;

       // The buttons
       case IDC_TFTPD_HELP :
            // WinHelp(hWnd, szTftpd32Help, HELP_CONTENTS, 0);
            ShellExecute (hWnd, "open", szTftpd32Help,  NULL, NULL, SW_NORMAL);
            break;

       case IDC_SETTINGS_BUTTON :
            OpenNewDialogBox (hWnd,
                              IDD_DIALOG_SETTINGS,
                              (DLGPROC) SettingsProc, 
							  0, NULL);
             break;

       // unselect the selected line as soon as the window loses its focus
       // en dehors
       case IDC_LB_LOG :
       case IDC_LB_SYSLOG :
	   case IDC_LB_DNS :
            if (HIWORD(wParam) == LBN_KILLFOCUS)
                SendMessage ((HWND) lParam, LB_SETCURSEL, (WPARAM) -1, 0);
            break;

       case IDC_BROWSE_BUTTON :
          // since 2.81 we have a combo box to manage
            if ( MyBrowseWindow (hWnd, sGuiSettings.szWorkingDirectory, TRUE) )
            {
                TftpDir_AddEntry (GetDlgItem (hWnd, IDC_CB_DIR), sGuiSettings.szWorkingDirectory);
                PostMessage (hWnd, WM_TFTP_CHG_WORKING_DIR, 0, 0);
            }
            break;

     case IDC_CB_DIR :
		  if (   HIWORD(wParam) == CBN_SELCHANGE )
		  {
                TftpDir_SelectEntry ((HWND) lParam) ;
                PostMessage (hWnd, WM_TFTP_CHG_WORKING_DIR, 0, 0);
		  }
         break;
#ifdef ALL_RELEASES_UNTIL_3_35
	  // DHCP controls
      case IDC_DHCP_OK :
               if (Gui_DHCPSaveConfig (hWnd))
                  CMsgBox (hWnd, "DHCP 设置成功保存", APPLICATION, MB_OK);
               PostMessage (hWnd, WM_SAVE_DHCP_SETTINGS, 0, 0);
               break;
#endif

      case ID_DELETE_ASSIGNATION :
           { 
		     HWND    hListV   = GetDlgItem (hWnd, IDC_LV_DHCP);
			 int  Ark;
			 // bug fixed by Colin : do nothing unless GetNextItem is successfull
             if ( (Ark = ListView_GetNextItem (hListV, -1, LVNI_FOCUSED)) != -1) 
			 {LVITEM LvItem;
			 	 LvItem.iItem = Ark;
				 LvItem.iSubItem =0;
                 LvItem.mask = LVIF_PARAM;
                 if ( ListView_GetItem (hListV, & LvItem) )
                     PostMessage ( hWnd, WM_DELETE_ASSIGNATION, 0, LvItem.lParam );
			 } // ListView_GetNextItem succesfull
           } //ID_DELETE_ASSIGNATION
           break;

	   // Syslog controls
		case IDC_SYSLOG_CLEAR :
			ListView_DeleteAllItems (GetDlgItem (hWnd, IDC_LB_SYSLOG));
			break;

		case IDC_SYSLOG_COPY :
			CopyListViewToClipboard (GetDlgItem (hWnd, IDC_LB_SYSLOG), 3);
			break;

		// Tftp server controls
		 case IDC_TFTP_CLEAR :
			 SendDlgItemMessage (hWnd, IDC_LB_LOG, LB_RESETCONTENT, 0, 0);
			 break;

	   case IDC_TFTP_COPYTOCLIP :
			 CopyListBoxToClipboard (GetDlgItem (hWnd, IDC_LB_LOG));
			 break;

	   case ID_STOP_TRANSFER :
		   { LVITEM LvItem;

			 LvItem.iItem = ListView_GetNextItem (GetDlgItem (hWnd, IDC_LV_TFTP), -1, LVNI_FOCUSED);
			 LvItem.iSubItem =0;
			 LvItem.mask = LVIF_PARAM;
			 if (ListView_GetItem (GetDlgItem (hWnd, IDC_LV_TFTP), & LvItem)  &&  LvItem.lParam!=(LPARAM) NULL)
				 PostMessage (hWnd, WM_TFTP_TRANFSER_TO_KILL, 0, LvItem.lParam);
		   }
		   break;
	   

   } // switch message
return FALSE;
} // Handle_VM_Command


static int Handle_VM_Notify (HWND hDlgWnd, WPARAM wParam, LPNMHDR pnmh)
{
WORD wItem = LOWORD (wParam);
HWND        hSSWindow;

    // hSSWindow =  (HWND) GetWindowLong (GetDlgItem (hDlgWnd, wItem), GWL_USERDATA);
    hSSWindow =  (HWND) GetWindowLongPtr (GetDlgItem (hDlgWnd, wItem), (LONG_PTR)GWLP_USERDATA);

    // The "manual" subclassing
    // If the button belongs to a sub window, we forward the message
    // This code has been inserted in order to allow the alternate background in ListViews
    if (hSSWindow!=NULL)
    {
       SendMessage (hSSWindow, WM_NOTIFY, wParam, (LPARAM) pnmh);
       // should return TRUE
       return pnmh->code==NM_CUSTOMDRAW;
    }
   
	
   switch ( wItem )
    {
	   // tab view
       case IDC_TAB_OPTION :
           if (pnmh->code == TCN_SELCHANGE)
                TR_ChangeTabControl (hDlgWnd);
           break;

	   // DHCP list view
	   case  IDC_LV_DHCP :
           switch (pnmh->code)
		   {
				case NM_RCLICK: // pop a drop down menu
					    DhcpServerListPopup (GetDlgItem (hDlgWnd, IDC_LV_DHCP));
						break;
		   }
		   break;


	   // syslog List view : item colorization
	   case IDC_LB_SYSLOG :
		   switch (pnmh->code)
		   {
				case NM_CUSTOMDRAW :
					  // SetWindowLong (hDlgWnd, DWL_MSGRESULT, (LONG) ProcessCustomDraw ( (LPARAM) pnmh) );
					  SetWindowLongPtr (hDlgWnd, DWLP_MSGRESULT, (LONG_PTR)ProcessCustomDraw ( (LPARAM) pnmh) );
					  return TRUE;

				case LVN_COLUMNCLICK :  // unused since our ListView does not support sorting
   					  ListView_SortItems (GetDlgItem (hDlgWnd, IDC_LB_SYSLOG), CompareStringFunc, pnmh);
					  break;
			} // Which action
		   break;

	   // tftp server view
	   case IDC_LV_TFTP :
		   switch (pnmh->code)
		   {
			   // alternate background colors
				case NM_CUSTOMDRAW :
					   // SetWindowLong (hDlgWnd, DWL_MSGRESULT, (LONG) ProcessCustomDraw ( (LPARAM) pnmh) );
					   SetWindowLongPtr (hDlgWnd, DWLP_MSGRESULT,(LONG_PTR) ProcessCustomDraw ( (LPARAM) pnmh) );
					   return TRUE;

				case LVN_COLUMNCLICK :  // unused
						ListView_SortItems (GetDlgItem (hDlgWnd, IDC_LV_TFTP), CompareStringFunc, pnmh);					
						break;

				case NM_RCLICK: // pop a drop down menu
						TftpServerListPopup (GetDlgItem (hDlgWnd, IDC_LV_TFTP));
					   break;
		   }
		   break;

    } // switch Action
    FORWARD_WM_NOTIFY (hDlgWnd, wParam, pnmh, DefWindowProc );
return FALSE;
} // Handle_VM_Notify



int Gui_CreateAndSubclassWindow (HWND hWnd)
{
HWND hNW;

   // Starts a background window for The TFTP client function
   // Client controls on the main window is attached to a background window
   // with SetWindowLong (Control, GWL_USERDATA, BackGroundWindow)
   // Once an action is reported in WM_COMMAND, the HandleVMCommand
   // function forward the message to the matching background window
   // This architecture allow a new function to be added without much damage
   // on the existing ones

   // TFTP client

      if (sGuiSettings.uServices & TFTPD32_TFTP_CLIENT)
      {
       hNW = CreateBckgWindow (hWnd, WM_INITCLIENT, (WNDPROC) TftpClientProc, TFTP_CLIENT_CLASS, APPLICATION);
#ifdef ONLY32BITS
       SetWindowLong (GetDlgItem (hWnd, IDC_CLIENT_BROWSE),      GWL_USERDATA, (LONG) hNW);
       SetWindowLong (GetDlgItem (hWnd, IDC_CLIENT_GET_BUTTON),  GWL_USERDATA, (LONG) hNW);
       SetWindowLong (GetDlgItem (hWnd, IDC_CLIENT_SEND_BUTTON), GWL_USERDATA, (LONG) hNW);
       SetWindowLong (GetDlgItem (hWnd, IDC_CLIENT_BREAK_BUTTON),GWL_USERDATA, (LONG) hNW);
#endif
	   SetWindowLongPtr (GetDlgItem (hWnd, IDC_CLIENT_BROWSE),      GWLP_USERDATA, (LONG_PTR) hNW);
       SetWindowLongPtr (GetDlgItem (hWnd, IDC_CLIENT_GET_BUTTON),  GWLP_USERDATA, (LONG_PTR) hNW);
       SetWindowLongPtr (GetDlgItem (hWnd, IDC_CLIENT_SEND_BUTTON), GWLP_USERDATA, (LONG_PTR) hNW);
       SetWindowLongPtr (GetDlgItem (hWnd, IDC_CLIENT_BREAK_BUTTON),GWLP_USERDATA, (LONG_PTR) hNW);
     }

return 0;
}  // Gui_CreateAndSubClassBckgWnd

 

static const struct S_LVHeader tColDhcp [] =
{
	{ LVCFMT_LEFT,     90, "分配于" },
	{ LVCFMT_LEFT,	  100, "IP地址" },
	{ LVCFMT_LEFT,    100, "MAC地址" },
	{ LVCFMT_LEFT,     90, "更新于" },
};
static const struct S_LVHeader tColTftp[] =
{
    { LVCFMT_LEFT,    120, "peer" },
    { LVCFMT_LEFT,	  100, "文件" },
    { LVCFMT_LEFT,     60, "开始时间" },
    { LVCFMT_CENTER,   55, "进度" },
    { LVCFMT_RIGHT,    80, "字节" },
    { LVCFMT_RIGHT,    80, "总数" },
    { LVCFMT_RIGHT,    50, "超时" },
};
static const struct S_LVHeader tColsysLog [] =
{
	{ LVCFMT_LEFT,    250, "文本" },
	{ LVCFMT_LEFT,	  100, "从" },
	{ LVCFMT_LEFT,    100, "日期" },
};
static const struct S_LVHeader tColDNS [] =
{
	{ LVCFMT_LEFT,    150, "主机名" },
	{ LVCFMT_LEFT,	  100, "ipv4" },
	{ LVCFMT_LEFT,    100, "ipv6" },
};


int Tftpd32InitGui (HWND hWnd, HICON *phIcon, HMENU *phMenu)
{
	SetWindowText (hWnd, APP_TITLE);
    hDbgMainWnd = hWnd;  // pour debug
	// prepare to receive messages
    WSAAsyncSelect (sService, hWnd, WM_RECV_FROM_THREAD, FD_READ | FD_CLOSE);
    Gui_RequestWorkingDir (sService);

	// WM_RESIZE_MAIN_WINDOW
   // Inits Windows : Register our (beautiful) Icon 
     *phIcon = LoadIcon  (GetWindowInstance(hWnd), MAKEINTRESOURCE (IDI_TFTPD32));
     // SetClassLong (hWnd, GCL_HICON, (LONG) *phIcon );
     SetClassLongPtr (hWnd, GCLP_HICON, (LONG_PTR) *phIcon );
     // Add the Hide Sysmenu item
     *phMenu = GetSystemMenu (hWnd, FALSE);
     if (*phMenu != NULL)
     {
        AppendMenu (*phMenu, MF_SEPARATOR, 0, NULL);
#ifdef STANDALONE_EDITION
        AppendMenu (*phMenu, MF_STRING, IDM_TFTP_HIDE, "隐藏窗口");
#endif
        AppendMenu (*phMenu, MF_STRING, IDM_TFTP_EXPLORER, "打开 TFTP 目录");
#ifdef SERVICE_EDITION
        AppendMenu (*phMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu (*phMenu, MF_STRING, IDM_RESTART_SERVICES, "Restart Tftpd32 services");
#endif
     }
     // Create Tabs and select The items to be displayed
     TR_InitTabControl (hWnd);

     // Call the resize window. 
     // Hmm its done in RetrievWindowPos : To be removed ???
     TR_ResizeWindow (hWnd, TRUE);

     // restore Tftpd32 size
     Tftpd32RetrieveWindowPos (hWnd);
     ////////////////
     // Add our icon into SysTray
     TrayMessage (hWnd, NIM_ADD, *phIcon, TASKTRAY_ID, WM_NOTIFYTASKTRAY);

     Button_Enable (GetDlgItem (hWnd, IDC_SETTINGS_BUTTON), FALSE);

     // Deactivate Browse Button
     if ( IsGuiConnectedToRemoteService () )
        Button_Enable (GetDlgItem (hWnd, IDC_BROWSE_BUTTON), FALSE);


    // Creates List Views
    InitTftpd32ListView (GetDlgItem (hWnd, IDC_LV_TFTP),   tColTftp, SizeOfTab (tColTftp), LVS_EX_FULLROWSELECT);
    InitTftpd32ListView (GetDlgItem (hWnd, IDC_LV_DHCP),   tColDhcp, SizeOfTab (tColDhcp), LVS_EX_FULLROWSELECT);
    InitTftpd32ListView (GetDlgItem (hWnd, IDC_LB_SYSLOG), tColsysLog, SizeOfTab (tColsysLog), LVS_EX_FULLROWSELECT);
    InitTftpd32ListView (GetDlgItem (hWnd, IDC_LB_DNS),    tColDNS, SizeOfTab (tColDNS), LVS_EX_FULLROWSELECT);

     // Create the sub TFTP client window
    Gui_CreateAndSubclassWindow (hWnd);

    Gui_RequestIPInterfaces (sService);


return TRUE;
} // Tftpd32InitGui

/* ------------------------------------------------- */
/* Main CallBack                                     */
/* ------------------------------------------------- */
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LONG lParam)
{
static HICON    hIcon;
static HMENU     hMenu;
int       Rc;

  switch (message)
  {

       // Ths Inits parts is splitted into 5 parts, at the end of a part a message is posted in order
       // to invoke the next part and let Windows (98 !) parse its own messages
       case WM_INITDIALOG :
			Tftpd32InitGui (hWnd, &hIcon, &hMenu);
            HostEntry.h_addr_list = NULL;
            SetTimer (hWnd, WM_INIT_DISPLAY, 500, NULL);
			break;

        case WM_INIT_DISPLAY :
             ShowWindow (hWnd, TRUE);
#ifdef STANDALONE_EDITION
			 SubClassCombo (hWnd, IDC_CB_DIR, CBDirSubClassProc);
#endif
             if (sGuiSettings.WinSize>0)
                   SendDlgItemMessage (hWnd, IDC_LB_LOG, LB_ADDSTRING,
                                           0, (LPARAM) "Warning: Anticipation window in use");
             PostMessage (hWnd, WM_DISPLAY_LISTEN, 0, 0);
             break;



        ////////////////////////////////////////////////////
        // End of inits
        // GUI and asynchronous events
        ////////////////////////////////////////////////////

       ////////////////////////////////
       // Message sent by Tftp server
       case WM_DISPLAY_LISTEN :
           if (sGuiSettings.bHide)
           {
             // this command moves the app off the screen without hiding it
              SetWindowPos (hWnd, 0,       // hAfterWnd
                            GetSystemMetrics(SM_CXSCREEN), 0,   // New pos (x, y)
                            0, 0, SWP_NOSIZE & SWP_NOZORDER);
            // this command will remove the taskbar entry in a while
              SetTimer (hWnd, WM_TIMER_HIDE_MAIN_WND,  2000, NULL);
              SetTimer (hWnd, WM_TIMER_REPOS_MAIN_WND, 1000, NULL);
           }           
           SetTimer (hWnd, WM_FREEMEM, 2000, NULL);
           break;

       ////////////////////////////////
       // End of Inits
       ////////////////////////////////

       // message received from daemon
       case WM_RECV_FROM_THREAD :
            WSAAsyncSelect(sService, hWnd, 0, 0);
            Rc = Gui_GetMessage (hWnd, sService, TRUE, 0);
			if (Rc == TCP4U_SOCKETCLOSED)
				PostMessage (hWnd, WM_SOCKET_CLOSED, 0, 0);
			else if (Rc<0)
				PostMessage (hWnd, WM_SOCKET_ERROR, 0, 0);
            else
				WSAAsyncSelect (sService, hWnd, WM_RECV_FROM_THREAD, FD_READ | FD_CLOSE);
            break;           

	   case WM_SOCKET_CLOSED :
		   CMsgBox (hWnd, "Tftpd32 服务结束\n这个会话将终止", "Tftpd32", MB_OK | MB_ICONEXCLAMATION);
		   PostMessage (hWnd, WM_CLOSE, 0, 0);
		   break;

	   case WM_SOCKET_ERROR :
		   CMsgBox (hWnd, "Tftpd32 已从服务断开\n这个会话将终止", "Tftpd32", MB_OK | MB_ICONEXCLAMATION);
		   PostMessage (hWnd, WM_CLOSE, 0, 0);
		   break;

       case WM_TIMER_HIDE_MAIN_WND :
#ifdef STANDALONE_EDITION
            ShowWindow (hWnd, SW_HIDE);
#endif
            break;

       case WM_TIMER_REPOS_MAIN_WND :
           // window is hidden outside the screen, repos it at its normal place
           SetWindowPos (hWnd, 0,
                           10, 10, 0, 0,  // New Pos (x, y)
                           SWP_NOSIZE & SWP_NOZORDER);
          break;

      case WM_FREEMEM :
          SetProcessWorkingSetSize (GetCurrentProcess (),-1,-1);
          break;

      case WM_TFTP_TRANFSER_TO_KILL :
          Gui_AbortTftpTransfer (sService, lParam);
          break; 
          
      case WM_TFTP_CHG_WORKING_DIR :
          Gui_ChangeWorkingDirectory (sService, sGuiSettings.szWorkingDirectory);
          // Ask console which is current directory
          Gui_RequestWorkingDir (sService);
          break;

      case WM_SAVE_SETTINGS :
          Gui_SaveSettings (sService, & sGuiSettings);
          // Working Directory should be set to base directory by daemon
          lstrcpy (sGuiSettings.szWorkingDirectory, sGuiSettings.szBaseDirectory);
          Gui_RequestWorkingDir (sService);
          break;
      
      case WM_SAVE_DHCP_SETTINGS :
          Gui_SaveDhcpSettings (sService, & sGuiParamDHCP);
          break;
          
      case WM_DELETE_ASSIGNATION :
          Gui_SuppressDHCPAllocation (sService, lParam  );
          break;

	  case WM_DESTROY_SETTINGS :
		  Gui_DestroySettings (sService);
		  break;

	  case WM_START_SERVICES :
		  Gui_StartAllServices (sService);
		  break;

	  // The edit box of the combo box IDC_CB_DIR has been modified 
	  // Change working directory
      case WM_ENTER_EDITBOX :
		  {char sz [MAX_PATH];
			   sz[MAX_PATH-1] = 0;
			   GetWindowText  ( (HWND) lParam, sz, sizeof sz - 1);
			   if (strcmp (sGuiSettings.szWorkingDirectory, sz)!=0)
			   {
				   if ( IsValidDirectory (sz) )  
				   {
						strcpy (sGuiSettings.szWorkingDirectory, sz);
						PostMessage (hWnd, WM_TFTP_CHG_WORKING_DIR, 0, 0);
						CMsgBox (hWnd, "New base directory : %s", "Tftpd32", MB_OK, sz);
		   		   }
				   else 
				   {
					   PostMessage (hWnd, WM_ESC_EDITBOX, 0, lParam);
					   CMsgBox (hWnd, "Invalid Directory : %s", "Tftpd32", MB_OK, sz);
				   }
			   } // text has been modified
		  }
		  break;
	  case WM_ESC_EDITBOX :
		  // restore previous settings
 		  ComboBox_SetText ((HWND) lParam, sGuiSettings.szWorkingDirectory);
		  break;


#ifdef OLD_CODE          
       ////////////////////////////////
       // Sent by IP address background window
       case WM_ADD_IP_CB :
           if ( (void *) lParam!=NULL )    HostEntry = * (struct hostent *) lParam;
           Rc = FillCBLocalIP (GetDlgItem (hWnd, IDC_CB_IP), TRUE, sGuiSettings.szLocalIP);
           SetDlgItemText (hWnd, IDC_TXT_ADDRESS, Rc>=2 ? "Server interface" : "Server interfaces" );
           // Settings button can be safely allowed
           Button_Enable (GetDlgItem (hWnd, IDC_SETTINGS_BUTTON), TRUE);
           break;
#endif

       ////////////////////////////////////////////////////
       // GUI
       ////////////////////////////////////////////////////

       case WM_NOTIFYTASKTRAY :
            switch (lParam)
            {POINT Pt;
                case WM_RBUTTONDOWN:
                   GetCursorPos (& Pt);
                   Rc = TrackPopupMenu (GetSystemMenu (hWnd, FALSE), TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD,
                                       Pt.x, Pt.y, 0, hWnd, NULL);
                   if (Rc!=0) PostMessage (hWnd, WM_SYSCOMMAND, Rc, 0);
                   break;
                case WM_LBUTTONDBLCLK :
                    ShowWindow (hWnd, SW_SHOWNORMAL);
                    SetForegroundWindow (hWnd);
                    CheckMenuItem (GetSystemMenu (hWnd, FALSE), IDM_TFTP_HIDE, MF_UNCHECKED);
                    break;
            }
            break;

            
       // request message
/*       case WM_SIZE :
            if (wParam==SIZE_MINIMIZED)
                 ShowWindow (hWnd, SW_HIDE);   // Hide Window
            break;
*/

       case WM_SIZE :
             TR_ResizeWindow (hWnd, FALSE);
             break;

       case WM_GETMINMAXINFO :
             return TR_MinMaxInfo (hWnd, (LPMINMAXINFO) lParam);


       case WM_QUERYDRAGICON :
          return (LONG) hIcon;

       case WM_CLOSE :
         {HWND hChildWnd;
            if ( GuiIsActiveTransfer (hWnd) )
                if (CMsgBox (hWnd, "取消当前传输 ?", APPLICATION, MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
                    return FALSE;

            // send stop message to services
             hChildWnd = GetWindow (hWnd, GW_CHILD);
             do
             { 
				 PostMessage (hChildWnd, WM_CLOSE, 0, 0); 
			 }
             while ( (hChildWnd=GetNextWindow (hChildWnd, GW_HWNDNEXT) ) != NULL);
	      } // end of hClidWnd
#ifdef STANDALONE_EDITION             
			 LogToMonitor ("------- 停止服务中\n");            
             Gui_StopAllServices (sService);
             closesocket (sService);
			 Sleep (200);
		     LogToMonitor ("------ 服务已停止\n");            
#endif
             // Save window pos and size
             // To be comment out to create a new Tftpd32-Portable version 
             Tftpd32SaveWindowPos (hWnd);

            TrayMessage (hWnd, NIM_DELETE, hIcon, TASKTRAY_ID, WM_NOTIFYTASKTRAY);
			 DestroyMenu (hMenu);
         // Fall Through

	   case WM_DESTROY :

            DestroyIcon (hIcon);
          EndDialog (hWnd, 0);
          break;

       case WM_SYSCOMMAND :
			 switch (wParam)
			 {
				case IDM_TFTP_HIDE :
                    CheckMenuItem (GetSystemMenu (hWnd, FALSE), IDM_TFTP_HIDE,IsWindowVisible(hWnd) ? MF_CHECKED : MF_UNCHECKED);
                    ShowWindow (hWnd, IsWindowVisible(hWnd) ? SW_HIDE  : SW_SHOW);
					break;
				case IDM_TFTP_EXPLORER :
                    StartExplorer ();
					break;
				case IDM_STOP_SERVICES :
					Gui_SuspendServices (sService);
					break;
				case IDM_START_SERVICES :
					Gui_StartAllServices (sService);
					break;
				case IDM_RESTART_SERVICES :
					Gui_SuspendServices (sService);
					SetTimer (hWnd, WM_START_SERVICES, 1000, NULL);
					break;
			 } // switch system menu choice
             break;

       case WM_COMMAND :
            Handle_VM_Command (hWnd, wParam, lParam);
            break;

       case WM_NOTIFY :
            return Handle_VM_Notify (hWnd,  wParam, (LPNMHDR) lParam);
            break;

       case WM_TIMER :
            KillTimer (hWnd, wParam);
            PostMessage (hWnd, (UINT) wParam, 0, 0);
            break;

	   case WM_SETCURSOR:
		   //If we are doing some operation, set the cursor away from the default
		   if (GetHourglass())
		   {
			   SetCursor(LoadCursor(NULL, IDC_WAIT));
			   // SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
			   SetWindowLongPtr(hWnd, DWLP_MSGRESULT, TRUE);
			   return TRUE;
		   }
           break;

  }
return FALSE;
} // CALLBACK Wndproc


// ---------------------------------------------------------------------------
// job before GUI
// ---------------------------------------------------------------------------

void ReturnToPreviousInstance (HANDLE myMutEx, LPCSTR lpszCmdLine)
{
HWND hPrevWnd;
    if (myMutEx!=NULL)  CloseHandle (myMutEx);
    // Get runnning session
    hPrevWnd = FindWindow (NULL, APP_TITLE);
    // option -kill -> close previous instance
    if (strstr (lpszCmdLine, "-kill") != NULL)
    {
        PostMessage (hPrevWnd, WM_CLOSE, 0, 0);
        Sleep (100);
        if (IsWindow (hPrevWnd)) { Sleep (1000); PostMessage (hPrevWnd, WM_DESTROY, 0, 0); }
    }
    else // Call previous session back
    {
       if (hPrevWnd == NULL)
          MessageBox (NULL, "Tftpd32 已经在运行", APPLICATION, MB_OK | MB_ICONSTOP);
       else
       {
          SetForegroundWindow (hPrevWnd);
          ShowWindow (hPrevWnd, SW_SHOWNORMAL);
        }
     }
} // ReturnToPreviousInstance



int InitsConsoleConnection (const char *szHost)
{
int Rc;
int Ark=0;
	do
	{
	    // conect to console, service may have passed port
		// through sSguiSettings structure
		sService = TcpConnect (szHost, 
							   "tftpd32", 
							   sGuiSettings.uConsolePort==0 ? TFTPD32_TCP_PORT : sGuiSettings.uConsolePort);
		if (sService==INVALID_SOCKET) { Rc = GetLastError ();  Sleep (500); }
	}
	while (Ark++<=3 && sService==INVALID_SOCKET);

	if (sService != INVALID_SOCKET) LogToMonitor ("已连接到控制台\n");
	else 
	{  
		CMsgBox (NULL, "不能连接到服务\n错误 %d (%s)", 
				 APPLICATION, 
				 MB_OK, 
				 GetLastError (),
				 LastErrorText ());
		return 0;
	}

// // Verify Versions 
    lstrcpy (sGuiSettings.szConsolePwd, DFLT_CONSOLE_PWD);	
    if (g_VERSION == SERVICE_EDITION_VALUE)
         GetEnvironmentVariable (TFTP_PWD, sGuiSettings.szConsolePwd, sizeof sGuiSettings.szConsolePwd);

    Rc = TcpExchangeChallenge (sService, 0xF3271, CURRENT_PROTOCOL_VERSION,  sGuiSettings.szConsolePwd);
    if ( Rc < 0 ) 
    {
        CMsgBox (NULL, 
                 Rc==TCP4U_BAD_AUTHENT ? "无效密码"  : "控制台版本不匹配服务", 
                 APPLICATION, 
                 MB_OK | MB_ICONERROR);
        return 0;
    }
LogToMonitor ( "GUI 版本检查成功\n" );             // permament listening socket

#ifdef STANDALONE_EDITION
	 // Standalone edition shoud wait for services to be started
	 Gui_GetMessage (NULL, sService, TRUE, C_SERVICES_STARTED);
#endif

	// services should be started : ask settings and what is started
     Gui_AskTFTPSettings (sService);
	 Gui_GetMessage (NULL, sService, TRUE, C_TFTP_RPLY_SETTINGS);
     Gui_AskDHCPSettings (sService); 
	 Gui_GetMessage (NULL, sService, TRUE, C_DHCP_RPLY_SETTINGS);
	 Gui_RequestRunningServices (sService);
	 Gui_GetMessage (NULL, sService, TRUE, C_REPLY_GET_SERVICES);

return 1;     
} // InitsConsoleConnection 


int GuiMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine, int nCmdShow)
{
#define  TFTPD32_MUTEX          "<Tftpd32> by Ph. Jounin MutEx"
HANDLE                myMutEx;
INITCOMMONCONTROLSEX  InitCtrls;

   // read command line and set environment variables
   ProcessCmdLine (lpszCmdLine);

   // Detect an already running Tftpd32
   myMutEx = CreateMutex (NULL, TRUE, TFTPD32_MUTEX);
   if (     strstr (lpszCmdLine, "-kill") != NULL
        || (myMutEx!=NULL  &&  GetLastError()==ERROR_ALREADY_EXISTS)  )
   {
        ReturnToPreviousInstance (myMutEx, lpszCmdLine);
        return 0;
   }
    // Locate help file
    SetIniFileName (HELPFILE, szTftpd32Help);

     // Inits extended controls (ListView). MSDN says it is required, but seems really optionnal !
     InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
     InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
     InitCommonControlsEx(&InitCtrls);

     // Opens socket
#ifdef SERVICE_EDITION
	 // read host which may be not local
	 GetEnvironmentVariable (TFTP_HOST, szConsoleHost, sizeof szConsoleHost);
#endif
     if (! InitsConsoleConnection (szConsoleHost)) return 0;

     // starts GUI
     OpenNewDialogBox (NULL, IDD_TFTPD32, (DLGPROC) WndProc, 0, hInstance);

     UnregisterClass (TFTPD32_ADDIP_CLASS, hInstance);
     UnregisterClass (TFTP_CLIENT_CLASS, hInstance);
     ReleaseMutex (myMutEx);
    CloseHandle (myMutEx);
return 1;
} // GuiMain 