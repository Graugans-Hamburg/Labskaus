#include "CCP_driver.h"


CCP_driver::CCP_driver()
{
    //ctor
    CommunicationChannel_Active = false;
    Device_Available = false;
    MessageCounter = 0;
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
        CCP_Frame* ptr_CCP_frame;
        ptr_CCP_frame = SerialPort.AnalyzeBytesRead();
        if(ptr_CCP_frame)
        {/* Falls der Zeiger nicht null ist muss er analysiert werden */
           Analyze(*ptr_CCP_frame);
        }
        /*Überprüfe ob akutell ein Timeout vorliegt.*/
        CRO_check_time_out();
}

/*
 * Send the Connect Command
 *
 * Description: It shall be tested if the microcontroller is reacting on the test command. And if
 * all the bytes from the answer that is send by the microcontoller are filled out correctly.
 *
 * Message that will be send over:
 *    Byte1   : 0x01  Command code for the CONNECT command
 *    Byte2   : 0x17  Command Counter ctr
 *    Byte3,4 : 0x00 0x00  station address
 *    Byte5-8 : doesn't matter
 */


void CCP_driver::Connect()
{
 CCP_Frame* CCP_Connect_Cmd = new CCP_Frame();

    CCP_Connect_Cmd->SetByte1(COMMAND_CONNECT);
    CCP_Connect_Cmd->SetByte2(MessageCounter++);
    // Station Address
    CCP_Connect_Cmd->SetByte3(0);
    CCP_Connect_Cmd->SetByte4(0);
    //
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


void CCP_driver::CCP_drv_state_machine(void)
{


}

void CCP_driver::Analyze(CCP_Frame& recieved_CCP_frame)
{

    CCP_Msg_Buffer.push_back(recieved_CCP_frame);

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
         }
    }
}


void CCP_driver::CRO_Tx(CCP_Frame& CCP_Tx_Frame)
{
    CRO_last_request_MessageCounter = MessageCounter;
    clock_gettime(CLOCK_REALTIME,&CRO_last_request_time);
    CRO_waiting_for_request = true;
    SerialPort.transmit_CCP_Frame(&CCP_Tx_Frame);
    CCP_Msg_Buffer.push_back(CCP_Tx_Frame);
}
