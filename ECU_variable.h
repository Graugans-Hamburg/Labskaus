#ifndef ECU_VARIABLE_H
#define ECU_VARIABLE_H
#include <string>
#include "type_definition.h"


class ECU_variable
{
    public:
        ECU_variable();
        virtual ~ECU_variable();
        unsigned int GetAddress() { return Address; }
        void SetAddress(unsigned int val) { Address = val; }
        EnumDataType GetDataType() { return Datatype; }
        void SetDataType(EnumDataType val) { Datatype = val; }
        std::string GetName() { return Name; }
        void SetName(std::string val) { Name = val; }
        std::string GetDescription() { return Description; }
        void SetDescription(std::string val) { Description = val; }
        void ParseDatatyp(const char *);
        std::string GetDatatypAsString(void);
    protected:
    private:
        unsigned int Address;
        EnumDataType Datatype;
        std::string Name;
        std::string Description;
};

#endif // ECU_VARIABLE_H
