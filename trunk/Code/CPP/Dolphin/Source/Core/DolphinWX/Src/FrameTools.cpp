﻿// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/


/*
1.1 Windows

CFrame is the main parent window. Inside CFrame there is m_Panel which is the
parent for the rendering window (when we render to the main window). In Windows
the rendering window is created by giving CreateWindow() m_Panel->GetHandle()
as parent window and creating a new child window to m_Panel. The new child
window handle that is returned by CreateWindow() can be accessed from
Core::GetWindowHandle().
*/


#include "Setup.h" // Common

#include "NetWindow.h"
#include "Common.h" // Common
#include "FileUtil.h"
#include "FileSearch.h"
#include "Timer.h"

#include "Globals.h" // Local
#include "Frame.h"
#include "ConfigMain.h"
#include "PluginManager.h"
#include "MemcardManager.h"
#include "CheatsWindow.h"
#include "LuaWindow.h"
#include "AboutDolphin.h"
#include "GameListCtrl.h"
#include "BootManager.h"
#include "LogWindow.h"
#include "WxUtils.h"

#include "ConfigManager.h" // Core
#include "Core.h"
#include "OnFrame.h"
#include "HW/CPU.h"
#include "PowerPC/PowerPC.h"
#include "HW/DVDInterface.h"
#include "HW/ProcessorInterface.h"
#include "IPC_HLE/WII_IPC_HLE_Device_usb.h"
#include "State.h"
#include "VolumeHandler.h"
#include "NANDContentLoader.h"
#include "WXInputBase.h"

#include <wx/datetime.h> // wxWidgets


// Resources
extern "C" {
#include "../resources/Dolphin.c" // Dolphin icon
#include "../resources/toolbar_browse.c"
#include "../resources/toolbar_file_open.c"
#include "../resources/toolbar_fullscreen.c"
#include "../resources/toolbar_help.c"
#include "../resources/toolbar_pause.c"
#include "../resources/toolbar_play.c"
#include "../resources/toolbar_plugin_dsp.c"
#include "../resources/toolbar_plugin_gfx.c"
#include "../resources/toolbar_plugin_options.c"
#include "../resources/toolbar_plugin_pad.c"
#include "../resources/toolbar_plugin_wiimote.c"
#include "../resources/toolbar_refresh.c"
#include "../resources/toolbar_stop.c"
#include "../resources/Boomy.h" // Theme packages
#include "../resources/Vista.h"
#include "../resources/X-Plastik.h"
#include "../resources/KDE.h"
};


// Other Windows
wxCheatsWindow* CheatsWindow;


// Create menu items
// ---------------------
void CFrame::CreateMenu()
{
	if (GetMenuBar()) GetMenuBar()->Destroy();

	m_MenuBar = new wxMenuBar(wxMB_DOCKABLE);

	// file menu
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, _T("打开游戏镜像(&O)...\tCtrl+O"));

	wxMenu *externalDrive = new wxMenu;
	m_pSubMenuDrive = fileMenu->AppendSubMenu(externalDrive, _T("从驱动器启动(&B)..."));
	
	drives = cdio_get_devices();
	// Windows Limitation of 24 character drives
	for (unsigned int i = 0; i < drives.size() && i < 24; i++) {
		externalDrive->Append(IDM_DRIVE1 + i, wxString::FromAscii(drives[i].c_str()));
	}

	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_REFRESH, _T("刷新游戏列表(&R)"));
	fileMenu->AppendSeparator();
	fileMenu->Append(IDM_BROWSE, _T("选择镜像目录(&B)..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _T("退出本模拟器(&X)\tAlt+F4"));
	m_MenuBar->Append(fileMenu, _T("游戏(&F)"));

	// Emulation menu
	wxMenu* emulationMenu = new wxMenu;
	emulationMenu->Append(IDM_PLAY, GetMenuLabel(HK_PLAY_PAUSE));
	emulationMenu->Append(IDM_STOP, GetMenuLabel(HK_STOP));
	emulationMenu->Append(IDM_RESET, _T("重置游戏(&R)"));
	emulationMenu->AppendSeparator();
	emulationMenu->Append(IDM_TOGGLE_FULLSCREEN, GetMenuLabel(HK_FULLSCREEN));	
	emulationMenu->AppendSeparator();
	emulationMenu->Append(IDM_RECORD, _T("开始录制(&C)..."));
	emulationMenu->Append(IDM_PLAYRECORD, _T("播放录制(&L)..."));
	emulationMenu->AppendSeparator();
	emulationMenu->Append(IDM_CHANGEDISC, _T("切换光盘(&D)"));
	
	emulationMenu->Append(IDM_FRAMESTEP, _T("帧数步进(&F)"), wxEmptyString, wxITEM_CHECK);

	wxMenu *skippingMenu = new wxMenu;
	m_pSubMenuFrameSkipping = emulationMenu->AppendSubMenu(skippingMenu, _T("跳帧加速(&K)"));
	for(int i = 0; i < 10; i++)
		skippingMenu->Append(IDM_FRAMESKIP0 + i, wxString::Format(_T("%i"), i), wxEmptyString, wxITEM_RADIO);

	emulationMenu->AppendSeparator();
	emulationMenu->Append(IDM_SCREENSHOT, _T("屏幕截图(&S)\tF9"));
	emulationMenu->AppendSeparator();
	wxMenu *saveMenu = new wxMenu;
	wxMenu *loadMenu = new wxMenu;
	m_pSubMenuLoad = emulationMenu->AppendSubMenu(loadMenu, _T("载入状态(&L)"));
	m_pSubMenuSave = emulationMenu->AppendSubMenu(saveMenu, _T("保存状态(&V)"));

	saveMenu->Append(IDM_SAVESTATEFILE, _T("保存状态文件..."));
	loadMenu->Append(IDM_UNDOSAVESTATE, _T("最后覆盖状态\tShift+F12"));
	saveMenu->AppendSeparator();

	loadMenu->Append(IDM_LOADSTATEFILE, _T("载入状态文件..."));
	loadMenu->Append(IDM_LOADLASTSTATE, _T("最后存档状态\tF11"));
	loadMenu->Append(IDM_UNDOLOADSTATE, _T("撤销载入状态\tF12"));
	loadMenu->AppendSeparator();

	for (int i = 1; i <= 8; i++) {
		loadMenu->Append(IDM_LOADSLOT1 + i - 1, wxString::Format(_T("插槽 %i\tF%i"), i, i));
		saveMenu->Append(IDM_SAVESLOT1 + i - 1, wxString::Format(_T("插槽 %i\tShift+F%i"), i, i));
	}
	m_MenuBar->Append(emulationMenu, _T("模拟(&E)"));

	// Options menu
	wxMenu* pOptionsMenu = new wxMenu;
	pOptionsMenu->Append(wxID_PREFERENCES, _T("程序设置(&N)..."));
	pOptionsMenu->AppendSeparator();
	pOptionsMenu->Append(IDM_CONFIG_GFX_PLUGIN, _T("图形设置(&G)"));
	pOptionsMenu->Append(IDM_CONFIG_DSP_PLUGIN, _T("音频设置(&D)"));
	pOptionsMenu->Append(IDM_CONFIG_PAD_PLUGIN, _T("手柄设置(&P)"));
	pOptionsMenu->Append(IDM_CONFIG_WIIMOTE_PLUGIN, _T("&Wiimote 设置"));
	if (g_pCodeWindow)
	{
		pOptionsMenu->AppendSeparator();
		g_pCodeWindow->CreateMenuOptions(NULL, pOptionsMenu);	
	}
	m_MenuBar->Append(pOptionsMenu, _T("选项(&O)"));

	// Tools menu
	wxMenu* toolsMenu = new wxMenu;
	toolsMenu->Append(IDM_LUA, _T("新 &Lua 控制台"));
	toolsMenu->Append(IDM_MEMCARD, _T("内存卡管理器(GC)(&M)"));
	toolsMenu->Append(IDM_IMPORTSAVE, _T("Wii 存档导入 (不稳定)"));
	toolsMenu->Append(IDM_CHEATS, _T("动作回放管理器(&R)"));

	toolsMenu->Append(IDM_NETPLAY, _T("开始网络游戏(&N)"));

	if (DiscIO::CNANDContentManager::Access().GetNANDLoader(std::string (File::GetUserPath(D_WIIMENU_IDX))).IsValid())
	{
		toolsMenu->Append(IDM_LOAD_WII_MENU, _T("载入 Wii 菜单"));
	}
	else
	{
		toolsMenu->Append(IDM_INSTALL_WII_MENU, _T("Install Wii Menu"));
	}
	toolsMenu->AppendSeparator();
	toolsMenu->AppendCheckItem(IDM_CONNECT_WIIMOTE1, GetMenuLabel(HK_WIIMOTE1_CONNECT));
	toolsMenu->AppendCheckItem(IDM_CONNECT_WIIMOTE2, GetMenuLabel(HK_WIIMOTE2_CONNECT));
	toolsMenu->AppendCheckItem(IDM_CONNECT_WIIMOTE3, GetMenuLabel(HK_WIIMOTE3_CONNECT));
	toolsMenu->AppendCheckItem(IDM_CONNECT_WIIMOTE4, GetMenuLabel(HK_WIIMOTE4_CONNECT));

	m_MenuBar->Append(toolsMenu, _T("工具(&T)"));

	wxMenu* viewMenu = new wxMenu;
	viewMenu->AppendCheckItem(IDM_TOGGLE_TOOLBAR, _T("显示工具栏(&T)"));
	viewMenu->Check(IDM_TOGGLE_TOOLBAR, SConfig::GetInstance().m_InterfaceToolbar);
	viewMenu->AppendCheckItem(IDM_TOGGLE_STATUSBAR, _T("显示状态栏 (&S)"));
	viewMenu->Check(IDM_TOGGLE_STATUSBAR, SConfig::GetInstance().m_InterfaceStatusbar);
	viewMenu->AppendSeparator();
	viewMenu->AppendCheckItem(IDM_LOGWINDOW, _T("显示日志窗口(&L)"));
	viewMenu->Check(IDM_LOGWINDOW, SConfig::GetInstance().m_InterfaceLogWindow);
	viewMenu->AppendCheckItem(IDM_CONSOLEWINDOW, _T("显示控制台(&C)"));
	viewMenu->Check(IDM_CONSOLEWINDOW, SConfig::GetInstance().m_InterfaceConsole);
	viewMenu->AppendSeparator();

	if (g_pCodeWindow)
	{
		g_pCodeWindow->CreateMenuView(NULL, viewMenu);
		viewMenu->AppendSeparator();
	}

	wxMenu *platformMenu = new wxMenu;
	viewMenu->AppendSubMenu(platformMenu, _T("显示平台"));
	platformMenu->AppendCheckItem(IDM_LISTWII, _T("显示 Wii"));
	platformMenu->Check(IDM_LISTWII, SConfig::GetInstance().m_ListWii);
	platformMenu->AppendCheckItem(IDM_LISTGC, _T("显示 GameCube"));
	platformMenu->Check(IDM_LISTGC, SConfig::GetInstance().m_ListGC);
	platformMenu->AppendCheckItem(IDM_LISTWAD, _T("显示 Wad"));
	platformMenu->Check(IDM_LISTWAD, SConfig::GetInstance().m_ListWad);

	wxMenu *regionMenu = new wxMenu;
	viewMenu->AppendSubMenu(regionMenu, _T("显示区域"));
	regionMenu->AppendCheckItem(IDM_LISTJAP, _T("显示 JAP (日版)"));
	regionMenu->Check(IDM_LISTJAP, SConfig::GetInstance().m_ListJap);
	regionMenu->AppendCheckItem(IDM_LISTPAL, _T("显示 PAL (欧版)"));
	regionMenu->Check(IDM_LISTPAL, SConfig::GetInstance().m_ListPal);
	regionMenu->AppendCheckItem(IDM_LISTUSA, _T("显示 USA (美版)"));
	regionMenu->Check(IDM_LISTUSA, SConfig::GetInstance().m_ListUsa);
	regionMenu->AppendSeparator();
	regionMenu->AppendCheckItem(IDM_LISTFRANCE, _T("显示法国"));
	regionMenu->Check(IDM_LISTFRANCE, SConfig::GetInstance().m_ListFrance);
	regionMenu->AppendCheckItem(IDM_LISTITALY, _T("显示意大利"));
	regionMenu->Check(IDM_LISTITALY, SConfig::GetInstance().m_ListItaly);
	regionMenu->AppendCheckItem(IDM_LISTKOREA, _T("显示韩国"));
	regionMenu->Check(IDM_LISTKOREA, SConfig::GetInstance().m_ListKorea);
	regionMenu->AppendCheckItem(IDM_LISTTAIWAN, _T("显示台湾"));
	regionMenu->Check(IDM_LISTTAIWAN, SConfig::GetInstance().m_ListTaiwan);
	regionMenu->AppendCheckItem(IDM_LIST_UNK, _T("显示未知"));
	regionMenu->Check(IDM_LIST_UNK, SConfig::GetInstance().m_ListUnknown);
	viewMenu->AppendCheckItem(IDM_LISTDRIVES, _T("显示驱动器"));
	viewMenu->Check(IDM_LISTDRIVES, SConfig::GetInstance().m_ListDrives);
	viewMenu->Append(IDM_PURGECACHE, _T("清理缓存"));
	m_MenuBar->Append(viewMenu, _T("查看(&V)"));	

	if (g_pCodeWindow) g_pCodeWindow->CreateMenu(SConfig::GetInstance().m_LocalCoreStartupParameter, m_MenuBar);

	// Help menu
	wxMenu* helpMenu = new wxMenu;
	// Re-enable when there's something useful to display */
	// helpMenu->Append(wxID_HELP, _T("&Help"));
	helpMenu->Append(IDM_HELPWEBSITE, _T("Dolphin (Mod) 网站(&W)"));
	helpMenu->Append(IDM_HELPGOOGLECODE, _T("Dolphin (Mod) &Google 代码"));
	helpMenu->AppendSeparator();
	helpMenu->Append(wxID_ABOUT, _T("关于(&A)..."));
	m_MenuBar->Append(helpMenu, _T("帮助(&H)"));

	// Associate the menu bar with the frame
	SetMenuBar(m_MenuBar);
}

wxString CFrame::GetMenuLabel(int Id)
{
	wxString Label;
	switch (Id)
	{
		case HK_FULLSCREEN:
			Label = _T("全屏显示(&F)\t");
			break;
		case HK_PLAY_PAUSE:
			if (Core::GetState() == Core::CORE_RUN)
				Label = _T("暂停游戏(&P)\t");
			else
				Label = _T("开始游戏(&P)\t");
			break;
		case HK_STOP:
			Label = _T("停止游戏(&S)\t");
			break;
		case HK_WIIMOTE1_CONNECT:
			Label = _T("连接 Wiimote 1\t");
			break;
		case HK_WIIMOTE2_CONNECT:
			Label = _T("连接 Wiimote 2\t");
			break;
		case HK_WIIMOTE3_CONNECT:
			Label = _T("连接 Wiimote 3\t");
			break;
		case HK_WIIMOTE4_CONNECT:
			Label = _T("连接 Wiimote 4\t");
			break;
	}

	wxString Modifier = InputCommon::WXKeymodToString
		(SConfig::GetInstance().m_LocalCoreStartupParameter.iHotkeyModifier[Id]);
	wxString Hotkey = InputCommon::WXKeyToString
		(SConfig::GetInstance().m_LocalCoreStartupParameter.iHotkey[Id]);

	return Label + Modifier + (Modifier.Len() ? _T("+") : _T("")) + Hotkey;
}


// Create toolbar items
// ---------------------
void CFrame::PopulateToolbar(wxAuiToolBar* ToolBar)
{
	int w = m_Bitmaps[Toolbar_FileOpen].GetWidth(),
	    h = m_Bitmaps[Toolbar_FileOpen].GetHeight();
		ToolBar->SetToolBitmapSize(wxSize(w, h));
		

	ToolBar->AddTool(wxID_OPEN,    _T("打开"),    m_Bitmaps[Toolbar_FileOpen], _T("打开文件..."));
	ToolBar->AddTool(wxID_REFRESH, _T("刷新"), m_Bitmaps[Toolbar_Refresh], _T("刷新游戏列表"));
	ToolBar->AddTool(IDM_BROWSE, _T("浏览"),   m_Bitmaps[Toolbar_Browse], _T("从浏览ISO目录..."));
	ToolBar->AddSeparator();
	ToolBar->AddTool(IDM_PLAY, wxT("开始"),   m_Bitmaps[Toolbar_Play], _T("开始"));
	ToolBar->AddTool(IDM_STOP, _T("停止"),   m_Bitmaps[Toolbar_Stop], _T("停止"));
	ToolBar->AddTool(IDM_TOGGLE_FULLSCREEN, _T("全屏"),  m_Bitmaps[Toolbar_FullScreen], _T("切换全屏"));
	ToolBar->AddTool(IDM_SCREENSHOT, _T("截图"),   m_Bitmaps[Toolbar_FullScreen], _T("屏幕截图"));
	ToolBar->AddSeparator();
	ToolBar->AddTool(wxID_PREFERENCES, _T("设置"), m_Bitmaps[Toolbar_PluginOptions], _T("设置..."));
	ToolBar->AddTool(IDM_CONFIG_GFX_PLUGIN, _T("图形"),  m_Bitmaps[Toolbar_PluginGFX], _T("图形设置"));
	ToolBar->AddTool(IDM_CONFIG_DSP_PLUGIN, _T("音频"),  m_Bitmaps[Toolbar_PluginDSP], _T("音频设置"));
	ToolBar->AddTool(IDM_CONFIG_PAD_PLUGIN, _T("手柄"),  m_Bitmaps[Toolbar_PluginPAD], _T("手柄设置"));
	ToolBar->AddTool(IDM_CONFIG_WIIMOTE_PLUGIN, _T("Wiimote"),  m_Bitmaps[Toolbar_Wiimote], _T("Wiimote 设置"));

	// after adding the buttons to the toolbar, must call Realize() to reflect
	// the changes
	ToolBar->Realize();
}

void CFrame::PopulateToolbarAui(wxAuiToolBar* ToolBar)
{
	int w = m_Bitmaps[Toolbar_FileOpen].GetWidth(),
	    h = m_Bitmaps[Toolbar_FileOpen].GetHeight();
	ToolBar->SetToolBitmapSize(wxSize(w, h));

	ToolBar->AddTool(IDM_SAVE_PERSPECTIVE,	wxT("保存"),	g_pCodeWindow->m_Bitmaps[Toolbar_GotoPC], wxT("保存当前 perspective"));
	ToolBar->AddTool(IDM_EDIT_PERSPECTIVES,	wxT("编辑"),	g_pCodeWindow->m_Bitmaps[Toolbar_GotoPC], wxT("编辑当前 perspective"));

	ToolBar->SetToolDropDown(IDM_SAVE_PERSPECTIVE, true);
	ToolBar->SetToolDropDown(IDM_EDIT_PERSPECTIVES, true);	

	ToolBar->Realize();
}


// Delete and recreate the toolbar
void CFrame::RecreateToolbar()
{
	if (m_ToolBar)
	{
		m_Mgr->DetachPane(m_ToolBar);
		m_ToolBar->Destroy();
	}

	m_ToolBar = new wxAuiToolBar(this, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, TOOLBAR_STYLE);

	PopulateToolbar(m_ToolBar);
	
	m_Mgr->AddPane(m_ToolBar, wxAuiPaneInfo().
				Name(wxT("TBMain")).Caption(wxT("TBMain")).
				ToolbarPane().Top().
				LeftDockable(false).RightDockable(false).Floatable(false));

	if (g_pCodeWindow && !m_ToolBarDebug)
	{
		m_ToolBarDebug = new wxAuiToolBar(this, ID_TOOLBAR_DEBUG, wxDefaultPosition, wxDefaultSize, TOOLBAR_STYLE);
		g_pCodeWindow->PopulateToolbar(m_ToolBarDebug);
		
		m_Mgr->AddPane(m_ToolBarDebug, wxAuiPaneInfo().
				Name(wxT("TBDebug")).Caption(wxT("TBDebug")).
				ToolbarPane().Top().
				LeftDockable(false).RightDockable(false).Floatable(false));

		m_ToolBarAui = new wxAuiToolBar(this, ID_TOOLBAR_AUI, wxDefaultPosition, wxDefaultSize, TOOLBAR_STYLE);
		PopulateToolbarAui(m_ToolBarAui);
		m_Mgr->AddPane(m_ToolBarAui, wxAuiPaneInfo().
				Name(wxT("TBAui")).Caption(wxT("TBAui")).
				ToolbarPane().Top().
				LeftDockable(false).RightDockable(false).Floatable(false));
	}

	UpdateGUI();
}

void CFrame::InitBitmaps()
{
	// Get selected theme
	int Theme = SConfig::GetInstance().m_LocalCoreStartupParameter.iTheme;

	// Save memory by only having one set of bitmaps loaded at any time. I mean, they are still
	// in the exe, which is in memory, but at least we wont make another copy of all of them. 
	switch (Theme)
	{
	case BOOMY:
	{
		// These are stored as 48x48
		m_Bitmaps[Toolbar_FileOpen]		= wxGetBitmapFromMemory(toolbar_file_open_png);
		m_Bitmaps[Toolbar_Refresh]		= wxGetBitmapFromMemory(toolbar_refresh_png);
		m_Bitmaps[Toolbar_Browse]		= wxGetBitmapFromMemory(toolbar_browse_png);
		m_Bitmaps[Toolbar_Play]			= wxGetBitmapFromMemory(toolbar_play_png);
		m_Bitmaps[Toolbar_Stop]			= wxGetBitmapFromMemory(toolbar_stop_png);
		m_Bitmaps[Toolbar_Pause]		= wxGetBitmapFromMemory(toolbar_pause_png);
		m_Bitmaps[Toolbar_PluginOptions]= wxGetBitmapFromMemory(toolbar_plugin_options_png);
		m_Bitmaps[Toolbar_PluginGFX]	= wxGetBitmapFromMemory(toolbar_plugin_gfx_png);
		m_Bitmaps[Toolbar_PluginDSP]	= wxGetBitmapFromMemory(toolbar_plugin_dsp_png);
		m_Bitmaps[Toolbar_PluginPAD]	= wxGetBitmapFromMemory(toolbar_plugin_pad_png);
		m_Bitmaps[Toolbar_Wiimote]		= wxGetBitmapFromMemory(toolbar_plugin_wiimote_png);
		m_Bitmaps[Toolbar_Screenshot]	= wxGetBitmapFromMemory(toolbar_fullscreen_png);
		m_Bitmaps[Toolbar_FullScreen]	= wxGetBitmapFromMemory(toolbar_fullscreen_png);
		m_Bitmaps[Toolbar_Help]			= wxGetBitmapFromMemory(toolbar_help_png);

		// Scale the 48x48 bitmaps to 24x24
		for (size_t n = Toolbar_FileOpen; n <= Toolbar_Help; n++)
		{
			m_Bitmaps[n] = wxBitmap(m_Bitmaps[n].ConvertToImage().Scale(24, 24));
		}
	}
	break;

	case VISTA:
	{
		// These are stored as 24x24 and need no scaling
		m_Bitmaps[Toolbar_FileOpen]		= wxGetBitmapFromMemory(Toolbar_Open1_png);
		m_Bitmaps[Toolbar_Refresh]		= wxGetBitmapFromMemory(Toolbar_Refresh1_png);
		m_Bitmaps[Toolbar_Browse]		= wxGetBitmapFromMemory(Toolbar_Browse1_png);
		m_Bitmaps[Toolbar_Play]			= wxGetBitmapFromMemory(Toolbar_Play1_png);
		m_Bitmaps[Toolbar_Stop]			= wxGetBitmapFromMemory(Toolbar_Stop1_png);
		m_Bitmaps[Toolbar_Pause]		= wxGetBitmapFromMemory(Toolbar_Pause1_png);
		m_Bitmaps[Toolbar_PluginOptions]= wxGetBitmapFromMemory(Toolbar_Options1_png);
		m_Bitmaps[Toolbar_PluginGFX]	= wxGetBitmapFromMemory(Toolbar_Gfx1_png);
		m_Bitmaps[Toolbar_PluginDSP]	= wxGetBitmapFromMemory(Toolbar_DSP1_png);
		m_Bitmaps[Toolbar_PluginPAD]	= wxGetBitmapFromMemory(Toolbar_Pad1_png);
		m_Bitmaps[Toolbar_Wiimote]		= wxGetBitmapFromMemory(Toolbar_Wiimote1_png);
		m_Bitmaps[Toolbar_Screenshot]	= wxGetBitmapFromMemory(Toolbar_Fullscreen1_png);
		m_Bitmaps[Toolbar_FullScreen]	= wxGetBitmapFromMemory(Toolbar_Fullscreen1_png);
		m_Bitmaps[Toolbar_Help]			= wxGetBitmapFromMemory(Toolbar_Help1_png);
	}
	break;

	case XPLASTIK:
	{
		m_Bitmaps[Toolbar_FileOpen]		= wxGetBitmapFromMemory(Toolbar_Open2_png);
		m_Bitmaps[Toolbar_Refresh]		= wxGetBitmapFromMemory(Toolbar_Refresh2_png);
		m_Bitmaps[Toolbar_Browse]		= wxGetBitmapFromMemory(Toolbar_Browse2_png);
		m_Bitmaps[Toolbar_Play]			= wxGetBitmapFromMemory(Toolbar_Play2_png);
		m_Bitmaps[Toolbar_Stop]			= wxGetBitmapFromMemory(Toolbar_Stop2_png);
		m_Bitmaps[Toolbar_Pause]		= wxGetBitmapFromMemory(Toolbar_Pause2_png);
		m_Bitmaps[Toolbar_PluginOptions]= wxGetBitmapFromMemory(Toolbar_Options2_png);
		m_Bitmaps[Toolbar_PluginGFX]	= wxGetBitmapFromMemory(Toolbar_Gfx2_png);
		m_Bitmaps[Toolbar_PluginDSP]	= wxGetBitmapFromMemory(Toolbar_DSP2_png);
		m_Bitmaps[Toolbar_PluginPAD]	= wxGetBitmapFromMemory(Toolbar_Pad2_png);
		m_Bitmaps[Toolbar_Wiimote]		= wxGetBitmapFromMemory(Toolbar_Wiimote2_png);
		m_Bitmaps[Toolbar_Screenshot]	= wxGetBitmapFromMemory(Toolbar_Fullscreen2_png);
		m_Bitmaps[Toolbar_FullScreen]	= wxGetBitmapFromMemory(Toolbar_Fullscreen2_png);
		m_Bitmaps[Toolbar_Help]			= wxGetBitmapFromMemory(Toolbar_Help2_png);
	}
	break;

	case KDE:
	{
		m_Bitmaps[Toolbar_FileOpen]		= wxGetBitmapFromMemory(Toolbar_Open3_png);
		m_Bitmaps[Toolbar_Refresh]		= wxGetBitmapFromMemory(Toolbar_Refresh3_png);
		m_Bitmaps[Toolbar_Browse]		= wxGetBitmapFromMemory(Toolbar_Browse3_png);
		m_Bitmaps[Toolbar_Play]			= wxGetBitmapFromMemory(Toolbar_Play3_png);
		m_Bitmaps[Toolbar_Stop]			= wxGetBitmapFromMemory(Toolbar_Stop3_png);
		m_Bitmaps[Toolbar_Pause]		= wxGetBitmapFromMemory(Toolbar_Pause3_png);
		m_Bitmaps[Toolbar_PluginOptions]= wxGetBitmapFromMemory(Toolbar_Options3_png);
		m_Bitmaps[Toolbar_PluginGFX]	= wxGetBitmapFromMemory(Toolbar_Gfx3_png);
		m_Bitmaps[Toolbar_PluginDSP]	= wxGetBitmapFromMemory(Toolbar_DSP3_png);
		m_Bitmaps[Toolbar_PluginPAD]	= wxGetBitmapFromMemory(Toolbar_Pad3_png);
		m_Bitmaps[Toolbar_Wiimote]		= wxGetBitmapFromMemory(Toolbar_Wiimote3_png);
		m_Bitmaps[Toolbar_Screenshot]	= wxGetBitmapFromMemory(Toolbar_Fullscreen3_png);
		m_Bitmaps[Toolbar_FullScreen]	= wxGetBitmapFromMemory(Toolbar_Fullscreen3_png);
		m_Bitmaps[Toolbar_Help]			= wxGetBitmapFromMemory(Toolbar_Help3_png);
	}
	break;

	default: PanicAlert("Theme selection went wrong");
	}

	// Update in case the bitmap has been updated
	if (m_ToolBar != NULL) RecreateToolbar();

	aNormalFile = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
}

// Menu items

// Start the game or change the disc.
// Boot priority:
// 1. Show the game list and boot the selected game.
// 2. Default ISO
// 3. Boot last selected game
void CFrame::BootGame(const std::string& filename)
{
	std::string bootfile = filename;
	SCoreStartupParameter& StartUp = SConfig::GetInstance().m_LocalCoreStartupParameter;

	if (Core::GetState() != Core::CORE_UNINITIALIZED)
		return;

	// Start filename if non empty.
	// Start the selected ISO, or try one of the saved paths.
	// If all that fails, ask to add a dir and don't boot
	if (bootfile.empty())
	{
		if (m_GameListCtrl->GetSelectedISO() != NULL)
		{
			if (m_GameListCtrl->GetSelectedISO()->IsValid())
				bootfile = m_GameListCtrl->GetSelectedISO()->GetFileName();
		}
		else if (!StartUp.m_strDefaultGCM.empty()
				&&	wxFileExists(wxString(StartUp.m_strDefaultGCM.c_str(), wxConvUTF8)))
			bootfile = StartUp.m_strDefaultGCM;
		else
		{
			if (!SConfig::GetInstance().m_LastFilename.empty()
					&& wxFileExists(wxString(SConfig::GetInstance().m_LastFilename.c_str(), wxConvUTF8)))
				bootfile = SConfig::GetInstance().m_LastFilename;
			else
			{
				m_GameListCtrl->BrowseForDirectory();
				return;
			}
		}
	}
	if (!bootfile.empty())
		StartGame(bootfile);
}

// Open file to boot
void CFrame::OnOpen(wxCommandEvent& WXUNUSED (event))
{
	DoOpen(true);
}

void CFrame::DoOpen(bool Boot)
{
    std::string currentDir = File::GetCurrentDir();

	wxString path = wxFileSelector(
			_T("选择须载入文件"),
			wxEmptyString, wxEmptyString, wxEmptyString,
			wxString::Format
			(
					_T("所有 GC/Wii 文件 (elf, dol, gcm, iso, wad)|*.elf;*.dol;*.gcm;*.iso;*.gcz;*.wad|所有文件 (%s)|%s"),
					wxFileSelectorDefaultWildcardStr,
					wxFileSelectorDefaultWildcardStr
			),
			wxFD_OPEN | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST,
			this);

	bool fileChosen = !path.IsEmpty();

    std::string currentDir2 = File::GetCurrentDir();

    if (currentDir != currentDir2)
    {
        PanicAlert("Current dir changed from %s to %s after wxFileSelector!",currentDir.c_str(),currentDir2.c_str());
        File::SetCurrentDir(currentDir.c_str());
    }


	// Should we boot a new game or just change the disc?
	if (Boot)
	{
		if (!fileChosen)
			return;
		BootGame(std::string(path.mb_str()));
	}
	else
	{
		strncpy(newDiscpath, path.mb_str(), strlen(path.mb_str())+1);
		DVDInterface::ChangeDisc(newDiscpath);
	}
}

void CFrame::OnFrameStep(wxCommandEvent& event)
{
	Frame::SetFrameStepping(event.IsChecked());
}

void CFrame::OnChangeDisc(wxCommandEvent& WXUNUSED (event))
{
	DoOpen(false);
}

void CFrame::OnRecord(wxCommandEvent& WXUNUSED (event))
{
	wxString path = wxFileSelector(
			_T("选择录制文件"),
			wxEmptyString, wxEmptyString, wxEmptyString,
			wxString::Format
			(
					_T("Dolphin TAS 电影 (*.dtm)|*.dtm|所有文件 (%s)|%s"),
					wxFileSelectorDefaultWildcardStr,
					wxFileSelectorDefaultWildcardStr
			),
			wxFD_SAVE | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST,
			this);

	if(path.IsEmpty())
		return;

	// TODO: Take controller settings from Gamecube Configuration menu
	if(Frame::BeginRecordingInput(path.mb_str(), 1))
		BootGame(std::string(""));
}

void CFrame::OnPlayRecording(wxCommandEvent& WXUNUSED (event))
{
	wxString path = wxFileSelector(
			_T("选择录制文件"),
			wxEmptyString, wxEmptyString, wxEmptyString,
			wxString::Format
			(
					_T("Dolphin TAS 电影 (*.dtm)|*.dtm|所有文件 (%s)|%s"),
					wxFileSelectorDefaultWildcardStr,
					wxFileSelectorDefaultWildcardStr
			),
			wxFD_OPEN | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST,
			this);

	if(path.IsEmpty())
		return;

	if(Frame::PlayInput(path.mb_str()))
		BootGame(std::string(""));
}

void CFrame::OnPlay(wxCommandEvent& WXUNUSED (event))
{
	if (Core::GetState() != Core::CORE_UNINITIALIZED)
	{
		// Core is initialized and emulator is running
		if (UseDebugger)
		{
			if (CCPU::IsStepping())
				CCPU::EnableStepping(false);
			else
				CCPU::EnableStepping(true);  // Break

			wxThread::Sleep(20);
			g_pCodeWindow->JumpToAddress(PC);
			g_pCodeWindow->Update();
			// Update toolbar with Play/Pause status
			UpdateGUI();
		}
		else
			DoPause();
	}
	else
		// Core is uninitialized, start the game
		BootGame(std::string(""));
}

void CFrame::OnRenderParentClose(wxCloseEvent& event)
{
	DoStop();
	event.Skip();
}

void CFrame::OnRenderParentMove(wxMoveEvent& event)
{
	if (Core::GetState() != Core::CORE_UNINITIALIZED &&
			!RendererIsFullscreen() && !m_RenderFrame->IsMaximized())
	{
		SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowXPos = m_RenderFrame->GetPosition().x;
		SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowYPos = m_RenderFrame->GetPosition().y;
	}
	event.Skip();
}

void CFrame::OnRenderParentResize(wxSizeEvent& event)
{
	if (Core::GetState() != Core::CORE_UNINITIALIZED)
	{
		int width, height;
		if (!SConfig::GetInstance().m_LocalCoreStartupParameter.bRenderToMain &&
				!RendererIsFullscreen() && !m_RenderFrame->IsMaximized())
		{
			m_RenderFrame->GetSize(&width, &height);
			SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowWidth = width;
			SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowHeight = height;
		}
#if defined(HAVE_X11) && HAVE_X11
		int x, y;
		m_RenderParent->GetSize(&width, &height);
		m_RenderParent->GetPosition(&x, &y);
		X11Utils::SendClientEvent("RESIZE", x, y, width, height);
#endif
	}
	event.Skip();
}

void CFrame::ToggleDisplayMode (bool bFullscreen)
{
#ifdef _WIN32
	if (bFullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		sscanf(SConfig::GetInstance().m_LocalCoreStartupParameter.strFullscreenResolution.c_str(),
			   	"%dx%d", &dmScreenSettings.dmPelsWidth, &dmScreenSettings.dmPelsHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		// Change to default resolution
		ChangeDisplaySettings(NULL, CDS_FULLSCREEN);
	}
#elif defined(HAVE_XRANDR) && HAVE_XRANDR
	m_XRRConfig->ToggleDisplayMode(bFullscreen);
#endif
}

// Prepare the GUI to start the game.
void CFrame::StartGame(const std::string& filename)
{
	m_bGameLoading = true;

	if (m_ToolBar)
		m_ToolBar->EnableTool(IDM_PLAY, false);
	GetMenuBar()->FindItem(IDM_PLAY)->Enable(false);

	// Game has been started, hide the game list
	if (m_GameListCtrl->IsShown())
	{
		m_GameListCtrl->Disable();
		m_GameListCtrl->Hide();
	}

	if (SConfig::GetInstance().m_LocalCoreStartupParameter.bRenderToMain)
	{
		m_RenderParent = m_Panel;
		m_RenderFrame = this;
	}
	else
	{
		wxPoint position(SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowXPos,
				SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowYPos);
		wxSize size(SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowWidth,
				SConfig::GetInstance().m_LocalCoreStartupParameter.iRenderWindowHeight);
		m_RenderFrame = new CRenderFrame((wxFrame*)this, wxID_ANY, _("Dolphin"), position, size);
		m_RenderFrame->Connect(wxID_ANY, wxEVT_CLOSE_WINDOW,
				wxCloseEventHandler(CFrame::OnRenderParentClose),
				(wxObject*)0, this);
		m_RenderFrame->Connect(wxID_ANY, wxEVT_ACTIVATE,
				wxActivateEventHandler(CFrame::OnActive),
				(wxObject*)0, this);
		m_RenderFrame->Connect(wxID_ANY, wxEVT_MOVE,
				wxMoveEventHandler(CFrame::OnRenderParentMove),
				(wxObject*)0, this);
		m_RenderParent = new CPanel(m_RenderFrame, wxID_ANY);
		m_RenderFrame->Show();
	}

	if (!BootManager::BootCore(filename))
	{
		// Destroy the renderer frame when not rendering to main
		if (!SConfig::GetInstance().m_LocalCoreStartupParameter.bRenderToMain)
			m_RenderFrame->Destroy();
		m_RenderParent = NULL;
		m_bGameLoading = false;
		UpdateGUI();
	}
	else
	{
		DoFullscreen(SConfig::GetInstance().m_LocalCoreStartupParameter.bFullscreen);

#ifdef _WIN32
		::SetFocus((HWND)m_RenderParent->GetHandle());
#else
		m_RenderParent->SetFocus();
#endif

		wxTheApp->Connect(wxID_ANY, wxEVT_KEY_DOWN, // Keyboard
				wxKeyEventHandler(CFrame::OnKeyDown),
				(wxObject*)0, this);
		wxTheApp->Connect(wxID_ANY, wxEVT_KEY_UP,
				wxKeyEventHandler(CFrame::OnKeyUp),
				(wxObject*)0, this);
		m_RenderFrame->Connect(wxID_ANY, wxEVT_SIZE,
				wxSizeEventHandler(CFrame::OnRenderParentResize),
				(wxObject*)0, this);
	}
}

void CFrame::OnBootDrive(wxCommandEvent& event)
{
	BootGame(drives[event.GetId()-IDM_DRIVE1]);
}


// Refresh the file list and browse for a favorites directory
void CFrame::OnRefresh(wxCommandEvent& WXUNUSED (event))
{
	if (m_GameListCtrl) m_GameListCtrl->Update();
}


void CFrame::OnBrowse(wxCommandEvent& WXUNUSED (event))
{
	if (m_GameListCtrl) m_GameListCtrl->BrowseForDirectory();
}

// Create screenshot
void CFrame::OnScreenshot(wxCommandEvent& WXUNUSED (event))
{
	Core::ScreenShot();
}

// Pause the emulation
void CFrame::DoPause()
{
	if (Core::GetState() == Core::CORE_RUN)
	{
		Core::SetState(Core::CORE_PAUSE);
		if (SConfig::GetInstance().m_LocalCoreStartupParameter.bHideCursor)
			m_RenderParent->SetCursor(wxCURSOR_ARROW);
	}
	else
	{
		Core::SetState(Core::CORE_RUN);
		if (SConfig::GetInstance().m_LocalCoreStartupParameter.bHideCursor &&
				RendererHasFocus())
			m_RenderParent->SetCursor(wxCURSOR_BLANK);
	}
	UpdateGUI();
}

// Stop the emulation
void CFrame::DoStop()
{
	if (Core::GetState() != Core::CORE_UNINITIALIZED)
	{
		// Ask for confirmation in case the user accidentally clicked Stop / Escape
		if (SConfig::GetInstance().m_LocalCoreStartupParameter.bConfirmStop)
		{
			// Suppress duplicate dialog boxes
			if (m_StopDlg)
				return;

			m_StopDlg = new wxMessageDialog(
				this,
				wxT("是否停止当前的模拟?"),
				wxT("请确认..."),
				wxYES_NO | wxSTAY_ON_TOP | wxICON_EXCLAMATION,
				wxDefaultPosition);

			int Ret = m_StopDlg->ShowModal();
			m_StopDlg->Destroy();
			m_StopDlg = NULL;
			if (Ret == wxID_NO)
				return;
		}

		// TODO: Show the author/description dialog here
		if(Frame::IsRecordingInput())
			Frame::EndRecordingInput();
		if(Frame::IsPlayingInput())
			Frame::EndPlayInput();
	
		Core::Stop();
		UpdateGUI();

		// Destroy the renderer frame when not rendering to main
		m_RenderFrame->Disconnect(wxID_ANY, wxEVT_SIZE,
				wxSizeEventHandler(CFrame::OnRenderParentResize),
				(wxObject*)0, this);
		wxTheApp->Disconnect(wxID_ANY, wxEVT_KEY_DOWN, // Keyboard
				wxKeyEventHandler(CFrame::OnKeyDown),
				(wxObject*)0, this);
		wxTheApp->Disconnect(wxID_ANY, wxEVT_KEY_UP,
				wxKeyEventHandler(CFrame::OnKeyUp),
				(wxObject*)0, this);
		if (SConfig::GetInstance().m_LocalCoreStartupParameter.bHideCursor)
			m_RenderParent->SetCursor(wxCURSOR_ARROW);
		DoFullscreen(FALSE);
		if (!SConfig::GetInstance().m_LocalCoreStartupParameter.bRenderToMain)
			m_RenderFrame->Destroy();
		m_RenderParent = NULL;

		// Clean framerate indications from the status bar.
		m_pStatusBar->SetStatusText(wxT(" "), 0);
	}
}

void CFrame::OnStop(wxCommandEvent& WXUNUSED (event))
{
	m_bGameLoading = false;
	DoStop();
}

void CFrame::OnReset(wxCommandEvent& WXUNUSED (event))
{
	ProcessorInterface::ResetButton_Tap();
}

void CFrame::OnConfigMain(wxCommandEvent& WXUNUSED (event))
{
	CConfigMain ConfigMain(this);
	if (ConfigMain.ShowModal() == wxID_OK)
		m_GameListCtrl->Update();
}

void CFrame::OnPluginGFX(wxCommandEvent& WXUNUSED (event))
{
	CPluginManager::GetInstance().OpenConfig(
			GetHandle(),
			SConfig::GetInstance().m_LocalCoreStartupParameter.m_strVideoPlugin.c_str(),
			PLUGIN_TYPE_VIDEO
			);
}

void CFrame::OnPluginDSP(wxCommandEvent& WXUNUSED (event))
{
	CPluginManager::GetInstance().OpenConfig(
			GetHandle(),
			SConfig::GetInstance().m_LocalCoreStartupParameter.m_strDSPPlugin.c_str(),
			PLUGIN_TYPE_DSP
			);
}

void CFrame::OnPluginPAD(wxCommandEvent& WXUNUSED (event))
{
	CPluginManager::GetInstance().OpenConfig(
			GetHandle(),
			SConfig::GetInstance().m_LocalCoreStartupParameter.m_strPadPlugin[0].c_str(),
			PLUGIN_TYPE_PAD
			);
}
void CFrame::OnPluginWiimote(wxCommandEvent& WXUNUSED (event))
{
	CPluginManager::GetInstance().OpenConfig(
			GetHandle(),
			SConfig::GetInstance().m_LocalCoreStartupParameter.m_strWiimotePlugin[0].c_str(),
			PLUGIN_TYPE_WIIMOTE
			);
}

void CFrame::OnHelp(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case wxID_ABOUT:
		{
			AboutDolphin frame(this);
			frame.ShowModal();
		}
		break;
	case IDM_HELPWEBSITE:
		WxUtils::Launch("http://www.dolphin-emu.com/");
		break;
	case IDM_HELPGOOGLECODE:
		WxUtils::Launch("http://code.google.com/p/dolphin-emu/");
		break;
	}
}

void CFrame::ClearStatusBar()
{
	if (this->GetStatusBar()->IsEnabled()) this->GetStatusBar()->SetStatusText(wxT(""),0);
}

void CFrame::StatusBarMessage(const char * Text, ...)
{
	const int MAX_BYTES = 1024*10;
	char Str[MAX_BYTES];
	va_list ArgPtr;
	int Cnt;
	va_start(ArgPtr, Text);
	Cnt = vsnprintf(Str, MAX_BYTES, Text, ArgPtr);
	va_end(ArgPtr);

	if (this->GetStatusBar()->IsEnabled()) this->GetStatusBar()->SetStatusText(wxString::FromAscii(Str),0);
}


// Miscellaneous menus
// ---------------------
// NetPlay stuff
void CFrame::OnNetPlay(wxCommandEvent& WXUNUSED (event))
{
	new NetPlaySetupDiag(this, m_GameListCtrl);
}

void CFrame::OnMemcard(wxCommandEvent& WXUNUSED (event))
{
	CMemcardManager MemcardManager(this);
	MemcardManager.ShowModal();
}

void CFrame::OnImportSave(wxCommandEvent& WXUNUSED (event)) 
{
	wxString path = wxFileSelector(_T("选择存档文件"),
			wxEmptyString, wxEmptyString, wxEmptyString,
			wxString::Format
			(
					_T("Wii 存档文件|data.bin|所有文件 (%s)|%s"),
					wxFileSelectorDefaultWildcardStr,
					wxFileSelectorDefaultWildcardStr
			),
			wxFD_OPEN | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST,
			this);

	if (!path.IsEmpty())
	{
		CWiiSaveCrypted* saveFile = new CWiiSaveCrypted(path.ToUTF8().data());
		delete saveFile;
	}
}

void CFrame::OnOpenLuaWindow(wxCommandEvent& WXUNUSED (event))
{
	new wxLuaWindow(this, wxDefaultPosition, wxSize(600, 390));
}

void CFrame::OnShow_CheatsWindow(wxCommandEvent& WXUNUSED (event))
{
	CheatsWindow = new wxCheatsWindow(this, wxDefaultPosition, wxSize(600, 390));
}

void CFrame::OnLoadWiiMenu(wxCommandEvent& event)
{
	if (event.GetId() == IDM_LOAD_WII_MENU)
	{
		BootGame(std::string (File::GetUserPath(D_WIIMENU_IDX)));
	}
	else
	{
		
		wxString path = wxFileSelector(
			_T("Select the System Menu wad extracted from the update partition of a disc"),
			wxEmptyString, wxEmptyString, wxEmptyString,
			wxString::Format
			(
					_T("System Menu wad|*.wad"),
					wxFileSelectorDefaultWildcardStr,
					wxFileSelectorDefaultWildcardStr
			),
			wxFD_OPEN | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST,
			this);

		if (CBoot::Install_WiiWAD(path.mb_str()))
		{
			GetMenuBar()->FindItem(IDM_INSTALL_WII_MENU)->Enable(false);
		}
	}
}

void CFrame::OnConnectWiimote(wxCommandEvent& event)
{
	if (Core::isRunning() && SConfig::GetInstance().m_LocalCoreStartupParameter.bWii)
	{
		int Id = event.GetId() - IDM_CONNECT_WIIMOTE1;
		bNoWiimoteMsg = !event.IsChecked();
		GetUsbPointer()->AccessWiiMote(Id | 0x100)->Activate(event.IsChecked());
		wxString msg(wxString::Format(wxT("Wiimote %i %s"), Id + 1, (event.IsChecked()) ? wxT("已连接") : wxT("已断开")));
		Core::DisplayMessage(msg.ToAscii(), 3000);
	}
}

// Toogle fullscreen. In Windows the fullscreen mode is accomplished by expanding the m_Panel to cover
// the entire screen (when we render to the main window).
void CFrame::OnToggleFullscreen(wxCommandEvent& WXUNUSED (event))
{
	DoFullscreen(!RendererIsFullscreen());
}

void CFrame::OnToggleDualCore(wxCommandEvent& WXUNUSED (event))
{
	SConfig::GetInstance().m_LocalCoreStartupParameter.bCPUThread = !SConfig::GetInstance().m_LocalCoreStartupParameter.bCPUThread;
	SConfig::GetInstance().SaveSettings();
}

void CFrame::OnToggleSkipIdle(wxCommandEvent& WXUNUSED (event))
{
	SConfig::GetInstance().m_LocalCoreStartupParameter.bSkipIdle = !SConfig::GetInstance().m_LocalCoreStartupParameter.bSkipIdle;
	SConfig::GetInstance().SaveSettings();
}

void CFrame::OnLoadStateFromFile(wxCommandEvent& WXUNUSED (event))
{
	wxString path = wxFileSelector(
		_T("选择需要载入的状态"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxString::Format
		(
		_T("所有存档状态 (sav, s##)|*.sav;*.s??|所有文件 (%s)|%s"),
		wxFileSelectorDefaultWildcardStr,
		wxFileSelectorDefaultWildcardStr
		),
		wxFD_OPEN | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST,
		this);

	if(!path.IsEmpty())
		State_LoadAs((const char*)path.mb_str());
}

void CFrame::OnSaveStateToFile(wxCommandEvent& WXUNUSED (event))
{
	wxString path = wxFileSelector(
		_T("Select the state to save"),
		wxEmptyString, wxEmptyString, wxEmptyString,
		wxString::Format
		(
		_T("所有存档状态 (sav, s##)|*.sav;*.s??|所有文件 (%s)|%s"),
		wxFileSelectorDefaultWildcardStr,
		wxFileSelectorDefaultWildcardStr
		),
		wxFD_SAVE,
		this);

	if(! path.IsEmpty())
		State_SaveAs((const char*)path.mb_str());
}

void CFrame::OnLoadLastState(wxCommandEvent& WXUNUSED (event))
{
	State_LoadLastSaved();
}

void CFrame::OnUndoLoadState(wxCommandEvent& WXUNUSED (event))
{
	State_UndoLoadState();
}

void CFrame::OnUndoSaveState(wxCommandEvent& WXUNUSED (event))
{
	State_UndoSaveState();
}


void CFrame::OnLoadState(wxCommandEvent& event)
{
	int id = event.GetId();
	int slot = id - IDM_LOADSLOT1 + 1;
	State_Load(slot);
}

void CFrame::OnSaveState(wxCommandEvent& event)
{
	int id = event.GetId();
	int slot = id - IDM_SAVESLOT1 + 1;
	State_Save(slot);
}

void CFrame::OnFrameSkip(wxCommandEvent& event)
{
	int amount = event.GetId() - IDM_FRAMESKIP0;

	Frame::SetFrameSkipping((unsigned int)amount);
}




// GUI
// ---------------------

// Update the enabled/disabled status
void CFrame::UpdateGUI()
{
	if (!m_bControlsCreated)
		return;

	// Save status
	bool Initialized = Core::isRunning();
	bool Running = Core::GetState() == Core::CORE_RUN;
	bool Paused = Core::GetState() == Core::CORE_PAUSE;

	// Make sure that we have a toolbar
	if (m_ToolBar)
	{
		// Enable/disable the Config and Stop buttons
		m_ToolBar->EnableTool(wxID_OPEN, !Initialized);
		m_ToolBar->EnableTool(wxID_REFRESH, !Initialized); // Don't allow refresh when we don't show the list
		m_ToolBar->EnableTool(IDM_STOP, Running || Paused);
		m_ToolBar->EnableTool(IDM_TOGGLE_FULLSCREEN, Running || Paused);
		m_ToolBar->EnableTool(IDM_SCREENSHOT, Running || Paused);
	}

	// File
	GetMenuBar()->FindItem(wxID_OPEN)->Enable(!Initialized);
	m_pSubMenuDrive->Enable(!Initialized);
	GetMenuBar()->FindItem(wxID_REFRESH)->Enable(!Initialized);
	GetMenuBar()->FindItem(IDM_BROWSE)->Enable(!Initialized);

	// Emulation
	GetMenuBar()->FindItem(IDM_STOP)->Enable(Running || Paused);
	GetMenuBar()->FindItem(IDM_RESET)->Enable(Running || Paused);
	GetMenuBar()->FindItem(IDM_RECORD)->Enable(!Initialized);
	GetMenuBar()->FindItem(IDM_PLAYRECORD)->Enable(!Initialized);
	GetMenuBar()->FindItem(IDM_FRAMESTEP)->Enable(Running || Paused);
	GetMenuBar()->FindItem(IDM_SCREENSHOT)->Enable(Running || Paused);
	GetMenuBar()->FindItem(IDM_TOGGLE_FULLSCREEN)->Enable(Running || Paused);

	// Update Menu Accelerators
	GetMenuBar()->FindItem(IDM_TOGGLE_FULLSCREEN)->SetItemLabel(GetMenuLabel(HK_FULLSCREEN));
	GetMenuBar()->FindItem(IDM_PLAY)->SetItemLabel(GetMenuLabel(HK_PLAY_PAUSE));
	GetMenuBar()->FindItem(IDM_STOP)->SetItemLabel(GetMenuLabel(HK_STOP));
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE1)->SetItemLabel(GetMenuLabel(HK_WIIMOTE1_CONNECT));
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE2)->SetItemLabel(GetMenuLabel(HK_WIIMOTE2_CONNECT));
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE3)->SetItemLabel(GetMenuLabel(HK_WIIMOTE3_CONNECT));
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE4)->SetItemLabel(GetMenuLabel(HK_WIIMOTE4_CONNECT));

	m_pSubMenuLoad->Enable(Initialized);
	m_pSubMenuSave->Enable(Initialized);

	// Misc
	GetMenuBar()->FindItem(IDM_CHANGEDISC)->Enable(Initialized);
	if (DiscIO::CNANDContentManager::Access().GetNANDLoader(std::string(File::GetUserPath(D_WIIMENU_IDX))).IsValid())
		GetMenuBar()->FindItem(IDM_LOAD_WII_MENU)->Enable(!Initialized);

	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE1)->Enable(Initialized  && SConfig::GetInstance().m_LocalCoreStartupParameter.bWii);
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE2)->Enable(Initialized  && SConfig::GetInstance().m_LocalCoreStartupParameter.bWii);
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE3)->Enable(Initialized  && SConfig::GetInstance().m_LocalCoreStartupParameter.bWii);
	GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE4)->Enable(Initialized  && SConfig::GetInstance().m_LocalCoreStartupParameter.bWii);
	if (Initialized && SConfig::GetInstance().m_LocalCoreStartupParameter.bWii)
	{
		GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE1)->Check(GetUsbPointer()->AccessWiiMote(0x0100)->IsConnected() == 3);
		GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE2)->Check(GetUsbPointer()->AccessWiiMote(0x0101)->IsConnected() == 3);
		GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE3)->Check(GetUsbPointer()->AccessWiiMote(0x0102)->IsConnected() == 3);
		GetMenuBar()->FindItem(IDM_CONNECT_WIIMOTE4)->Check(GetUsbPointer()->AccessWiiMote(0x0103)->IsConnected() == 3);
	}

	if (Running)
	{
		if (m_ToolBar)
		{
			m_ToolBar->SetToolBitmap(IDM_PLAY, m_Bitmaps[Toolbar_Pause]);
			m_ToolBar->SetToolShortHelp(IDM_PLAY, _("暂停"));
			m_ToolBar->SetToolLabel(IDM_PLAY, _("暂停"));
		}
	}
	else
	{
		if (m_ToolBar)
		{
			m_ToolBar->SetToolBitmap(IDM_PLAY, m_Bitmaps[Toolbar_Play]);
			m_ToolBar->SetToolShortHelp(IDM_PLAY, _("开始"));
			m_ToolBar->SetToolLabel(IDM_PLAY, wxT(" 开始"));
		}
	}
	
	if (!Initialized)
	{
		if (m_GameListCtrl->IsEnabled())
		{
			// Prepare to load Default ISO, enable play button
			if (!SConfig::GetInstance().m_LocalCoreStartupParameter.m_strDefaultGCM.empty())
			{
				if (m_ToolBar)
					m_ToolBar->EnableTool(IDM_PLAY, true);					
				GetMenuBar()->FindItem(IDM_PLAY)->Enable(true);
			}
			// Prepare to load last selected file, enable play button
			else if (!SConfig::GetInstance().m_LastFilename.empty()
			&& wxFileExists(wxString(SConfig::GetInstance().m_LastFilename.c_str(), wxConvUTF8)))
			{
				if (m_ToolBar)
					m_ToolBar->EnableTool(IDM_PLAY, true);					
				GetMenuBar()->FindItem(IDM_PLAY)->Enable(true);
			}
			else
			{
				// No game has been selected yet, disable play button
				if (m_ToolBar)
					m_ToolBar->EnableTool(IDM_PLAY, false);
				GetMenuBar()->FindItem(IDM_PLAY)->Enable(false);
			}
		}

		if (m_GameListCtrl && !m_bGameLoading)
		{
			// Game has not started, show game list
			if (!m_GameListCtrl->IsShown())
			{
				m_GameListCtrl->Reparent(m_Panel);
				m_GameListCtrl->Enable();
				m_GameListCtrl->Show();
				sizerPanel->FitInside(m_Panel);
			}
			// Game has been selected but not started, enable play button
			if (m_GameListCtrl->GetSelectedISO() != NULL && m_GameListCtrl->IsEnabled() && !m_bGameLoading)
			{
				if (m_ToolBar)
					m_ToolBar->EnableTool(IDM_PLAY, true);
				GetMenuBar()->FindItem(IDM_PLAY)->Enable(true);				
			}
		}
	}
	else
	{
		// Game has been loaded, enable the play button
		if (m_ToolBar)
			m_ToolBar->EnableTool(IDM_PLAY, true);
		GetMenuBar()->FindItem(IDM_PLAY)->Enable(true);

		// Reset game loading flag
		m_bGameLoading = false;
	}

	if (m_ToolBar) m_ToolBar->Refresh();
	if (g_pCodeWindow) g_pCodeWindow->Update();

	// Commit changes to manager
	m_Mgr->Update();
}

void CFrame::UpdateGameList()
{
	m_GameListCtrl->Update();
}

void CFrame::GameListChanged(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case IDM_LISTWII:
		SConfig::GetInstance().m_ListWii = event.IsChecked();
		break;
	case IDM_LISTGC:
		SConfig::GetInstance().m_ListGC = event.IsChecked();
		break;
	case IDM_LISTWAD:
		SConfig::GetInstance().m_ListWad = event.IsChecked();
		break;
	case IDM_LISTJAP:
		SConfig::GetInstance().m_ListJap = event.IsChecked();
		break;
	case IDM_LISTPAL:
		SConfig::GetInstance().m_ListPal = event.IsChecked();
		break;
	case IDM_LISTUSA:
		SConfig::GetInstance().m_ListUsa = event.IsChecked();
		break;
	case IDM_LISTFRANCE:
		SConfig::GetInstance().m_ListFrance = event.IsChecked();
		break;
	case IDM_LISTITALY:
		SConfig::GetInstance().m_ListItaly = event.IsChecked();
		break;
	case IDM_LISTKOREA:
		SConfig::GetInstance().m_ListKorea = event.IsChecked();
		break;
	case IDM_LISTTAIWAN:
		SConfig::GetInstance().m_ListTaiwan = event.IsChecked();
		break;
	case IDM_LIST_UNK:
		SConfig::GetInstance().m_ListUnknown = event.IsChecked();
		break;
	case IDM_LISTDRIVES:
		SConfig::GetInstance().m_ListDrives = event.IsChecked();
		break;
	case IDM_PURGECACHE:
		CFileSearch::XStringVector Directories;
		Directories.push_back(File::GetUserPath(D_CACHE_IDX));
		CFileSearch::XStringVector Extensions;
		Extensions.push_back("*.cache");
		
		CFileSearch FileSearch(Extensions, Directories);
		const CFileSearch::XStringVector& rFilenames = FileSearch.GetFileNames();
		
		for (u32 i = 0; i < rFilenames.size(); i++)
		{
			File::Delete(rFilenames[i].c_str());
		}
		break;
	}
	
	if (m_GameListCtrl) m_GameListCtrl->Update();
}

// Enable and disable the toolbar
void CFrame::OnToggleToolbar(wxCommandEvent& event)
{
	SConfig::GetInstance().m_InterfaceToolbar = event.IsChecked();
	DoToggleToolbar(event.IsChecked());
}
void CFrame::DoToggleToolbar(bool _show)
{
	if (_show)
	{
		m_Mgr->GetPane(wxT("TBMain")).Show();
		if (g_pCodeWindow) { m_Mgr->GetPane(wxT("TBDebug")).Show(); m_Mgr->GetPane(wxT("TBAui")).Show(); }
		m_Mgr->Update();
	}
	else
	{
		m_Mgr->GetPane(wxT("TBMain")).Hide();
		if (g_pCodeWindow) { m_Mgr->GetPane(wxT("TBDebug")).Hide(); m_Mgr->GetPane(wxT("TBAui")).Hide(); }
		m_Mgr->Update();
	}
}

// Enable and disable the status bar
void CFrame::OnToggleStatusbar(wxCommandEvent& event)
{
	SConfig::GetInstance().m_InterfaceStatusbar = event.IsChecked();
	if (SConfig::GetInstance().m_InterfaceStatusbar == true)
		m_pStatusBar->Show();
	else
		m_pStatusBar->Hide();

	this->SendSizeEvent();
}

