#ifndef CCP_CRO_SCHEDULAR_H
#define CCP_CRO_SCHEDULAR_H

#include <vector>
#include "CCP_Schedular_List_Element.h"


class CCP_CRO_Schedular
{
    public:
        CCP_CRO_Schedular();
        virtual ~CCP_CRO_Schedular();
        void addvariable2ActionPlan(ECU_variable& var2add);
    protected:
    private:
        std::vector<CCP_Schedular_List_Element> ActionTable;
};

#endif // CCP_CRO_SCHEDULAR_H
