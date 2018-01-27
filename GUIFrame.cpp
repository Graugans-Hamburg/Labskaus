///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 248, 248, 248 ) );
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menu_conf_open;
	menu_conf_open = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Open configuration") ) , wxT("Restore a previous configuration (a measurement setup) which has been saved before."), wxITEM_NORMAL );
	fileMenu->Append( menu_conf_open );
	
	wxMenuItem* menu_conf_save;
	menu_conf_save = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Save configuration") ) , wxT("Restore a previous configuration (a measurement setup) which has been saved before."), wxITEM_NORMAL );
	fileMenu->Append( menu_conf_save );
	
	wxMenuItem* m_separator1;
	m_separator1 = fileMenu->AppendSeparator();
	
	wxMenuItem* menu_file_open;
	menu_file_open = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Open ECU-XML file") ) , wxT("Open an XML or A2L file as a Basis for the Controler"), wxITEM_NORMAL );
	fileMenu->Append( menu_file_open );
	
	wxMenuItem* menuSelectLogFolder;
	menuSelectLogFolder = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Log folder") ) , wxT("Define the folder where to save the results."), wxITEM_NORMAL );
	fileMenu->Append( menuSelectLogFolder );
	
	wxMenuItem* m_separator2;
	m_separator2 = fileMenu->AppendSeparator();
	
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("&File") );
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_OpenSerial;
	m_OpenSerial = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Start Measurement") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_OpenSerial );
	
	wxMenuItem* m_CloseSerial;
	m_CloseSerial = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Stop Measurement") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_CloseSerial );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_menu3->AppendSeparator();
	
	wxMenuItem* m_ComSettings;
	m_ComSettings = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Settings") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_ComSettings );
	
	mbar->Append( m_menu3, wxT("Communication") );
	
	helpMenu = new wxMenu();
	wxMenuItem* menuIOpenDocumentation;
	menuIOpenDocumentation = new wxMenuItem( helpMenu, idMenuHelp, wxString( wxT("Documentation") ) , wxT("Open Documentation"), wxITEM_NORMAL );
	helpMenu->Append( menuIOpenDocumentation );
	
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("&Help") );
	
	this->SetMenuBar( mbar );
	
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	statusBar->SetBackgroundColour( wxColour( 229, 229, 229 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("Variable List"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText26->Wrap( -1 );
	m_staticText26->SetFont( wxFont( 12, 70, 90, 92, false, wxEmptyString ) );
	
	bSizer2->Add( m_staticText26, 0, wxALL, 5 );
	
	m_listBox1 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxHSCROLL|wxVSCROLL );
	m_listBox1->Append( wxT("first load the xml file") );
	m_listBox1->SetMinSize( wxSize( -1,100 ) );
	
	bSizer2->Add( m_listBox1, 10, wxALL|wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText261 = new wxStaticText( this, wxID_ANY, wxT("Variable Information"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText261->Wrap( -1 );
	m_staticText261->SetFont( wxFont( 12, 70, 90, 92, false, wxEmptyString ) );
	
	bSizer2->Add( m_staticText261, 0, wxALL, 5 );
	
	m_VarInfoField = new wxStaticText( this, wxID_ANY, wxT("Name: \nAddress: \nDatatype:\nDescription:"), wxDefaultPosition, wxSize( -1,68 ), 0 );
	m_VarInfoField->Wrap( -1 );
	bSizer2->Add( m_VarInfoField, 0, wxALL|wxEXPAND, 5 );
	
	m_staticDescription = new wxTextCtrl( this, wxID_ANY, wxT("select a variable to see the description"), wxDefaultPosition, wxSize( -1,-1 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_READONLY );
	m_staticDescription->SetBackgroundColour( wxColour( 248, 248, 248 ) );
	
	bSizer2->Add( m_staticDescription, 4, wxALL|wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText2611 = new wxStaticText( this, wxID_ANY, wxT("Actions "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText2611->Wrap( -1 );
	m_staticText2611->SetFont( wxFont( 12, 70, 90, 92, false, wxEmptyString ) );
	
	bSizer2->Add( m_staticText2611, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_Add_var2list = new wxButton( this, wxID_ANY, wxT("Variable +"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_Add_var2list, 0, wxALL, 5 );
	
	m_Del_var2list = new wxButton( this, wxID_ANY, wxT("Variable -"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_Del_var2list, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_Add_Cal2List = new wxButton( this, wxID_ANY, wxT("Variable o"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_Add_Cal2List, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer31, 0, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer2, 2, wxEXPAND, 5 );
	
	m_staticline10 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer1->Add( m_staticline10, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_VarInfoField11 = new wxStaticText( this, wxID_ANY, wxT("Measurement List"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_VarInfoField11->Wrap( -1 );
	m_VarInfoField11->SetFont( wxFont( 12, 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9->Add( m_VarInfoField11, 0, wxALL, 5 );
	
	m_MeasList = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_MeasList->CreateGrid( 0, 3 );
	m_MeasList->EnableEditing( false );
	m_MeasList->EnableGridLines( true );
	m_MeasList->SetGridLineColour( wxColour( 190, 190, 190 ) );
	m_MeasList->EnableDragGridSize( false );
	m_MeasList->SetMargins( 0, 0 );
	
	// Columns
	m_MeasList->SetColSize( 0, 334 );
	m_MeasList->SetColSize( 1, 133 );
	m_MeasList->SetColSize( 2, 80 );
	m_MeasList->EnableDragColMove( true );
	m_MeasList->EnableDragColSize( true );
	m_MeasList->SetColLabelSize( 20 );
	m_MeasList->SetColLabelValue( 0, wxT("Variable") );
	m_MeasList->SetColLabelValue( 1, wxT("Value") );
	m_MeasList->SetColLabelValue( 2, wxT("Unit") );
	m_MeasList->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_MeasList->EnableDragRowSize( true );
	m_MeasList->SetRowLabelSize( 20 );
	m_MeasList->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	m_MeasList->SetLabelBackgroundColour( wxColour( 248, 248, 248 ) );
	m_MeasList->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_MeasList->SetLabelTextColour( wxColour( 64, 64, 64 ) );
	
	// Cell Defaults
	m_MeasList->SetDefaultCellFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Caladea") ) );
	m_MeasList->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer9->Add( m_MeasList, 5, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( bSizer9, 4, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( menu_conf_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_load_config_dialog ) );
	this->Connect( menu_conf_save->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_save_config_dialog ) );
	this->Connect( menu_file_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_load_dialog ) );
	this->Connect( menuSelectLogFolder->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_log_dialog ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( m_OpenSerial->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventOpenSerial ) );
	this->Connect( m_CloseSerial->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventCloseSerial ) );
	this->Connect( m_ComSettings->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventOpenComSettings ) );
	this->Connect( menuIOpenDocumentation->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnOpenDocumentation ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	m_listBox1->Connect( wxEVT_CHAR, wxKeyEventHandler( GUIFrame::VarListKeyPressed ), NULL, this );
	m_listBox1->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrame::VarListSelected ), NULL, this );
	m_Add_var2list->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddVar2List ), NULL, this );
	m_Del_var2list->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventDelVar2List ), NULL, this );
	m_Add_Cal2List->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddCalVal2List ), NULL, this );
	m_MeasList->Connect( wxEVT_CHAR, wxKeyEventHandler( GUIFrame::EventMeaListKeyPres ), NULL, this );
	m_MeasList->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( GUIFrame::EventMeasListKeyDn ), NULL, this );
	m_MeasList->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GUIFrame::EventMeasListKeyUp ), NULL, this );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_load_config_dialog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_save_config_dialog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_load_dialog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_log_dialog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventOpenSerial ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventCloseSerial ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventOpenComSettings ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnOpenDocumentation ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	m_listBox1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GUIFrame::VarListKeyPressed ), NULL, this );
	m_listBox1->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrame::VarListSelected ), NULL, this );
	m_Add_var2list->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddVar2List ), NULL, this );
	m_Del_var2list->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventDelVar2List ), NULL, this );
	m_Add_Cal2List->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddCalVal2List ), NULL, this );
	m_MeasList->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GUIFrame::EventMeaListKeyPres ), NULL, this );
	m_MeasList->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( GUIFrame::EventMeasListKeyDn ), NULL, this );
	m_MeasList->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GUIFrame::EventMeasListKeyUp ), NULL, this );
}

Dialog_SetValue::Dialog_SetValue( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,400 ), wxSize( -1,-1 ) );
	this->SetBackgroundColour( wxColour( 248, 248, 248 ) );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Variable:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	m_staticText25->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer24->Add( m_staticText25, 0, wxALL, 5 );
	
	m_DLVarName = new wxStaticText( this, wxID_ANY, wxT("ECU Variablenname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DLVarName->Wrap( -1 );
	m_DLVarName->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer24->Add( m_DLVarName, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer24, 0, wxEXPAND, 5 );
	
	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer4->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, wxT("Unit:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	m_staticText20->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer21->Add( m_staticText20, 0, wxALL, 5 );
	
	m_staticUnit1 = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticUnit1->Wrap( -1 );
	bSizer21->Add( m_staticUnit1, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer21, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Min:"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText8->Wrap( -1 );
	bSizer5->Add( m_staticText8, 0, wxALL, 5 );
	
	m_DL_min = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DL_min->Wrap( -1 );
	bSizer5->Add( m_DL_min, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer5, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer22->Add( m_staticText6, 0, wxALL, 5 );
	
	m_DL_max = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DL_max->Wrap( -1 );
	m_DL_max->SetMinSize( wxSize( 80,-1 ) );
	
	bSizer22->Add( m_DL_max, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText22 = new wxStaticText( this, wxID_ANY, wxT("Datatype:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	m_staticText22->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer23->Add( m_staticText22, 0, wxALL, 5 );
	
	m_staticDataType = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticDataType->Wrap( -1 );
	bSizer23->Add( m_staticDataType, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer23, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer7->Add( m_staticText12, 0, wxALL, 5 );
	
	m_staticDescription = new wxTextCtrl( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxSize( 350,210 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_READONLY );
	bSizer7->Add( m_staticDescription, 0, wxALIGN_TOP|wxALL|wxEXPAND, 5 );
	
	bSizer4->Add( bSizer7, 0, wxEXPAND, 5 );
	
	m_staticline10 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_staticline10->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer4->Add( m_staticline10, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("New Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	m_staticText14->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer8->Add( m_staticText14, 0, wxALL, 5 );
	
	m_DL_NewVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_DL_NewVal, 0, wxALL, 5 );
	
	m_staticUnit = new wxStaticText( this, wxID_ANY, wxT("Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticUnit->Wrap( -1 );
	bSizer8->Add( m_staticUnit, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer8, 0, wxEXPAND, 5 );
	
	m_staticline11 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer4->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_buttonCancel, 0, wxALL, 5 );
	
	m_BtnConfirm = new wxButton( this, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_BtnConfirm, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer9, 0, wxALIGN_CENTER|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	// Connect Events
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_SetValue::event_CancelCalibration ), NULL, this );
	m_BtnConfirm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_SetValue::EventTakeOverVal ), NULL, this );
}

Dialog_SetValue::~Dialog_SetValue()
{
	// Disconnect Events
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_SetValue::event_CancelCalibration ), NULL, this );
	m_BtnConfirm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_SetValue::EventTakeOverVal ), NULL, this );
}

DialogSettings::DialogSettings( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 700,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 248, 248, 248 ) );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap3 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("pic/CommunicationSettings.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_bitmap3, 1, wxALL, 5 );
	
	bSizer11->Add( bSizer12, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer14->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("ECU_Information"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer14->Add( m_staticText10, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("ECU Byte Order"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText13->Wrap( -1 );
	bSizer35->Add( m_staticText13, 0, wxALL, 5 );
	
	wxString m_choiceECUByteOrderChoices[] = { wxT("Little Endian (intel)"), wxT("Big Endian (motorola)") };
	int m_choiceECUByteOrderNChoices = sizeof( m_choiceECUByteOrderChoices ) / sizeof( wxString );
	m_choiceECUByteOrder = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceECUByteOrderNChoices, m_choiceECUByteOrderChoices, 0 );
	m_choiceECUByteOrder->SetSelection( 0 );
	m_choiceECUByteOrder->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer35->Add( m_choiceECUByteOrder, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer35, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Station Address"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText14->Wrap( -1 );
	bSizer36->Add( m_staticText14, 0, wxALL, 5 );
	
	m_textECUStationAddress = new wxTextCtrl( this, wxID_ANY, wxT("unkown"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_textECUStationAddress->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer36->Add( m_textECUStationAddress, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer36, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer14->Add( bSizer15, 1, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer14->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Interface"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	m_staticText11->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer14->Add( m_staticText11, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("Interface"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText15->Wrap( -1 );
	bSizer34->Add( m_staticText15, 0, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("UART"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer34->Add( m_staticText16, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer34, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Port Number"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText17->Wrap( -1 );
	bSizer33->Add( m_staticText17, 0, wxALL, 5 );
	
	m_textDevice = new wxTextCtrl( this, wxID_ANY, wxT("unkown"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_textDevice->SetToolTip( wxT("0: ttyS0 or COM1\n1: ttyS1 or COM2\nfor everything else:\ntake a look at the documentation") );
	m_textDevice->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer33->Add( m_textDevice, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer33, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer341;
	bSizer341 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText151 = new wxStaticText( this, wxID_ANY, wxT("Baudrate"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText151->Wrap( -1 );
	bSizer341->Add( m_staticText151, 0, wxALL, 5 );
	
	m_textBaudRate = new wxTextCtrl( this, wxID_ANY, wxT("unkown"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_textBaudRate->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer341->Add( m_textBaudRate, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer341, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer342;
	bSizer342 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText152 = new wxStaticText( this, wxID_ANY, wxT("Mode"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText152->Wrap( -1 );
	bSizer342->Add( m_staticText152, 0, wxALL, 5 );
	
	m_textSerialMode = new wxTextCtrl( this, wxID_ANY, wxT("unkown"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_textSerialMode->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer342->Add( m_textSerialMode, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer342, 1, wxEXPAND, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer14->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Protocol"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer14->Add( m_staticText12, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("Startbyte"), wxDefaultPosition, wxSize( 120,-1 ), 0 );
	m_staticText18->Wrap( -1 );
	bSizer37->Add( m_staticText18, 0, wxALL, 5 );
	
	wxString m_choiceStartByteChoices[] = { wxT("0xB0") };
	int m_choiceStartByteNChoices = sizeof( m_choiceStartByteChoices ) / sizeof( wxString );
	m_choiceStartByte = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceStartByteNChoices, m_choiceStartByteChoices, 0 );
	m_choiceStartByte->SetSelection( 0 );
	m_choiceStartByte->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer37->Add( m_choiceStartByte, 0, wxALL, 5 );
	
	bSizer14->Add( bSizer37, 1, wxEXPAND, 5 );
	
	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_staticline6->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer14->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( m_buttonCancel, 0, wxALIGN_RIGHT|wxALL|wxRIGHT, 5 );
	
	m_buttonApply = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( m_buttonApply, 0, wxALIGN_RIGHT|wxALL|wxRIGHT, 5 );
	
	bSizer14->Add( bSizer38, 1, wxALIGN_RIGHT, 5 );
	
	bSizer11->Add( bSizer14, 3, wxEXPAND, 5 );
	
	this->SetSizer( bSizer11 );
	this->Layout();
	
	// Connect Events
	m_choiceECUByteOrder->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogSettings::event_ChangeByteOrder ), NULL, this );
	m_textECUStationAddress->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeStationAddress ), NULL, this );
	m_textDevice->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeSerialDevice ), NULL, this );
	m_textBaudRate->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeSerialBaudRate ), NULL, this );
	m_textSerialMode->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeSerialMode ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogSettings::event_Cancel ), NULL, this );
	m_buttonApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogSettings::event_Apply ), NULL, this );
}

DialogSettings::~DialogSettings()
{
	// Disconnect Events
	m_choiceECUByteOrder->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogSettings::event_ChangeByteOrder ), NULL, this );
	m_textECUStationAddress->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeStationAddress ), NULL, this );
	m_textDevice->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeSerialDevice ), NULL, this );
	m_textBaudRate->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeSerialBaudRate ), NULL, this );
	m_textSerialMode->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DialogSettings::event_ChangeSerialMode ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogSettings::event_Cancel ), NULL, this );
	m_buttonApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogSettings::event_Apply ), NULL, this );
}
