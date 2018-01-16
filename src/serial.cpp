#include "serial.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>


/*******************************************************************************************
 *  System dependent includes
 ******************************************************************************************/
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        #include <termios.h>
#endif






#if defined(__WXMSW__)
            /*******************************************************************************************
             * Function: This function opens the serial port. The device that is used is defined inside
             *           the string device name. One example for the device name is /dev/ttyUSB0 which is
             *  WINDOOF  also the default device. This device can be changed inside the Settings.
             *
             *           The settings of the UART interface are currently hard coded inside this function.
             *           They are:
             *          - 115200 Baud
             *          - 8 bits/byte
             *          - no parity
             *          - no handshake
             *          - 1 stop bit
             ******************************************************************************************/
            void serial::open_port()
             {

               struct termios options;
               fd = open(device_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

               if (fd >= 0)
               {

               }
               else
               {
                    std::cerr << "Serial Port could not be opened" << std::endl;
               }
            }

            serial::serial()
            {
                //ctor
                ptr_read_buffer = &serial_input_buffer[0];
                ptr_write_buffer = &serial_input_buffer[0];
                device_name = "D:test.txt";
                vec_input_buffer.clear();
            }

#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
            /*******************************************************************************************
             * Function: This function opens the serial port. The device that is used is defined inside
             *           the string device name. One example for the device name is /dev/ttyUSB0 which is
             *           also the default device. This device can be changed inside the Settings.
             *
             *           The settings of the UART interface are currently hard coded inside this function.
             *           They are:
             *          - 115200 Baud
             *          - 8 bits/byte
             *          - no parity
             *          - no handshake
             *          - 1 stop bit
             *
             * LINUX-SPECIFIC
             ******************************************************************************************/
             void serial::open_port()
             {

               struct termios options;
               fd = open(device_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

               if (fd >= 0)
               {
                 /*get current options */
                 fcntl(fd, F_SETFL,0);
                 if(tcgetattr(fd, &options) != 0)
                 {
                    std::cerr << "Serial Problem, Couldn't get attributes" << std::endl;
                 }

                 cfsetspeed(&options,B115200);		/* setze 115200 bps */
                 options.c_cflag &= ~PARENB;		/* kein paritybit */
                 options.c_cflag &= ~CSTOPB;		/* 1 Stoppbit */
                 options.c_cflag &= ~CSIZE;			/* 8 Datenbits */
                 options.c_cflag |=CS8;
                 options.c_cflag |= (CLOCAL |CREAD);	/* CD-Signal ignoriernen */
                 options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);	/* kein echo keine steuerzeichen keine interrupts */
                 options.c_oflag &= ~OPOST;
                 options.c_cc[VMIN] =0;			/* warten auf mindestens 0 Zeichen */
                 options.c_cc[VTIME] = 0;
                 options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
                 tcflush(fd,TCIOFLUSH);
                 if (tcsetattr(fd, TCSAFLUSH, &options) != 0)
                 {
                    std::cerr << "Serial Problem, Couldn't set attributes" << std::endl;
                 }
                 fcntl(fd, F_SETFL, FNDELAY);
                 fcntl(fd, F_SETFL, 0);
               }
               else
               {
                    std::cerr << "Serial Port could not be opened" << std::endl;
               }
             }

             /*******************************************************************************************
             * Function: Whith the constructor of the serial Objekt also the device which need to be
             *           opened need to be defined.
             * LINUX-SPECIFIC
             ******************************************************************************************/

             serial::serial()
            {
                //ctor
                ptr_read_buffer = &serial_input_buffer[0];
                ptr_write_buffer = &serial_input_buffer[0];
                device_name = "/dev/ttyUSB0";
                vec_input_buffer.clear();
            }

#endif



serial::~serial()
{
    //dtor
}

 void serial::close_port( void )
 {
    int answer = 0;
    answer = close(fd);
    if (answer == -1)
    {
        std::cerr << "Something went from while closing the fd" << std::endl;
    }
}


CCP_Frame* serial::AnalyzeBytesRead(void)
{
    unsigned char tmp_input_buffer[200];

    int bytes_read = read(fd, tmp_input_buffer, 200);
    if(bytes_read > 0)
    {
        for(int index = 0; index < bytes_read; index++)
        {
            vec_input_buffer.push_back(tmp_input_buffer[index]);
        }
    }

    //Lösche alles bis auf das erst B0
    cleanVector2FirstCCPSign();
    return createReceivedCCPFrame();
}

void serial::transmit_CCP_Frame(CCP_Frame& CCP_Msg)
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
    int results = write(fd, Serial_CCP_Frame, sizeof(Serial_CCP_Frame));
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
    // Erstelle einen CCP Frame und schicke ihn auf die Reise
    if(vec_input_buffer.size() > 8 )
    {
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
            /* // DEBUGSHIT
            std::cout << "Size: " << vec_input_buffer.size() << std::endl;
            for (unsigned i=0; i < vec_input_buffer.size(); i++)
            {
                std::cout <<" 0x" << std::uppercase << std::hex << (int)vec_input_buffer[i];
            }
            std::cout << std::endl;
            // DEBUGSHIT */
        // Lösche den aktuellen Frame aus dem in buffer
        vec_input_buffer.erase(vec_input_buffer.begin(),vec_input_buffer.begin()+9);

        return ReceivedCCP;
    }

    return NULL;
}

void serial::set_ptr_CCPDriver(void)
{
    //ptr2CCP_Driver = in_ptr_CCPDriver;
}
