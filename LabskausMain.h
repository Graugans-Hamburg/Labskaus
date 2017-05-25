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
#include "ECU_variable.h"
#include "CCP_driver.h"
#include "wx/timer.h"
#include "GUIFrame.h"

enum // Timer events IDs
{
    Rec_Timer = wxID_HIGHEST,
    DAL_Timer = wxID_HIGHEST -1,
};

const int interval = 10;
const int data_acq_list_timer_ms = 10;

class LabskausFrame: public GUIFrame
{
    public:
        LabskausFrame(wxFrame *frame);
        std::vector<ECU_variable> XML_list;
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
        void read_last_config(void);
        void Read_XML_file(void);
};

#endif // LABSKAUSMAIN_H
