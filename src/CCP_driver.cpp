#include "CCP_driver.h"
#include "CCP_Frame.h"
#include "serial.h"
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

    CCP_Connect_Cmd->SetByte1(1);
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
}
