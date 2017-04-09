#ifndef SERIAL_H
#define SERIAL_H

#include <vector>

#include "CCP_Frame.h"



#define SERIAL_BUFFERSIZE 200


class serial
{
    public:
        serial();
        virtual ~serial();
        int GetFileDes() { return fd; }
        void open_port(int port);
        void close_port();
        CCP_Frame* AnalyzeBytesRead(void);
        void transmit_CCP_Frame(CCP_Frame* CCP_Msg);
        void set_ptr_CCPDriver(void);

    protected:
    private:
        int fd;
        std::vector<unsigned char> vec_input_buffer;
        //CCP_driver* ptr2CCP_Driver;
        void cleanVector2FirstCCPSign(void);
        CCP_Frame* createReceivedCCPFrame(void);
        unsigned char serial_input_buffer[SERIAL_BUFFERSIZE];
        unsigned char serial_output_buffer[SERIAL_BUFFERSIZE];
        unsigned char * ptr_read_buffer;
        unsigned char * ptr_write_buffer;

};

#endif // SERIAL_H
