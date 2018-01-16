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
        void open_port();
        void close_port();
        CCP_Frame* AnalyzeBytesRead(void);
        void transmit_CCP_Frame(CCP_Frame& CCP_Msg);
        void set_ptr_CCPDriver(void);
        void Set_device_name(std::string tmp){device_name = tmp;}
        std::string Get_device_name(void){return device_name;}
    protected:
    private:
        int fd;
        std::string device_name;
        std::vector<unsigned char> vec_input_buffer;
        void cleanVector2FirstCCPSign(void);
        CCP_Frame* createReceivedCCPFrame(void);
        unsigned char serial_input_buffer[SERIAL_BUFFERSIZE];
        unsigned char serial_output_buffer[SERIAL_BUFFERSIZE];
        unsigned char * ptr_read_buffer;
        unsigned char * ptr_write_buffer;
        serial* SerialPort;
};

#endif // SERIAL_H
