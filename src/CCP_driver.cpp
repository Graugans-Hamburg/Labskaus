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

    #if PLOT_COMMUNICATION_TO_TERMINAL == TRUE
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
 * Send the Connect to get the CCP Version
 *
 * Description: It shall be tested if the microcontroller is reacting on the test command. And if
 * all the bytes from the answer that is send by the microcontoller are filled out correctly.
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
        case SM_Init:
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
        case SM_Connect:
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
        case SM_CCP_Version:
            if(SM_enterleave_state == true)
            { /* Enter the state */
                TxCRO_GetCCP_Version();
                SM_enterleave_state = false;
                break;
            }

            if(CRO_waiting_for_request == false &&
               ECU_CCP_Version_Main != 0)
            {  /* Exit the state, Connection established */
                //SM_actl_state = SM_CCP_Version;
                //SM_enterleave_state = true;
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

    #if PLOT_COMMUNICATION_TO_TERMINAL == TRUE
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
                    default:
                        std::cerr << "CRM empfangen aber es ist keine Auswertefunktion bekannt"
                        << "CRO Command Code:" << CRO_last_CRO_Command_type << std::endl;
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
