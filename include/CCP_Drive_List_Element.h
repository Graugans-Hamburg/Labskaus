#ifndef CCP_DRIVE_LIST_ELEMENT_H
#define CCP_DRIVE_LIST_ELEMENT_H

#include "../ECU_variable.h"

enum data_acquisition_type
{
    one_time_sample,
    periodic_poll,
    one_time_calibration,
};


class CCP_Drive_List_Element : public ECU_variable
{
    public:
        CCP_Drive_List_Element();
        virtual ~CCP_Drive_List_Element();
    protected:
    private:
        unsigned int SampleTime_ms;
        timer_t Time_till_next_DA;

};

#endif // CCP_DRIVE_LIST_ELEMENT_H
