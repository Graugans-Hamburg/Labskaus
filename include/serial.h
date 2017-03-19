#ifndef SERIAL_H
#define SERIAL_H


class serial
{
    public:
        serial();
        virtual ~serial();
        int GetFileDes() { return fd; }
        void open_port(int port);
        void close_port();
        void bytes_read(void);
    protected:
    private:
        int fd;
        unsigned char serial_input_buffer[200];
        unsigned char serial_output_buffer[200];
};

#endif // SERIAL_H
