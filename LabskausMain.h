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



#include "LabskausApp.h"
#include "ECU_VarListElement.h"
#include "serial.h"


#include "GUIFrame.h"

enum // Timer events IDs
{
    Rec_Timer = wxID_HIGHEST,
};
const int interval = 100;

class LabskausFrame: public GUIFrame
{
    public:
        LabskausFrame(wxFrame *frame);
        ECU_VarListElement* MatzeListe;
        serial* SerialPort;
        wxTimer* recTimer; // declaration of Timer object
        ~LabskausFrame();

    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void open_load_dialog(wxCommandEvent& event);
        virtual void VarListSelected(wxCommandEvent& event);
        virtual void EventOpenSerial(wxCommandEvent& event);
        virtual void EventCloseSerial(wxCommandEvent& event);
        virtual void OnRecTimer(wxTimerEvent& event);
};

#endif // LABSKAUSMAIN_H
