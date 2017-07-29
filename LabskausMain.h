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
        void DEBUGOfflineTest(void);
        wxTimer* recTimer; // declaration of Timer object
        wxTimer* data_acquisition_timer;
        CCP_driver* CCP_Master;
        wxString ECU_XML_full_Path;
        wxString ECU_XML_file_dir;
        wxString ECU_XML_file_name;
        wxString LOG_dir;
        ~LabskausFrame();

    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void open_load_dialog(wxCommandEvent& event);
        virtual void open_log_dialog(wxCommandEvent& event);
        virtual void VarListSelected(wxCommandEvent& event);
        virtual void EventOpenSerial(wxCommandEvent& event);
        virtual void EventCloseSerial(wxCommandEvent& event);
        virtual void OnRecTimer(wxTimerEvent& event);
        virtual void DA_List_Timer(wxTimerEvent& event);
        virtual void EventAddVar2List(wxCommandEvent &event);
        virtual void EventAddCalVal2List(wxCommandEvent &event);
        virtual void EventMeaListKeyPres( wxKeyEvent& event );
        void read_last_config(void);
        void Read_XML_file(void);
        void determine_next_free_row();
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
        //virtual void OnAbout(wxCommandEvent& event);
        virtual void EventTakeOverVal(wxCommandEvent &event);
};

#endif // LABSKAUSMAIN_H
