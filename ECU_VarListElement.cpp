#include "ECU_VarListElement.h"

ECU_VarListElement::ECU_VarListElement()
{
    //ctor
    nxtVarListElement = NULL;
    preVarListElement = NULL;
    PostnListElemnt = 0;
}

ECU_VarListElement::~ECU_VarListElement()
{
    //dtor
}

void ECU_VarListElement::addListElement(ECU_variable* src)
{
    ECU_VarListElement* Ptr2FindEndOfList=this;
    while(Ptr2FindEndOfList->GetnxtVarListElement() != NULL)
    {
        Ptr2FindEndOfList = Ptr2FindEndOfList->GetnxtVarListElement();
    }

    ECU_VarListElement* NewListElemnt;

    if(Ptr2FindEndOfList->GetpreVarListElement() == NULL
       && Ptr2FindEndOfList->GetPostnListElemnt() == 0)
    {
        // Do nothing the list is empty because it has no previous element
        NewListElemnt = Ptr2FindEndOfList;
        NewListElemnt->SetPostnListElemnt(1);
    }
    else
    {
        // Create a new list element and attach it to the list
        NewListElemnt = new ECU_VarListElement();
        Ptr2FindEndOfList->SetnxtVarListElement(NewListElemnt);
        NewListElemnt->SetpreVarListElement(Ptr2FindEndOfList);
        NewListElemnt->SetPostnListElemnt(Ptr2FindEndOfList->GetPostnListElemnt() + 1);
    }

    // Update the List with the new informations
    NewListElemnt->SetName(src->GetName());
    NewListElemnt->SetAddress(src->GetAddress());
    NewListElemnt->SetDescription(src->GetDescription());
    NewListElemnt->SetDataType(src->GetDataType());


}


ECU_VarListElement* ECU_VarListElement::getPtr2ListElement(int RequestedPostion)
{
    ECU_VarListElement* Ptr2FindEndOfList=this;
    while(Ptr2FindEndOfList->GetPostnListElemnt() != RequestedPostion)
    {
        if(Ptr2FindEndOfList->GetnxtVarListElement() == NULL)
        {
            return NULL;
        }
        Ptr2FindEndOfList = Ptr2FindEndOfList->GetnxtVarListElement();
    }
        return Ptr2FindEndOfList;
}
