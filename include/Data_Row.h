#ifndef DATA_ROW_H
#define DATA_ROW_H


#include <time.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "type_definition.h"

struct measure_point_u8
{
    struct timespec time;
    uint8_t value;
};

struct measure_point_s8
{
    struct timespec time;
    int8_t value;
};

struct measure_point_u16
{
    struct timespec time;
    uint16_t value;
};

struct measure_point_s16
{
    struct timespec time;
    int8_t value;
};

struct measure_point_u32
{
    struct timespec time;
    uint16_t value;
};

struct measure_point_s32
{
    struct timespec time;
    uint16_t value;
};

class Data_Row
{
    public:
        Data_Row();
        virtual ~Data_Row();
        void SetVariableAdress(uint32_t val){variable_address = val;}
        uint32_t GetVariableAdress(void){return variable_address;}
        void Append_u8(uint8_t val, struct timespec time_point);
        void Append_s8(int8_t val, struct timespec time_point);
        void Append_u16(uint16_t val, struct timespec time_point);
        void Append_s16(int16_t val, struct timespec time_point);
        void Append_u32(uint32_t val, struct timespec time_point);
        void Append_s32(int32_t val, struct timespec time_point);
    protected:
    private:
        std::string variable_name;
        uint32_t variable_address;
        EnumDataType data_type;
        std::vector<measure_point_u8> datarow_u8;
        std::vector<measure_point_s8> datarow_s8;
        std::vector<measure_point_u16> datarow_u16;
        std::vector<measure_point_s16> datarow_s16;
        std::vector<measure_point_u32> datarow_u32;
        std::vector<measure_point_s32> datarow_s32;
};

#endif // DATA_ROW_H
