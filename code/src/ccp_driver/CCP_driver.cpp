#include "CCP_driver.h"


CCP_driver::CCP_driver()
{
    //ctor
    ECU_Connected = false;
    ECU_CCP_Version_Main = 0;
    ECU_CCP_Version_Release = 0;
    Device_Available = false;
    MessageCounter = 0;
    SM_actl_state = 0;
    SMT_req_establish_connection = false;
    ECU_MTA_Number1 = 0;
    ECU_MTA_Number2 = 0;
    m_MeasurementActive= false;

    /* find out the byte order */
    ECU_byte_order = little_endian;
    union
    {
        int  i;
        char b[sizeof(int)];
    } u;
    u.i = 0x01020304;
    (u.b[0] == 0x01) ? PC_byte_order = big_endian : PC_byte_order = little_endian;

    /* reserve some memory for the frames */
    CCP_Msg_Buffer.reserve(100000);
}

CCP_driver::~CCP_driver()
{
    //dtor
}

bool CCP_driver::open_communication_port(void)
{
    return SerialPort.open_port();
}

void CCP_driver::close_communication_port(void)
{
    SerialPort.close_port();
}

void CCP_driver::periodic_check(void)
{
    /* Check if new data is available */
    CCP_Frame* ptr_CCP_frame;
    ptr_CCP_frame = SerialPort.AnalyzeBytesRead();
    if(ptr_CCP_frame)
    {/* Falls der Zeiger nicht null ist muss er analysiert werden */
       Analyze(*ptr_CCP_frame);
    }
    /*Überprüfe ob akutell ein Timeout vorliegt.*/
    CRO_check_time_out();
    /* See if there is anything new for the state machine */

    uint16_t SM_actl_state_old;
    uint16_t  interation_counter = 0;
    do
    {
        SM_actl_state_old = SM_actl_state;
        SM_run_state_machine();
        interation_counter++;
        if(interation_counter > 100)
        {
            std::cerr << "More than 100 iteration and the state is still changing. Looks"
                      << "somebody programmed shit." << std::endl;
            break;
        }

    }while(SM_actl_state_old != SM_actl_state);

}

/*
 * Send the Connect Command
 *
 * Description: It shall be tested if the microcontroller is reacting on the test command. And if
 * all the bytes from the answer that is send by the microcontoller are filled out correctly.
 *
 * Message that will be send over:
 *    Byte1   : 0x01  Command code for the CONNECT command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3,4 : 0x00 0x00  station address
 *    Byte5-8 : doesn't matter
 */
void CCP_driver::TxCRO_Connect()
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_CONNECT);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3( (uint8_t)(ECU_station_address & 0x00FF) );
    CCP_Connect_Cmd->SetByte4( (uint8_t)((ECU_station_address & 0xFF00) >> 8));    //
    CCP_Connect_Cmd->SetByte5(0);
    CCP_Connect_Cmd->SetByte6(0);
    CCP_Connect_Cmd->SetByte7(0);
    CCP_Connect_Cmd->SetByte8(0);

    CCP_Connect_Cmd->setCCPFrameTime();

    CRO_Tx(*CCP_Connect_Cmd);

    #ifdef PLOT_COMMUNICATION_TO_TERMINAL
    std::cout << " Tx: 0x"
              << " " << std::setw(2) << std::uppercase <<std::hex << (int)CCP_Msg_Buffer.back().GetByte1()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte2()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte3()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte4()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte5()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte6()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte7()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte8()
              << " at:" << CCP_Msg_Buffer.back().getCCPFrameTime()
              << std::endl;
    #endif /*PLOT_COMMUNICATION_TO_TERMINAL*/
}

/*
 * Send the command to get the CCP Version.
 *
 * Description:
 *
 * Message that will be send over:
 *    Byte1   : 0x1B  Command code for the GET_CCP_VERSION command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3,4 : 0x02 0x01  Requested CCP Version 2.1
 *    Byte5-8 : doesn't matter
 */
void CCP_driver::TxCRO_GetCCP_Version(void)
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_GET_CCP_VERSION);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3(2);
    CCP_Connect_Cmd->SetByte4(1);
    CCP_Connect_Cmd->SetByte5(0);
    CCP_Connect_Cmd->SetByte6(0);
    CCP_Connect_Cmd->SetByte7(0);
    CCP_Connect_Cmd->SetByte8(0);
    CCP_Connect_Cmd->setCCPFrameTime();
    CRO_Tx(*CCP_Connect_Cmd);
}

/*
 * Send the command to set the MTA (memory transfer adress)
 *
 * Description:
 *
 * Message that will be send over:
 *    Byte1   : 0x02  Command code for the SETMTA command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3   : 0x02  MTA which shall be changed
 *    Byte4   : 0x??  Address extention
 *    Byte5-8 : 0x??  New Address
 */
void CCP_driver::TxCRO_SetMTA(uint8_t MTA_number, uint8_t AddressExtention, uint32_t MTA_adress)
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_SET_MTA);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3(MTA_number);
    CCP_Connect_Cmd->SetByte4(AddressExtention);
    if(ECU_byte_order == PC_byte_order)
    {
        CCP_Connect_Cmd->SetByte5(uint8_t( MTA_adress & 0x000000FF));
        CCP_Connect_Cmd->SetByte6(uint8_t((MTA_adress & 0x0000FF00) >> 8));
        CCP_Connect_Cmd->SetByte7(uint8_t((MTA_adress & 0x00FF0000) >> 16));
        CCP_Connect_Cmd->SetByte8(uint8_t( MTA_adress >> 24));
    }
    if(ECU_byte_order != PC_byte_order)
    {
        CCP_Connect_Cmd->SetByte5(uint8_t( MTA_adress >> 24));
        CCP_Connect_Cmd->SetByte6(uint8_t((MTA_adress & 0x00FF0000) >> 16));
        CCP_Connect_Cmd->SetByte7(uint8_t((MTA_adress & 0x0000FF00) >> 8));
        CCP_Connect_Cmd->SetByte8(uint8_t( MTA_adress & 0x000000FF));
    }

    CCP_Connect_Cmd->setCCPFrameTime();
    CRO_Tx(*CCP_Connect_Cmd);
}


/*
 * Send the command to Upload data from the ECU to the CCP_driver
 *
 * Description:
 *
 * Message that will be send over:
 *    Byte1   : 0x04  Command code for U command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3   : 0x??  Size of the data block
 *    Byte4-8 : doesn't matter
 */
void CCP_driver::TxCRO_Upload(uint8_t num_of_bytes)
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_UPLOAD);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3(num_of_bytes);   /* Request the CCP Version 2 */
    CCP_Connect_Cmd->SetByte4(0);   /* Request the CCP Release 1 */
    CCP_Connect_Cmd->SetByte5(0);
    CCP_Connect_Cmd->SetByte6(0);
    CCP_Connect_Cmd->SetByte7(0);
    CCP_Connect_Cmd->SetByte8(0);
    CCP_Connect_Cmd->setCCPFrameTime();
    CRO_Tx(*CCP_Connect_Cmd);
}


/*
 * Send the command to upload a small amount of data SHORT_UP
 *
 * Description:
 *
 * Message that will be send over:
 *    Byte1   : 0x02  Command code for the SETMTA command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3   : 0x02  Number of Bytes that shall be send back from the ECU
 *    Byte4   : 0x??  Address extention
 *    Byte5-8 : 0x??  New Address
 */
void CCP_driver::TxCRO_ShortUp(uint8_t number_of_bytes, uint8_t AddressExtention, uint32_t u32_address)
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_SHORT_UP);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3(number_of_bytes);
    CCP_Connect_Cmd->SetByte4(AddressExtention);
    if(ECU_byte_order == PC_byte_order)
    {
        CCP_Connect_Cmd->SetByte5(uint8_t( u32_address & 0x000000FF));
        CCP_Connect_Cmd->SetByte6(uint8_t((u32_address & 0x0000FF00) >> 8));
        CCP_Connect_Cmd->SetByte7(uint8_t((u32_address & 0x00FF0000) >> 16));
        CCP_Connect_Cmd->SetByte8(uint8_t( u32_address >> 24));
    }
    if(ECU_byte_order != PC_byte_order)
    {
        CCP_Connect_Cmd->SetByte5(uint8_t( u32_address >> 24));
        CCP_Connect_Cmd->SetByte6(uint8_t((u32_address & 0x00FF0000) >> 16));
        CCP_Connect_Cmd->SetByte7(uint8_t((u32_address & 0x0000FF00) >> 8));
        CCP_Connect_Cmd->SetByte8(uint8_t( u32_address & 0x000000FF));
    }

    CRO_Tx(*CCP_Connect_Cmd);
}

/*
 * Send the command to Dnload data to the ECU from the CCP_driver
 *
 * Description:
 *
 * Message that will be send over:
 *    Byte1   : 0x03  Command code for DNLOAD command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3   : 0x??  Size of the data block
 *    Byte4-8 : 0x??  Data that shall be transmitted to the CPU
 */
void CCP_driver::TxCRO_Dnload(uint8_t * ptr_data, uint8_t num_of_bytes )
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_DNLOAD);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3(num_of_bytes);
    if(num_of_bytes >= 1) CCP_Connect_Cmd->SetByte4(*(ptr_data + 0));
    if(num_of_bytes >= 2) CCP_Connect_Cmd->SetByte5(*(ptr_data + 1));
    if(num_of_bytes >= 3) CCP_Connect_Cmd->SetByte6(*(ptr_data + 2));
    if(num_of_bytes >= 4) CCP_Connect_Cmd->SetByte7(*(ptr_data + 3));
    if(num_of_bytes >= 5) CCP_Connect_Cmd->SetByte8(*(ptr_data + 4));
    CCP_Connect_Cmd->setCCPFrameTime();
    CRO_Tx(*CCP_Connect_Cmd);
}

/*
 *  The state machine controls which action shall be taken as the next step.
 *
 *  State: Init
 *  In first place the state machine is just in a kind of sleeping state where it does notanything.
 *  It is just waiting for somebody how wants to start the communication to an ECU.
 */
void CCP_driver::SM_run_state_machine(void)
{

    switch(SM_actl_state)
    {
        case SM_Init:{
            ECU_Connected = false;
            ECU_CCP_Version_Main = 0;
            ECU_CCP_Version_Release = 0;
            if(SMT_req_establish_connection == true)
            { /* Exit the State */
                SM_actl_state = SM_Connect;
                SMT_req_establish_connection = false;
                SM_enterleave_state = true;
            }
            break;
            }
        case SM_Connect:{
            if(SM_enterleave_state == true)
            { /* Enter the state */
                Messagebuffer_clear();
                log_database.VariableLog_clear();
                log_database.Set_XMLListReference(m_XML_list);
                SetMeasurementStartTime();
                TxCRO_Connect();
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               ECU_Connected == true)
            {  /* Exit the state, Connection established */
                SM_actl_state = SM_CCP_Version;
                SM_enterleave_state = true;
                break;
            }
            if(CRO_waiting_for_request == false &&
               ECU_Connected == false)
            {  /* Exit the state, Connection not established */
                SM_actl_state = SM_Init;
                SM_enterleave_state = true;
                break;
            }
            break;
            }
        case SM_CCP_Version:{
            if(SM_enterleave_state == true)
            { /* Enter the state */
                TxCRO_GetCCP_Version();
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               ECU_CCP_Version_Main != 0)
            {  /* Exit the state, Connection established */
                SM_actl_state = SM_Wait;
                SM_enterleave_state = true;
                break;
            }
            if(CRO_waiting_for_request == false &&
               ECU_CCP_Version_Main == 0 &&
               ECU_CCP_Version_Release == 0)
            {  /* Exit the state, Connection not established */
                SM_actl_state = SM_Init;
                SM_enterleave_state = true;
                break;
            }
            break;
            }
        case SM_Wait:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                SM_enterleave_state = false;
                break;
            }
            updateSchedular();
            if(SMT_read_variable == true)
            {  /* Exit the state, start to set the MTA */
                SM_actl_state = SM_read_variable_ShortUp;
                SM_enterleave_state = true;
                SMT_read_variable = false;
                break;
            }
            if(SMT_calibrate_variable == true)
            { /* Exit the state, start to set the MTA */
                SM_actl_state = SM_calibrate_variable_SetMTA;
                SM_enterleave_state = true;
                SMT_calibrate_variable = false;
                break;
            }
            break;
        case SM_read_variable_ShortUp:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                if(SMI_read_variable_type == type_u8)TxCRO_ShortUp(1,0,SMI_read_variable_address);
                if(SMI_read_variable_type == type_i8)TxCRO_ShortUp(1,0,SMI_read_variable_address);
                if(SMI_read_variable_type == type_u16)TxCRO_ShortUp(2,0,SMI_read_variable_address);
                if(SMI_read_variable_type == type_i16)TxCRO_ShortUp(2,0,SMI_read_variable_address);
                if(SMI_read_variable_type == type_u32)TxCRO_ShortUp(4,0,SMI_read_variable_address);
                if(SMI_read_variable_type == type_i32)TxCRO_ShortUp(4,0,SMI_read_variable_address);
                if(SMI_read_variable_type == type_f32)TxCRO_ShortUp(4,0,SMI_read_variable_address);
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received == CRC_ACKNOWLEGE)
            {  /* Exit the state, Everything received */
                SM_actl_state = SM_Wait;
                SMI_read_variable_successfull = true;
                log_database.add_new_value(SMI_read_variable_address,
                                           SMI_read_variable_type,
                                           SMI_read_variable_uint8,
                                           SMI_read_variable_sint8,
                                           SMI_read_variable_uint16,
                                           SMI_read_variable_sint16,
                                           SMI_read_variable_uint32,
                                           SMI_read_variable_sint32,
                                           SMI_read_variable_f32,
                                           time_of_last_received_CRM);
                SM_enterleave_state = true;
                break;
            }
            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received != CRC_ACKNOWLEGE)
            {  /* Exit the state, Everything received */
                SM_actl_state = SM_Wait;
                SMI_read_variable_successfull = false;
                SM_enterleave_state = true;
                break;
            }
            break;
        case SM_calibrate_variable_SetMTA:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                TxCRO_SetMTA(0,SMI_read_address_extention,SMI_read_variable_address);
                SM_enterleave_state = false;
                SMT_calibrate_variable = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received == CRC_ACKNOWLEGE)
            {  /* Exit the state, MTA is positioned */
                SM_actl_state = SM_calibrate_variable_DataDNLoad;
                SM_enterleave_state = true;
                break;
            }
            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received != CRC_ACKNOWLEGE)
            {  /* Exit the state, MTA is positioned */
                SM_actl_state = SM_Wait;
                SM_enterleave_state = true;
                break;
            }
            break;
        case SM_calibrate_variable_DataDNLoad:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                if(ECU_byte_order == PC_byte_order)
                {
                    if(SMI_read_variable_type == type_u8)  TxCRO_Dnload((uint8_t*)(&SMI_read_variable_uint8),1);
                    if(SMI_read_variable_type == type_i8)  TxCRO_Dnload((uint8_t*)(&SMI_read_variable_sint8),1);
                    if(SMI_read_variable_type == type_u16) TxCRO_Dnload((uint8_t*)(&SMI_read_variable_uint16),2);
                    if(SMI_read_variable_type == type_i16) TxCRO_Dnload((uint8_t*)(&SMI_read_variable_sint16),2);
                    if(SMI_read_variable_type == type_u32) TxCRO_Dnload((uint8_t*)(&SMI_read_variable_uint32),4);
                    if(SMI_read_variable_type == type_i32) TxCRO_Dnload((uint8_t*)(&SMI_read_variable_sint32),4);
                    if(SMI_read_variable_type == type_f32) TxCRO_Dnload((uint8_t*)(&SMI_read_variable_f32),4);
                }
                if(ECU_byte_order != PC_byte_order)
                {
                    if(SMI_read_variable_type == type_u8)  TxCRO_Dnload((uint8_t*)(&SMI_read_variable_uint8),1);
                    if(SMI_read_variable_type == type_i8)  TxCRO_Dnload((uint8_t*)(&SMI_read_variable_sint8),1);
                    if(SMI_read_variable_type == type_u16)
                    {
                        uint8_t array_new_byte_order[2];
                        uint8_t* ptr_variable = (uint8_t*)(&SMI_read_variable_uint16);
                        array_new_byte_order[0] = *( ptr_variable + 1);
                        array_new_byte_order[1] = *( ptr_variable + 0);
                        TxCRO_Dnload((uint8_t*)(&(array_new_byte_order[0])),2);
                    }
                    if(SMI_read_variable_type == type_i16)
                    {
                        uint8_t array_new_byte_order[2];
                        uint8_t* ptr_variable = (uint8_t*)(&SMI_read_variable_sint16);
                        array_new_byte_order[0] = *( ptr_variable + 1);
                        array_new_byte_order[1] = *( ptr_variable + 0);
                        TxCRO_Dnload((uint8_t*)(&(array_new_byte_order[0])),2);
                    }
                    if(SMI_read_variable_type == type_u32)
                    {
                        uint8_t array_new_byte_order[4];
                        uint8_t* ptr_variable = (uint8_t*)(&SMI_read_variable_uint32);
                        array_new_byte_order[0] = *( ptr_variable + 3);
                        array_new_byte_order[1] = *( ptr_variable + 2);
                        array_new_byte_order[2] = *( ptr_variable + 1);
                        array_new_byte_order[3] = *( ptr_variable + 0);
                        TxCRO_Dnload((uint8_t*)(&(array_new_byte_order[0])),4);
                    }
                    if(SMI_read_variable_type == type_i32)
                    {
                        uint8_t array_new_byte_order[4];
                        uint8_t* ptr_variable = (uint8_t*)(&SMI_read_variable_sint32);
                        array_new_byte_order[0] = *( ptr_variable + 3);
                        array_new_byte_order[1] = *( ptr_variable + 2);
                        array_new_byte_order[2] = *( ptr_variable + 1);
                        array_new_byte_order[3] = *( ptr_variable + 0);
                        TxCRO_Dnload((uint8_t*)(&(array_new_byte_order[0])),4);
                    }
                    if(SMI_read_variable_type == type_f32)
                    {
                        uint8_t array_new_byte_order[4];
                        uint8_t* ptr_variable = (uint8_t*)(&SMI_read_variable_f32);
                        array_new_byte_order[0] = *( ptr_variable + 3);
                        array_new_byte_order[1] = *( ptr_variable + 2);
                        array_new_byte_order[2] = *( ptr_variable + 1);
                        array_new_byte_order[3] = *( ptr_variable + 0);
                        TxCRO_Dnload((uint8_t*)(&(array_new_byte_order[0])),4);
                    }
                }
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received == CRC_ACKNOWLEGE)
            {  /* Exit the state, Everything received */
                SM_actl_state = SM_Wait;
                SM_enterleave_state = true;
                break;
            }
            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received != CRC_ACKNOWLEGE)
            {  /* Exit the state, Everything received */
                SM_actl_state = SM_Wait;
                SM_enterleave_state = true;
                break;
            }
            break;
        default:
            std::cerr << "The CCP driver state machine is trapped in a unknown state." << std::endl
            << "The state number is:" << SM_actl_state << std::endl;
    }
}


/*
 *  Diese Funktion wird aufgerufen wenn mindestens 8 Bytes empfangen wurden und diese zu einem
 *  Frame zusammengestellt wurden.
 *
 *  a) sollte es sich bei der Funktion um eine Antwort auf einen CRO Frame handeln, so wird diese
 *     Antwort entsprechend analysiert.
 */
void CCP_driver::Analyze(CCP_Frame& received_CCP_frame)
{

    CCP_Msg_Buffer.push_back(received_CCP_frame);

    #ifdef PLOT_COMMUNICATION_TO_TERMINAL
    std::cout << " Rx: 0x"
              << " " << std::setw(2) << std::uppercase <<std::hex << (int)CCP_Msg_Buffer.back().GetByte1()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte2()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte3()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte4()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte5()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte6()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte7()
              << " " << std::setw(2) << std::uppercase << std::hex << (int)CCP_Msg_Buffer.back().GetByte8()
              << " at:" << CCP_Msg_Buffer.back().getCCPFrameTime()
              << std::endl;
    #endif /*PLOT_COMMUNICATION_TO_TERMINAL*/

    // Check what kind of data object is received
    unsigned char CCP_Msg_DTO_type = received_CCP_frame.GetByte1();

    //time_of_last_received_CRM = received_CCP_frame.GetCCPFrameTimestruct();
    switch (CCP_Msg_DTO_type)
    {
        case DTO_type_CRM:
            time_of_last_received_CRM = received_CCP_frame.GetCCPFrameTimestruct();
            if(received_CCP_frame.GetByte3() != CRO_last_request_MessageCounter)
            {
                std::cerr << "Die CRO Antwort hatte den Falschen Message Counter" << std::endl
                << "Gesendet : " << (int)CRO_last_request_MessageCounter << std::endl
                << "Empfangen: " << (int)received_CCP_frame.GetByte3()   << std::endl;
            }
            else
            {
            CRM_ErrorCode_last_received    = received_CCP_frame.GetByte2();
                /*
                 * Der folgende Teil wird über die switch Anweisung wissen wie die Antwort
                 * auszuwerten ist. Der CCP Treiber hat sich gemerkt welchen Kommandotyp an
                 * die ECU geschickt wurde. Entsprechend wird das Kommando ausgewerter
                 */
                CRO_waiting_for_request = false;
                switch(CRO_last_CRO_Command_type)
                {
                    case COMMAND_CONNECT:
                        if(received_CCP_frame.GetByte2() == CRC_ACKNOWLEGE)
                        {
                            ECU_Connected = true;
                            std::cout << "Connected" << std::endl;
                        }
                        else
                        {
                            ECU_Connected = false;
                            std::cerr << "ECU did not ACK the Command to connect" <<
                            std::endl;
                        }
                        break;
                    case COMMAND_GET_CCP_VERSION:
                        if(received_CCP_frame.GetByte2() == CRC_ACKNOWLEGE)
                        {
                            ECU_CCP_Version_Main    = received_CCP_frame.GetByte4();
                            ECU_CCP_Version_Release = received_CCP_frame.GetByte5();
                            std::cout << "CCP Version: " << (int)ECU_CCP_Version_Main
                            << "." << (int)ECU_CCP_Version_Release << std::endl;
                        }
                        else
                        {
                            std::cerr << "ECU did not ACK the Command to get the CCP version" <<
                            std::endl;
                        }
                        break;
                    case COMMAND_SET_MTA:
                        if(received_CCP_frame.GetByte2() == CRC_ACKNOWLEGE)
                        {

                        }
                        else
                        {
                            std::cerr << "ECU did not ACK the Command to set the MTA" <<
                            std::endl;
                        }
                        break;
                    case COMMAND_UPLOAD:
                        if(received_CCP_frame.GetByte2() == CRC_ACKNOWLEGE)
                        {
                          analyze_CRM_Upload(received_CCP_frame);
                        }
                        else
                        {
                            std::cerr << "ECU did not ACK the Command to Upload" <<
                            std::endl;
                        }
                        break;
                    case COMMAND_SHORT_UP:
                        if(received_CCP_frame.GetByte2() == CRC_ACKNOWLEGE)
                        {
                          analyze_CRM_Upload(received_CCP_frame);
                        }
                        else
                        {
                            std::cerr << "ECU did not ACK the Command to Upload" <<
                            std::endl;
                        }
                        break;
                    default:
                        std::cerr << "CRM empfangen aber es ist keine Auswertefunktion bekannt"
                        << "CRO Command Code:" << (int)CRO_last_CRO_Command_type << std::endl;
                        break;
                }
            }

            break;
        case DTO_type_Event:
            break;
        default: /* DAQ Object received */
            break;
    }
    /* Run the state machine as a new command is received. */
    SM_run_state_machine();
}


void CCP_driver::CRO_check_time_out(void)
{
    /* Check if the CCP_driver is waiting for an answer */
    if(CRO_waiting_for_request)
    {
        struct timespec current_time;
        if( clock_gettime(CLOCK_REALTIME,&current_time))
         {
            std::cerr << "Die Systemzeit konnte nicht geholt werden" << std::endl;
         }

         /*Checke ob ein TimeOut standgefunden ist*/
         if((current_time.tv_sec - CRO_last_request_time.tv_sec) > 2)
         {
            // Set back the waiting flag
            CRO_waiting_for_request = false;
            std::cerr << "Es wurde keine Antwort auf einen CRO empfangen" << std::endl;
         }
    }
}


void CCP_driver::CRO_Tx(CCP_Frame& CCP_Tx_Frame)
{

    CRO_last_request_MessageCounter = MessageCounter;
    CRO_waiting_for_request = true;
    CRO_last_CRO_Command_type = CCP_Tx_Frame.GetByte1();
    SerialPort.transmit_CCP_Frame(CCP_Tx_Frame);
    CRO_last_request_time = CCP_Tx_Frame.GetCCPFrameTimestruct();
    CCP_Msg_Buffer.push_back(CCP_Tx_Frame);
}

void CCP_driver::analyze_CRM_Upload(CCP_Frame& received_CCP_frame)
{
    if(ECU_byte_order == PC_byte_order)
    {
        if(SMI_read_variable_type == type_u8)
        {
            SMI_read_variable_uint8 = received_CCP_frame.GetByte4();
        }
        if(SMI_read_variable_type == type_i8)
        {
            uint8_t tmp = received_CCP_frame.GetByte4();
            uint8_t* ptr_tmp = &tmp;
            SMI_read_variable_sint8 = *((int8_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_u16)
        {
            uint8_t ptr_tmp[2];
            ptr_tmp[0] = received_CCP_frame.GetByte4();
            ptr_tmp[1] = received_CCP_frame.GetByte5();
            SMI_read_variable_uint16 = *((uint16_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_i16)
        {
            uint8_t ptr_tmp[2];
            ptr_tmp[0] = received_CCP_frame.GetByte4();
            ptr_tmp[1] = received_CCP_frame.GetByte5();
            SMI_read_variable_sint16 = *((int16_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_u32)
        {
            uint8_t ptr_tmp[4];
            ptr_tmp[0] = received_CCP_frame.GetByte4();
            ptr_tmp[1] = received_CCP_frame.GetByte5();
            ptr_tmp[2] = received_CCP_frame.GetByte6();
            ptr_tmp[3] = received_CCP_frame.GetByte7();
            SMI_read_variable_uint32 = *((uint32_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_i32)
        {

            uint8_t ptr_tmp[4];
            ptr_tmp[0] = received_CCP_frame.GetByte4();
            ptr_tmp[1] = received_CCP_frame.GetByte5();
            ptr_tmp[2] = received_CCP_frame.GetByte6();
            ptr_tmp[3] = received_CCP_frame.GetByte7();
            SMI_read_variable_sint32 = *((int32_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_f32)
        {
            uint8_t ptr_tmp[4];
            ptr_tmp[0] = received_CCP_frame.GetByte4();
            ptr_tmp[1] = received_CCP_frame.GetByte5();
            ptr_tmp[2] = received_CCP_frame.GetByte6();
            ptr_tmp[3] = received_CCP_frame.GetByte7();
            SMI_read_variable_f32 = *((float*)ptr_tmp);
        }
    }
    else
    {
        if(SMI_read_variable_type == type_u8)
        {
            SMI_read_variable_uint8 = received_CCP_frame.GetByte4();
        }
        if(SMI_read_variable_type == type_i8)
        {
            uint8_t tmp = received_CCP_frame.GetByte4();
            uint8_t* ptr_tmp = &tmp;
            SMI_read_variable_sint8 = *((int8_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_u16)
        {
            uint8_t ptr_tmp[2];
            ptr_tmp[0] = received_CCP_frame.GetByte5();
            ptr_tmp[1] = received_CCP_frame.GetByte4();
            SMI_read_variable_uint16 = *((uint16_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_i16)
        {
            uint8_t ptr_tmp[2];
            ptr_tmp[0] = received_CCP_frame.GetByte5();
            ptr_tmp[1] = received_CCP_frame.GetByte4();
            SMI_read_variable_sint16 = *((int16_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_u32)
        {
            uint8_t ptr_tmp[4];
            ptr_tmp[0] = received_CCP_frame.GetByte7();
            ptr_tmp[1] = received_CCP_frame.GetByte6();
            ptr_tmp[2] = received_CCP_frame.GetByte5();
            ptr_tmp[3] = received_CCP_frame.GetByte4();
            SMI_read_variable_uint32 = *((uint32_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_i32)
        {

            uint8_t ptr_tmp[4];
            ptr_tmp[0] = received_CCP_frame.GetByte7();
            ptr_tmp[1] = received_CCP_frame.GetByte6();
            ptr_tmp[2] = received_CCP_frame.GetByte5();
            ptr_tmp[3] = received_CCP_frame.GetByte4();
            SMI_read_variable_sint32 = *((int32_t*)ptr_tmp);
        }
        if(SMI_read_variable_type == type_f32)
        {

            uint8_t ptr_tmp[4];
            ptr_tmp[0] = received_CCP_frame.GetByte7();
            ptr_tmp[1] = received_CCP_frame.GetByte6();
            ptr_tmp[2] = received_CCP_frame.GetByte5();
            ptr_tmp[3] = received_CCP_frame.GetByte4();
            SMI_read_variable_f32 = *((float*)ptr_tmp);
        }
    }
}
 // only for testing
void CCP_driver::test_read_variable(void)
{
    if (Next_variable_type == type_unknown)
    {
        std::cout << "Variable can not be read because of a unknown datatype" << std::endl;
        return;
    }
    if (next_variable_address2read == 0x00)
    {
        std::cout << "Read out value at address 0x0, I don't think that this is a good idea"
        << std::endl;
        return;
    }
    SMI_read_address_extention = 0;
    SMT_read_variable = true;
}



void CCP_driver::SetMeasurementStartTime(void)
{
     if( clock_gettime(CLOCK_REALTIME,&start_time_measurement))
     {
        std::cerr << "Die Systemzeit konnte nicht geholt werden" << std::endl;
     }
}


void CCP_driver::Messagebuffer_export(void)
{
    /* Determine the file name */
    std::stringstream file_name;
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

            file_name << (now->tm_year + 1900) << '-'
                  << std::setfill('0') << std::setw(2) <<(now->tm_mon + 1) << '-'
                  << std::setfill('0') << std::setw(2) << now->tm_mday << "_"
                  << std::setfill('0') << std::setw(2) << now->tm_hour
                  << std::setfill('0') << std::setw(2) << now->tm_min
                  << std::setfill('0') << std::setw(2) << now->tm_sec
                  << "_log.csv";


    std::string path_name;

    if(log_folder.empty() == true)
    {
        path_name = file_name.str();
    }
    else
    {
        path_name = log_folder;
        path_name.append("/");
        path_name.append(file_name.str());
    }


    std::ofstream logfile(path_name);
    if ( ! logfile)
    {
        std::cerr << "[Error001] Logfile for CCP frames could not be opened" << std::endl;
        return;
    }

    if (CCP_Msg_Buffer.empty())
    {
        logfile << "% Datafile is empty!" << std::endl;
    }
    else
    {
        /*
         *  Log the files. It follows a example how it should look like.
         *
         *   Tx oder Rx , unix timestap s, ns, d1, d2, d3, d4, d5, d6, d7, d8;
         */
        logfile << "Direction, Unix timestamp UTC0, nanosec, Byte1, Byte2, Byte3, Byte4,"
                << "Byte5, Byte6, Byte7, Byte8" << std::endl;
         uint64_t idx_i = 0;
         struct timespec ptr_tmp_timespec;
         for(idx_i = 0; idx_i < CCP_Msg_Buffer.size(); idx_i++)
         {
            /*  Select the first variable log*/
            CCP_Frame* tmp_CCP_Frame;
            tmp_CCP_Frame = &CCP_Msg_Buffer.at(idx_i);
            ptr_tmp_timespec = tmp_CCP_Frame->GetCCPFrameTimestruct();
            // Direction
            if (tmp_CCP_Frame->GetCCPDirection() == dir_Tx){logfile << "Tx ,";}
            if (tmp_CCP_Frame->GetCCPDirection() == dir_Rx){logfile << "Rx ,";}
            if (tmp_CCP_Frame->GetCCPDirection() == dir_unknown){logfile << "unknown ,";}

            //unix timestamp in s and the ns

            logfile << std::dec << ptr_tmp_timespec.tv_sec << ",";
            logfile << std::dec << ptr_tmp_timespec.tv_nsec << ",";

            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte1() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte2() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte3() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte4() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte5() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte6() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte7() << ",";
            logfile << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)tmp_CCP_Frame->GetByte8() ;

            logfile << std::endl;
         }
         logfile << std::endl;
    }
}

void CCP_driver::Messagebuffer_clear(void)
{
    CCP_Msg_Buffer.clear();
}

int CCP_driver::addvariable2ActionPlan(ECU_VarInfo& var2add)
{
    bool add_variable2list = false;

    /*What to do if the list is empty?, Add a new element */
    if(ActionTable.empty())
    {
        add_variable2list = true;
    }
    else
    {
    /* check something else */
        CCP_Schedular_List_Element* ptr_tmp_list_element;
        uint32_t idx;
        for(idx = 0; idx < ActionTable.size(); idx++)
        {
            ptr_tmp_list_element = &ActionTable.at(idx);
            if(ptr_tmp_list_element->GetAddress() == var2add.GetAddress())
            {
                add_variable2list = false;
                std::cerr << "Die Variable befindet sich bereits einmal in der Liste" << std::endl;
                return -1;
            }
            else
            {
                add_variable2list = true;
            }
        }
    }

    if(add_variable2list == true)
    {
        CCP_Schedular_List_Element* tmp = new(CCP_Schedular_List_Element);
        tmp->SetAddress(var2add.GetAddress());
        tmp->SetDataType(var2add.GetDataType());
        tmp->SetName(var2add.GetName());
        tmp->SetSampleTime(10);
        tmp->SetMode_Polling();
        tmp->SetLastRequest_2_now();
        ActionTable.push_back(*tmp);
    }
    return 0;
}

int CCP_driver::rmVariableFromActionPlan(std::string var2rm)
{
    CCP_Schedular_List_Element* ptr_tmp_list_element;
    uint32_t idx;
    for(idx = 0; idx < ActionTable.size(); idx++)
    {
        ptr_tmp_list_element = &ActionTable.at(idx);
        if(!var2rm.compare(ptr_tmp_list_element->GetName()))
        {
            ActionTable.erase(ActionTable.begin() + idx);
            break;
        }
    }
    return 0;
}

void CCP_driver::clearActionPlan(void)
{
    ActionTable.clear();
}


void CCP_driver::updateSchedular(void)
{
    uint32_t memory_list_element = 0;
    // simple command to just take the next element inside the list
    if (!ActionTable.empty())
    {
        /* Experimental driver */
        int64_t highest_prio=-10000000000;
        int64_t tmp_prio;
        uint32_t idx;
        CCP_Schedular_List_Element* ptr_tabel_element;
        for(idx = 0; idx < ActionTable.size(); idx++)
        {
            ptr_tabel_element = &ActionTable.at(idx);
            tmp_prio = ptr_tabel_element->GetPrio();
            if(tmp_prio > highest_prio)
            {
                highest_prio = tmp_prio;
                memory_list_element = idx;
            }
        }
        /*
        std::cout << "Highest Prio: " << highest_prio << " Element: " << memory_list_element
                  << std::endl;
        */

        /*
         *Select the next List element
         */
        if(highest_prio > 0)
        {
            if(memory_list_element >= ActionTable.size())
            {
                memory_list_element = 0; // Ist das hier nicht eigentlich ein Fehlerfall?
            }
            ptr_tabel_element= &ActionTable.at(memory_list_element);
            if(ptr_tabel_element->GetAquisitionMode()  == E_Calibration)
            {
                // TODO place ECU_VarInfo here
                SMI_Actl_ECU_Variable = ptr_tabel_element;
                SMI_read_variable_address = ptr_tabel_element->GetAddress();
                SMI_read_variable_type    = ptr_tabel_element->GetDataType();
                SMI_read_variable_uint8   =  (uint8_t) ptr_tabel_element->GetCalValue_Int();
                SMI_read_variable_sint8   =   (int8_t) ptr_tabel_element->GetCalValue_Int();
                SMI_read_variable_uint16  = (uint16_t) ptr_tabel_element->GetCalValue_Int();
                SMI_read_variable_sint16  =  (int16_t) ptr_tabel_element->GetCalValue_Int();
                SMI_read_variable_uint32  = (uint32_t) ptr_tabel_element->GetCalValue_Int();
                SMI_read_variable_sint32  =  (int32_t) ptr_tabel_element->GetCalValue_Int();
                SMI_read_variable_f32     =    (float) ptr_tabel_element->GetCalValue_Float();
                // Activate the state machine
                SMT_calibrate_variable = true;
                ActionTable.erase(ActionTable.begin() + memory_list_element);
            }
            if(ptr_tabel_element->GetAquisitionMode()  == E_periodic_sample)
            {
                SMI_read_variable_address = ptr_tabel_element->GetAddress();
                SMI_read_variable_type = ptr_tabel_element->GetDataType();
                ptr_tabel_element->SetLastRequest_2_now();
                // Gebe der Statemachine den Auftrag die nächste variable zu lesen
                SMT_read_variable = true;
            }
        }
    }
}


void CCP_driver::VariableLog_export(void)
{
    log_database.VariableLog_export(Get_time_measurement_started(),log_folder, FileName_date);
    log_database.VariableLog_export(Get_time_measurement_started(),log_folder, FileName_general);
}

/*
 * 1. Argument: ECU Variable Objekt. Dieses beinhaltet in der Regel aber nur wo die Variable liegt
 *              wie sie heißt und den Datentyp. Deshalb werden noch weitere Informationen benötigt
 *
 * 2. Argument: Falls die Variable welche calibriert werden soll vom Typ Integer ist so kann hier
 *              der entsprechende Wert mit übergeben werden.
 *
 * 3. Argument: Falls die Variable welche calibriert werden soll vom Typ Float ist so kann hier
 *              der entsprechende Wert mit übergeben werden.
 *
 */

void CCP_driver::addCalibration2ActionPlan(ECU_VarInfo& Cal2Add, int64_t Cal_Int, float Cal_Float)
{
        CCP_Schedular_List_Element* tmp = new(CCP_Schedular_List_Element);
        tmp->SetAddress(Cal2Add.GetAddress());
        tmp->SetDataType(Cal2Add.GetDataType());
        tmp->SetSampleTime(0);
        tmp->SetMode_Calibration();
        tmp->SetLastRequest_2_now();
        tmp->SetCalValue_Int(Cal_Int);
        tmp->SetCalValue_Float(Cal_Float);
        ActionTable.push_back(*tmp);
}

/*
 * 1. Argument: Zeiger zur Extrahierten XML Liste
 *
 */


void CCP_driver::Set_XMLExtract(std::vector<ECU_VarInfo>* ptr_XML_List)
{
    m_XML_list = ptr_XML_List;
}
