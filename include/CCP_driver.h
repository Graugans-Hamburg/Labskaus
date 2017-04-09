#ifndef CCP_DRIVER_H
#define CCP_DRIVER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "serial.h"
#include "CCP_Frame.h"
#include "CCP_Drive_List_Element.h"

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


#define PLOT_COMMUNICATION_TO_TERMINAL TRUE


class CCP_driver
{
    public:
        CCP_driver();
        virtual ~CCP_driver();
        void SendCCPFrame();
        void AnalyzeCCPFrame();
        void Connect(serial* Serial_Port);
        void Analyze(CCP_Frame& recieved_CCP_frame);
        void CCP_drv_state_machine(void);
    protected:
    private:
        bool CommunicationChannel_Active;
        bool Device_Available;
        unsigned char MessageCounter;
        std::vector<CCP_Drive_List_Element> Action_Plan;
        std::vector<CCP_Frame> CCP_Msg_Buffer;
};

#endif // CCP_DRIVER_H
