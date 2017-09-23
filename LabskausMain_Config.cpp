#include "LabskausMain.h"
#include <fstream>
#include <string>
#include <sstream>
#include <wx/timer.h>
#include <streambuf>
#include <wx/wx.h>


#include "tinyxml2.h"
#include "CCP_driver.h"


/*******************************************************************************************
 * Function to save the active configuration to a location which is defined by the first
 * argument.
 ******************************************************************************************/
void LabskausFrame::SaveConfiguration(std::string file_full_path)
{
    using namespace tinyxml2;

    XMLDocument xmlDoc;
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
        std::cerr<< "The configuration file " << config_file << "could not be loaded." << std::endl;
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
        return;
    }
    /*******************************************************************************************
     * Part 3: Check if the next root element has the value ecu_xml_location an if this XML-
     *         element does has a value. If all is correct the XML file should be loaded.
     ******************************************************************************************/
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
    n_conf = n_conf->NextSiblingElement();
    /* Information about the log folder is expected */
    if(n_conf != nullptr)
    {
        if (    (strcmp(n_conf->Value( ), "log_folder_location") != 0)
             || ( n_conf->GetText() == nullptr                       )  )
        {
            wxMessageBox(_("Log folder was not found inside the configuration file.\n\n Log folder is set to the default directory"),_("Configuration Load Warning"));
            LOG_dir = "/home/mattes/.Labskaus/logs";
        }
        else
        {
            LOG_dir = n_conf->GetText();
            CCP_Master->SetLogFolder(LOG_dir);
            //TODO check if log folder is existing.
        }
    }
    /*******************************************************************************************
     * Part 5: Load the lastest variables into the ActionTable. This can only be done with the
     *         if the ECU-XML file has been successfully loaded (see Part1 of this function).
     ******************************************************************************************/
    if(ECU_XML_file_found == true)
    {
        n_conf = n_conf->NextSiblingElement();
        n_measurement_setup = n_conf->FirstChildElement();
        /* Information about the log folder is expected */
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


