/***************************************************************
 * Name:      LabskausApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2017-03-01
 * Copyright:  ()
 * License:
 **************************************************************/
#include <iostream>
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "LabskausApp.h"
#include "LabskausMain.h"

IMPLEMENT_APP(LabskausApp);

bool LabskausApp::OnInit()
{
    LabskausFrame* frame = new LabskausFrame(0L);
    frame->Show();
    return true;
}


