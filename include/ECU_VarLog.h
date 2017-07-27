#ifndef ECU_VARLOG_H
#define ECU_VARLOG_H


#include <time.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "type_definition.h"
#include "ECU_VarInfo.h"



class ECU_VarLog : public ECU_VarInfo
{
    public:
        ECU_VarLog();
        virtual ~ECU_VarLog();
        void plot_values_csv(std::ofstream&);
        void plot_time_csv(std::ofstream& logfile, struct timespec time_measurement_started);
        void Append_u8(uint8_t val, struct timespec time_point);
        void Append_s8(int8_t val, struct timespec time_point);
        void Append_u16(uint16_t val, struct timespec time_point);
        void Append_s16(int16_t val, struct timespec time_point);
        void Append_u32(uint32_t val, struct timespec time_point);
        void Append_s32(int32_t val, struct timespec time_point);
        void Append_f32(float val, struct timespec time_point);
    protected:
    private:
        EnumDataType data_type;
        std::vector<uint8_t> datarow_u8;
        std::vector<int8_t> datarow_s8;
        std::vector<uint16_t> datarow_u16;
        std::vector<int16_t> datarow_s16;
        std::vector<uint32_t> datarow_u32;
        std::vector<int32_t> datarow_s32;
        std::vector<float> datarow_f32;
        std::vector<struct timespec> timerow;
};

#endif // ECU_VARLOG_H
