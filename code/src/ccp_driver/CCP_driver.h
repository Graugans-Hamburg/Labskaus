#ifndef CCP_DRIVER_H
#define CCP_DRIVER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdint>
#include <string>
#include "../serial/serial.h"
#include "../ccp_driver/CCP_Frame.h"
#include "../ccp_driver/type_definition.h"
#include "../ccp_driver/LOG_Container.h"
#include "../ccp_driver/CCP_Schedular_List_Element.h"


#define NOT_CONNECTED				 0x00
#define CONNECTED					 0x01
#define BYTE_POSITION_CRC            0x01
#define BYTE_POSITION_PID            0x00
#define BYTE_POSITION_CTR            0x02
#define BYTE_POSITION_VERSION        0x03
#define BYTE_POSITION_SUBVERSION     0x04
#define BYTE_POSITION_DATA           0x04
#define MTA_NUMBER_00                0x00
#define MTA_NUMBER_01                0x01
#define FOUR_BYTES                   0x04

/*
 * Table of Commands
 */
#define COMMAND_CONNECT              0x01
#define COMMAND_GET_CCP_VERSION      0x1B
#define COMMAND_EXCHANGE_ID          0x17
#define COMMAND_GET_SEED             0x12
#define COMMAND_UNLOCK               0x13
#define COMMAND_SET_MTA              0x02
#define COMMAND_DNLOAD               0x03
#define COMMAND_DNLOAD_6             0x23
#define COMMAND_UPLOAD               0x04
#define COMMAND_SHORT_UP             0x0F
#define COMMAND_SELECT_CAL_PAGE      0x11
#define COMMAND_GET_DAQ_SIZE         0x14
#define COMMAND_SET_DAQ_PTR          0x15
#define COMMAND_WRITE_DAQ            0x16
#define COMMAND_START_STOP           0x06
#define COMMAND_DISCONNECT           0x07
#define COMMAND_SET_S_STATUS         0x0C
#define COMMAND_GET_S_STATUS         0x0D
#define COMMAND_BUILD_CHKSUM         0x0E
#define COMMAND_CLEAR_MEMORY         0x10
#define COMMAND_PROGRAM              0x18
#define COMMAND_PROGRAM_6            0x22
#define COMMAND_MOVE                 0x19
#define COMMAND_TEST                 0x05
#define COMMAND_GET_ACTIVE_CAL_PAGE  0x09
#define COMMAND_START_STOP_ALL       0x08
#define COMMAND_DIAG_SERVICE         0x20
#define COMMAND_ACTION_SERVICE       0x21
/*
 * Table of Command Return Codes
 */
#define CRC_ACKNOWLEGE               0x00
#define CRC_DAQ_PRCESSOR_OVERLOAD    0x01
#define CRC_COMMAND_PROCESSOR_BUSY   0x10
#define CRC_DAQ_PROCESSOR_BUSY       0x11
#define CRC_INTERNAL_TIMEOUT         0x12
#define CRC_KEY_REQUEST              0x18
#define CRC_SESSION_STATUS_REQUEST   0x19
#define CRC_COLD_START_REQUEST       0x20
#define CRC_CAL_DATA_INIT_REQUEST    0x21
#define CRC_DAQ_LIST_INIT_REQUEST    0x22
#define CRC_CODE_UPDATE_REQUEST      0x23
#define CRC_UNKNOWN_COMMAND          0x30
#define CRC_COMMAND_SYNTAX           0x31
#define CRC_PARAMETER_OUT_OF_RANGE   0x32
#define CRC_ACCESS_DENIED            0x33
#define CRC_OVERLOAD                 0x34
#define CRC_ACCESS_LOCKED            0x35
#define CRC_RESOURCE_FUNC_NOT_AVAIL  0x36


//#define PLOT_COMMUNICATION_TO_TERMINAL

enum DTO_types
{
    DTO_type_CRM = 0xFF,
    DTO_type_Event = 0xFE
};

enum CCP_driver_states
{
    SM_Init,
    SM_Connect,
    SM_CCP_Version,
    SM_Wait,
    SM_read_variable_SetMTA,
    SM_read_variable_DataUpload,
    SM_read_variable_ShortUp,
    SM_calibrate_variable_SetMTA,
    SM_calibrate_variable_DataDNLoad,
};

enum endian
{
    big_endian  = 1,
    little_endian = 0 ,
};

class CCP_driver
{
    public:
        CCP_driver();
        CCP_driver(std::vector<ECU_VarInfo>* ptr_XML_List);
        virtual ~CCP_driver();
        void Set_XMLExtract(std::vector<ECU_VarInfo>* ptr_XML_List);
        void SendCCPFrame();
        void AnalyzeCCPFrame();
        void TxCRO_Connect();
        void TxCRO_GetCCP_Version();
        void TxCRO_SetMTA(uint8_t MTA_number, uint8_t AddressExtention, uint32_t MTA_adress);
        void TxCRO_Upload(uint8_t num_of_bytes);
        void TxCRO_ShortUp(uint8_t number_of_bytes, uint8_t AddressExtention, uint32_t u32_address);
        void TxCRO_Dnload(uint8_t * ptr_data, uint8_t num_of_bytes);
        void Analyze(CCP_Frame& received_CCP_frame);
        void analyze_CRM_Upload(CCP_Frame& received_CCP_frame);
        void ECU_SetECU_MTA_Number1(uint32_t val){ECU_MTA_Number1 = val;}
        void ECU_SetECU_MTA_Number2(uint32_t val){ECU_MTA_Number2 = val;}
        void Set_ECU_station_address(uint16_t val){ECU_station_address = val;}
        uint16_t Get_ECU_station_address(void){return ECU_station_address;}
        void Set_ECU_endianness(endian val){ECU_byte_order = val;}
        endian Get_ECU_endianness(void){return ECU_byte_order;}
        void CCP_drv_state_machine(void);
        void CRO_check_time_out(void);
        void CRO_Tx(CCP_Frame& CCP_Tx_Frame);
        bool open_communication_port(void);
        void close_communication_port(void);
        void periodic_check(void);
        void Set_SMT_req_establish_connection(void){SMT_req_establish_connection = true;}
        void SM_run_state_machine(void);
        void SM_reset_state_machine(void){SM_actl_state = SM_Init;}
        int  GetCCPLogSize(void){return CCP_Msg_Buffer.size();}
        void SetSMI_read_variable_type(EnumDataType val){SMI_read_variable_type = val;}
        void SetSMI_read_variable_address(uint32_t val){SMI_read_variable_address = val;}
        void SetMeasurementStartTime(void);
        struct timespec Get_time_measurement_started(void){return start_time_measurement;}

        void Messagebuffer_export(void);
        void Messagebuffer_clear(void);
        void VariableLog_export(void);
        void SetLogFolder(std::string val){log_folder = val;}
        /* Following functions are only for testing */
        void test_read_variable(void);
        // Public variable
        LOG_Container log_database;
        CCP_Schedular_List_Element* SMI_Actl_ECU_Variable;
        // CCP Treiber
        int addvariable2ActionPlan(ECU_VarInfo& var2add);
        int rmVariableFromActionPlan(std::string var2rm);
        void clearActionPlan(void);
        void addCalibration2ActionPlan(ECU_VarInfo& Cal2Add, int64_t Cal_Int, float Cal_Float);

        void updateSchedular(void);
        void SetNext_variable_address2read(uint32_t val){next_variable_address2read = val;}
        void SetNext_variable_type(EnumDataType val){Next_variable_type = val;}
        bool Get_MessStatus (void){return m_MeasurementActive; };
        void Set_MessStatus2Run (void){m_MeasurementActive = true;};
        void Set_MessStatus2Stop(void){m_MeasurementActive = false;};
        serial SerialPort;

    protected:
    private:
        bool      m_MeasurementActive;
        bool      ECU_Connected; /*Connect Cmd, positive Antwort */
        uint8_t   ECU_CCP_Version_Main;
        uint8_t   ECU_CCP_Version_Release;
        uint32_t  ECU_MTA_Number1; /* Shows the last set position of the MTA1 */
        uint32_t  ECU_MTA_Number2; /* Shows the last set position of the MTA1 */
        uint16_t  ECU_station_address;
        endian    ECU_byte_order;
        endian    PC_byte_order;


        bool Device_Available;
        uint8_t MessageCounter;

        struct timespec CRO_last_request_time;
        struct timespec time_of_last_received_CRM;
        struct timespec start_time_measurement;
        uint8_t   CRO_last_request_MessageCounter;
        uint8_t   CRO_last_CRO_Command_type;
        bool      CRO_waiting_for_request;
        uint8_t   CRM_ErrorCode_last_received;

        uint16_t  SM_actl_state;
        bool      SM_enterleave_state;
        bool      SMT_req_establish_connection; /*State machine transition*/
        bool      SMT_read_variable; /*When this Transition is set to true and the information from
        the variables SMI_read_variable_type and SMI_read_variable_address will used. They define
        the datatype and the address. */
        bool            SMT_calibrate_variable;
        EnumDataType    SMI_read_variable_type;
        uint32_t        SMI_read_variable_address;
        uint8_t         SMI_read_address_extention;
        bool            SMI_read_variable_successfull;

        uint8_t   SMI_read_variable_uint8;
        int8_t    SMI_read_variable_sint8;
        uint16_t  SMI_read_variable_uint16;
        int16_t   SMI_read_variable_sint16;
        uint32_t  SMI_read_variable_uint32;
        int32_t   SMI_read_variable_sint32;
        float     SMI_read_variable_f32;
        double    SMI_read_variable_f64;

        std::vector<CCP_Frame> CCP_Msg_Buffer;
        std::string log_folder;
        std::vector<ECU_VarInfo>* m_XML_list;
        std::vector<CCP_Schedular_List_Element> ActionTable;
        uint32_t   next_variable_address2read;
        EnumDataType Next_variable_type;

};

#endif // CCP_DRIVER_H
