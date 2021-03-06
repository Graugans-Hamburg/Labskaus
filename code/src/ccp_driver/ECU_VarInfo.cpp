#include "ECU_VarInfo.h"

ECU_VarInfo::ECU_VarInfo()
{
    //ctor
    Address = 0;
    Datatype = type_unknown;
    Min_Value_Valid = false;
    Max_Value_Valid = false;
}

ECU_VarInfo::~ECU_VarInfo()
{
    //dtor
}

void ECU_VarInfo::ParseDatatyp(const char * Cstr)
{
    if(strcmp(Cstr,"uint8") == 0)
    {
        SetDataType(type_u8);
        return;
    }
    if(strcmp(Cstr,"int8") == 0)
    {
        SetDataType(type_i8);
        return;
    }
    if(strcmp(Cstr,"uint16") == 0)
    {
        SetDataType(type_u16);
        return;
    }
    if(strcmp(Cstr,"int16") == 0)
    {
        SetDataType(type_i16);
        return;
    }
    if(strcmp(Cstr,"uint32") == 0)
    {
        SetDataType(type_u32);
        return;
    }
    if(strcmp(Cstr,"int32") == 0)
    {
        SetDataType(type_i32);
        return;
    }

    if(strcmp(Cstr,"float") == 0)
    {
        SetDataType(type_f32);
        return;
    }
    if(strcmp(Cstr,"float") == 0)
    {
        SetDataType(type_f64);
        return;
    }

    // defualt
    SetDataType(type_unknown);
}

void ECU_VarInfo::ParseDatatyp(std::string str)
{
    if(str.compare("uint8") == 0)
    {
        SetDataType(type_u8);
        return;
    }
    if(str.compare("int8") == 0)
    {
        SetDataType(type_i8);
        return;
    }
    if(str.compare("uint16") == 0)
    {
        SetDataType(type_u16);
        return;
    }
    if(str.compare("int16") == 0)
    {
        SetDataType(type_i16);
        return;
    }
    if(str.compare("uint32") == 0)
    {
        SetDataType(type_u32);
        return;
    }
    if(str.compare("int32") == 0)
    {
        SetDataType(type_i32);
        return;
    }

    if(str.compare("float") == 0)
    {
        SetDataType(type_f32);
        return;
    }
    if(str.compare("float") == 0)
    {
        SetDataType(type_f64);
        return;
    }

    // default
    SetDataType(type_unknown);
}

std::string ECU_VarInfo::GetDatatypAsString(void)
{
    std::string typeAsString;
    switch(Datatype)
    {
        case type_f32:
            typeAsString = "float";
            break;
        case type_u8:
            typeAsString = "uint8_t";
            break;
        case type_i8:
            typeAsString = "int8_t";
            break;
        case type_u16:
            typeAsString = "uint16_t";
            break;
        case type_i16:
            typeAsString = "int16_t";
            break;
        case type_u32:
            typeAsString = "uint32_t";
            break;
        case type_i32:
            typeAsString = "int32_t";
            break;
        default:
            typeAsString = "unkown";
            break;
    }
    return typeAsString;
}
