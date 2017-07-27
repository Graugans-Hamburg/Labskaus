#include "LOG_Container.h"

LOG_Container::LOG_Container()
{
    //ctor
}

LOG_Container::~LOG_Container()
{
    //dtor
}


void LOG_Container::add_new_value(uint32_t var_add,EnumDataType var_type, uint8_t val_u8,
                                int8_t val_i8, uint16_t val_u16, int16_t val_i16, uint32_t val_u32,
                                int32_t val_i32,float val_f32, struct timespec time_point)
{
    bool datarow_found = false;
    uint64_t i = 0;
    ECU_VarLog* tmp_VarLog;

    // Checke ob es schon ein Datenreihe für diese Variable gibt
    for(i = 0; i < log_data_base.size(); i++)
    {
        tmp_VarLog = &log_data_base.at(i);
        if(tmp_VarLog->GetAddress() == var_add)
        {
            datarow_found = true;
            break;
        }
    }
    // Erstellen der Datenreihe falls noch nicht gefunden
    if(datarow_found == false)
    {
        ECU_VarLog* new_VarLog = new(ECU_VarLog);
        new_VarLog->SetAddress(var_add);
        log_data_base.push_back(*new_VarLog);
    }

    tmp_VarLog = &log_data_base.at(i);
    // add the new element

    switch(var_type)
    {
        case type_u8:
            tmp_VarLog->Append_u8(val_u8,time_point);
            break;
        case type_i8:
            tmp_VarLog->Append_s8(val_i8,time_point);
            break;
        case type_u16:
            tmp_VarLog->Append_u16(val_u16,time_point);
            break;
        case type_i16:
            tmp_VarLog->Append_s16(val_i16,time_point);
            break;
        case type_u32:
            tmp_VarLog->Append_u32(val_u32,time_point);
            break;
        case type_i32:
            tmp_VarLog->Append_s32(val_i32,time_point);
            break;
        case type_f32:
            tmp_VarLog->Append_f32(val_f32,time_point);
            break;
        default:
            std::cerr << "Unknown dlsdjfo23u" << std::cerr;
            break;

    }

}


void LOG_Container::VariableLog_export(struct timespec time_measurement_started, std::string log_folder, EnumLogFileName E_Nametype)
{
    /* Determin the date */

    std::stringstream file_name;
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    if(E_Nametype == FileName_date)
    {
        file_name << (now->tm_year + 1900) << '-'
                  << std::setfill('0') << std::setw(2) <<(now->tm_mon + 1) << '-'
                  << std::setfill('0') << std::setw(2) << now->tm_mday << "_"
                  << std::setfill('0') << std::setw(2) << now->tm_hour
                  << std::setfill('0') << std::setw(2) << now->tm_min
                  << std::setfill('0') << std::setw(2) << now->tm_sec
                  << "_log.m";
    }
    if(E_Nametype == FileName_general)
    {
        file_name << "data.m";
    }
    /*
     *Determine the file name
     */

    std::string file_path;

    if(log_folder.empty() == true)
    {
        file_path = file_name.str();
    }
    else
    {
        file_path = log_folder;
        file_path.append("/");
        file_path.append(file_name.str());
    }

    std::ofstream logfile(file_path);
    if ( ! logfile)
    {
        std::cerr << "Logfile could not be opened" << std::endl;
        return;
    }
    /*
     *  Write some header information
     */
    logfile << "%Labskaus measurement data log." << std::endl;
    if (log_data_base.empty())
    {
        logfile << "% Datafile is empty!" << std::endl;
    }
    else
    {
        /*
         *  Log the files. It follows a example how it should look like.
         *
         *   var(1).name = 'darrieus_speed';
         *   var(1).unit = 'omega';
         *   var(1).value = [0,0.1,0.2];
         *   var(1).time = [0,1,2];
         *
         */

         uint64_t idx_i = 0;
         for(idx_i = 0; idx_i < log_data_base.size(); idx_i++)
         {
            uint64_t one_based_idx = idx_i + 1;
            /*  Select the first variable log*/
            ECU_VarLog* tmp_variable_datarow;
            tmp_variable_datarow = &log_data_base.at(idx_i);

            logfile << "var" << "(" << one_based_idx << ").value = [";
            tmp_variable_datarow->plot_values_csv(logfile);
            logfile << "];" << std::endl;

            logfile << "var" << "(" << one_based_idx << ").time  = [";
            tmp_variable_datarow->plot_time_csv(logfile, time_measurement_started);
            logfile << "];" << std::endl;

            logfile << std::endl;
         }
    }
}

void LOG_Container::VariableLog_clear(void)
{
    log_data_base.clear();
}
