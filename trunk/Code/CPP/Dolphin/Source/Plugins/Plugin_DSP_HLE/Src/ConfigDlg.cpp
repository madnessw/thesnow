// Copyright (C) 2003 Dolphin Project.

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


#include "Config.h"
#include "ConfigDlg.h"

BEGIN_EVENT_TABLE(DSPConfigDialogHLE, wxDialog)
EVT_BUTTON(wxID_OK, DSPConfigDialogHLE::SettingsChanged)
EVT_CHECKBOX(ID_ENABLE_HLE_AUDIO, DSPConfigDialogHLE::SettingsChanged)
EVT_CHECKBOX(ID_ENABLE_DTK_MUSIC, DSPConfigDialogHLE::SettingsChanged)
EVT_CHECKBOX(ID_ENABLE_THROTTLE, DSPConfigDialogHLE::SettingsChanged)
EVT_CHECKBOX(ID_ENABLE_RE0_FIX, DSPConfigDialogHLE::SettingsChanged)
EVT_COMMAND_SCROLL(ID_VOLUME, DSPConfigDialogHLE::VolumeChanged)
END_EVENT_TABLE()

DSPConfigDialogHLE::DSPConfigDialogHLE(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	// Center window
	CenterOnParent();

	m_OK = new wxButton(this, wxID_OK, wxT("ȷ��"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);

	// Create items
	m_buttonEnableHLEAudio = new wxCheckBox(this, ID_ENABLE_HLE_AUDIO, wxT("���� HLE ��Ƶ"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	m_buttonEnableDTKMusic = new wxCheckBox(this, ID_ENABLE_DTK_MUSIC, wxT("���� DTK ����"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	m_buttonEnableThrottle = new wxCheckBox(this, ID_ENABLE_THROTTLE, wxT("���� ��Ƶ Throttle"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	m_buttonEnableRE0Fix = new wxCheckBox(this, ID_ENABLE_RE0_FIX, wxT("���� RE0 ��Ƶ Hack"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	wxStaticText *BackendText = new wxStaticText(this, wxID_ANY, wxT("��Ƶ Backend"), wxDefaultPosition, wxDefaultSize, 0);
	m_BackendSelection = new wxComboBox(this, ID_BACKEND, wxEmptyString, wxDefaultPosition, wxSize(90, 20), wxArrayBackends, wxCB_READONLY, wxDefaultValidator);

	m_volumeSlider = new wxSlider(this, ID_VOLUME, ac_Config.m_Volume, 1, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL|wxSL_INVERSE);
	m_volumeText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%d %%"), ac_Config.m_Volume), wxDefaultPosition, wxDefaultSize, 0);

	// Update values
	m_buttonEnableHLEAudio->SetValue(g_Config.m_EnableHLEAudio ? true : false);
	m_buttonEnableDTKMusic->SetValue(ac_Config.m_EnableDTKMusic ? true : false);
	m_buttonEnableThrottle->SetValue(ac_Config.m_EnableThrottle ? true : false);
	m_buttonEnableRE0Fix->SetValue(g_Config.m_EnableRE0Fix ? true : false);

	// Add tooltips
	m_buttonEnableHLEAudio->SetToolTip(wxT("This is usually used to play voice and sound effects."));
	m_buttonEnableDTKMusic->SetToolTip(wxT("This is used to play music tracks, like BGM."));
	m_buttonEnableThrottle->SetToolTip(wxT("This is used to control game speed by sound throttle.\n")
		wxT("Disabling this could cause abnormal game speed, such as too fast.\n")
		wxT("But sometimes enabling this could cause constant noise.\n")
		wxT("\nKeyboard Shortcut <TAB>:  Hold down to instantly disable Throttle."));
	m_buttonEnableRE0Fix->SetToolTip(wxT("This fixes audio in Resident Evil Zero and maybe some other games."));
	m_BackendSelection->SetToolTip(wxT("Changing this will have no effect while the emulator is running!"));

	// Create sizer and add items to dialog
	wxBoxSizer *sMain = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sSettings = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sBackend = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sButtons = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer *sbSettings = new wxStaticBoxSizer(wxVERTICAL, this, wxT("��������"));
	sbSettings->Add(m_buttonEnableHLEAudio, 0, wxALL, 5);
	sbSettings->Add(m_buttonEnableDTKMusic, 0, wxALL, 5);
	sbSettings->Add(m_buttonEnableThrottle, 0, wxALL, 5);
	sbSettings->Add(m_buttonEnableRE0Fix, 0, wxALL, 5);
	sBackend->Add(BackendText, 0, wxALIGN_CENTER|wxALL, 5);
	sBackend->Add(m_BackendSelection, 0, wxALL, 1);
	sbSettings->Add(sBackend, 0, wxALL, 2);

	wxStaticBoxSizer *sbSettingsV = new wxStaticBoxSizer(wxVERTICAL, this, wxT("����"));
	sbSettingsV->Add(m_volumeSlider, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER, 6);
	sbSettingsV->Add(m_volumeText, 0, wxALL|wxALIGN_LEFT, 4);

	sSettings->Add(sbSettings, 0, wxALL|wxEXPAND, 4);
	sSettings->Add(sbSettingsV, 0, wxALL|wxEXPAND, 4);
	sMain->Add(sSettings, 0, wxALL|wxEXPAND, 4);
	
	sButtons->AddStretchSpacer(); 
	sButtons->Add(m_OK, 0, wxALL, 1);
	sMain->Add(sButtons, 0, wxALL|wxEXPAND, 4);
	SetSizerAndFit(sMain);
}

// Add audio output options
void DSPConfigDialogHLE::AddBackend(const char* backend)
{
	// Update values
    m_BackendSelection->Append(wxString::FromAscii(backend));
#ifdef __APPLE__
	m_BackendSelection->SetValue(wxString::FromAscii(ac_Config.sBackend));
#else
	m_BackendSelection->SetValue(wxString::FromAscii(ac_Config.sBackend.c_str()));
#endif

	// Unfortunately, DSound is the only API having a volume setting...
#ifndef _WIN32
	m_volumeSlider->Disable();
	m_volumeText->Disable();
#endif
}

void DSPConfigDialogHLE::ClearBackends()
{
	m_BackendSelection->Clear();
}

DSPConfigDialogHLE::~DSPConfigDialogHLE()
{
}

void DSPConfigDialogHLE::VolumeChanged(wxScrollEvent& WXUNUSED(event))
{
	ac_Config.m_Volume = m_volumeSlider->GetValue();
	ac_Config.Update();

	m_volumeText->SetLabel(wxString::Format(wxT("%d %%"), m_volumeSlider->GetValue()));
}

void DSPConfigDialogHLE::SettingsChanged(wxCommandEvent& event)
{
	g_Config.m_EnableHLEAudio = m_buttonEnableHLEAudio->GetValue();
	ac_Config.m_EnableDTKMusic = m_buttonEnableDTKMusic->GetValue();
	ac_Config.m_EnableThrottle = m_buttonEnableThrottle->GetValue();
	g_Config.m_EnableRE0Fix = m_buttonEnableRE0Fix->GetValue();

#ifdef __APPLE__
	strncpy(ac_Config.sBackend, m_BackendSelection->GetValue().mb_str(), 128);
#else
	ac_Config.sBackend = m_BackendSelection->GetValue().mb_str();
#endif
	ac_Config.Update();
	g_Config.Save();

	if (event.GetId() == wxID_OK)
		EndModal(wxID_OK);
}