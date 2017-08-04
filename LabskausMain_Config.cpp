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
    // Rootelement
    XMLNode * pRoot = xmlDoc.NewElement("labskaus_configuration_file");
    xmlDoc.InsertFirstChild(pRoot);
    // Config file version
    XMLElement * p_ConfigVersion = xmlDoc.NewElement("version_config_file");
    pRoot->InsertEndChild(p_ConfigVersion);
    p_ConfigVersion->SetText("Not used by now.");
    // ECU XML location
    XMLElement * p_ECU_XMLlocation = xmlDoc.NewElement("ecu_xml_location");
    pRoot->InsertEndChild(p_ECU_XMLlocation);
    p_ECU_XMLlocation->SetText(ECU_XML_full_Path.ToAscii());
    // Log file version
    XMLElement * p_LOGlocation = xmlDoc.NewElement("log_folder_location");
    pRoot->InsertEndChild(p_LOGlocation);
    p_LOGlocation->SetText(LOG_dir.ToAscii());
    // measurement setup
    XMLElement * p_measuremet_setup = xmlDoc.NewElement("measurement_setup");
    pRoot->InsertEndChild(p_measuremet_setup);
    for(unsigned int idx_i = 0; idx_i < m_MeasList->GetNumberRows(); idx_i++)
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
    ConfigXML.LoadFile(config_file.c_str());
    tinyxml2::XMLNode* n_root = ConfigXML.FirstChildElement();
    tinyxml2::XMLElement* n_conf = n_root->FirstChildElement();

    //Container for extracted Information
    std::string str_XML_ECU_full_path;
    std::string str_Measuerment_log_folder;
    /*******************************************************************************************
     * Part 1: Check if the root element of the XML file is correct and if the first child
     *         element of this is the one with <version_config_file>
     ******************************************************************************************/
    if (    (strcmp(n_root->Value( ), "labskaus_configuration_file") != 0)
         || (strcmp(n_conf->Value( ), "version_config_file")         != 0)  )
    {
        std::cerr << "XML file with the last configuration could not be loaded" << endl;
        return;
    }
    /*******************************************************************************************
     * Part 2: Check if the next root element has the value ecu_xml_location an if this XML-
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
            str_XML_ECU_full_path = n_conf->GetText();
            Read_XML_file(n_conf->GetText());
        }
    }
    /*******************************************************************************************
     * Part 3: Check if the next root element has the value log_folder_location an if this XML-
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
            str_Measuerment_log_folder = "/home/mattes/.Labskaus/logs";
        }
        else
        {
            str_Measuerment_log_folder = n_conf->GetText();
        }
    }
}
