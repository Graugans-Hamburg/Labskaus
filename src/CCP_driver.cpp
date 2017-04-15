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

void CCP_driver::open_communication_port(void)
{
    SerialPort.open_port(4); //TODO Number inside the code
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
    SM_run_state_machine();
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
    CCP_Connect_Cmd->SetByte3(0);
    CCP_Connect_Cmd->SetByte4(0);    //
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
    CCP_Connect_Cmd->SetByte3(2);   /* Request the CCP Version 2 */
    CCP_Connect_Cmd->SetByte4(1);   /* Request the CCP Release 1 */
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
 *    Byte1   : 0x02  Command code for the GET_CCP_VERSION command
 *    Byte2   : 0x??  Command Counter ctr
 *    Byte3,4 : 0x02 0x01  Requested CCP Version 2.1
 *    Byte5-8 : doesn't matter
 */
void CCP_driver::TxCRO_SetMTA(uint8_t MTA_number, uint8_t AdressExtention, uint32_t MTA_adress)
{
    CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();
    CCP_Connect_Cmd->SetByte1(COMMAND_SET_MTA);
    CCP_Connect_Cmd->SetByte2(++MessageCounter);
    CCP_Connect_Cmd->SetByte3(MTA_number);   /* Request the CCP Version 2 */
    CCP_Connect_Cmd->SetByte4(AdressExtention);   /* Request the CCP Release 1 */
    CCP_Connect_Cmd->SetByte5(uint8_t(MTA_adress >> 24));
    CCP_Connect_Cmd->SetByte6(uint8_t((MTA_adress & 0x00FFFFFF) >> 16));
    CCP_Connect_Cmd->SetByte7(uint8_t((MTA_adress & 0x0000FFFF) >> 8));
    CCP_Connect_Cmd->SetByte8(uint8_t(MTA_adress & 0x00000FF));
    CCP_Connect_Cmd->setCCPFrameTime();
    CRO_Tx(*CCP_Connect_Cmd);
}


/*
 * Send the command to Upload data from the ECU to the CCP_driver
 *
 * Description:
 *
 * Message that will be send over:
 *    Byte1   : 0x04  Command code for the GET_CCP_VERSION command
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

            if(SMT_read_variable)
            {  /* Exit the state, Connection established */
                SM_actl_state = SM_read_variable_SetMTA;
                SMT_read_variable = false;
                SM_enterleave_state = true;
                break;
            }
            break;
        case SM_read_variable_SetMTA:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                TxCRO_SetMTA(0,SMI_read_address_extention,SMI_read_variable_address);
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received == CRC_ACKNOWLEGE)
            {  /* Exit the state, MTA is positioned */
                SM_actl_state = SM_read_variable_DataUpload;
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
        case SM_read_variable_DataUpload:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                if(SMI_read_variable_type == type_u8)TxCRO_Upload(1);
                if(SMI_read_variable_type == type_i8)TxCRO_Upload(1);
                if(SMI_read_variable_type == type_u16)TxCRO_Upload(2);
                if(SMI_read_variable_type == type_i16)TxCRO_Upload(2);
                if(SMI_read_variable_type == type_u32)TxCRO_Upload(4);
                if(SMI_read_variable_type == type_i32)TxCRO_Upload(4);
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               CRM_ErrorCode_last_received == CRC_ACKNOWLEGE)
            {  /* Exit the state, Everything received */

                #ifdef PLOT_COMMUNICATION_TO_TERMINAL
                std::cout << "Empfangene Werte:" << std::endl
                          << "u8  : " << std::dec<< (int)SMI_read_variable_uint8  << std::endl
                          << "s8  : " << std::dec<< (int)SMI_read_variable_sint8  << std::endl
                          << "u16 : " << std::dec<< SMI_read_variable_uint16 << std::endl
                          << "s16 : " << std::dec<< SMI_read_variable_sint16 << std::endl
                          << "u32 : " << std::dec<< SMI_read_variable_uint32 << std::endl
                          << "s32 : " << std::dec <<SMI_read_variable_sint32 << std::endl;
                #endif /* PLOT_COMMUNICATION_TO_TERMINAL*/


                SM_actl_state = SM_Wait;
                SMI_read_variable_successfull = true;
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
    switch (CCP_Msg_DTO_type)
    {
        case DTO_type_CRM:
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
    clock_gettime(CLOCK_REALTIME,&CRO_last_request_time);
    CRO_waiting_for_request = true;
    CRO_last_CRO_Command_type = CCP_Tx_Frame.GetByte1();
    SerialPort.transmit_CCP_Frame(&CCP_Tx_Frame);
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
    }
}
 // only for testing
void CCP_driver::test_read_variable(void)
{
    SMI_read_variable_type = type_i32;
    SMI_read_variable_address = 0x20002aa4;
    SMI_read_address_extention = 0;
    SMT_read_variable = true;
}
