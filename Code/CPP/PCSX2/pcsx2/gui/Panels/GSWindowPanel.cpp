﻿/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2010  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"
#include "ConfigurationPanels.h"

using namespace pxSizerFlags;

// --------------------------------------------------------------------------------------
//  GSWindowSetting Implementation
// --------------------------------------------------------------------------------------

Panels::GSWindowSettingsPanel::GSWindowSettingsPanel( wxWindow* parent )
	: BaseApplicableConfigPanel_SpecificConfig( parent )
{
	const wxString aspect_ratio_labels[] =
	{
		_("适合窗口/屏幕"),
		_("标准 (4:3)"),
		_("宽屏 (16:9)")
	};

	m_combo_AspectRatio	= new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		ArraySize(aspect_ratio_labels), aspect_ratio_labels, wxCB_READONLY );

	m_text_WindowWidth	= CreateNumericalTextCtrl( this, 5 );
	m_text_WindowHeight	= CreateNumericalTextCtrl( this, 5 );

	// Linux/Mac note: Exclusive Fullscreen mode is probably Microsoft specific, though
	// I'm not yet 100% certain of that.

	m_check_SizeLock	= new pxCheckBox( this, _("禁用窗口大小重设边框") );
	m_check_HideMouse	= new pxCheckBox( this, _("总是隐藏鼠标光标") );
	m_check_CloseGS		= new pxCheckBox( this, _("挂起时隐藏窗口") );
	m_check_Fullscreen	= new pxCheckBox( this, _("打开时默认全屏模式") );
	m_check_VsyncEnable	= new pxCheckBox( this, _("刷新时等待场同步") );
    m_check_DclickFullscreen = new pxCheckBox( this, _("Double-click Toggles Full-Screen mode") );
	m_check_ExclusiveFS = new pxCheckBox( this, _("使用独占全屏模式(如果可用)") );

	m_check_VsyncEnable->SetToolTip( pxEt( "!ContextTip:Window:Vsync",
		L"Vsync eliminates screen tearing but typically has a big performance hit. "
		L"It usually only applies to fullscreen mode, and may not work with all GS plugins."
	) );

	m_check_HideMouse->SetToolTip( pxEt( "!ContextTip:Window:HideMouse",
		L"Check this to force the mouse cursor invisible inside the GS window; useful if using "
		L"the mouse as a primary control device for gaming.  By default the mouse auto-hides after "
		L"2 seconds of inactivity."
	) );

	m_check_Fullscreen->SetToolTip( pxEt( "!ContextTip:Window:Fullscreen",
		L"Enables automatic mode switch to fullscreen when starting or resuming emulation. "
		L"You can still toggle fullscreen display at any time using alt-enter."
	) );

	m_check_ExclusiveFS->SetToolTip( pxEt( "!ContextTip:Window:FullscreenExclusive",
		L"Fullscreen Exclusive Mode may look better on older CRTs and might be a little faster on older video cards, "
		L"but typically can lead to memory leaks or random crashes when entering/leaving fullscreen mode."
	) );

	m_check_CloseGS->SetToolTip( pxEt( "!ContextTip:Window:HideGS",
		L"Completely closes the often large and bulky GS window when pressing "
		L"ESC or suspending the emulator."
	) );

	// ----------------------------------------------------------------------------
	//  Layout and Positioning

	wxBoxSizer& s_customsize( *new wxBoxSizer( wxHORIZONTAL ) );
	s_customsize	+= m_text_WindowWidth;
	s_customsize	+= Label( L"x" )	| StdExpand();
	s_customsize	+= m_text_WindowHeight;

	wxFlexGridSizer& s_AspectRatio( *new wxFlexGridSizer( 2, StdPadding, StdPadding ) );
	//s_AspectRatio.AddGrowableCol( 0 );
	s_AspectRatio.AddGrowableCol( 1 );

	s_AspectRatio += Label(_("高宽比:"))		| pxMiddle;
	s_AspectRatio += m_combo_AspectRatio			| pxExpand;
	s_AspectRatio += Label(_("自定义窗口大小:"))| pxMiddle;
	s_AspectRatio += s_customsize					| pxAlignRight;

	*this += s_AspectRatio				| StdExpand();
	*this += m_check_SizeLock;
	*this += m_check_HideMouse;
	*this += m_check_CloseGS;
	*this += new wxStaticLine( this )	| StdExpand();

	*this += m_check_Fullscreen;
    *this += m_check_DclickFullscreen;;

	*this += m_check_ExclusiveFS;
	*this += m_check_VsyncEnable;

	wxBoxSizer* centerSizer = new wxBoxSizer( wxVERTICAL );
	*centerSizer += GetSizer()	| pxCenter;
	SetSizer( centerSizer, false );

	AppStatusEvent_OnSettingsApplied();
}

void Panels::GSWindowSettingsPanel::AppStatusEvent_OnSettingsApplied()
{
    ApplyConfigToGui( *g_Conf );
}

void Panels::GSWindowSettingsPanel::ApplyConfigToGui( AppConfig& configToApply, bool manuallyPropagate )
{
	const AppConfig::GSWindowOptions& conf( configToApply.GSWindow );

	m_check_CloseGS		->SetValue( conf.CloseOnEsc );
	m_check_Fullscreen	->SetValue( conf.DefaultToFullscreen );
	m_check_HideMouse	->SetValue( conf.AlwaysHideMouse );
	m_check_SizeLock	->SetValue( conf.DisableResizeBorders );

	m_combo_AspectRatio	->SetSelection( (int)conf.AspectRatio );

	m_check_VsyncEnable	->SetValue( configToApply.EmuOptions.GS.VsyncEnable );

    m_check_DclickFullscreen ->SetValue ( conf.IsToggleFullscreenOnDoubleClick );

	m_text_WindowWidth	->SetValue( wxsFormat( L"%d", conf.WindowSize.GetWidth() ) );
	m_text_WindowHeight	->SetValue( wxsFormat( L"%d", conf.WindowSize.GetHeight() ) );

	m_check_VsyncEnable->Enable(!configToApply.EnablePresets);
}

void Panels::GSWindowSettingsPanel::Apply()
{
	AppConfig::GSWindowOptions& appconf( g_Conf->GSWindow );
	Pcsx2Config::GSOptions& gsconf( g_Conf->EmuOptions.GS );

	appconf.CloseOnEsc				= m_check_CloseGS	->GetValue();
	appconf.DefaultToFullscreen		= m_check_Fullscreen->GetValue();
	appconf.AlwaysHideMouse			= m_check_HideMouse	->GetValue();
	appconf.DisableResizeBorders	= m_check_SizeLock	->GetValue();

	appconf.AspectRatio		= (AspectRatioType)m_combo_AspectRatio->GetSelection();

	gsconf.VsyncEnable		= m_check_VsyncEnable->GetValue();

    appconf.IsToggleFullscreenOnDoubleClick = m_check_DclickFullscreen->GetValue();

	long xr, yr;

	if( !m_text_WindowWidth->GetValue().ToLong( &xr ) || !m_text_WindowHeight->GetValue().ToLong( &yr ) )
		throw Exception::CannotApplySettings( this )
			.SetDiagMsg(L"User submitted non-numeric window size parameters!")
			.SetUserMsg(_("Invalid window dimensions specified: Size cannot contain non-numeric digits! >_<"));

	appconf.WindowSize.x	= xr;
	appconf.WindowSize.y	= yr;
}
