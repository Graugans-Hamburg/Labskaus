#ifndef CCP_SCHEDULAR_LIST_ELEMENT_H
#define CCP_SCHEDULAR_LIST_ELEMENT_H

#include "../ECU_variable.h"

enum EnumMode
{
    periodic_sample,
    one_time_shot
};


class CCP_Schedular_List_Element : public ECU_variable
{
    public:
        CCP_Schedular_List_Element();
        virtual ~CCP_Schedular_List_Element();
    protected:
    private:
        uint32_t SampleTime_ms;
        EnumMode AquisitionMode;

};

#endif // CCP_SCHEDULAR_LIST_ELEMENT_H
