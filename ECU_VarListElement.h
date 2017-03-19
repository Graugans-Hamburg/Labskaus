#ifndef ECU_VARLISTELEMENT_H
#define ECU_VARLISTELEMENT_H

#include "ECU_variable.h"


class ECU_VarListElement : public ECU_variable
{
    public:
        ECU_VarListElement();
        virtual ~ECU_VarListElement();
        ECU_VarListElement* GetnxtVarListElement() { return nxtVarListElement; }
        void SetnxtVarListElement(ECU_VarListElement * val) { nxtVarListElement = val; }
        ECU_VarListElement* GetpreVarListElement() { return preVarListElement; }
        void SetpreVarListElement(ECU_VarListElement * val) { preVarListElement = val; }
        int GetPostnListElemnt() { return PostnListElemnt; }
        void SetPostnListElemnt(int val) { PostnListElemnt = val; }
        void addListElement(ECU_variable* src);
        ECU_VarListElement* getPtr2ListElement(int Position);
    protected:
    private:
        ECU_VarListElement* nxtVarListElement;
        ECU_VarListElement* preVarListElement;
        int PostnListElemnt;
};

#endif // ECU_VARLISTELEMENT_H
