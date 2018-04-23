#ifndef SERIAL_H
#define SERIAL_H

#include <vector>
#include "../ccp_driver/CCP_Frame.h"
#define SERIAL_BUFFERSIZE 200

enum StartByteEnum
{
    no_startbyte = 0,
    Char0xB0 = 1
};


class serial
{
    public:
        serial();
        virtual ~serial();
        int GetFileDes() { return fd; }
        bool open_port();
        void close_port();
        CCP_Frame* AnalyzeBytesRead(void);
        void transmit_CCP_Frame(CCP_Frame& CCP_Msg);
        void set_ptr_CCPDriver(void);
        void Set_device_name(std::string tmp){device_name = tmp;} // TODO removed later
        std::string Get_device_name(void){return device_name;}  // TODO remove later
        void Set_port_number(int tmp){port_number = tmp;}
        int Get_port_number(void){return port_number;}
        int Get_baud_rate(void){return baud_rate;}
        void Set_baud_rate(int tmp){baud_rate = tmp;}
        void Set_serial_mode(std::string tmp){serial_mode = tmp;} // TODO removed later
        std::string Get_serial_mode(void){return serial_mode;}  // TODO remove later
        void Set_StartByte(StartByteEnum tmp){ConfiguredStartByte = tmp;} // TODO removed later
        StartByteEnum Get_StartByte(void){return ConfiguredStartByte;}  // TODO remove later

    protected:
    private:
        int fd; // TODO removed later
        int port_number;
        int baud_rate;
        StartByteEnum ConfiguredStartByte;
        std::string serial_mode;
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
