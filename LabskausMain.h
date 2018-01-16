/***************************************************************
 * Name:      LabskausMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2017-03-01
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef LABSKAUSMAIN_H
#define LABSKAUSMAIN_H

#include <streambuf>
#include <vector>
#include "LabskausApp.h"
#include "ECU_VarInfo.h"
#include "CCP_driver.h"
#include "wx/timer.h"
#include "GUIFrame.h"

enum // Timer events IDs
{
    Rec_Timer = wxID_HIGHEST,
    DAL_Timer = wxID_HIGHEST -1,
};

const int interval_state_machine_ms = 10;
const int data_acq_list_timer_ms = 500;

class LabskausFrame: public GUIFrame
{
    public:
        LabskausFrame(wxFrame *frame);
        std::vector<ECU_VarInfo> XML_list;
        uint64_t m_next_free_row;
        void updateMeasListValues(void);
        wxTimer* recTimer; // declaration of Timer object
        wxTimer* data_acquisition_timer;
        CCP_driver* CCP_Master;
        std::string ECU_XML_full_Path;
        std::string ECU_XML_file_name;
        std::string LOG_dir;
        ~LabskausFrame();

    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnOpenDocumentation( wxCommandEvent& event);
        virtual void open_load_dialog(wxCommandEvent& event);
        virtual void open_load_config_dialog(wxCommandEvent &event);
        virtual void open_save_config_dialog(wxCommandEvent &event);
        virtual void open_log_dialog(wxCommandEvent& event);
        virtual void VarListSelected(wxCommandEvent& event);
        virtual void EventOpenSerial(wxCommandEvent& event);
        virtual void EventCloseSerial(wxCommandEvent& event);
        virtual void EventOpenComSettings( wxCommandEvent& event );
        virtual void OnRecTimer(wxTimerEvent& event);
        virtual void DA_List_Timer(wxTimerEvent& event);
        virtual void EventAddVar2List(wxCommandEvent &event);
        virtual void VarListKeyPressed(wxKeyEvent& event);
        virtual void EventAddCalVal2List(wxCommandEvent &event);
        virtual void EventMeaListKeyPres( wxKeyEvent& event );
        virtual void EventMeaListKeyUp( wxKeyEvent& event );
        virtual void EventDelVar2List( wxCommandEvent &event );
        void apply_config_file(std::string ECU_XML_file_full_path);
        void Read_XML_file(std::string ECU_XML_file_full_path);
        void determine_next_free_row(void);
        void AddVar2List(void);
        void RmVarElementActiontable(void);
        // LabskausMain_Config.cpp
        void SaveConfiguration(std::string location);
        void read_last_config(std::string config_file);
        void createDefaultDir(void);
        void setLogDir2Default(void);
        void LoadLastConfig(void);
        void SaveLastConfig(void);
        std::string getDefaultDir(void);
        std::string getDefaultLogDir(void);
        std::string getFileNameLastConfig(void);
        ECU_VarInfo* find_ECUVarByName(std::string VarName);
};


class LabskausFrameSetCal: public Dialog_SetValue
{
    public:
        LabskausFrameSetCal(CCP_driver *ptr_ccp_driver, ECU_VarInfo *ptr_ECU_VarInfo);
        CCP_driver* CCP_Master;
        ECU_VarInfo* m_ECU_Variable;
        ~LabskausFrameSetCal();

    private:
        //virtual void OnClose(wxCloseEvent& event);
        //virtual void OnQuit(wxCommandEvent& event);
        virtual void event_CancelCalibration( wxCommandEvent& event );
        virtual void EventTakeOverVal(wxCommandEvent &event);
};

class LabskausFrameSettings: public DialogSettings
{
    public:
        LabskausFrameSettings(CCP_driver *ptr_ccp_driver);
        ~LabskausFrameSettings();
        CCP_driver* CCP_Master;
        endian newECUByteOrder;
        uint16_t newStationAddress;
        std::string newDevice;
        bool ByteOrder_changed;
        bool Station_Address_changed;
        bool Device_changed;

    private:
		virtual void event_ChangeByteOrder( wxCommandEvent& event );
		virtual void event_ChangeStationAddress( wxCommandEvent& event );
		virtual void event_Cancel( wxCommandEvent& event );
		virtual void event_Apply( wxCommandEvent& event );
		virtual void event_ChangeDevice( wxCommandEvent& event );
};

#endif // LABSKAUSMAIN_H
