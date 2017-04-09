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

void CCP_driver::Connect(serial* Serial_Port)
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
    Serial_Port->transmit_CCP_Frame(CCP_Connect_Cmd);
    CCP_Msg_Buffer.push_back(*CCP_Connect_Cmd);

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
