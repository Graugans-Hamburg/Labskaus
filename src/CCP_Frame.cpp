#include "CCP_Frame.h"

CCP_Frame::CCP_Frame()
{
    //ctor
    Data[0] = 0;
    Data[1] = 0;
    Data[2] = 0;
    Data[3] = 0;
    Data[4] = 0;
    Data[5] = 0;
    Data[6] = 0;
    Data[7] = 0;
}

CCP_Frame::~CCP_Frame()
{
    //dtor
}


void CCP_Frame::setCCPFrameTime(void)
{
     if( clock_gettime(CLOCK_REALTIME,&CCPFrameTime))
     {
        std::cerr << "Die Systemzeit konnte nicht geholt werden" << std::endl;
     }
     /*  Only if you want to display when the time had been received
     else
     {
        std::cout << "Empfangen " << std::dec <<CCPFrameTime.tv_sec << ","
                  << std::dec << std::setw(9) << std::setfill('0') << CCPFrameTime.tv_nsec << " Sekunden" << std::endl;
     }
     */
}
