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
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	wxMenuItem* menu_file_open;
	menu_file_open = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Open XML") ) , wxT("Open an XML or A2L file as a Basis for the Controler"), wxITEM_NORMAL );
	fileMenu->Append( menu_file_open );
	
	wxMenuItem* menuSelectLogFolder;
	menuSelectLogFolder = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Log folder") ) , wxT("Define the folder where to save the results."), wxITEM_NORMAL );
	fileMenu->Append( menuSelectLogFolder );
	
	mbar->Append( fileMenu, wxT("&File") );
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("&Help") );
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_OpenSerial;
	m_OpenSerial = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Start Measurement") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_OpenSerial );
	
	wxMenuItem* m_CloseSerial;
	m_CloseSerial = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Stop Measurement") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_CloseSerial );
	
	mbar->Append( m_menu3, wxT("Communication") );
	
	this->SetMenuBar( mbar );
	
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_listBox1 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxHSCROLL|wxVSCROLL );
	m_listBox1->Append( wxT("first load the xml file") );
	m_listBox1->SetMinSize( wxSize( 200,300 ) );
	
	bSizer2->Add( m_listBox1, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_VarInfoField = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_VarInfoField->Wrap( -1 );
	m_VarInfoField->SetMinSize( wxSize( 200,100 ) );
	
	bSizer2->Add( m_VarInfoField, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_Add_var2list = new wxButton( this, wxID_ANY, wxT("Variable messen"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_Add_var2list, 0, wxALL, 5 );
	
	m_Add_Cal2List = new wxButton( this, wxID_ANY, wxT("Variable setzten"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_Add_Cal2List, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_MeasList = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_MeasList->CreateGrid( 0, 2 );
	m_MeasList->EnableEditing( false );
	m_MeasList->EnableGridLines( true );
	m_MeasList->SetGridLineColour( wxColour( 190, 190, 190 ) );
	m_MeasList->EnableDragGridSize( false );
	m_MeasList->SetMargins( 0, 0 );
	
	// Columns
	m_MeasList->SetColSize( 0, 345 );
	m_MeasList->SetColSize( 1, 68 );
	m_MeasList->EnableDragColMove( true );
	m_MeasList->EnableDragColSize( true );
	m_MeasList->SetColLabelSize( 20 );
	m_MeasList->SetColLabelValue( 0, wxT("Variable") );
	m_MeasList->SetColLabelValue( 1, wxT("Wert") );
	m_MeasList->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_MeasList->EnableDragRowSize( true );
	m_MeasList->SetRowLabelSize( 20 );
	m_MeasList->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	m_MeasList->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_MeasList->SetLabelTextColour( wxSystemSettings::GetColour( wxSYS_COLOUR_CAPTIONTEXT ) );
	
	// Cell Defaults
	m_MeasList->SetDefaultCellFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Caladea") ) );
	m_MeasList->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer9->Add( m_MeasList, 5, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( bSizer9, 3, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menu_file_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_load_dialog ) );
	this->Connect( menuSelectLogFolder->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_log_dialog ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	this->Connect( m_OpenSerial->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventOpenSerial ) );
	this->Connect( m_CloseSerial->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventCloseSerial ) );
	m_listBox1->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrame::VarListSelected ), NULL, this );
	m_Add_var2list->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddVar2List ), NULL, this );
	m_Add_Cal2List->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddCalVal2List ), NULL, this );
	m_MeasList->Connect( wxEVT_CHAR, wxKeyEventHandler( GUIFrame::EventMeaListKeyPres ), NULL, this );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_load_dialog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::open_log_dialog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventOpenSerial ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::EventCloseSerial ) );
	m_listBox1->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrame::VarListSelected ), NULL, this );
	m_Add_var2list->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddVar2List ), NULL, this );
	m_Add_Cal2List->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::EventAddCalVal2List ), NULL, this );
	m_MeasList->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GUIFrame::EventMeaListKeyPres ), NULL, this );
}

Dialog_SetValue::Dialog_SetValue( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,230 ), wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_DLVarName = new wxStaticText( this, wxID_ANY, wxT("ECU Variablenname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DLVarName->Wrap( -1 );
	bSizer4->Add( m_DLVarName, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer5->Add( m_staticText6, 0, wxALL, 5 );
	
	m_DL_max = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DL_max->Wrap( -1 );
	m_DL_max->SetMinSize( wxSize( 80,-1 ) );
	
	bSizer5->Add( m_DL_max, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer5->Add( m_staticText8, 0, wxALL, 5 );
	
	m_DL_min = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DL_min->Wrap( -1 );
	bSizer5->Add( m_DL_min, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Aktueller Wert:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer7->Add( m_staticText12, 0, wxALL, 5 );
	
	m_DL_Actl = new wxStaticText( this, wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DL_Actl->Wrap( -1 );
	bSizer7->Add( m_DL_Actl, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Neuer Wert"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer8->Add( m_staticText14, 0, wxALL, 5 );
	
	m_DL_NewVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_DL_NewVal, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_BtnConfirm = new wxButton( this, wxID_ANY, wxT("Wert übernehmen"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_BtnConfirm, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer9, 1, wxALIGN_CENTER, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	bSizer4->Fit( this );
	
	// Connect Events
	m_BtnConfirm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_SetValue::EventTakeOverVal ), NULL, this );
}

Dialog_SetValue::~Dialog_SetValue()
{
	// Disconnect Events
	m_BtnConfirm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dialog_SetValue::EventTakeOverVal ), NULL, this );
}
