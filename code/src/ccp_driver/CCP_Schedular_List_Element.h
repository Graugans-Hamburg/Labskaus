#ifndef CCP_SCHEDULAR_LIST_ELEMENT_H
#define CCP_SCHEDULAR_LIST_ELEMENT_H

#include "ECU_VarInfo.h"
#include <time.h>

enum EnumMode
{
    E_periodic_sample,
    E_Calibration
};


class CCP_Schedular_List_Element : public ECU_VarInfo
{
    public:
        CCP_Schedular_List_Element();
        virtual ~CCP_Schedular_List_Element();
        void SetSampleTime(uint32_t val){SampleTime_ms = val;}
        void SetMode_Polling(void){AquisitionMode = E_periodic_sample;}
        void SetMode_Calibration(void){AquisitionMode = E_Calibration;}
        void SetLastRequest_2_now(void);
        void SetCalValue_Int(int64_t val){CalValue_Int = val;}
        void SetCalValue_Float(float val){CalValue_Float = val;}
        EnumMode GetAquisitionMode(void){return AquisitionMode;}
        int64_t GetPrio(void);
        int64_t GetCalValue_Int(void){return CalValue_Int;}
        float GetCalValue_Float(void){return CalValue_Float;}
    protected:
    private:
        uint32_t SampleTime_ms;
        EnumMode AquisitionMode;
        struct timespec last_request;
        int64_t CalValue_Int;
        float CalValue_Float;

};

#endif // CCP_SCHEDULAR_LIST_ELEMENT_H
