#include "CCP_Schedular_List_Element.h"

CCP_Schedular_List_Element::CCP_Schedular_List_Element()
{
    //ctor
}

CCP_Schedular_List_Element::~CCP_Schedular_List_Element()
{
    //dtor
}

void CCP_Schedular_List_Element::SetLastRequest_2_now(void)
{
    clock_gettime(CLOCK_REALTIME,&last_request);
}


int64_t CCP_Schedular_List_Element::GetPrio(void)
{

    struct timespec tmp_time_now;
    clock_gettime(CLOCK_REALTIME,&tmp_time_now);

    /* Targetdate die Zeit in Sekunden*/
    int64_t prio_ms = tmp_time_now.tv_sec * 1000 - last_request.tv_sec * 1000;
    prio_ms = prio_ms + (tmp_time_now.tv_nsec / 1000000) - (last_request.tv_nsec / 1000000 );
    prio_ms = prio_ms - SampleTime_ms;

    return prio_ms;
}
