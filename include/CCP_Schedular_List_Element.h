#ifndef CCP_SCHEDULAR_LIST_ELEMENT_H
#define CCP_SCHEDULAR_LIST_ELEMENT_H

#include "../ECU_variable.h"
#include <time.h>

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
        void SetSampleTime(uint32_t val){SampleTime_ms = val;}
        void SetMode_Polling(void){AquisitionMode = periodic_sample;}
        void SetLastRequest_2_now(void);
        int64_t GetPrio(void);
    protected:
    private:
        uint32_t SampleTime_ms;
        EnumMode AquisitionMode;
        struct timespec last_request;

};

#endif // CCP_SCHEDULAR_LIST_ELEMENT_H
