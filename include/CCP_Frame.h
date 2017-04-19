#ifndef CCP_FRAME_H
#define CCP_FRAME_H

#include <ctime>
#include <time.h>
#include <iostream>
#include <iomanip>

enum Enum_direction
{
    dir_unknown,
    dir_Tx,
    dir_Rx
};


class CCP_Frame
{
    public:
        CCP_Frame();
        virtual ~CCP_Frame();
        void SetByte1(unsigned char val) {Data[0] = val;}
        void SetByte2(unsigned char val) {Data[1] = val;}
        void SetByte3(unsigned char val) {Data[2] = val;}
        void SetByte4(unsigned char val) {Data[3] = val;}
        void SetByte5(unsigned char val) {Data[4] = val;}
        void SetByte6(unsigned char val) {Data[5] = val;}
        void SetByte7(unsigned char val) {Data[6] = val;}
        void SetByte8(unsigned char val) {Data[7] = val;}
        unsigned char GetByte1(void){return Data[0];}
        unsigned char GetByte2(void){return Data[1];}
        unsigned char GetByte3(void){return Data[2];}
        unsigned char GetByte4(void){return Data[3];}
        unsigned char GetByte5(void){return Data[4];}
        unsigned char GetByte6(void){return Data[5];}
        unsigned char GetByte7(void){return Data[6];}
        unsigned char GetByte8(void){return Data[7];}
        void setCCPFrameTime(void);
        void SetCCPDirection_Tx(void){direction = dir_Tx;}
        void SetCCPDirection_Rx(void){direction = dir_Rx;}
        Enum_direction GetCCPDirection(void){return direction;}
        time_t getCCPFrameTime(void){return CCPFrameTime.tv_sec;}
        struct timespec GetCCPFrameTimestruct(void){return CCPFrameTime;}
    protected:
    private:
        unsigned char Data[8];
        Enum_direction direction;
        struct timespec CCPFrameTime;

};

#endif // CCP_FRAME_H
