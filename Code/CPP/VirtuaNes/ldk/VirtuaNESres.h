#ifndef	__VIRTUANES_RESOURCE_INCLUDED__
#define	__VIRTUANES_RESOURCE_INCLUDED__

// VirtuaNES version
#define	VIRTUANES_VERSION		0x0097
#define	VIRTUANES_FIXVERSION		""
#define	VIRTUANES_PLUGIN_VERSION	0x0097

// VirtuaNES window class
#define	VIRTUANES_WNDCLASS		L"VirtuaNESwndclass"
#define	VIRTUANES_CAPTION		L"VirtuaNES"
#define	VIRTUANES_MUTEX			L"VirtuaNESmutex"

// VirtuaNES Website
#define	VIRTUANES_WEBSITE		L"http://virtuanes.s1.xrea.com:8080/"

// VirtuaNES Author E-mail
#define	VIRTUANES_EMAILNAME		L"norix_v"
#define	VIRTUANES_EMAILDOMAIN		L"yahoo.co.jp"

// VirtuaNES private windows message
#define	WM_VNS_ERRORMSG			(WM_APP+0)
#define	WM_VNS_COMMANDLINE		(WM_APP+1)
#define	WM_VNS_LAUNCHERCMD		(WM_APP+2)
#define	WM_VNS_SHORTCUTENABLE		(WM_APP+3)
#define	WM_VNS_CHATPOPUP		(WM_APP+4)	// Netplay

// VirtuaNES Resource define
// Error string
#define	IDS_ERROR			0x0001
#define	IDS_ERROR_STARTUP		0x0002
#define	IDS_ERROR_UNKNOWN		0x0003
#define	IDS_ERROR_OPEN			0x0004
#define	IDS_ERROR_READ			0x0005
#define	IDS_ERROR_WRITE			0x0006
#define	IDS_ERROR_OUTOFMEMORY		0x0007

#define	IDS_ERROR_ILLEGALOPCODE		0x0010
#define	IDS_ERROR_UNSUPPORTFORMAT	0x0011
#define	IDS_ERROR_INVALIDNESHEADER	0x0012
#define	IDS_ERROR_SMALLFILE		0x0013
#define	IDS_ERROR_UNSUPPORTMAPPER	0x0014
#define	IDS_ERROR_NODISKBIOS		0x0015
#define	IDS_ERROR_UNSUPPORTDISK		0x0016
#define	IDS_ERROR_ILLEGALDISKSIZE	0x0017

#define	IDS_ERROR_ILLEGALMAPPERNO	0x0020
#define	IDS_ERROR_ILLEGALHEADER		0x0021
#define	IDS_ERROR_ILLEGALSTATECRC	0x0022
#define	IDS_ERROR_ILLEGALMOVIECRC	0x0023
#define	IDS_ERROR_ILLEGALMOVIEVER	0x0024	// Movie playback
#define	IDS_ERROR_ILLEGALMOVIEOLD	0x0025
#define	IDS_ERROR_ILLEGALMOVIECRC_A	0x0026	// Movie append record
#define	IDS_ERROR_ILLEGALMOVIEVER_A	0x0027
#define	IDS_ERROR_ILLEGALMOVIEOLD_A	0x0028

#define	IDS_ERROR_NETWORKDISCONNECT	0x0030
#define	IDS_ERROR_NETWORKERROR		0x0031
#define	IDS_ERROR_NETWORKERROR_VERSION	0x0032	// Different version
#define	IDS_ERROR_NETWORKERROR_CRC	0x0033	// Different CRC

// User interface
#define	IDS_UI_OPENROM			0x0100
#define	IDS_UI_OPENPALETTE		0x0101
#define	IDS_UI_BROWSE			0x0102
#define	IDS_UI_SAVEPALETTE		0x0103
#define	IDS_UI_WAVERECORD		0x0110
#define	IDS_UI_PLAYMOVIE		0x0118
#define	IDS_UI_RECMOVIE			0x0119
#define	IDS_UI_APPENDMOVIE		0x011A

#define	IDS_UI_LOADCHEATCODE		0x0120
#define	IDS_UI_SAVECHEATCODE		0x0121
#define	IDS_UI_LOADGENIECODE		0x0122

#define	IDS_UI_PLAYTAPE			0x0128
#define	IDS_UI_RECTAPE			0x0129

// Emulator priority
#define	IDS_EMU_PRIORITY_IDLE		0x0200
#define	IDS_EMU_PRIORITY_MOSTLOWER	0x0201
#define	IDS_EMU_PRIORITY_LOWER		0x0202
#define	IDS_EMU_PRIORITY_NORMAL		0x0203
#define	IDS_EMU_PRIORITY_HIGHER		0x0204
#define	IDS_EMU_PRIORITY_MOSTHIGHER	0x0205
#define	IDS_EMU_PRIORITY_REALTIME	0x0206

// Sound filter
#define	IDS_SND_FILTER_NONE		0x0210
#define	IDS_SND_FILTER_TYPE1		0x0211
#define	IDS_SND_FILTER_TYPE2		0x0212
#define	IDS_SND_FILTER_TYPE3		0x0213
#define	IDS_SND_FILTER_TYPE4		0x0214

// Controller setting select
#define	IDS_CTR_SELECT1			0x0220
#define	IDS_CTR_SELECT2			0x0221

// GameOptions
#define	IDS_OPT_RENDER_POST_ALL		0x0230
#define	IDS_OPT_RENDER_PRE_ALL		0x0231
#define	IDS_OPT_RENDER_POST		0x0232
#define	IDS_OPT_RENDER_PRE		0x0233
#define	IDS_OPT_RENDER_TILE		0x0234
#define	IDS_OPT_IRQ_HSYNC		0x0235
#define	IDS_OPT_IRQ_CLOCK		0x0236
#define	IDS_OPT_VIDEOMODE_NTSC		0x0237
#define	IDS_OPT_VIDEOMODE_PAL		0x0238

// Shortcut
#define	IDS_CUT_TITLE			0x0400
#define	IDS_CUT_TYPE			0x0401
#define	IDS_CUT_KEY			0x0402
#define	IDS_CUT_KEY2			0x0403

#define	IDS_CUT_OPEN			0x0410
#define	IDS_CUT_CLOSE			0x0411
#define	IDS_CUT_LAUNCHER		0x0414
#define	IDS_CUT_ROMINFO			0x0415
#define	IDS_CUT_WAVERECORD		0x0416
#define	IDS_CUT_NETPLAY_CONNECT		0x0418
#define	IDS_CUT_NETPLAY_DISCONNECT	0x0419
#define	IDS_CUT_NETPLAY_CHAT		0x041A
#define	IDS_CUT_EXIT			0x041F

#define	IDS_CUT_HWRESET			0x0420
#define	IDS_CUT_SWRESET			0x0421
#define	IDS_CUT_PAUSE			0x0422
#define	IDS_CUT_THROTTLE		0x0423
#define	IDS_CUT_FRAMESKIP_AUTO		0x0424
#define	IDS_CUT_FRAMESKIP_UP		0x0425
#define	IDS_CUT_FRAMESKIP_DOWN		0x0426
#define	IDS_CUT_ONEFRAME		0x0427
#define	IDS_CUT_KEYTHROTTLE		0x0428

#define	IDS_CUT_STATE_LOAD		0x0430
#define	IDS_CUT_STATE_SAVE		0x0431
#define	IDS_CUT_STATE_UP		0x0432
#define	IDS_CUT_STATE_DOWN		0x0433
#define	IDS_CUT_STATE_SLOT0		0x0440
#define	IDS_CUT_STATE_SLOT1		0x0441
#define	IDS_CUT_STATE_SLOT2		0x0442
#define	IDS_CUT_STATE_SLOT3		0x0443
#define	IDS_CUT_STATE_SLOT4		0x0444
#define	IDS_CUT_STATE_SLOT5		0x0445
#define	IDS_CUT_STATE_SLOT6		0x0446
#define	IDS_CUT_STATE_SLOT7		0x0447
#define	IDS_CUT_STATE_SLOT8		0x0448
#define	IDS_CUT_STATE_SLOT9		0x0449

#define	IDS_CUT_DISK_EJECT		0x0450
#define	IDS_CUT_DISK_0A			0x0451
#define	IDS_CUT_DISK_0B			0x0452
#define	IDS_CUT_DISK_1A			0x0453
#define	IDS_CUT_DISK_1B			0x0454
#define	IDS_CUT_DISK_2A			0x0455
#define	IDS_CUT_DISK_2B			0x0456
#define	IDS_CUT_DISK_3A			0x0457
#define	IDS_CUT_DISK_3B			0x0458

#define	IDS_CUT_MOVIE_INFO		0x0460
#define	IDS_CUT_MOVIE_PLAY		0x0461
#define	IDS_CUT_MOVIE_REC		0x0462
#define	IDS_CUT_MOVIE_REC_APPEND	0x0463
#define	IDS_CUT_MOVIE_STOP		0x0464
#define	IDS_CUT_MOVIE_CONVERT		0x0465

#define	IDS_CUT_TAPE_PLAY		0x0468
#define	IDS_CUT_TAPE_REC		0x0469
#define	IDS_CUT_TAPE_STOP		0x046A

#define	IDS_CUT_ZOOMx1			0x0470
#define	IDS_CUT_ZOOMx2			0x0471
#define	IDS_CUT_ZOOMx3			0x0472
#define	IDS_CUT_ZOOMx4			0x0473
#define	IDS_CUT_FULLSCREEN		0x0478

#define	IDS_CUT_SNAPSHOT		0x0480
#define	IDS_CUT_FPSDISP			0x0481
#define	IDS_CUT_TVASPECT		0x0482
#define	IDS_CUT_TVFRAME			0x0483
#define	IDS_CUT_SCANLINE		0x0484
#define	IDS_CUT_ALLLINE			0x0485
#define	IDS_CUT_ALLSPRITE		0x0486
#define	IDS_CUT_SYNCDRAW		0x0487
#define	IDS_CUT_FITSCREEN		0x0488
#define	IDS_CUT_LEFTCLIP		0x0489

#define	IDS_CUT_MUTE_MASTER		0x0490
#define	IDS_CUT_MUTE_RECTANGLE1		0x0491
#define	IDS_CUT_MUTE_RECTANGLE2		0x0492
#define	IDS_CUT_MUTE_TRIANGLE		0x0493
#define	IDS_CUT_MUTE_NOISE		0x0494
#define	IDS_CUT_MUTE_DPCM		0x0495
#define	IDS_CUT_MUTE_EXTERNAL1		0x0496
#define	IDS_CUT_MUTE_EXTERNAL2		0x0497
#define	IDS_CUT_MUTE_EXTERNAL3		0x0498
#define	IDS_CUT_MUTE_EXTERNAL4		0x0499
#define	IDS_CUT_MUTE_EXTERNAL5		0x049A
#define	IDS_CUT_MUTE_EXTERNAL6		0x049B
#define	IDS_CUT_MUTE_EXTERNAL7		0x049C
#define	IDS_CUT_MUTE_EXTERNAL8		0x049D

#define	IDS_CUT_SEARCH			0x04A0
#define	IDS_CUT_CHEAT			0x04A1
#define	IDS_CUT_CHEAT_ENABLE		0x04A2
#define	IDS_CUT_CHEAT_DISABLE		0x04A3
#define	IDS_CUT_GENIE			0x04A4

#define	IDS_CUT_VIEW_PATTERN		0x04B0
#define	IDS_CUT_VIEW_NAMETABLE		0x04B1
#define	IDS_CUT_VIEW_PALETTE		0x04B2
#define	IDS_CUT_VIEW_MEMORY		0x04B3
#define	IDS_CUT_VIEW_WATCH		0x04B4

#define	IDS_CUT_QUICKLOAD_SLOT0		0x04C0
#define	IDS_CUT_QUICKLOAD_SLOT1		0x04C1
#define	IDS_CUT_QUICKLOAD_SLOT2		0x04C2
#define	IDS_CUT_QUICKLOAD_SLOT3		0x04C3
#define	IDS_CUT_QUICKLOAD_SLOT4		0x04C4
#define	IDS_CUT_QUICKLOAD_SLOT5		0x04C5
#define	IDS_CUT_QUICKLOAD_SLOT6		0x04C6
#define	IDS_CUT_QUICKLOAD_SLOT7		0x04C7
#define	IDS_CUT_QUICKLOAD_SLOT8		0x04C8
#define	IDS_CUT_QUICKLOAD_SLOT9		0x04C9

#define	IDS_CUT_QUICKSAVE_SLOT0		0x04D0
#define	IDS_CUT_QUICKSAVE_SLOT1		0x04D1
#define	IDS_CUT_QUICKSAVE_SLOT2		0x04D2
#define	IDS_CUT_QUICKSAVE_SLOT3		0x04D3
#define	IDS_CUT_QUICKSAVE_SLOT4		0x04D4
#define	IDS_CUT_QUICKSAVE_SLOT5		0x04D5
#define	IDS_CUT_QUICKSAVE_SLOT6		0x04D6
#define	IDS_CUT_QUICKSAVE_SLOT7		0x04D7
#define	IDS_CUT_QUICKSAVE_SLOT8		0x04D8
#define	IDS_CUT_QUICKSAVE_SLOT9		0x04D9

// Launcher
#define	IDS_LCH_LISTUPDATE		0x0500
#define	IDS_LCH_HEADERREWRITE		0x0501
#define	IDS_LCH_FILENAME		0x0510
#define	IDS_LCH_PATH			0x0511
#define	IDS_LCH_MAPPER			0x0512
#define	IDS_LCH_PRG			0x0513
#define	IDS_LCH_CHR			0x0514
#define	IDS_LCH_ALLCRC			0x0515
#define	IDS_LCH_PRGCRC			0x0516
#define	IDS_LCH_INFO			0x0517
#define	IDS_LCH_DATABASE		0x0518
#define	IDS_LCH_TITLE			0x0519
#define	IDS_LCH_COUNTRY			0x051A
#define	IDS_LCH_MANUFACTURER		0x051B
#define	IDS_LCH_SALEDATE		0x051C
#define	IDS_LCH_PRICE			0x051D
#define	IDS_LCH_GENRE			0x051E

// NetPlay
#define	IDS_NET_NOCONNECT		0x0600
#define	IDS_NET_ACCEPTING		0x0601
#define	IDS_NET_CONNECTING		0x0602
#define	IDS_NET_CALCULATING		0x0603
#define	IDS_NET_DISCONNECT		0x0610
#define	IDS_NET_ERROR			0x0611

// Cheat
#define	IDS_CHT_CHEATCODE		0x0700
#define	IDS_CHT_COMMENT			0x0701
#define	IDS_CHT_ADDRESS			0x0702
#define	IDS_CHT_DATA_NOW		0x0703
#define	IDS_CHT_DATA_OLD		0x0704

// コントロールIDs
// UI
#define	IDDEFAULT			101
#define	IDR_MENU			110
#define	IDI_ICON			111
#define	IDB_CHEATIMAGELIST		112
#define	IDB_LAUNCHERIMAGELIST		113
#define	IDI_SORT_DOWN			114
#define	IDI_SORT_UP			115

// Dialogs
#define	IDD_VERSION			200
#define	IDD_ROMINFO			210
#define	IDD_MOVIEINFO			220

#define	IDD_MOVIECONV			230

// For configuration
#define	IDD_CFG_EMULATOR		300
#define	IDD_CFG_GRAPHICS		301
#define	IDD_CFG_SOUND			302
#define	IDD_CFG_FOLDER			303
#define	IDD_CFG_SHORTCUT		304
#define	IDD_CFG_LANGUAGE		305
#define	IDD_CFG_MOVIE			306

#define	IDD_CFG_PALETTE			320

#define	IDD_CFG_GAMEOPTION		350

// For Controller configuration
#define	IDD_CFG_CONTROLLER		400
#define	IDD_CTR_PLAYER1			401
#define	IDD_CTR_PLAYER2			402
#define	IDD_CTR_PLAYER3			403
#define	IDD_CTR_PLAYER4			404
#define	IDD_CTR_CRAZYCLIMBER		405
#define	IDD_CTR_NSFPLAYER		406

#define	IDD_CTR_FAMLYTRAINER		407
#define	IDD_CTR_EXCITINGBOXING		408
#define	IDD_CTR_MAHJANG			409

#define	IDD_CTR_VSUNISYSTEM		410

// For Joypad Axis setting
#define	IDD_CFG_AXISSETTING		450

// For Extension device
#define	IDD_EXT_BARCODEBATTLER		480
#define	IDD_EXT_VSUNISYSTEM		481

// For Launcher
#define	IDD_LAUNCHER			500	// Dialog
#define	IDR_LCH_MENU			501	// Menu
#define	IDD_LCH_DISPEDIT		502
#define	IDD_LCH_HEADEREDIT		503
#define	IDD_LCH_FOLDER			504

// For Cheat
#define	IDD_SEARCH			600
#define	IDD_CHEAT			601
#define	IDD_CODEEDIT			602	// Code create
#define	IDD_CODEINPUT			603	// Code input

// For NetPlay
#define	IDD_NETPLAY			800
#define	IDD_NETPLAY_CHAT		801

// For Debugger
#define	IDD_DBG_DEBUGGER		900
#define	IDD_DBG_DEBUGGER_MENU		901
#define	IDD_DBG_REGVIEW			902
#define	IDD_DBG_REGVIEW_MENU		903
#define	IDD_DBG_MEMVIEW			904
#define	IDD_DBG_MEMVIEW_MENU		905
#define	IDD_DBG_MEMSEARCH		906
#define	IDD_DBG_MEMSEARCH_MENU		907

// ダイアログ内コントロールIDs
// IDD_VERSION
#define	IDC_VER_VERSION			1000
#define	IDC_VER_ICON			1001
#define	IDC_VER_WEBSITE			1002
#define	IDC_VER_COPYRIGHT		1003
#define	IDC_VER_EMAIL			1004

// IDD_ROMINFO
#define	IDC_ROM_NAME			1100
#define	IDC_ROM_MAPPER			1101
#define	IDC_ROM_PRG			1102
#define	IDC_ROM_CHR			1103
#define	IDC_ROM_MIRROR			1104
#define	IDC_ROM_SRAM			1105
#define	IDC_ROM_4SCREEN			1106
#define	IDC_ROM_TRAINER			1107
#define	IDC_ROM_CRC			1108
#define	IDC_ROM_CRCALL			1109
#define	IDC_ROM_CRCCHR			1110
#define	IDC_ROM_VSUNISYSTEM		1111

// IDD_MOVIEINFO
#define	IDC_MIF_VERSION			1150
#define	IDC_MIF_TIMES			1151
#define	IDC_MIF_FRAMES			1152
#define	IDC_MIF_RERECORDTIMES		1153
#define	IDC_MIF_RERECORDVERSION		1154

// IDD_CFG_EMULATOR
#define	IDC_EMU_ILLEGALOP		1200
#define	IDC_EMU_AUTOFRAMESKIP		1201
#define	IDC_EMU_THROTTLE		1202
#define	IDC_EMU_THROTTLE_SLIDER		1203
#define	IDC_EMU_PRIORITY_COMBO		1204
#define	IDC_EMU_FPS			1205
#define	IDC_EMU_BACKGROUND		1206
#define	IDC_EMU_FOURPLAYER		1207
#define	IDC_EMU_DOUBLEEXECUTE		1208
#define	IDC_EMU_DISKTHROTTLE		1209
#define	IDC_EMU_LOADFULLSCREEN		1210
#define	IDC_EMU_CRCCHECK		1211
#define	IDC_EMU_STARTUPLAUNCHER		1212
#define	IDC_EMU_PNGSNAPSHOT		1213
#define	IDC_EMU_NOJOYSTICKID		1214
#define	IDC_EMU_PAUSELAUNCHER		1215

// IDD_CFG_GRAPHICS
#define	IDC_GRA_ASPECT			1300
#define	IDC_GRA_ALLSPRITE		1301
#define	IDC_GRA_ALLLINE			1302
#define	IDC_GRA_FPS			1303
#define	IDC_GRA_TVFRAME			1304
#define	IDC_GRA_SCANLINE		1305
#define	IDC_GRA_SCANLINE_SLIDER		1306
#define	IDC_GRA_SCANLINE_COLOR		1307
#define	IDC_GRA_PALETTE_USE		1308
#define	IDC_GRA_PALETTE_EDIT		1309
#define	IDC_GRA_PALETTE_BROWSE		1310
#define	IDC_GRA_SYNCDRAW		1311
#define	IDC_GRA_FITZOOM			1312
#define	IDC_GRA_RESOLUTION_COMBO	1313
#define	IDC_GRA_SYSTEMMEMORY		1314
#define	IDC_GRA_DOUBLESIZE		1315
#define	IDC_GRA_LEFTCLIP		1316
#define	IDC_GRA_USEHEL			1317
#define	IDC_GRA_WAITVSYNC		1318

#define	IDC_GRA_DISKACCESSLAMP		1319

#define	IDC_GRA_SYNCNOSLEEP		1320
#define	IDC_GRA_NOSQUARELIST		1321

// IDD_CFG_SOUND
#define	IDC_SND_ENABLE			1400
#define	IDC_SND_SAMPLERATE_COMBO	1401
#define	IDC_SND_BUFFERSIZE_COMBO	1402
#define	IDC_SND_MASTER_SLIDER		1403	// Master
#define	IDC_SND_CH0_SLIDER		1404	// Rectangle 1
#define	IDC_SND_CH1_SLIDER		1405	// Rectangle 2
#define	IDC_SND_CH2_SLIDER		1406	// Triangle
#define	IDC_SND_CH3_SLIDER		1407	// Noise
#define	IDC_SND_CH4_SLIDER		1408	// DPCM
#define	IDC_SND_EX0_SLIDER		1409	// VRC6
#define	IDC_SND_EX1_SLIDER		1410	// VRC7
#define	IDC_SND_EX2_SLIDER		1411	// FDS
#define	IDC_SND_EX3_SLIDER		1412	// MMC5
#define	IDC_SND_EX4_SLIDER		1413	// N106
#define	IDC_SND_EX5_SLIDER		1414	// FME7
#define	IDC_SND_FILTERTYPE_COMBO	1420

#define	IDC_SND_VOLEFFECT_ENABLE	1421
#define	IDC_SND_EXTSOUND_ENABLE		1422

// IDD_CFG_FOLDER
#define	IDC_FLD_ROM_USE			1500
#define	IDC_FLD_ROM_EDIT		1501
#define	IDC_FLD_ROM_BROWSE		1502
#define	IDC_FLD_SAVE_USE		1503
#define	IDC_FLD_SAVE_EDIT		1504
#define	IDC_FLD_SAVE_BROWSE		1505
#define	IDC_FLD_STATE_USE		1506
#define	IDC_FLD_STATE_EDIT		1507
#define	IDC_FLD_STATE_BROWSE		1508
#define	IDC_FLD_SNAPSHOT_USE		1509
#define	IDC_FLD_SNAPSHOT_EDIT		1510
#define	IDC_FLD_SNAPSHOT_BROWSE		1511
#define IDC_FLD_MOVIE_USE		1512
#define IDC_FLD_MOVIE_EDIT		1513
#define IDC_FLD_MOVIE_BROWSE		1514
#define IDC_FLD_WAVE_USE		1515
#define IDC_FLD_WAVE_EDIT		1516
#define IDC_FLD_WAVE_BROWSE		1517
#define IDC_FLD_CHEAT_USE		1518
#define IDC_FLD_CHEAT_EDIT		1519
#define IDC_FLD_CHEAT_BROWSE		1520
#define IDC_FLD_IPS_USE			1521
#define IDC_FLD_IPS_EDIT		1522
#define IDC_FLD_IPS_BROWSE		1523

// IDD_CFG_SHORTCUT
#define	IDC_CUT_LIST			1600
#define	IDC_CUT_BUTTON1			1601
#define	IDC_CUT_BUTTON2			1602

// IDD_CFG_LANGUAGE
#define	IDC_LNG_LIST			1700

// IDD_CFG_MOVIE
#define	IDC_MVI_1P_USE			1800
#define	IDC_MVI_2P_USE			1801
#define	IDC_MVI_3P_USE			1802
#define	IDC_MVI_4P_USE			1803
#define	IDC_MVI_RERECORD_DISABLE	1804
#define	IDC_MVI_LOOPPLAY		1805
#define	IDC_MVI_RESETRECORD		1806
#define	IDC_MVI_PADDISPLAY		1807
#define	IDC_MVI_TIMEDISPLAY		1808

// IDD_CFG_GAMEOPTION
#define	IDC_OPT_RENDER_COMBO		1900
#define	IDC_OPT_IRQTYPE_COMBO		1901
#define	IDC_OPT_VIDEOMODE_COMBO		1902
#define	IDC_OPT_FRAMEIRQ		1903
#define	IDC_OPT_NOTSAVE			1904

// IDD_CFG_CONTROLLER
#define	IDC_CTR_TAB			2000
#define	IDC_CTR_SELECT_COMBO		2001

// IDD_CTR_PLAYER1..4,CRAZYCLIMBER
#define	IDC_CTR_UP			2010
#define	IDC_CTR_DOWN			2011
#define	IDC_CTR_LEFT			2012
#define	IDC_CTR_RIGHT			2013
#define	IDC_CTR_A			2014
#define	IDC_CTR_B			2015
#define	IDC_CTR_A_RAPID			2016
#define	IDC_CTR_B_RAPID			2017
#define	IDC_CTR_EXA			2018
#define	IDC_CTR_EXB			2019
#define	IDC_CTR_EXC			2020
#define	IDC_CTR_EXD			2021
#define	IDC_CTR_EXE			2022
#define	IDC_CTR_EXF			2023
#define	IDC_CTR_EXG			2024
#define	IDC_CTR_EXH			2025

#define	IDC_CTR_EXI			2026
#define	IDC_CTR_EXJ			2027
#define	IDC_CTR_EXK			2028
#define	IDC_CTR_EXL			2029
#define	IDC_CTR_EXM			2030
#define	IDC_CTR_EXN			2031
#define	IDC_CTR_EXO			2032
#define	IDC_CTR_EXP			2033
#define	IDC_CTR_EXQ			2034
#define	IDC_CTR_EXR			2035
#define	IDC_CTR_EXS			2036
#define	IDC_CTR_EXT			2037
#define	IDC_CTR_EXU			2038
#define	IDC_CTR_EXV			2039
#define	IDC_CTR_EXW			2040
#define	IDC_CTR_EXX			2041

#define	IDC_CTR_END			2041	// 終端

#define	IDC_CTR_A_RAPID_LIST		2050
#define	IDC_CTR_B_RAPID_LIST		2051

// IDD_CFG_AXISSETTING
#define	IDC_AST_ID_COMBO		2200
#define	IDC_AST_XAXIS			2210
#define	IDC_AST_YAXIS			2211
#define	IDC_AST_ZAXIS			2212
#define	IDC_AST_RXAXIS			2213
#define	IDC_AST_RYAXIS			2214
#define	IDC_AST_RZAXIS			2215
#define	IDC_AST_XAXIS_PROGRESS		2220
#define	IDC_AST_YAXIS_PROGRESS		2221
#define	IDC_AST_ZAXIS_PROGRESS		2222
#define	IDC_AST_RXAXIS_PROGRESS		2223
#define	IDC_AST_RYAXIS_PROGRESS		2224
#define	IDC_AST_RZAXIS_PROGRESS		2225

// IDD_EXT_BACODEBATTLER
#define	IDC_EBB_CODE			2100
#define	IDC_EBB_TRANSFER		2101
#define	IDC_EBB_RANDOM			2102

// IDD_EXT_VSUNISYSTEM
#define	IDC_EVS_DIPNAME0		2150
#define	IDC_EVS_DIPNAME1		2151
#define	IDC_EVS_DIPNAME2		2152
#define	IDC_EVS_DIPNAME3		2153
#define	IDC_EVS_DIPNAME4		2154
#define	IDC_EVS_DIPNAME5		2155
#define	IDC_EVS_DIPNAME6		2156
#define	IDC_EVS_DIPNAME7		2157
#define	IDC_EVS_DIPCOMBO0		2160
#define	IDC_EVS_DIPCOMBO1		2161
#define	IDC_EVS_DIPCOMBO2		2162
#define	IDC_EVS_DIPCOMBO3		2163
#define	IDC_EVS_DIPCOMBO4		2164
#define	IDC_EVS_DIPCOMBO5		2165
#define	IDC_EVS_DIPCOMBO6		2166
#define	IDC_EVS_DIPCOMBO7		2167

// IDD_CFG_PALETTE
#define	IDC_PAL_R_EDIT			2300
#define	IDC_PAL_G_EDIT			2301
#define	IDC_PAL_B_EDIT			2302
#define	IDC_PAL_R_UPDOWN		2310
#define	IDC_PAL_G_UPDOWN		2311
#define	IDC_PAL_B_UPDOWN		2312
#define	IDC_PAL_R_SLIDER		2320
#define	IDC_PAL_G_SLIDER		2321
#define	IDC_PAL_B_SLIDER		2322
#define	IDC_PAL_COPY			2330
#define	IDC_PAL_EXCHANGE		2331
#define	IDC_PAL_UNDO			2340
#define	IDC_PAL_LOAD			2341
#define	IDC_PAL_SAVE			2342
#define	IDC_PAL_NO			2350

// IDR_LAUNCER
#define	IDC_LCH_LIST			3000
#define	IDC_LCH_STATUS			3001

// IDD_LCH_DISPEDIT
#define	IDC_DED_HIDELIST		3100
#define	IDC_DED_VIEWLIST		3101
#define	IDC_DED_DEL			3102
#define	IDC_DED_ADD			3103
#define	IDC_DED_UP			3104
#define	IDC_DED_DOWN			3105

// IDD_LCH_HEADEREDIT
#define	IDC_HED_EDIT			3200
#define	IDC_HED_VMIRROR			3201
#define	IDC_HED_SRAM			3202
#define	IDC_HED_4SCREEN			3203
#define	IDC_HED_TRAINER			3204
#define	IDC_HED_VSUNISYSTEM		3205

// IDD_LCH_FOLDER
#define	IDC_LFL_LIST			3300
#define	IDC_LFL_FOLDERADD		3301
#define	IDC_LFL_ADD			3302
#define	IDC_LFL_DEL			3303

// IDD_SEARCH
#define	IDR_SCH_MENU			4000	// Menu

#define	IDC_SCH_RESULT_LIST		4001
#define	IDC_SCH_RADIX_DEC		4002
#define	IDC_SCH_RADIX_HEX		4003
#define	IDC_SCH_LENGTH_1BYTE		4004
#define	IDC_SCH_LENGTH_2BYTE		4005
#define	IDC_SCH_LENGTH_3BYTE		4006
#define	IDC_SCH_LENGTH_4BYTE		4007
#define	IDC_SCH_AREA_RAM		4008
#define	IDC_SCH_AREA_SRAM		4009
#define	IDC_SCH_AREA_EXRAM		4010
#define	IDC_SCH_START			4011
#define	IDC_SCH_UPDATE			4012
#define	IDC_SCH_UNDO			4013
#define	IDC_SCH_EQUAL			4014
#define	IDC_SCH_NOTEQUAL		4015
#define	IDC_SCH_LESS			4016
#define	IDC_SCH_GRATER			4017
#define	IDC_SCH_LESSEQUAL		4018
#define	IDC_SCH_GRATEREQUAL		4019
#define	IDC_SCH_DATA_EDIT		4020	// Search data
#define	IDC_SCH_SEARCH			4021	// Search button
#define	IDC_SCH_WADDR_EDIT		4022	// Write address
#define	IDC_SCH_WDATA_EDIT		4023	// Write data
#define	IDC_SCH_WRITE			4024	// Write button

// IDD_CHEAT
#define	IDC_CHT_CODE_LIST		4101
#define	IDC_CHT_LOAD			4102
#define	IDC_CHT_SAVE			4103
#define	IDC_CHT_INPUT			4104
#define	IDC_CHT_EDIT			4105
#define	IDC_CHT_ENABLE			4106
#define	IDC_CHT_DISABLE			4107
#define	IDC_CHT_CLEAR			4108
#define	IDC_CHT_REMOVE			4109

// IDD_CODEEDIT
#define	IDC_CED_ADDR			4200
#define	IDC_CED_DATA			4201
#define	IDC_CED_COMMENT			4202
#define	IDC_CED_RADIX_DEC		4203
#define	IDC_CED_RADIX_HEX		4204
#define	IDC_CED_LENGTH_1BYTE		4205
#define	IDC_CED_LENGTH_2BYTE		4206
#define	IDC_CED_LENGTH_3BYTE		4207
#define	IDC_CED_LENGTH_4BYTE		4208
#define	IDC_CED_TYPE_ALWAYS		4209
#define	IDC_CED_TYPE_ONCE		4210
#define	IDC_CED_TYPE_GREATER		4211
#define	IDC_CED_TYPE_LESS		4212

// IDD_CHEATCODE/IDD_GENIE
#define	IDC_CHC_CODE			4300
#define	IDC_CHC_COMMENT			4301

// IDD_NETPLAY
#define	IDC_NET_PORT_COMBO		8000	// Use Port(Server only)
#define	IDC_NET_LATENCY_COMBO		8001	// Latency (Server only)
#define	IDC_NET_HOST_COMBO		8002	// Hostname(hostaddr:port)(Client only)
#define	IDC_NET_NICKNAME		8003	// Nickname
#define	IDC_NET_SERVER			8010
#define	IDC_NET_CLIENT			8011
#define	IDC_NET_CONNECT			8020
#define	IDC_NET_STATUS			8030

// IDD_NETPLAY_CHAT
#define	IDC_NCT_MESSAGE			8100	// Message window
#define	IDC_NCT_EDIT			8101	// Message edit window
#define	IDC_NCT_SEND			8110	// Send button

// IDD_MOVIECONV
#define	IDC_MCV_ROMBROWSE		5000
#define	IDC_MCV_MOVIEBROWSE		5001
#define	IDC_MCV_AVIBROWSE		5002
#define	IDC_MCV_ROMFILE			5003
#define	IDC_MCV_MOVIEFILE		5004
#define	IDC_MCV_AVIFILE			5005
#define	IDC_MCV_CONVERT			5006
#define	IDC_MCV_FRAMERATE_COMBO		5011
#define	IDC_MCV_SAMPLERATE_COMBO	5012
#define	IDC_MCV_FILTERTYPE_COMBO	5013
#define	IDC_MCV_VIDEOSELECT		5020
#define	IDC_MCV_AUDIOSELECT		5021

// IDD_DEBUGGER
//

// メニューコマンド IDs
//
// ファイル(&F)
#define	ID_OPEN				0x8000
#define	ID_CLOSE			0x8001
#define	ID_EXIT				0x8002
#define	ID_LAUNCHER			0x8010
#define	ID_LAUNCHER_CLOSE		0x8011	// 内部コマンド
#define	ID_ROMINFO			0x8020
#define	ID_WAVERECORD			0x8028
#define	ID_ABOUT			0x8080
#define	ID_HTMLHELP			0x8081

// "ﾈｯﾄﾜｰｸ"POPUP
#define	ID_NETPLAY_CONNECT		0x80D0
#define	ID_NETPLAY_DISCONNECT		0x80D1
#define	ID_NETPLAY_CHAT			0x80D2	// ChatWindow

// "ﾑｰﾋﾞｰ"POPUP
#define	ID_MOVIE_REC			0x80E0
#define	ID_MOVIE_REC_APPEND		0x80E1
#define	ID_MOVIE_PLAY			0x80E2
#define	ID_MOVIE_STOP			0x80E3
#define	ID_MOVIE_INFO			0x80E4
#define	ID_MOVIE_CONVERT		0x80E8

// 最近使ったファイル
#define	ID_MRU_FILE0			0x8300
#define	ID_MRU_FILE1			0x8301
#define	ID_MRU_FILE2			0x8302
#define	ID_MRU_FILE3			0x8303
#define	ID_MRU_FILE4			0x8304
#define	ID_MRU_FILE5			0x8305
#define	ID_MRU_FILE6			0x8306
#define	ID_MRU_FILE7			0x8307
#define	ID_MRU_FILE8			0x8308
#define	ID_MRU_FILE9			0x8309
// 最近使ったパス
#define	ID_MRU_PATH0			0x8310
#define	ID_MRU_PATH1			0x8311
#define	ID_MRU_PATH2			0x8312
#define	ID_MRU_PATH3			0x8313
#define	ID_MRU_PATH4			0x8314
#define	ID_MRU_PATH5			0x8315
#define	ID_MRU_PATH6			0x8316
#define	ID_MRU_PATH7			0x8317
#define	ID_MRU_PATH8			0x8318
#define	ID_MRU_PATH9			0x8319

// 編集(&E)
#define	ID_HWRESET			0x8100
#define	ID_SWRESET			0x8101
#define	ID_PAUSE			0x8102
#define	ID_STATE_LOAD			0x8103
#define	ID_STATE_SAVE			0x8104
// "ｽﾃｰﾄｽﾛｯﾄ"POPUP
#define	ID_STATE_SLOT0			0x8110
#define	ID_STATE_SLOT1			0x8111
#define	ID_STATE_SLOT2			0x8112
#define	ID_STATE_SLOT3			0x8113
#define	ID_STATE_SLOT4			0x8114
#define	ID_STATE_SLOT5			0x8115
#define	ID_STATE_SLOT6			0x8116
#define	ID_STATE_SLOT7			0x8117
#define	ID_STATE_SLOT8			0x8118
#define	ID_STATE_SLOT9			0x8119

// "ﾃﾞｨｽｸ"POPUP
#define	ID_DISK_EJECT			0x8120
#define	ID_DISK_0A			0x8121
#define	ID_DISK_0B			0x8122
#define	ID_DISK_1A			0x8123
#define	ID_DISK_1B			0x8124
#define	ID_DISK_2A			0x8125
#define	ID_DISK_2B			0x8126
#define	ID_DISK_3A			0x8127
#define	ID_DISK_3B			0x8128


// "拡張ｺﾝﾄﾛｰﾗ"POPUP
#define	ID_EXCTR_NONE			0x8130
#define	ID_EXCTR_PADDLE			0x8131
#define	ID_EXCTR_HYPERSHOT		0x8132
#define	ID_EXCTR_ZAPPER			0x8133
#define	ID_EXCTR_KEYBOARD		0x8134
#define	ID_EXCTR_CRAZYCLIMBER		0x8135
#define	ID_EXCTR_TOPRIDER		0x8136
#define	ID_EXCTR_SPACESHADOWGUN		0x8137

#define	ID_EXCTR_FAMILYTRAINER_A	0x8138
#define	ID_EXCTR_FAMILYTRAINER_B	0x8139
#define	ID_EXCTR_EXCITINGBOXING		0x813A
#define	ID_EXCTR_MAHJANG		0x813B
#define	ID_EXCTR_OEKAKIDS_TABLET	0x813C
#define	ID_EXCTR_TURBOFILE		0x813D

#define	ID_EXCTR_VSUNISYSTEM		0x813E
#define	ID_EXCTR_VSUNISYSTEM_ZAPPER	0x813F

#define	ID_EXCTR_END			0x813F		// 終端

// "Quickﾛｰﾄﾞ"POPUP
#define	ID_QUICKLOAD_SLOT0		0x8140
#define	ID_QUICKLOAD_SLOT1		0x8141
#define	ID_QUICKLOAD_SLOT2		0x8142
#define	ID_QUICKLOAD_SLOT3		0x8143
#define	ID_QUICKLOAD_SLOT4		0x8144
#define	ID_QUICKLOAD_SLOT5		0x8145
#define	ID_QUICKLOAD_SLOT6		0x8146
#define	ID_QUICKLOAD_SLOT7		0x8147
#define	ID_QUICKLOAD_SLOT8		0x8148
#define	ID_QUICKLOAD_SLOT9		0x8149
// "Quickｾｰﾌﾞ"POPUP
#define	ID_QUICKSAVE_SLOT0		0x8150
#define	ID_QUICKSAVE_SLOT1		0x8151
#define	ID_QUICKSAVE_SLOT2		0x8152
#define	ID_QUICKSAVE_SLOT3		0x8153
#define	ID_QUICKSAVE_SLOT4		0x8154
#define	ID_QUICKSAVE_SLOT5		0x8155
#define	ID_QUICKSAVE_SLOT6		0x8156
#define	ID_QUICKSAVE_SLOT7		0x8157
#define	ID_QUICKSAVE_SLOT8		0x8158
#define	ID_QUICKSAVE_SLOT9		0x8159

// 拡張ﾃﾞﾊﾞｲｽ
// "ﾃｰﾌﾟﾃﾞﾊﾞｲｽ"POPUP
#define	ID_TAPE_PLAY			0x8180
#define	ID_TAPE_REC			0x8181
#define	ID_TAPE_STOP			0x8182

#define	ID_BARCODEBATTLER		0x8190
#define	ID_VSUNISYSTEM_DIPSWITCH	0x8194

// "ﾀｰﾎﾞﾌｧｲﾙ"POPUP
#define	ID_TURBOFILE_BANK0		0x81A0
#define	ID_TURBOFILE_BANK1		0x81A1
#define	ID_TURBOFILE_BANK2		0x81A2
#define	ID_TURBOFILE_BANK3		0x81A3

// 設定(&C)
#define	ID_CFG_EMULATOR			0x8200
#define	ID_CFG_GRAPHICS			0x8201
#define	ID_CFG_SOUND			0x8202
#define	ID_CFG_CONTROLLER		0x8203
#define	ID_CFG_SHORTCUT			0x8204
#define	ID_CFG_FOLDER			0x8205
#define	ID_CFG_LANGUAGE			0x8206
#define	ID_CFG_MOVIE			0x8207
#define	ID_CFG_GAMEOPTION		0x8208
#define	ID_CFG_JOYAXIS			0x8209
#define	ID_CFG_PALETTE			0x820A
// "画面ｻｲｽﾞ"POPUP
#define	ID_ZOOMx1			0x8220
#define	ID_ZOOMx2			0x8221
#define	ID_ZOOMx3			0x8222
#define	ID_ZOOMx4			0x8223

// サウンドミュート
#define	ID_MUTE_0			0x8230
#define	ID_MUTE_1			0x8231
#define	ID_MUTE_2			0x8232
#define	ID_MUTE_3			0x8233
#define	ID_MUTE_4			0x8234
#define	ID_MUTE_5			0x8235
#define	ID_MUTE_6			0x8236
#define	ID_MUTE_7			0x8237
#define	ID_MUTE_8			0x8238
#define	ID_MUTE_9			0x8239
#define	ID_MUTE_A			0x823A
#define	ID_MUTE_B			0x823B
#define	ID_MUTE_C			0x823C
#define	ID_MUTE_D			0x823D
#define	ID_MUTE_E			0x823E
#define	ID_MUTE_F			0x823F

// ツール(&T)
#define	ID_SEARCH			0x8800
#define	ID_CHEAT			0x8801
#define	ID_GENIE			0x8802

#define	ID_CHEAT_ENABLE			0x8803
#define	ID_CHEAT_DISABLE		0x8804

#define	ID_AUTOIPS			0x8810

#define	ID_VIEW_PATTERN			0x8900
#define	ID_VIEW_NAMETABLE		0x8901
#define	ID_VIEW_PALETTE			0x8902

#define	ID_VIEW_MEMORY			0x8910
#define	ID_VIEW_WATCH			0x8911

// その他のコマンド(メニュー表示されるものもある)
#define	ID_STATE_UP			0x9000
#define	ID_STATE_DOWN			0x9001
#define	ID_THROTTLE			0x9002
#define	ID_FULLSCREEN			0x9003
#define	ID_FPSDISP			0x9004
#define	ID_TVASPECT			0x9005
#define	ID_TVFRAME			0x9006
#define	ID_SCANLINE			0x9007
#define	ID_ALLLINE			0x9008
#define	ID_ALLSPRITE			0x9009
#define	ID_SYNCDRAW			0x900A
#define	ID_FITSCREEN			0x900B
#define	ID_SNAPSHOT			0x900C
#define	ID_FRAMESKIP_AUTO		0x900D
#define	ID_FRAMESKIP_UP			0x900E
#define	ID_FRAMESKIP_DOWN		0x900F
#define	ID_LEFTCLIP			0x9010
#define	ID_ONEFRAME			0x9011
#define	ID_KEYTHROTTLE			0x9012

// 画面フィルタ
#define	ID_FILTER_NONE			0x9100
#define	ID_FILTER_2XSAI			0x9101
#define	ID_FILTER_SUPER2XSAI		0x9102
#define	ID_FILTER_SUPEREAGLE		0x9103
#define	ID_FILTER_SCALE2X		0x9104
#define	ID_FILTER_HQ2X			0x9105
#define	ID_FILTER_LQ2X			0x9106
#define	ID_FILTER_END			0x9106

// チートメニュー
#define	ID_SCH_APPEND			0x9800

// ランチャーメニュー
//
#define	ID_LCH_FOLDER			0xA000
#define	ID_LCH_DISPEDIT			0xA001
#define	ID_LCH_HEADEREDIT		0xA002
#define	ID_LCH_REFRESH			0xA003
#define	ID_LCH_LIST0			0xA010
#define	ID_LCH_LIST1			0xA011
#define	ID_LCH_LIST2			0xA012
#define	ID_LCH_LIST3			0xA013
#define	ID_LCH_LIST4			0xA014
#define	ID_LCH_LIST5			0xA015
#define	ID_LCH_LIST6			0xA016
#define	ID_LCH_LIST7			0xA017
#define	ID_LCH_LIST8			0xA018
#define	ID_LCH_LIST9			0xA019

#endif	// !__VIRTUANES_RESOURCE_INCLUDED__
