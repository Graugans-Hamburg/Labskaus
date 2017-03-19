#include "serial.h"
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>



serial::serial()
{
    //ctor
}

serial::~serial()
{
    //dtor
}

 void serial::open_port(int port)
 {
   /*Oeffnet den seiellen Port
    *Gibt das Filehandel zurueck oder -1 bei Fehler
    *der Parameter port muss 0,1,2oder 3 sein
    *
    *RS232
    * - 115200 Baud
    * - 8 bits/byte
    * - no parity
    * - no handshake
    * - 1 stop bit
    */
   struct termios options;
   switch (port)
   {
     case 0: fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY); break;
     case 1: fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY); break;
     case 2: fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY); break;
     case 3: fd = open("/dev/ttyS3", O_RDWR | O_NOCTTY | O_NDELAY); break;
     case 4: fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY); break;
     default: fd = -1;
   }
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


 void serial::close_port( void )
 {
    int answer = 0;
    answer = close(fd);
    if (answer == -1)
    {
        std::cerr << "Something went from while closing the fd" << std::endl;
    }
}


void serial::bytes_read(void)
{
    read(fd, serial_input_buffer, 200);
}


