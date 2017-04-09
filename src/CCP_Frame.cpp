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
    CCPFrameTime = time(0);
}

void CCP_Frame::setCCPFrameTime(time_t in_FrameTime)
{
    CCPFrameTime = in_FrameTime;
}
