#include "LabskausMain.h"
#include <fstream>
#include <string>
#include <sstream>
#include <wx/timer.h>
#include <streambuf>
#include <wx/wx.h>


#include "../tinyxml2/tinyxml2.h"
#include "../ccp_driver/CCP_driver.h"


LabskausFrameSetCal::LabskausFrameSetCal(CCP_driver *ptr_ccp_driver, ECU_VarInfo *ptr_ECU_VarInfo) : Dialog_SetValue(0L)
{
    CCP_Master = ptr_ccp_driver;
    m_ECU_Variable = ptr_ECU_VarInfo;
}


LabskausFrameSetCal::~LabskausFrameSetCal()
{
}

/*******************************************************************************************
 * Function: This event is called when the user presses the Cancel button. The dialog will be
 *           closed and no variables will be calibrated.
 ******************************************************************************************/
void LabskausFrameSetCal::event_CancelCalibration( wxCommandEvent& event )
{
    this->Close(true);
}

/*******************************************************************************************
 * Function: This event is called when the user presses the button to close the window.
 *           The dialog will be closed and no variables will be calibrated.
 ******************************************************************************************/
void LabskausFrameSetCal::OnClose(wxCloseEvent& event)
{
    this->Destroy();
}

/*******************************************************************************************
 * Function: This event is called when the user presses the Set button. The input will be checked
 *           and if it is valid a calibration command will be send to the ECU to change the
 *           requested value.
 ******************************************************************************************/
void LabskausFrameSetCal::EventTakeOverVal(wxCommandEvent &event)
{
    // Check if the value had been entered

    if( m_DL_NewVal->IsEmpty())
    {
         wxMessageBox(_("A value can only be changed if you enter one, really."),_("Why?"));
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
            wxMessageBox(_("Please enter a integer."),_("Why?"));
            std::cerr << "New value could not be converted to long." << std::endl;
            return;
        }
    }

    if(m_ECU_Variable->GetDataType() == type_f32)
    {
        if(!number.ToDouble(&double_value))
        {
            wxMessageBox(_("Value could not be converted to float.\n\n Format example: 3.1415"),_("Why?"));
            std::cerr << "New value could not be converted to double." << std::endl;
            return;
        }
    }

    // Rufe die Funktion des CCP Treibers auf
    CCP_Master->addCalibration2ActionPlan(*m_ECU_Variable,(int64_t)long_value,(float)double_value);
}

/*******************************************************************************************
 * Function: This event is called when the user presses the Calibration button (o).
 *           The function read out all properties of the variable that shall be changed and
 *           display them in the new dialog.
 ******************************************************************************************/
void LabskausFrame::EventAddCalVal2List(wxCommandEvent &event)
{
    if(!XML_list.empty())
    {
        if(m_listBox1->GetSelection() == wxNOT_FOUND)
        {
            wxMessageBox(_("Select a variable from the left table."),_("What did you do?"));
            std::cerr << "No element from the variable list was selected" << std::endl;
            return;
        }
        ECU_VarInfo* Ptr2SelectedElement = &XML_list.at(m_listBox1->GetSelection());
        if(!Ptr2SelectedElement)
        {
            wxMessageBox(_("Error the Positon was not found inside the List."),_("Error"));
            std::cerr << "Error the Positon was not found inside the List" << std::endl;
            return;
        }

        LabskausFrameSetCal* SetVar_dia = new LabskausFrameSetCal(CCP_Master,Ptr2SelectedElement);
        SetVar_dia->Show();
        // Define the name
        {
            std::stringstream stream;
            stream << Ptr2SelectedElement->GetName();
            SetVar_dia->m_DLVarName->SetLabel(stream.str());
        }
        // Define the min value
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
            stream_min << "N/A";
        }
        SetVar_dia->m_DL_min->SetLabel(stream_min.str());

        // Define the max value
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
            stream_max << "N/A";
        }
        SetVar_dia->m_DL_max->SetLabel(stream_max.str());

        // Define the unit
        {
            std::stringstream stream;
            stream << Ptr2SelectedElement->GetUnit() << std::endl;
            std::string str_tmp;
            str_tmp = Ptr2SelectedElement->GetUnit();

            if(str_tmp.empty())
            {
                SetVar_dia->m_staticUnit1->SetLabel("N/A");
            }
            else
            {
                SetVar_dia->m_staticUnit1->SetLabel(stream.str());

            }
            SetVar_dia->m_staticUnit->SetLabel(stream.str());
        }

        // Define the datatype
        {
            std::stringstream stream;
            switch(Ptr2SelectedElement->GetDataType())
            {
            case type_unknown : stream << "N/A"    ; break;
            case type_u8      : stream << "uint8"  ; break;
            case type_i8      : stream << "int8"   ; break;
            case type_u16     : stream << "uint16" ; break;
            case type_i16     : stream << "int16"  ; break;
            case type_u32     : stream << "uint32" ; break;
            case type_i32     : stream << "int32"  ; break;
            case type_f32     : stream << "float32"; break;
            case type_f64     : stream << "float64"; break;
            default           : stream << "N/A"    ; break;
            }

            SetVar_dia->m_staticDataType->SetLabel(stream.str());
        }
        // Take over the description
        // Define the unit
        {
            std::stringstream stream;
            stream << Ptr2SelectedElement->GetDescription() << std::endl;
            std::string str_tmp;
            str_tmp = Ptr2SelectedElement->GetDescription();

            if(str_tmp.empty())
            {
                SetVar_dia->m_staticDescription->SetLabel("N/A");
            }
            else
            {
                SetVar_dia->m_staticDescription->SetValue(stream.str());
            }
        }
    }
}
