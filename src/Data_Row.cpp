#include "Data_Row.h"

Data_Row::Data_Row()
{
    //ctor
}

Data_Row::~Data_Row()
{
    //dtor
}

void Data_Row::Append_u8(uint8_t val, struct timespec time_point)
{
    struct measure_point_u8 tmp;
    tmp.time = time_point;
    tmp.value = val;
    datarow_u8.push_back(tmp);
}

void Data_Row::Append_s8(int8_t val, struct timespec time_point)
{
    struct measure_point_s8 tmp;
    tmp.time = time_point;
    tmp.value = val;
    datarow_s8.push_back(tmp);
}

void Data_Row::Append_u16(uint16_t val, struct timespec time_point)
{
    struct measure_point_u16 tmp;
    tmp.time = time_point;
    tmp.value = val;
    datarow_u16.push_back(tmp);
}

void Data_Row::Append_s16(int16_t val, struct timespec time_point)
{
    struct measure_point_s16 tmp;
    tmp.time = time_point;
    tmp.value = val;
    datarow_s16.push_back(tmp);
}

void Data_Row::Append_u32(uint32_t val, struct timespec time_point)
{
    struct measure_point_u32 tmp;
    tmp.time = time_point;
    tmp.value = val;
    datarow_u32.push_back(tmp);
}

void Data_Row::Append_s32(int32_t val, struct timespec time_point)
{
    struct measure_point_s32 tmp;
    tmp.time = time_point;
    tmp.value = val;
    datarow_s32.push_back(tmp);
}
