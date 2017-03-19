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


#include "GUIFrame.h"

class LabskausFrame: public GUIFrame
{
    public:
        LabskausFrame(wxFrame *frame);
        ECU_VarListElement* MatzeListe;
        ~LabskausFrame();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void open_load_dialog(wxCommandEvent& event);
        virtual void VarListSelected(wxCommandEvent& event);
};

#endif // LABSKAUSMAIN_H
