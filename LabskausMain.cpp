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
#include "ECU_VarListElement.h"
#include "serial.h"
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


LabskausFrame::LabskausFrame(wxFrame *frame)
    : GUIFrame(frame)
{
    CCP_Master = new CCP_driver();
    recTimer = NULL;
    data_acquisition_timer = NULL;
    read_last_config();

}

LabskausFrame::~LabskausFrame()
{
}

void LabskausFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void LabskausFrame::OnQuit(wxCommandEvent &event)
{

    // Save the lastest settings and pathes

    std::ofstream logfile("config_last_session.txt");
    if ( ! logfile)
    {
        std::cerr << "Logfile could not be opened" << std::endl;
    }
    logfile << ECU_XML_full_Path << std::endl;
    logfile << ECU_XML_file_dir << std::endl;
    logfile << ECU_XML_file_name << std::endl;
    logfile << LOG_dir << std::endl;
    logfile.close();
    Destroy();
}

void LabskausFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Labskaus"));
}

void LabskausFrame::open_load_dialog(wxCommandEvent &event)
{
    wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a ECU-xml file"), wxEmptyString, wxEmptyString,
		_("XML-files (*.xml)|*.xml"),
		wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		ECU_XML_full_Path = OpenDialog->GetPath();
		ECU_XML_file_dir = OpenDialog->GetDirectory();
		ECU_XML_file_name =OpenDialog->GetFilename();
	}
	else
	{
        std::cout << "XML read canceled;" << std::endl;
        return;
	}
	// Clean up after ourselves
	OpenDialog->Destroy();
    Read_XML_file();
}

void LabskausFrame::Read_XML_file(void)
{
    // remove the content of the last XML file
    XML_list.clear();

	// Disconnect Events
    std::cout << ECU_XML_full_Path << std::endl;
    std::cout << ECU_XML_file_dir << std::endl;
    std::cout << ECU_XML_file_name << std::endl;

    std::ifstream xmlfile(ECU_XML_full_Path);
    if ( ! xmlfile)
    {
        std::cerr << "File measurement_darrieusV1.xml existiert nicht" << std::endl;
        return;
    }

    // Hier kann die Datei gelesen werden. Benutze die Macht
    std::string one_line_from_xml_file;
    std::getline(xmlfile, one_line_from_xml_file);

    if(xmlfile.is_open())
    {
        xmlfile.close();
    }


    /*********************************************************************************

    Start to work with tinyXML2

    *********************************************************************************/
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    doc.LoadFile("measurement_darrieusV1.xml");
    tinyxml2::XMLNode* root = doc.FirstChildElement();
    m_listBox1->Clear();

    // Counter to know how many list elements are available
    int postn = 0;


    if (strcmp(root->Value( ), "measurement_file") != 0) {
        cout << string("bad root: Wrong xml? ") + root->Value( ) << endl;
        }
    else{
        //cout << "Found the element "<< root->Value() <<" in line "<<root->GetLineNum() << endl;
        }
    XMLNode* variable = root->FirstChildElement();
    if (strcmp(variable->Value( ), "variable") != 0) {
        cout << string("bad root: Wrong XML? ") + variable->Value( ) << endl;
        }
    else{
        //cout << "Found the element "<< variable->Value() <<" in line "<<variable->GetLineNum() << endl;
        }
    do{
    //cout << "---------------------" << endl;
        XMLElement* varelement = variable->FirstChildElement();
        const char* var_name;
        const char* var_address;
        const char* var_type;
        do{
            if (!varelement) {
                cout << "No Child found." << endl;
                }
            else{
                const char* str = varelement->GetText();
                if(str){
                    /*cout << "Found the element "<< varelement->Value() <<" in line "<<varelement->GetLineNum() <<
                    " Content: "<< str <<endl;*/

                    if(strcmp(varelement->Value(),"name")== 0)
                        {
                            /*cout << "Name found" << endl;*/
                            var_name = str;
                        }
                    if(strcmp(varelement->Value(),"address")== 0)
                        {
                            /*cout << "Address found" << endl;*/
                            var_address = str;
                        }
                    if(strcmp(varelement->Value(),"datatype")==0)
                        {
                            /*cout << "Datatype found" << endl;*/
                            var_type = str;
                        }
                    }
                else{
                    /* cout << "Found the element "<< varelement->Value() <<" in line "
                    << varelement->GetLineNum() << " has more children" << endl; */
                    }
                }

                varelement = varelement->NextSiblingElement();


        }while(varelement);

        postn = postn +1;
        m_listBox1->Append(var_name);

        ECU_variable* tmp_var_element = new(ECU_variable);
        tmp_var_element->SetName(var_name);
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << var_address;
        ss >> x;
        tmp_var_element->SetAddress(x);
        tmp_var_element->ParseDatatyp(var_type);
        XML_list.push_back(*tmp_var_element);

        variable = variable->NextSiblingElement();
    }while(variable);

}


void LabskausFrame::open_log_dialog(wxCommandEvent &event)
{
    wxDirDialog* OpenDialog = new wxDirDialog(
		this, _("Choose a folder for the log files"), wxEmptyString, wxFD_OPEN,
		 wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		LOG_dir = OpenDialog->GetPath();
		CCP_Master->SetLogFolder(std::string(LOG_dir.mb_str()));

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
        ECU_variable* Ptr2SelectedElement = &XML_list.at(m_listBox1->GetSelection());
        if(!Ptr2SelectedElement)
        {
            std::cerr << "Error the Positon was not found inside the List" << std::endl;
            return;
        }
        std::stringstream stream;
        stream << "Name: " << Ptr2SelectedElement->GetName() << std::endl;
        stream << "Addr: 0x" << std::uppercase << std::hex << Ptr2SelectedElement->GetAddress() << std::endl;
        stream << "Type: " << Ptr2SelectedElement->GetDatatypAsString() << std::endl;
        stream << "Desc: " << std::endl;

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
        recTimer->Start(interval);
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
}




void LabskausFrame::EventAddVar2List(wxCommandEvent &event)
{
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
            CCP_Master->addvariable2ActionPlan(XML_list.at(m_listBox1->GetSelection()));
        }
    }

}

void LabskausFrame::EventAddCalVal2List(wxCommandEvent &event)
{


    if(!XML_list.empty())
    {
        ECU_variable* Ptr2SelectedElement = &XML_list.at(m_listBox1->GetSelection());
        if(!Ptr2SelectedElement)
        {
            std::cerr << "Error the Positon was not found inside the List" << std::endl;
            return;
        }

        LabskausFrameSetCal* SetVar_dia = new LabskausFrameSetCal(CCP_Master,Ptr2SelectedElement);
        SetVar_dia->Show();

        std::stringstream stream;
        stream << "Name: " << Ptr2SelectedElement->GetName() << std::endl;
        SetVar_dia->m_DLVarName->SetLabel(stream.str());
        std::stringstream stream_min; // wird hier nur gemacht da stream nicht gelöscht werden kann
        if(Ptr2SelectedElement->MinValueValid())
        {
            if((Ptr2SelectedElement->GetDataType() == type_f32) ||
               (Ptr2SelectedElement->GetDataType() == type_f64) )
            {
                stream_min << Ptr2SelectedElement->GetMinValue_F();
            }
            else /* Falls keine Gleitkommazahl gelesen werden soll */
            {
                stream_min << Ptr2SelectedElement->GetMinValue_Int();
            }

        }
        else
        {
            stream_min << "unknown";
        }
        SetVar_dia->m_DL_min->SetLabel(stream_min.str());

        std::stringstream stream_max; // wird hier nur gemacht da stream nicht gelöscht werden kann
        if(Ptr2SelectedElement->MaxValueValid())
        {
            if((Ptr2SelectedElement->GetDataType() == type_f32) ||
               (Ptr2SelectedElement->GetDataType() == type_f64) )
            {
                stream_max << Ptr2SelectedElement->GetMaxValue_F();
            }
            else /* Falls keine Gleitkommazahl gelesen werden soll */
            {
                stream_max << Ptr2SelectedElement->GetMaxValue_Int();
            }

        }
        else
        {
            stream_max << "unknown";
        }
        SetVar_dia->m_DL_max->SetLabel(stream_max.str());
    }
}

void LabskausFrame::read_last_config(void)
{
    std::ifstream logfile("config_last_session.txt");
    if ( ! logfile)
    {
        std::cerr << "No latest configuration found" << std::endl;
    }
    std::string tmp;
    std::getline(logfile,tmp);
    wxString wxStr_ECU_XML_full_Path(tmp);
    ECU_XML_full_Path =wxStr_ECU_XML_full_Path;

    std::getline(logfile,tmp);
    wxString wxStr_ECU_XML_file_dir(tmp);
    ECU_XML_file_dir = wxStr_ECU_XML_file_dir;

    std::getline(logfile,tmp);
    wxString wxStr_ECU_XML_file_name(tmp);
    ECU_XML_file_name = wxStr_ECU_XML_file_name;

    std::getline(logfile,tmp);
    wxString wxStr_LOG_dir(tmp);
    LOG_dir = wxStr_LOG_dir;

    CCP_Master->SetLogFolder(std::string(LOG_dir.mb_str()));
    Read_XML_file();
    logfile.close();
}


LabskausFrameSetCal::LabskausFrameSetCal(CCP_driver *ptr_ccp_driver, ECU_variable *ptr_ECU_variable) : Dialog_SetValue(0L)
{
    CCP_Master = ptr_ccp_driver;
    m_ECU_Variable = ptr_ECU_variable;
}


LabskausFrameSetCal::~LabskausFrameSetCal()
{
}

void LabskausFrameSetCal::EventTakeOverVal(wxCommandEvent &event)
{
    // Überprüfe ob das Feld leer ist

    if( m_DL_NewVal->IsEmpty())
    {
        std::cerr << "No new value entered" << std::endl;
        return;
    }
    wxString number = m_DL_NewVal->GetValue();

    // Überprüfe welcher Datentyp benötigt wird

    std::cout << "Value entered:" << number << std::endl;
    long long_value;
    double double_value;
    if(m_ECU_Variable->GetDataType() == type_u8  ||
       m_ECU_Variable->GetDataType() == type_i8  ||
       m_ECU_Variable->GetDataType() == type_u16 ||
       m_ECU_Variable->GetDataType() == type_i16 ||
       m_ECU_Variable->GetDataType() == type_i32 ||
       m_ECU_Variable->GetDataType() == type_u32)
    {
         if(!number.ToLong(&long_value))
        {
            std::cerr << "New value could not be converted to long." << std::endl;
            return;
        }
    }

    if(m_ECU_Variable->GetDataType() == type_f32)
    {
        if(!number.ToDouble(&double_value))
        {
            std::cerr << "New value could not be converted to double." << std::endl;
            return;
        }
    }

    // Rufe die Funktion des CCP Treibers auf
    CCP_Master->addCalibration2ActionPlan(*m_ECU_Variable,(int64_t)long_value,(float)double_value);

}
