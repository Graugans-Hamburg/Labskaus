#include "ECU_VarLog.h"

ECU_VarLog::ECU_VarLog()
{
    //ctor
}

ECU_VarLog::~ECU_VarLog()
{
    //dtor
}

void ECU_VarLog::Append_u8(uint8_t val, struct timespec time_point)
{
    datarow_u8.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::Append_s8(int8_t val, struct timespec time_point)
{

    datarow_s8.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::Append_u16(uint16_t val, struct timespec time_point)
{
    datarow_u16.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::Append_s16(int16_t val, struct timespec time_point)
{
    datarow_s16.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::Append_u32(uint32_t val, struct timespec time_point)
{
    datarow_u32.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::Append_s32(int32_t val, struct timespec time_point)
{
    datarow_s32.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::Append_f32(float val, struct timespec time_point)
{
    datarow_f32.push_back(val);
    timerow.push_back(time_point);
}

void ECU_VarLog::plot_values_csv(std::ofstream& logfile)
{
     /*
      * number_of_non_empty_structs beinhaltet die Anzahl der nicht leeren strukturen. Es
      * darf nur eine Struktur mit Werte gefüllt sein. Alles andere wäre ein Fehler.
      */
    uint8_t number_of_non_empty_structs = 0;

    if(!datarow_u8.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_u8.size(); idx_i++)
        {
            logfile << std::to_string(datarow_u8.at(idx_i));
            if(idx_i < datarow_u8.size() -1)
            {
                 logfile << ",";
            }
        }
    }
    if(!datarow_s8.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_s8.size(); idx_i++)
        {
            logfile << std::to_string(datarow_s8.at(idx_i));
            if(idx_i < datarow_s8.size()-1 )
            {
                 logfile << ",";
            }
        }
    }
    if(!datarow_u16.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_u16.size(); idx_i++)
        {
            logfile << std::to_string(datarow_u16.at(idx_i));
            if(idx_i < datarow_u16.size()-1)
            {
                 logfile << ",";
            }
        }
    }
    if(!datarow_s16.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_s16.size(); idx_i++)
        {
            logfile << std::to_string(datarow_s16.at(idx_i));
            if(idx_i < datarow_s16.size()-1)
            {
                 logfile << ",";
            }
        }
    }
    if(!datarow_u32.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_u32.size(); idx_i++)
        {
            logfile << std::to_string(datarow_u32.at(idx_i));
            if(idx_i < datarow_u32.size()-1)
            {
                 logfile << ",";
            }
        }
    }
    if(!datarow_s32.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_s32.size(); idx_i++)
        {
            logfile << std::to_string(datarow_s32.at(idx_i));
            if(idx_i < datarow_s32.size()-1)
            {
                 logfile << ",";
            }
        }
    }
    if(!datarow_f32.empty())
    {
        number_of_non_empty_structs++;
        for(uint64_t idx_i = 0; idx_i < datarow_f32.size(); idx_i++)
        {
            logfile << std::to_string(datarow_f32.at(idx_i));
            if(idx_i < datarow_f32.size()-1)
            {
                 logfile << ",";
            }
        }
    }
    if (number_of_non_empty_structs != 1)
    {
        std::cerr << "Datarow (one variable) has been filled more than one datatype"<< std::endl;
    }
}


void ECU_VarLog::plot_time_csv(std::ofstream& logfile, struct timespec time_measurement_started)
{
     /*
      * number_of_non_empty_structs beinhaltet die Anzahl der nicht leeren strukturen. Es
      * darf nur eine Struktur mit Werte gefüllt sein. Alles andere wäre ein Fehler.
      */
    if(timerow.empty())
    {
        std::cerr << "No time is saved inside the datarow";
    }
    for(uint64_t idx_i = 0; idx_i < timerow.size(); idx_i++)
    {
        struct timespec* ptr_timespec= &timerow.at(idx_i);

        if (ptr_timespec->tv_nsec - time_measurement_started.tv_nsec >= 0)
        {
        logfile << std::to_string(ptr_timespec->tv_sec - time_measurement_started.tv_sec) << "."
                << std::setfill('0') << std::setw(9)<< std::to_string(ptr_timespec->tv_nsec - time_measurement_started.tv_nsec);
        }
        else
        {
        logfile << std::to_string(ptr_timespec->tv_sec - time_measurement_started.tv_sec -1) << "."
                << std::setfill('0') << std::setw(9)<< std::to_string(1000000000 - time_measurement_started.tv_nsec + ptr_timespec->tv_nsec);
        }



        if(idx_i < timerow.size() - 1 )
        {
             logfile << ",";
        }

    }
}

