#include "History_Log.h"

History_Log::History_Log()
{
    //ctor
}

History_Log::~History_Log()
{
    //dtor
}


void History_Log::add_new_value(uint32_t var_add,EnumDataType var_type, uint8_t val_u8,
                                int8_t val_i8, uint16_t val_u16, int16_t val_i16, uint32_t val_u32,
                                int32_t val_i32, struct timespec time_point)
{
    bool datarow_found = false;
    uint64_t i = 0;
    Data_Row* tmp_data_row;

    // Checke ob es schon ein Datenreihe für diese Variable gibt
    for(i = 0; i < log_data_base.size(); i++)
    {
        tmp_data_row = &log_data_base.at(i);
        if(tmp_data_row->GetVariableAdress() == var_add)
        {
            datarow_found = true;
            break;
        }
    }
    // Erstellen der Datenreihe falls noch nicht gefunden
    if(datarow_found == false)
    {
        Data_Row* new_variablen_row = new(Data_Row);
        new_variablen_row->SetVariableAdress(var_add);
        log_data_base.push_back(*new_variablen_row);
    }

    tmp_data_row = &log_data_base.at(i);
    // add the new element

    switch(var_type)
    {
        case type_u8:
            tmp_data_row->Append_u8(val_u8,time_point);
            break;
        case type_i8:
            tmp_data_row->Append_s8(val_i8,time_point);
            break;
        case type_u16:
            tmp_data_row->Append_u16(val_u16,time_point);
            break;
        case type_i16:
            tmp_data_row->Append_s16(val_i16,time_point);
            break;
        case type_u32:
            tmp_data_row->Append_u32(val_u32,time_point);
            break;
        case type_i32:
            tmp_data_row->Append_s32(val_i32,time_point);
            break;
        default:
            std::cerr << "Unknown dlsdjfo23u" << std::cerr;
            break;

    }

}
