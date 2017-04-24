#include "CCP_CRO_Schedular.h"

CCP_CRO_Schedular::CCP_CRO_Schedular()
{
    //ctor
}

CCP_CRO_Schedular::~CCP_CRO_Schedular()
{
    //dtor
}

void CCP_CRO_Schedular::addvariable2ActionPlan(ECU_variable& var2add)
{
    bool add_variable2list = false;

    /*What to do if the list is empty?, Add a new element */
    if(ActionTable.empty())
    {
        add_variable2list = true;
    }
    else
    {
    /* check something else */
        CCP_Schedular_List_Element* ptr_tmp_list_element;
        for(uint32_t idx = 0 : ActionTable.size())
        {
            ptr_tmp_list_element = &ActionTable.at(idx);
            if(ptr_tmp_list_element->GetAddress() != var2add.GetAddress())
            {
                add_variable2list = true;
            }
        }
    }

    if(add_variable2list == true)
    {
        CCP_Schedular_List_Element* tmp = new(CCP_Schedular_List_Element);
        tmp->SetAddress(var2add.GetAddress());
        tmp->SetDataType(var2add.GetDataType());
        ActionTable.push_back(*tmp);
    }
}
