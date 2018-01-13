///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFrame__
#define __GUIFrame__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/listbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuHelp 1001
#define idMenuAbout 1002

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* m_menu3;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxListBox* m_listBox1;
		wxStaticLine* m_staticline1;
		wxStaticText* m_VarInfoField;
		wxStaticLine* m_staticline2;
		wxButton* m_Add_var2list;
		wxButton* m_Del_var2list;
		wxButton* m_Add_Cal2List;
		wxGrid* m_MeasList;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void open_load_config_dialog( wxCommandEvent& event ) { event.Skip(); }
		virtual void open_save_config_dialog( wxCommandEvent& event ) { event.Skip(); }
		virtual void open_load_dialog( wxCommandEvent& event ) { event.Skip(); }
		virtual void open_log_dialog( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventOpenSerial( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventCloseSerial( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventOpenComSettings( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpenDocumentation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void VarListKeyPressed( wxKeyEvent& event ) { event.Skip(); }
		virtual void VarListSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventAddVar2List( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventDelVar2List( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventAddCalVal2List( wxCommandEvent& event ) { event.Skip(); }
		virtual void EventMeaListKeyPres( wxKeyEvent& event ) { event.Skip(); }
		virtual void EventMeasListKeyDn( wxKeyEvent& event ) { event.Skip(); }
		virtual void EventMeasListKeyUp( wxKeyEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Labskaus"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 850,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUIFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Dialog_SetValue
///////////////////////////////////////////////////////////////////////////////
class Dialog_SetValue : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_DL_NewVal;
		wxButton* m_BtnConfirm;
		
		// Virtual event handlers, overide them in your derived class
		virtual void EventTakeOverVal( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticText* m_DLVarName;
		wxStaticText* m_DL_max;
		wxStaticText* m_DL_min;
		wxStaticText* m_DL_Actl;
		
		Dialog_SetValue( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Set value"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~Dialog_SetValue();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogSettings
///////////////////////////////////////////////////////////////////////////////
class DialogSettings : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap3;
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText10;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_textCtrl2;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText15;
		wxStaticText* m_staticText16;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_textCtrl3;
		wxStaticLine* m_staticline5;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText18;
		wxTextCtrl* m_textCtrl4;
	
	public:
		wxChoice* m_choiceECUByteOrder;
		
		DialogSettings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Communication Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,600 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogSettings();
	
};

#endif //__GUIFrame__
