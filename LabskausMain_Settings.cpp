#include "LabskausMain.h"
#include <fstream>
#include <string>
#include <sstream>
#include <wx/timer.h>
#include <streambuf>
#include <wx/wx.h>


#include "tinyxml2.h"
#include "CCP_driver.h"
/*******************************************************************************************
 * Function: This event is called when the user clicks on the menu settings. Function will
 *           create the new Diaglog and initial the values and text fields used inside the
 *           the Diaglog.
 ******************************************************************************************/
void LabskausFrame::EventOpenComSettings( wxCommandEvent& event )
{
    if(CCP_Master->Get_MessStatus())
    {
        wxMessageBox(_("The Settings can only be changed if the measurment is not active.\n\n First stop the Measurement"),_("Labskaus Information"));
        return;
    }
    LabskausFrameSettings* Settings_dia = new LabskausFrameSettings(CCP_Master);
    /**
     *   Show the current ECU Byte Order
     **/

        endian    ECU_byte_order = CCP_Master->Get_ECU_endianness();
        Settings_dia->newECUByteOrder = ECU_byte_order;
        if(ECU_byte_order == little_endian) Settings_dia->m_choiceECUByteOrder->SetSelection(0);
        if(ECU_byte_order == big_endian)    Settings_dia->m_choiceECUByteOrder->SetSelection(1);
    /**
     *   Show the current ECU Station Address
     **/
    {
        uint16_t ECU_Station_Address = CCP_Master->Get_ECU_station_address();
        Settings_dia->newStationAddress = ECU_Station_Address;
        std::stringstream stream;
        stream << ECU_Station_Address;
        Settings_dia->m_textECUStationAddress->SetValue(stream.str());
     }
     /**
     *   Show the device that is configure inside the serial objekt
     **/
     {
        int Serial_Port = CCP_Master->SerialPort.Get_port_number();
        Settings_dia->newSerialPort = Serial_Port;
        std::stringstream stream;
        stream << Serial_Port;
        Settings_dia->m_textDevice->SetValue(stream.str());
     }
    /**
     *   Show the Baud Rate that is configure inside the serial objekt
     **/
     {
        int BaudRate = CCP_Master->SerialPort.Get_baud_rate();
        Settings_dia->newSerialBaudRate = BaudRate;
        std::stringstream stream;
        stream << BaudRate;
        Settings_dia->m_textBaudRate->SetValue(stream.str());
     }
    /**
     *   Show the SerialMode that is configure inside the serial objekt
     **/
     {
        std::string SerialMode = CCP_Master->SerialPort.Get_serial_mode();
        Settings_dia->newSerialMode = SerialMode;
        std::stringstream stream;
        stream << SerialMode;
        Settings_dia->m_textSerialMode->SetValue(stream.str());
     }
     /**
     *   Show the StartByte that is configure inside the serial objekt
     **/
     {
        StartByteEnum ConfiguredStartByte = CCP_Master->SerialPort.Get_StartByte();
        Settings_dia->newStartByte = ConfiguredStartByte;
        if(ConfiguredStartByte == no_startbyte) Settings_dia->m_choiceStartByte->SetSelection(0);
        if(ConfiguredStartByte == Char0xB0)     Settings_dia->m_choiceStartByte->SetSelection(1);
     }
    Settings_dia->Show();
}

LabskausFrameSettings::LabskausFrameSettings(CCP_driver *ptr_ccp_driver) : DialogSettings(0L)
{
    CCP_Master = ptr_ccp_driver;
}


LabskausFrameSettings::~LabskausFrameSettings()
{
}
/*******************************************************************************************
 * Function: This event is called when the user changes the byte order inside the Settings
 *           Dialog. The value will be stored inside a temp variable which will be take over
 *           if the new value is applied
 *
 *           The changed flag need to be set if a valid change had been entered.
 ******************************************************************************************/
void LabskausFrameSettings::event_ChangeByteOrder( wxCommandEvent& event )
{
    newECUByteOrder = (endian)m_choiceECUByteOrder->GetSelection();
    std::cout << "Value changed to " << newECUByteOrder << std::endl;
    ByteOrder_changed = true;
}

/*******************************************************************************************
 * Function: This event is called when the user changes the Start Byte inside the Settings
 *           Dialog. The value will be stored inside a temp variable which will be take over
 *           if the new value is applied
 *
 *           The changed flag need to be set if a valid change had been entered.
 ******************************************************************************************/
void LabskausFrameSettings::event_ChangeStartByte( wxCommandEvent& event )
{
    newStartByte = (StartByteEnum)m_choiceStartByte->GetSelection();
    std::cout << "Value changed to " << newStartByte << std::endl;
    StartByte_changed = true;
}
/*******************************************************************************************
 * Function: This event is called when the user changes the ecu address inside the Settings
 *           Dialog. The value will be stored inside a temp variable which will be take over
 *           if the new value is applied
 *
 *           The value will be taken over from the text field. Therefore it need to be
 *           if the value which is entered is a valid number with can be stored inside a
 *           uint16_t.
 *
 *           If any invalid number had been entered, the text field need to be set back to the
 *           station address the is currently used by the CCP-Master. This value also need to be
 *           stored inside the tmp variable which might be taken over if the user click on the
 *           apply button.
 *
 *           The changed flag need to be set if a valid change had been entered.
 ******************************************************************************************/
void LabskausFrameSettings::event_ChangeStationAddress( wxCommandEvent& event )
{
    if( m_textECUStationAddress->IsEmpty())
    {
        wxMessageBox(_("Please enter a integer in the range of [0 - 65535]."),_("What did you do?"));
        std::cerr << "No new value entered" << std::endl;
        return;
    }
    wxString user_input_as_string = m_textECUStationAddress->GetValue();
    long entered_station_address;
    if(!user_input_as_string.ToLong(&entered_station_address))
    {
        uint16_t ECU_Station_Address = CCP_Master->Get_ECU_station_address();
        newStationAddress = ECU_Station_Address;
        std::stringstream stream;
        stream << ECU_Station_Address;
        m_textECUStationAddress->SetValue(stream.str());
        wxMessageBox(_("Please enter a integer in the range of [0 - 65535]."),_("Why?"));
        std::cerr << "New value could not be converted to long." << std::endl;
        return;
    }
    if(entered_station_address < 0 || entered_station_address > 65535)
    {
        uint16_t ECU_Station_Address = CCP_Master->Get_ECU_station_address();
        newStationAddress = ECU_Station_Address;
        std::stringstream stream;
        stream << ECU_Station_Address;
        m_textECUStationAddress->SetValue(stream.str());
        wxMessageBox(_("Please enter a integer in the range of [0 - 65535]."),_("Why?"));
        std::cerr << "Value is out of range. The value need to be in the range of [0 65535]." << std::endl;
        return;
    }

    newStationAddress = uint16_t(entered_station_address);
    Station_Address_changed = true;
}
/*******************************************************************************************
 * Function: This event is called when the user presses the Cancel button. The dialog will be
 *           closed and no values will be taken over into the CCP-Master.
 ******************************************************************************************/
void LabskausFrameSettings::event_Cancel( wxCommandEvent& event )
{
    this->Close(true);
}

/*******************************************************************************************
 * Function: This event is called when the user changes the device inside the Settings
 *           Dialog. The value will be stored inside a temp variable which will be take over
 *           if the new value is applied
 ******************************************************************************************/
void LabskausFrameSettings::event_ChangeSerialDevice( wxCommandEvent& event )
{
    if( m_textDevice->IsEmpty())
    {
        wxMessageBox(_("Please enter a integer in the range of [0 - 37]."),_("What did you do?"));
        std::cerr << "No new value entered" << std::endl;
        return;
    }
    wxString user_input_as_string = m_textDevice->GetValue();
    long entered_port_number;
    if(!user_input_as_string.ToLong(&entered_port_number))
    {
        int last_SerialPortNumber = CCP_Master->SerialPort.Get_port_number();
        newSerialPort = last_SerialPortNumber;
        std::stringstream stream;
        stream << newStationAddress;
        m_textECUStationAddress->SetValue(stream.str());
        wxMessageBox(_("Please enter a integer in the range of [0 - 37]."),_("Why?"));
        std::cerr << "New value could not be converted to long." << std::endl;
        return;
    }
    if(entered_port_number < 0 || entered_port_number > 37)
    {
        int last_SerialPortNumber = CCP_Master->SerialPort.Get_port_number();
        newSerialPort = last_SerialPortNumber;
        std::stringstream stream;
        stream << last_SerialPortNumber;
        m_textECUStationAddress->SetValue(stream.str());
        wxMessageBox(_("Please enter a integer in the range of [0 - 37]."),_("Why?"));
        std::cerr << "Value is out of range. The value need to be in the range of [0 65535]." << std::endl;
        return;
    }

    newSerialPort = int(entered_port_number);
    changed_SerialDevice = true;
}

/*******************************************************************************************
 * Function: This event is called when the user changes the baud rate inside the Settings
 *           Dialog. The value will be stored inside a temp variable which will be take over
 *           if the new value is applied
 ******************************************************************************************/
void LabskausFrameSettings::event_ChangeSerialBaudRate( wxCommandEvent& event )
{
    if( m_textBaudRate->IsEmpty())
    {
        wxMessageBox(_("Please enter a integer."),_("What did you do?"));
        std::cerr << "No new value entered" << std::endl;
        return;
    }
    wxString user_input_as_string = m_textBaudRate->GetValue();
    long entered_number;
    if(!user_input_as_string.ToLong(&entered_number))
    {
        int last_SerialBaudRate = CCP_Master->SerialPort.Get_baud_rate();
        newSerialBaudRate = last_SerialBaudRate;
        std::stringstream stream;
        stream << newStationAddress;
        m_textBaudRate->SetValue(stream.str());
        wxMessageBox(_("Please enter a integer > 0."),_("Why?"));
        std::cerr << "New value could not be converted to long." << std::endl;
        return;
    }
    if(entered_number < 0 )
    {
        int last_SerialBaudRate = CCP_Master->SerialPort.Get_baud_rate();
        newSerialBaudRate = last_SerialBaudRate;
        std::stringstream stream;
        stream << newStationAddress;
        m_textBaudRate->SetValue(stream.str());
        wxMessageBox(_("Please enter a integer > 0."),_("Why?"));
        std::cerr << "New value could not be converted to long." << std::endl;
        return;
    }

    newSerialBaudRate = int(entered_number);
    changed_SerialBaudRate = true;
}

/*******************************************************************************************
 * Function: This event is called when the user changes the serial mode inside the Settings
 *           Dialog. The value will be stored inside a temp variable which will be take over
 *           if the new value is applied
 ******************************************************************************************/
void LabskausFrameSettings::event_ChangeSerialMode( wxCommandEvent& event )
{

    if( m_textSerialMode->IsEmpty())
    {
        wxMessageBox(_("Please enter string defining the serial mode."),_("What did you do?"));
        std::cerr << "No new value entered" << std::endl;
        return;
    }
    wxString user_input_as_string = m_textSerialMode->GetValue();
    newSerialMode = user_input_as_string.ToStdString();
    changed_SerialMode = true;
    return;

}

/*******************************************************************************************
 * Function: This event is called when the user presses the Apply button. The dialog will be
 *           closed and all values will be taken over into the CCP-Master.
 *
 *           The events for evaluating the text fields are only triggered if the user presses
 *           enter after editing the text field. If the user changes a text field and directly
 *           presses the Apply button then the values will not be taken over there the events
 *           for evalutation of the text fields are trigger again in this function.
 ******************************************************************************************/
void LabskausFrameSettings::event_Apply( wxCommandEvent& event )
{
    wxCommandEvent apply_event;
    event_ChangeSerialDevice(apply_event);
    event_ChangeStationAddress(apply_event);
    event_ChangeSerialBaudRate(apply_event);
    event_ChangeSerialMode(apply_event);

    if(Station_Address_changed)   CCP_Master->Set_ECU_station_address(newStationAddress);
    if(ByteOrder_changed)         CCP_Master->Set_ECU_endianness(newECUByteOrder);
    if(StartByte_changed)         CCP_Master->SerialPort.Set_StartByte(newStartByte);
    if(changed_SerialDevice)      CCP_Master->SerialPort.Set_port_number(newSerialPort);
    if(changed_SerialBaudRate)    CCP_Master->SerialPort.Set_baud_rate(newSerialBaudRate);
    if(changed_SerialMode)        CCP_Master->SerialPort.Set_serial_mode(newSerialMode);
    this->Close(true);
}
