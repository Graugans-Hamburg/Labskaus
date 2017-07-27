#ifndef LOG_CONTAINER_H_
#define LOG_CONTAINER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include "ECU_VarLog.h"
#include "type_definition.h"

enum EnumLogFileName
{
    FileName_general,
    FileName_date
};

class LOG_Container
{
    public:
        LOG_Container();
        void Set_XMLListReference(std::vector<ECU_VarInfo>* ptr_XML_List);
        virtual ~LOG_Container();
        void add_new_value(uint32_t var_add,EnumDataType var_type,
                           uint8_t val_u8,
                            int8_t val_i8,
                          uint16_t val_u16,
                           int16_t val_i16,
                          uint32_t val_u32,
                           int32_t val_i32,
                           float   val_f32,
                           struct timespec time_point);
        void Update_ECUInfo(ECU_VarLog& loc_ECU_VarLog, uint32_t Address);
        uint32_t GetNmOfLogVariables(void){return log_data_base.size();}
        void VariableLog_export(struct timespec time_measurement_started, std::string log_folder, EnumLogFileName E_Nametype);
        void VariableLog_clear(void);
    protected:
    private:
        std::vector<ECU_VarInfo>* m_XML_list;
        std::vector<ECU_VarLog> log_data_base;

};

#endif // LOG_CONTAINER_H_
