#include "LabskausMain.h"
#include <fstream>
#include <string>
#include <sstream>
#include <wx/timer.h>
#include <streambuf>
#include <wx/wx.h>


#include "tinyxml2.h"
#include "CCP_driver.h"

#if defined(__WXMSW__)
    #include <windows.h>
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif

// Required to get the user name and the name of the home directory






/*******************************************************************************************
 * Function to save the active configuration to a location which is defined by the first
 * argument.
 ******************************************************************************************/
void LabskausFrame::SaveConfiguration(std::string file_full_path)
{
    using namespace tinyxml2;

    tinyxml2::XMLDocument xmlDoc;
    // Declaration
    XMLDeclaration * pdec = xmlDoc.NewDeclaration( "xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
    xmlDoc.InsertFirstChild(pdec);
    // Rootelement
    XMLNode * pRoot = xmlDoc.NewElement("labskaus_configuration_file");
    xmlDoc.InsertEndChild(pRoot);
    // Config file version
    XMLElement * p_ConfigVersion = xmlDoc.NewElement("version_config_file");
    pRoot->InsertEndChild(p_ConfigVersion);
    p_ConfigVersion->SetText("Not used by now.");
    // ECU XML location
    XMLElement * p_ECU_XMLlocation = xmlDoc.NewElement("ecu_xml_location");
    pRoot->InsertEndChild(p_ECU_XMLlocation);
    p_ECU_XMLlocation->SetText(ECU_XML_full_Path.c_str());
    // Log file version
    XMLElement * p_LOGlocation = xmlDoc.NewElement("log_folder_location");
    pRoot->InsertEndChild(p_LOGlocation);
    p_LOGlocation->SetText(LOG_dir.c_str());
    // Save the lastest Portnumber that had been used
    XMLElement * p_Portnumber = xmlDoc.NewElement("Portnumber");
    pRoot->InsertEndChild(p_Portnumber);
    std::string tmp1 = std::to_string(CCP_Master->SerialPort.Get_port_number());
    p_Portnumber->SetText(tmp1.c_str());
    // Save the lastest Startbyte that had been used
    XMLElement * p_Startbyte = xmlDoc.NewElement("Startbyte");
    pRoot->InsertEndChild(p_Startbyte);
    std::string tmp2 = std::to_string(CCP_Master->SerialPort.Get_StartByte());
    p_Startbyte->SetText(tmp2.c_str());
    // Save the lastest Baudrate that had been used
    XMLElement * p_Baudrate = xmlDoc.NewElement("Baudrate");
    pRoot->InsertEndChild(p_Baudrate);
    std::string tmp3 = std::to_string(CCP_Master->SerialPort.Get_baud_rate());
    p_Baudrate->SetText(tmp3.c_str());
    // Save the lastest Serial Mode that had been used
    XMLElement * p_SerialMode = xmlDoc.NewElement("SerialMode");
    pRoot->InsertEndChild(p_SerialMode);
    std::string tmp4 = CCP_Master->SerialPort.Get_serial_mode();
    p_SerialMode->SetText(tmp4.c_str());
    // measurement setup
    XMLElement * p_measuremet_setup = xmlDoc.NewElement("measurement_setup");
    pRoot->InsertEndChild(p_measuremet_setup);
    for(int idx_i = 0; idx_i < m_MeasList->GetNumberRows(); idx_i++)
    {
        XMLElement * pName = xmlDoc.NewElement("variable");
        p_measuremet_setup->InsertEndChild(pName);
        wxString tmp_wxString = m_MeasList->GetCellValue(idx_i,0);
        pName->SetText(tmp_wxString.ToAscii());
    }

    XMLError eResult = xmlDoc.SaveFile(file_full_path.c_str());
    // TODO: Do some checks and create a error if the save function fails
    std::cout << "Configuration Saved!" << std::endl;
}




/*******************************************************************************************
 * Function to load the configuration which is defined by the first argument
 ******************************************************************************************/
void LabskausFrame::apply_config_file(std::string config_file)
{
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument ConfigXML;
    XMLError result;
    /*******************************************************************************************
     * Part 0: Check if the argument that is handed over exists and that it can be opened.
     *         next step open it with tinyxml2
     ******************************************************************************************/
    result = ConfigXML.LoadFile(config_file.c_str());
    if(result != 0)
    {
        std::cerr<< "The configuration file " << config_file << " could not be loaded." << std::endl;
        setLogDir2Default();
        return;
    }
    tinyxml2::XMLNode* n_root = ConfigXML.FirstChildElement();
    tinyxml2::XMLElement* n_conf = n_root->FirstChildElement();
    tinyxml2::XMLElement* n_measurement_setup;

    bool ECU_XML_file_found = false;
    /*******************************************************************************************
     * Part 1: Clear the ActionTable
     ******************************************************************************************/
    CCP_Master->clearActionPlan();
    m_MeasList->ClearGrid();
    if(m_MeasList->GetNumberRows()>0)
    {
        m_MeasList->DeleteRows(0,m_MeasList->GetNumberRows());
    }
    /*******************************************************************************************
     * Part 2: Check if the root element of the XML file is correct and if the first child
     *         element of this is the one with <version_config_file>
     ******************************************************************************************/
    if (    (strcmp(n_root->Value( ), "labskaus_configuration_file") != 0)
         || (strcmp(n_conf->Value( ), "version_config_file")         != 0)  )
    {
        std::cerr << "XML file with the last configuration could not be loaded" << endl;
        setLogDir2Default();
        return;
    }
    /*******************************************************************************************
     * Part 3: Check if the next root element has the value ecu_xml_location an if this XML-
     *         element does has a value. If all is correct the XML file should be loaded.
     ******************************************************************************************/
    if(n_conf == NULL) return;
    n_conf = n_conf->NextSiblingElement();
    /* Information about the full path of the xml file is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "ecu_xml_location") != 0)
             || ( n_conf->GetText() == nullptr                    )  )
        {
            wxMessageBox(_("ECU-XML file could not be found inside the XML document.\n\n Stop to read config"),_("Configuration Load Warning"));

        }
        else
        {
            ECU_XML_full_Path = n_conf->GetText();
            Read_XML_file(n_conf->GetText());
            ECU_XML_file_found = true;
        }
    }
    /*******************************************************************************************
     * Part 4: Check if the next root element has the value log_folder_location an if this XML-
     *         element does has a value. If all is correct the location for the log files should
     *         should be set. If no log folder is defined by the configuration file, a warning
     *         shall inform the user about it and a base location shall be defined.
     ******************************************************************************************/
    if(n_conf == NULL) return;
    n_conf = n_conf->NextSiblingElement();
    /* Information about the log folder is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "log_folder_location") != 0)
             || ( n_conf->GetText() == nullptr                       )  )
        {
            wxMessageBox(_("Log folder was not found inside the configuration file.\n\n Log folder is set to the default directory"),_("Configuration Load Warning"));
            setLogDir2Default();
        }
        else
        {
            LOG_dir = n_conf->GetText();
        }
        //TODO check if log folder is existing.
        CCP_Master->SetLogFolder(LOG_dir);
    }
    /*******************************************************************************************
     * Part 5: Check if the next root element has the value portnumber an if this XML-
     *         element does has a value. If all is correct the location for the log files should
     *         should be set. If no log folder is defined by the configuration file, a warning
     *         shall inform the user about it and a base location shall be defined.
     ******************************************************************************************/
    if(n_conf == NULL) return;
    n_conf = n_conf->NextSiblingElement();
    /* Information about the log folder is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "Portnumber") != 0)
             || ( n_conf->GetText() == nullptr                       )  )
        {
            wxMessageBox(_("Portnumber was not found inside the configuration file.\n\n Portnumber will be set to default value"),_("Configuration Load Warning"));
        }
        else
        {
            std::string tmp = n_conf->GetText();
            CCP_Master->SerialPort.Set_port_number(std::stoi(tmp));
        }
    }
    /*******************************************************************************************
     * Part 6: Check if the next root element has the value startbyte an if this XML-
     *         element does has a value. If all is correct the location for the log files should
     *         should be set. If no log folder is defined by the configuration file, a warning
     *         shall inform the user about it and a base location shall be defined.
     ******************************************************************************************/
    if(n_conf == NULL) return;
    n_conf = n_conf->NextSiblingElement();
    /* Information about the log folder is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "Startbyte") != 0)
             || ( n_conf->GetText() == nullptr                       )  )
        {
            wxMessageBox(_("Start was not found inside the configuration file.\n\n Startbyte will be set to default value"),_("Configuration Load Warning"));
        }
        else
        {
            std::string tmp = n_conf->GetText();
            CCP_Master->SerialPort.Set_StartByte((StartByteEnum)std::stoi(tmp));
        }
    }
    /*******************************************************************************************
     * Part 7: Check if the next root element has the value portnumber an if this XML-
     *         element does has a value. If all is correct the location for the log files should
     *         should be set. If no log folder is defined by the configuration file, a warning
     *         shall inform the user about it and a base location shall be defined.
     ******************************************************************************************/
    if(n_conf == NULL) return;
    n_conf = n_conf->NextSiblingElement();
    /* Information about the log folder is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "Baudrate") != 0)
             || ( n_conf->GetText() == nullptr                       )  )
        {
            wxMessageBox(_("Baudrate was not found inside the configuration file.\n\n Baudrate will be set to default value"),_("Configuration Load Warning"));
        }
        else
        {
            std::string tmp = n_conf->GetText();
            CCP_Master->SerialPort.Set_baud_rate(std::stoi(tmp));
        }
    }
    /*******************************************************************************************
     * Part 8: Check if the next root element has the value startbyte an if this XML-
     *         element does has a value. If all is correct the location for the log files should
     *         should be set. If no log folder is defined by the configuration file, a warning
     *         shall inform the user about it and a base location shall be defined.
     ******************************************************************************************/
    if(n_conf == NULL) return;
    n_conf = n_conf->NextSiblingElement();
    /* Information about the log folder is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "SerialMode") != 0)
             || ( n_conf->GetText() == nullptr                       )  )
        {
            wxMessageBox(_("SerialMode was not found inside the configuration file.\n\n SerialMode will be set to default value"),_("Configuration Load Warning"));
        }
        else
        {
            std::string tmp = n_conf->GetText();
            CCP_Master->SerialPort.Set_serial_mode(tmp);
        }
    }
    /*******************************************************************************************
     * Part 9: Load the lastest variables into the ActionTable. This can only be done with the
     *         if the ECU-XML file has been successfully loaded (see Part1 of this function).
     ******************************************************************************************/
    if(ECU_XML_file_found == true)
    {
        if(n_conf == NULL) return;
        n_conf = n_conf->NextSiblingElement();
        if(n_conf == NULL) return;
        n_measurement_setup = n_conf->FirstChildElement();
        /* Information about the log variables is expected */
        if (    (strcmp(n_conf->Value(), "measurement_setup") != 0)
             || ( n_measurement_setup == nullptr                  )  )
        {
            wxMessageBox(_("No measurement setup was found inside the configuration file.\n\n Labskaus will start with a emtpy measurement setup"),_("Configuration Load Warning"));

        }
        else
        {
            //str_Measuerment_log_folder = n_conf->GetText();
            //TODO check if log folder is existing.
            while(n_measurement_setup)
            {
                if(n_measurement_setup->GetText())
                {
                    //Find the ECU variable inside the list by name
                    int result;
                    if(find_ECUVarByName(n_measurement_setup->GetText()) != nullptr)
                    {
                        ECU_VarInfo& ref_VarInfo = *(find_ECUVarByName(n_measurement_setup->GetText()));
                        result = CCP_Master->addvariable2ActionPlan(ref_VarInfo);
                        if(!result)
                        {
                            m_MeasList->AppendRows(1);
                            determine_next_free_row();
                            ECU_VarInfo& tmp_ECU_VarInfo = ref_VarInfo;
                            wxString tmp_wxString(tmp_ECU_VarInfo.GetName());
                            wxString tmp2_wxString(tmp_ECU_VarInfo.GetUnit());
                            m_MeasList->SetCellValue(m_next_free_row,0,tmp_wxString);
                            m_MeasList->SetCellValue(m_next_free_row,2,tmp2_wxString);
                        }
                    }
                }
                n_measurement_setup = n_measurement_setup->NextSiblingElement();
            }
        }
    }
}
/*******************************************************************************************
 * Function to open a dialog to load an already saved configuration
 ******************************************************************************************/
void LabskausFrame::open_load_config_dialog(wxCommandEvent &event)
{

    std::string tmp_config_file;

    wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a Labskaus configuration file"), wxEmptyString, wxEmptyString,
		_("Labskaus Config File (*.lcf)|*.lcf"),
		wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		tmp_config_file = OpenDialog->GetPath();
	}
	else
	{
        std::cout << "XML read canceled;" << std::endl;
        return;
	}
	// Clean up after ourselves
	OpenDialog->Destroy();
    apply_config_file(tmp_config_file);
}

/*******************************************************************************************
 * Function to open a dialog to save a new configuration
 ******************************************************************************************/
void LabskausFrame::open_save_config_dialog(wxCommandEvent &event)
{
    std::string tmp_config_file;
    wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a Labskaus configuration file"), wxEmptyString, wxEmptyString,
		_("Labskaus Config File (*.lcf)|*.lcf"),
		wxFD_SAVE, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		tmp_config_file = OpenDialog->GetPath();
	}
	else
	{
        std::cout << "XML save canceled;" << std::endl;
        return;
	}
	// Clean up after ourselves
	OpenDialog->Destroy();
    SaveConfiguration(tmp_config_file.append(".lcf"));
}

/*******************************************************************************************
 * Function: Ask for the default directory and if it not exists, create it. Create as well
 *           the sub folder inside the base folder with the name logs.
 ******************************************************************************************/
void LabskausFrame::createDefaultDir(void)
 {
    std::string default_dir = getDefaultDir();
    std::string default_log = getDefaultLogDir();

    // Generate System Command
    std::string create_default_dir = "mkdir ";
    std::string create_default_log = "mkdir ";
    create_default_dir.append(default_dir);
    create_default_log.append(default_log);

    // Fire System Commands
    std::cout << "Create Default     dirctory : " << create_default_dir << std::endl;
    std::cout << "Create Default log dirctory : " << create_default_log << std::endl;

    system(create_default_dir.c_str());
    system(create_default_log.c_str());
    return;
 }



/*******************************************************************************************
* Function: Set the default log directory
******************************************************************************************/
 void LabskausFrame::setLogDir2Default(void)
 {
    // This is what need to be supported
    std::cout << "Set default log path. Path: " << getDefaultLogDir() << std::endl;
    CCP_Master->SetLogFolder(getDefaultLogDir());
 }

/*******************************************************************************************
* Function: Load the last configuration
******************************************************************************************/
 void LabskausFrame::LoadLastConfig(void)
 {
    // This is what need to be supported
    std::cout << "Open the last configuration. File: " << getFileNameLastConfig() << std::endl;
    apply_config_file(getFileNameLastConfig());
 }

/*******************************************************************************************
* Function: Save the last configuration
******************************************************************************************/
void LabskausFrame::SaveLastConfig(void)
 {
    // This is what need to be supported
    std::cout << "Save the last configuration. File: " << getFileNameLastConfig() << std::endl;
    SaveConfiguration(getFileNameLastConfig());
 }
#if defined(__WXMSW__)
/*******************************************************************************************
 * Function: This Functions are returning the pathes the default configuration and the logs
 *            On a windows system this shall be:
 *
 *           default     :c:\Users\<username>\AppData\Local\Labskaus
 *           log         :c:\Users\<username>\AppData\Local\Labskaus\Logs
 *           last config :c:\Users\<username>\AppData\Local\Labskaus\base.lcf
 *
 * Windows-SPECIFIC
 ******************************************************************************************/

 std::string LabskausFrame::getDefaultDir(void)
 {
    char username[256+1];
    DWORD username_len = 256+1;
    GetUserNameA(username, &username_len);

    std::string default_dir;
    std::string default_log;
    default_dir = "c:\\Users\\";
    default_dir.append(username);
    default_dir.append("\\AppData\\Local\\Labskaus");

    return default_dir;
 }

std::string LabskausFrame::getDefaultLogDir(void)
 {
    std::string default_log;
    default_log = getDefaultDir();
    default_log.append("\\Logs");

    return default_log;
 }

  std::string LabskausFrame::getFileNameLastConfig(void)
 {
    std::string filenamelastconfig = getDefaultDir();
    filenamelastconfig.append("\\base.lcf");

    return filenamelastconfig;
 }

#elif defined(__WXMAC__)
        // not defined by now.
#elif defined(__UNIX__)
 /*******************************************************************************************
 * Function: This Functions are returning the pathes the default configuration and the logs
 *           On a windows system this shall be:
 *
 *           default     :/home/<Users>./Labskaus
 *           log         :/home/<Users>./Labskaus/Logs
 *           last config :/home/<Users>./Labskaus/base.lcf
 *
 * Linux-SPECIFIC
 ******************************************************************************************/

 std::string LabskausFrame::getDefaultDir(void)
 {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string default_dir = homedir;
    default_dir.append("/.Labskaus");
    return default_dir;
 }

std::string LabskausFrame::getDefaultLogDir(void)
 {
    std::string default_log;
    default_log = getDefaultDir();
    default_log.append("/logs");

    return default_log;
 }

  std::string LabskausFrame::getFileNameLastConfig(void)
 {
    std::string filenamelastconfig = getDefaultDir();
    filenamelastconfig.append("/base.lcf");

    return filenamelastconfig;
 }
#endif






