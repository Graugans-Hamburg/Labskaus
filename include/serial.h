#ifndef SERIAL_H
#define SERIAL_H

#include "CCP_Frame.h"
#include <vector>

#define SERIAL_BUFFERSIZE 200


class serial
{
    public:
        serial();
        virtual ~serial();
        int GetFileDes() { return fd; }
        void open_port(int port);
        void close_port();
        void AnalyzeBytesRead(void);
        void transmit_CCP_Frame(CCP_Frame* CCP_Msg);

    protected:
    private:
        int fd;
        std::vector<unsigned char> vec_input_buffer;
        void cleanVector2FirstCCPSign(void);
        void createReceivedCCPFrame(void);
        unsigned char serial_input_buffer[SERIAL_BUFFERSIZE];
        unsigned char serial_output_buffer[SERIAL_BUFFERSIZE];
        unsigned char * ptr_read_buffer;
        unsigned char * ptr_write_buffer;
};

#endif // SERIAL_H
