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
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);
    statusBar->SetStatusText(wxbuildinfo(short_f), 1);
#endif

    MatzeListe = new ECU_VarListElement();
    MatzeListe->SetnxtVarListElement(NULL);
    MatzeListe->SetpreVarListElement(NULL);
    SerialPort = new serial();
    CCP_Master = new CCP_driver();
    recTimer = NULL;

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
    Destroy();
}

void LabskausFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to Tines Demonstator"));
}

void LabskausFrame::open_load_dialog(wxCommandEvent &event)
{
	// Disconnect Events
    std::cout << "Gost awoken" << std::endl;

    std::ifstream xmlfile("measurement_darrieusV1.xml");
    if ( ! xmlfile)
    {
        std::cerr << "File measurement_darrieusV1.xml existiert nicht" << std::endl;
        return;
    }

    // Hier kann die Datei gelesen werden. Benutze die Macht
    std::string one_line_from_xml_file;
    std::getline(xmlfile, one_line_from_xml_file);

    std::cout << "Line: " << one_line_from_xml_file << std::endl;

    if(xmlfile.is_open())
    {
        xmlfile.close();
    }

    // Probiere mal eine neues item hinzuzufügen




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
        cout << "Found the element "<< root->Value() <<" in line "<<root->GetLineNum() << endl;
        }
    XMLNode* variable = root->FirstChildElement();
    if (strcmp(variable->Value( ), "variable") != 0) {
        cout << string("bad root: Wrong XML? ") + variable->Value( ) << endl;
        }
    else{
        cout << "Found the element "<< variable->Value() <<" in line "<<variable->GetLineNum() << endl;
        }
    do{
    cout << "---------------------" << endl;
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
                    cout << "Found the element "<< varelement->Value() <<" in line "<<varelement->GetLineNum() <<
                    " Content: "<< str <<endl;

                    if(strcmp(varelement->Value(),"name")== 0)
                        {
                            cout << "Name found" << endl;
                            var_name = str;
                        }
                    if(strcmp(varelement->Value(),"address")== 0)
                        {
                            cout << "Address found" << endl;
                            var_address = str;
                        }
                    if(strcmp(varelement->Value(),"datatype")==0)
                        {
                            cout << "Datatype found" << endl;
                            var_type = str;
                        }
                    }
                else{
                    cout << "Found the element "<< varelement->Value() <<" in line "<<varelement->GetLineNum() << " has more children" << endl;
                    }
                }

                varelement = varelement->NextSiblingElement();


        }while(varelement);

        postn = postn +1;
        m_listBox1->Append(var_name);

        ECU_variable tmp_var_element;
        tmp_var_element.SetName(var_name);
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << var_address;
        ss >> x;
        tmp_var_element.SetAddress(x);
        tmp_var_element.ParseDatatyp(var_type);
        MatzeListe->addListElement(&tmp_var_element);

        variable = variable->NextSiblingElement();
    }while(variable);

    ECU_VarListElement* tmpptr = MatzeListe;


    do{
        cout << "ENr: " << tmpptr->GetPostnListElemnt() << "     Variable Name: ";
        cout << tmpptr->GetName() << endl;
        tmpptr = tmpptr->GetnxtVarListElement();
    }while(tmpptr != NULL);

}


void LabskausFrame::VarListSelected(wxCommandEvent &event)
{
    ECU_VarListElement* Ptr2SelectedElement = MatzeListe->getPtr2ListElement(m_listBox1->GetSelection()+1);
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

void LabskausFrame::EventOpenSerial(wxCommandEvent &event)
{
    SerialPort->open_port(4); //TODO Number inside the code

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

}

void LabskausFrame::EventCloseSerial(wxCommandEvent &event)
{
    SerialPort->close_port();

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


}


void LabskausFrame::OnRecTimer(wxTimerEvent& event)
{

    SerialPort->AnalyzeBytesRead();
}

void LabskausFrame::EventStartMea(wxCommandEvent &event)
{
    CCP_Master->Connect(SerialPort);

}
