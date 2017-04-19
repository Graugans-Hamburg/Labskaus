#ifndef HISTORY_LOG_H
#define HISTORY_LOG_H

#include <iostream>
#include <fstream>
#include "Data_Row.h"
#include "type_definition.h"


class History_Log
{
    public:
        History_Log();
        virtual ~History_Log();
        void add_new_value(uint32_t var_add,EnumDataType var_type,
                           uint8_t val_u8,
                            int8_t val_i8,
                          uint16_t val_u16,
                           int16_t val_i16,
                          uint32_t val_u32,
                           int32_t val_i32,
                           struct timespec time_point);
        uint32_t GetNmOfLogVariables(void){return log_data_base.size();}
        void saveLogFile(struct timespec time_measurement_started);
    protected:
    private:
        std::vector<Data_Row> log_data_base;

};

#endif // HISTORY_LOG_H
