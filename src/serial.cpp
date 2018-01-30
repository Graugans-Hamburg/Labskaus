#include "serial.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include "rs232.h"


 /*******************************************************************************************
 * Function: Whith the constructor of the serial Objekt also the device which need to be
 *           opened is defined.
 *
 *           Initial Setup of the port
 *          - 115200 Baud
 *          - 8 bits/byte
 *          - no parity
 *          - no handshake
 *          - 1 stop bit
 *
 ******************************************************************************************/
 serial::serial()
{
    //ctor
    ptr_read_buffer = &serial_input_buffer[0];
    ptr_write_buffer = &serial_input_buffer[0];
    port_number = 16;
    baud_rate = 115200;
    serial_mode = "8N1";
    ConfiguredStartByte = Char0xB0;
    vec_input_buffer.clear();
}
 /*******************************************************************************************
 * Function: deconstructor
 *
 ******************************************************************************************/
serial::~serial()
{
    RS232_CloseComport(16);
}
/*******************************************************************************************
 * Function: This function opens the serial port.
 ******************************************************************************************/
bool serial::open_port()
{

   if (RS232_OpenComport(port_number, baud_rate,serial_mode.c_str()) != 0)
    {
        std::cerr << "Serial Port could not be opened" << std::endl;
        return false;
   }

   return true;
}

 /*******************************************************************************************
 * Function: This function close the serial port.
 ******************************************************************************************/
void serial::close_port( void )
{
    RS232_CloseComport(port_number);
}


CCP_Frame* serial::AnalyzeBytesRead(void)
{
    unsigned char tmp_input_buffer[4096];

    int bytes_read = RS232_PollComport(port_number, tmp_input_buffer, 4096);
    if(bytes_read > 0)
    {
        for(int index = 0; index < bytes_read; index++)
        {
            vec_input_buffer.push_back(tmp_input_buffer[index]);
        }
    }
    if(ConfiguredStartByte == Char0xB0)
    {
        //Lösche alles bis auf das erst B0
        cleanVector2FirstCCPSign();
    }
    return createReceivedCCPFrame();
}

void serial::transmit_CCP_Frame(CCP_Frame& CCP_Msg)
{
    int results;
    switch(ConfiguredStartByte)
    {
        case Char0xB0:
            {
            unsigned char Serial_CCP_Frame[9];
            Serial_CCP_Frame[0] = 0xB0;
            Serial_CCP_Frame[1] = CCP_Msg.GetByte1();
            Serial_CCP_Frame[2] = CCP_Msg.GetByte2();
            Serial_CCP_Frame[3] = CCP_Msg.GetByte3();
            Serial_CCP_Frame[4] = CCP_Msg.GetByte4();
            Serial_CCP_Frame[5] = CCP_Msg.GetByte5();
            Serial_CCP_Frame[6] = CCP_Msg.GetByte6();
            Serial_CCP_Frame[7] = CCP_Msg.GetByte7();
            Serial_CCP_Frame[8] = CCP_Msg.GetByte8();
            CCP_Msg.SetCCPDirection_Tx();
            CCP_Msg.setCCPFrameTime();
            results = RS232_SendBuf(port_number, Serial_CCP_Frame, sizeof(Serial_CCP_Frame));
            break;
            }
        case no_startbyte:
            {
            unsigned char Serial_CCP_Frame[0];
            Serial_CCP_Frame[0] = CCP_Msg.GetByte1();
            Serial_CCP_Frame[1] = CCP_Msg.GetByte2();
            Serial_CCP_Frame[2] = CCP_Msg.GetByte3();
            Serial_CCP_Frame[3] = CCP_Msg.GetByte4();
            Serial_CCP_Frame[4] = CCP_Msg.GetByte5();
            Serial_CCP_Frame[5] = CCP_Msg.GetByte6();
            Serial_CCP_Frame[6] = CCP_Msg.GetByte7();
            Serial_CCP_Frame[7] = CCP_Msg.GetByte8();
            CCP_Msg.SetCCPDirection_Tx();
            CCP_Msg.setCCPFrameTime();
            results = RS232_SendBuf(port_number, Serial_CCP_Frame, sizeof(Serial_CCP_Frame));
            break;
            }
        default:
            results = -1;
            break;
    }

    if (results == -1)
    {
        std::cerr << "The requested Frame had not been written to the serial interface." << std::endl;
    }
}


void serial::cleanVector2FirstCCPSign(void)
{
    //Lösche alles bis auf das erst B0
    while(vec_input_buffer.size())
    {
        if(vec_input_buffer[0] == 0xB0)
        {
            break;
        }
        else
        {
            std::cout << (char)(vec_input_buffer.at(0));
            vec_input_buffer.erase(vec_input_buffer.begin());
        }
    }
}

CCP_Frame* serial::createReceivedCCPFrame(void)
{

    switch(ConfiguredStartByte)
    {
        case Char0xB0:
        {
            if(vec_input_buffer.size() > 8 )
            {
                // Erstelle einen CCP Frame und schicke ihn auf die Reise
                CCP_Frame* ReceivedCCP = new CCP_Frame;
                ReceivedCCP->SetByte1(vec_input_buffer[1]);
                ReceivedCCP->SetByte2(vec_input_buffer[2]);
                ReceivedCCP->SetByte3(vec_input_buffer[3]);
                ReceivedCCP->SetByte4(vec_input_buffer[4]);
                ReceivedCCP->SetByte5(vec_input_buffer[5]);
                ReceivedCCP->SetByte6(vec_input_buffer[6]);
                ReceivedCCP->SetByte7(vec_input_buffer[7]);
                ReceivedCCP->SetByte8(vec_input_buffer[8]);
                ReceivedCCP->setCCPFrameTime();
                ReceivedCCP->SetCCPDirection_Rx();
                //Delete the last frame from the buffer;
                vec_input_buffer.erase(vec_input_buffer.begin(),vec_input_buffer.begin()+9);
                return ReceivedCCP;
            }
            break;
        }
        case no_startbyte:
        {
            if(vec_input_buffer.size() > 7 )
            {
                // Erstelle einen CCP Frame und schicke ihn auf die Reise
                CCP_Frame* ReceivedCCP = new CCP_Frame;
                ReceivedCCP->SetByte1(vec_input_buffer[0]);
                ReceivedCCP->SetByte2(vec_input_buffer[1]);
                ReceivedCCP->SetByte3(vec_input_buffer[2]);
                ReceivedCCP->SetByte4(vec_input_buffer[3]);
                ReceivedCCP->SetByte5(vec_input_buffer[4]);
                ReceivedCCP->SetByte6(vec_input_buffer[5]);
                ReceivedCCP->SetByte7(vec_input_buffer[6]);
                ReceivedCCP->SetByte8(vec_input_buffer[7]);
                ReceivedCCP->setCCPFrameTime();
                ReceivedCCP->SetCCPDirection_Rx();
                //Delete the last frame from the buffer;
                vec_input_buffer.erase(vec_input_buffer.begin(),vec_input_buffer.begin()+8);
                return ReceivedCCP;
            }
            break;
        }
        default:
            return NULL;
            break;
    }
    return NULL;
}

void serial::set_ptr_CCPDriver(void)
{
    //ptr2CCP_Driver = in_ptr_CCPDriver;
}
