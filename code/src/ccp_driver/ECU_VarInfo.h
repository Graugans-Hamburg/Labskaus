#ifndef ECU_VARINFO_H
#define ECU_VARINFO_H

#include <string>
#include "type_definition.h"
#include <stdint.h>
#include <cstring>

class ECU_VarInfo
{
    public:
        ECU_VarInfo();
        virtual ~ECU_VarInfo();
        uint32_t GetAddress() { return Address; }
        void SetAddress(uint32_t val) { Address = val; }
        EnumDataType GetDataType() { return Datatype; }
        void SetDataType(EnumDataType val) { Datatype = val; }
        std::string GetName() { return Name; }
        void SetName(std::string val) { Name = val; }
        std::string GetUnit() { return Unit; }
        void SetUnit(std::string val) { Unit = val; }
        std::string GetDescription() { return Description; }
        void SetDescription(std::string val) { Description = val; }
        void ParseDatatyp(const char *);
        void ParseDatatyp(std::string str);
        std::string GetDatatypAsString(void);
        bool MinValueValid(void) { return Min_Value_Valid; };
        bool MaxValueValid(void) { return Max_Value_Valid; };
        void SetMinValue_Fl(float val) { Min_Value_Float = val; }
        float GetMinValue_F() { return Min_Value_Float; }
        void SetMaxValue_Fl(float val) { Max_Value_Float = val; }
        float GetMaxValue_F() { return Max_Value_Float; }
        void SetMaxValue_Int(float val) { Max_Value_Int = val; }
        float GetMaxValue_Int() { return Max_Value_Int; }
        void SetMinValue_Int(int64_t val) { Max_Value_Int = val; }
        int64_t GetMinValue_Int() { return Max_Value_Int; }
    protected:
    private:
        uint32_t Address;
        EnumDataType Datatype;
        float Min_Value_Float;
        float Max_Value_Float;
        int64_t Min_Value_Int;
        int64_t Max_Value_Int;
        bool Max_Value_Valid;
        bool Min_Value_Valid;
        std::string Name;
        std::string Description;
        std::string Unit;
};

#endif // ECU_VARINFO_H
