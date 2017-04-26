#ifndef CCP_CRO_SCHEDULAR_H
#define CCP_CRO_SCHEDULAR_H

#include <vector>
#include <iostream>

#include "CCP_Schedular_List_Element.h"
//#include "CCP_driver.h"


class CCP_CRO_Schedular
{
    public:
        CCP_CRO_Schedular();
        virtual ~CCP_CRO_Schedular();
        void addvariable2ActionPlan(ECU_variable& var2add);
        void updateSchedular(void);
        void SetNext_variable_address2read(uint32_t val){next_variable_address2read = val;}
        void SetNext_variable_type(EnumDataType val){Next_variable_type = val;}
        //void Set_Ptr2CCPDriver(CCP_driver* val){ptr_CCP_driver = val;}
    protected:
    private:
        std::vector<CCP_Schedular_List_Element> ActionTable;
        //CCP_driver ptr_CCP_driver;

        //only for testing
         uint32_t   next_variable_address2read;
         EnumDataType Next_variable_type;
};

#endif // CCP_CRO_SCHEDULAR_H
