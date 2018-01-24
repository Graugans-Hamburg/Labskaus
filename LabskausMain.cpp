/***************************************************************
 * Name:      LabskausMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2017-03-01
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include "LabskausMain.h"
#include <fstream>
#include <string>
#include <sstream>
#include <wx/timer.h>
#include <streambuf>


#include "tinyxml2.h"
#include "CCP_driver.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

LabskausFrame::LabskausFrame(wxFrame *frame): GUIFrame(frame)
{
    CCP_Master = new CCP_driver();
    recTimer = NULL;
    data_acquisition_timer = NULL;
    createDefaultDir();
    LoadLastConfig();
    m_MeasList->SetSelectionMode(wxGrid::wxGridSelectRows);
}

LabskausFrame::~LabskausFrame()
{
}

void LabskausFrame::OnClose(wxCloseEvent &event)
{
    SaveLastConfig();
    Destroy();
}

void LabskausFrame::OnQuit(wxCommandEvent &event)
{
    SaveLastConfig();
    Destroy();
}

void LabskausFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = _("Version: Only known for a Release \nGit-Checksum: Only known for a release\n\n Main-developer: Matthias Baumann");

    wxMessageBox(msg, _("Labskaus version information"));
}

void LabskausFrame::OnOpenDocumentation(wxCommandEvent &event)
{
#if defined(__WXMSW__)
    ShellExecute(0, 0, L"overview\\index.html", 0, 0 , SW_SHOW );
#elif defined(__WXMAC__)
        // not defined by now.
#elif defined(__UNIX__)
    system("xdg-open \'./help/overview/index.html\'");
#endif
}

void LabskausFrame::open_load_dialog(wxCommandEvent &event)
{
    if(CCP_Master->Get_MessStatus())
    {
        wxMessageBox(_("XML file can only be changed if the measurment is not active!\n\n First stop the Measurement"),_("Labskaus Information"));
        return;
    }

    wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a ECU-xml file"), wxEmptyString, wxEmptyString,
		_("XML-files (*.xml)|*.xml"),
		wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		ECU_XML_full_Path = OpenDialog->GetPath();
		ECU_XML_file_name = OpenDialog->GetFilename();
	}
	else
	{
        std::cout << "XML read canceled;" << std::endl;
        return;
	}
	// Clean up after ourselves
	OpenDialog->Destroy();
    Read_XML_file(ECU_XML_full_Path);
}

void LabskausFrame::open_log_dialog(wxCommandEvent &event)
{
    wxDirDialog* OpenDialog = new wxDirDialog(
		this, _("Choose a folder for the log files"), wxEmptyString, wxFD_OPEN,
		 wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		wxString wxLOG_dir = OpenDialog->GetPath();
		CCP_Master->SetLogFolder(wxLOG_dir.ToStdString());
		LOG_dir = wxLOG_dir.ToStdString();

	}
	else
	{
        std::cout << "Get log folder canceled;" << std::endl;
        return;
	}

}

void LabskausFrame::VarListSelected(wxCommandEvent &event)
{
    if(!XML_list.empty())
    {
        ECU_VarInfo* Ptr2SelectedElement = &XML_list.at(m_listBox1->GetSelection());
        if(!Ptr2SelectedElement)
        {
            std::cerr << "Error the Positon was not found inside the List" << std::endl;
            return;
        }
        std::stringstream stream;
        std::string tmp_string;
        stream << "Name: " << Ptr2SelectedElement->GetName() << std::endl;
        stream << "Addr: 0x" << std::uppercase << std::hex << Ptr2SelectedElement->GetAddress() << std::endl;
        stream << "Type: " << Ptr2SelectedElement->GetDatatypAsString() << std::endl;
        /* TODO clean up Die Beschreibung wird nicht mit in der GUI angezeigt.
        tmp_string = Ptr2SelectedElement->GetDescription();
        std::cout << tmp_string << std::endl;
        if(!tmp_string.empty())
        {
            stream << "Desc: " << Ptr2SelectedElement->GetDescription() << std::endl;
        }
        */
        m_VarInfoField->SetLabel(stream.str());

    }
}

void LabskausFrame::EventOpenSerial(wxCommandEvent &event)
{
    CCP_Master->open_communication_port();

    if(recTimer)
    {
        std::cerr << "recTimer is already existing" << std::endl;
    }
    else
    {
        recTimer = new wxTimer(this, Rec_Timer);
        recTimer->Start(interval_state_machine_ms);
        this->Connect( recTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler( LabskausFrame::OnRecTimer ) );
    }

    if( data_acquisition_timer)
    {
        std::cerr << "data_acquisition_timer is already existing" << std::endl;
    }
    else
    {
        data_acquisition_timer = new wxTimer(this, wxID_ANY);
        data_acquisition_timer->Start(data_acq_list_timer_ms);
        this->Connect( data_acquisition_timer->GetId(), wxEVT_TIMER, wxTimerEventHandler( LabskausFrame::DA_List_Timer ) );
    }
    CCP_Master->Set_SMT_req_establish_connection();
    CCP_Master->SetMeasurementStartTime();
    CCP_Master->Set_MessStatus2Run();

}

void LabskausFrame::EventCloseSerial(wxCommandEvent &event)
{
    CCP_Master->close_communication_port();

    if(recTimer)
    {
        this->Disconnect(wxID_ANY, wxEVT_TIMER,wxTimerEventHandler( LabskausFrame::OnRecTimer ));
        delete(recTimer);
        recTimer = NULL;
    }
    else
    {
        std::cerr << "There is no Object which could be deleted" << std::endl;
    }

    if(data_acquisition_timer)
    {
        this->Disconnect(wxID_ANY, wxEVT_TIMER,wxTimerEventHandler( LabskausFrame::DA_List_Timer ));
        delete(data_acquisition_timer);
        data_acquisition_timer = NULL;
    }
    else
    {
        std::cerr << "There is no Object which could be deleted" << std::endl;
    }
    CCP_Master->SM_reset_state_machine();
    CCP_Master->VariableLog_export();
    CCP_Master->Messagebuffer_export();
    CCP_Master->Set_MessStatus2Stop();
}

void LabskausFrame::OnRecTimer(wxTimerEvent& event)
{
    CCP_Master->periodic_check();
}

void LabskausFrame::DA_List_Timer(wxTimerEvent& event)
{
    std::string tmp;
    tmp.append("Rx+Tx Frames = ");
    tmp.append(std::to_string(CCP_Master->GetCCPLogSize()));
    tmp.append(" Logged variables: ");
    tmp.append(std::to_string(CCP_Master->log_database.GetNmOfLogVariables()));
    statusBar->SetStatusText(_(tmp), 0);
    statusBar->SetStatusText(wxbuildinfo(short_f), 1);
    updateMeasListValues();
}

void LabskausFrame::updateMeasListValues(void)
{
    std::string lastValue;
    wxString tmp_wxString;
    for(int idx = 0; idx < m_MeasList->GetNumberRows(); idx++)
    {
        tmp_wxString = m_MeasList->GetCellValue(idx,0);
        lastValue = CCP_Master->log_database.LastRecValue(tmp_wxString.ToStdString());
        wxString newwxshit(lastValue);
        m_MeasList->SetCellValue(idx,1,newwxshit);
    }

}

void LabskausFrame::EventAddVar2List(wxCommandEvent &event)
{
        AddVar2List();
}

void LabskausFrame::VarListKeyPressed(wxKeyEvent& event)
{
    wxChar uc = event.GetUnicodeKey();
    if ( uc != ' ' )
    {
        event.Skip();
        return;
    }
    else
    {
        AddVar2List();
    }
}

void LabskausFrame::AddVar2List(void)
{
    std::cout << "Add element to ActionList" << std::endl;
    if(XML_list.empty())
    {
        std::cerr << "There is no variable to log. First load a xml file."<< std::endl;
    }
    else
    {
        if(m_listBox1->GetSelection() == wxNOT_FOUND)
        {
            std::cerr << "A variable should have been added to the measurement list but"
            << "no variable was selected so nothing was added to the list" << std::endl;
        }
        else
        {
            int result;
            result = CCP_Master->addvariable2ActionPlan(XML_list.at(m_listBox1->GetSelection()));
            if(!result)
            {
                m_MeasList->AppendRows(1);
                determine_next_free_row();
                ECU_VarInfo& tmp_ECU_VarInfo = XML_list.at(m_listBox1->GetSelection());
                wxString tmp_wxString(tmp_ECU_VarInfo.GetName());
                wxString tmp2_wxString(tmp_ECU_VarInfo.GetUnit());
                m_MeasList->SetCellValue(m_next_free_row,0,tmp_wxString);
                m_MeasList->SetCellValue(m_next_free_row,2,tmp2_wxString);
            }
        }
    }
}

void LabskausFrame::EventMeaListKeyPres( wxKeyEvent& event )
{
    RmVarElementActiontable();

    wxGridCellCoordsArray selection_cells = m_MeasList->GetSelectedCells();
    std::cout << "Number of selected cells: " << selection_cells.size() << std::endl;
    if(selection_cells.size() > 0)
    {
        std::cout << "Selected Cells: " << selection_cells.GetCount() << std::endl;
        std::cout << "Selected Cells: " << selection_cells.size() << std::endl;
        wxGridCellCoords grid_coordinate;
        grid_coordinate = selection_cells.Item(0);
        std::cout << "Grid coordinate: " << grid_coordinate.GetCol() << std::endl;
        std::cout << "Grid coordinate: " << grid_coordinate.GetRow() << std::endl;
    }
}

void LabskausFrame::EventMeaListKeyUp( wxKeyEvent& event )
{
    // TODO create a ticket a wxWidgets here is something not working correctly.
    std::cout << "Key Down Event" << std::endl;
}

void LabskausFrame::EventDelVar2List( wxCommandEvent &event )
{
    RmVarElementActiontable();
}

void LabskausFrame::RmVarElementActiontable( void)
{
    wxArrayInt selection_rows = m_MeasList->GetSelectedRows();
    if(selection_rows.size() == 1)
    {
        int selected_row = selection_rows.Item(0);
        wxString tmp_xxString = m_MeasList->GetCellValue(selected_row,0);
        std::string var2rm_str = tmp_xxString.ToStdString();
        m_MeasList->DeleteRows(selection_rows.Item(0));
        CCP_Master->rmVariableFromActionPlan(var2rm_str);
    }
}

void LabskausFrame::determine_next_free_row()
{
    wxString analyse_wxstring;
    uint64_t analyse_row = 0;
    do
    {
        analyse_wxstring = m_MeasList->GetCellValue(analyse_row,0);
        m_next_free_row = analyse_row;
        analyse_row++;
    }while(!analyse_wxstring.IsEmpty());
}
