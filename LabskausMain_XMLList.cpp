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
 * Function: All variables information are listed inside the ECU-XML file. This need to be
 *           loaded to do any measurement. After the file is loaded the information will be
 *           extracted and collected inside a vector calle XML_List.
 *
 *           The function is searching for a ECU variable inside this vector by its variable
 *           name. If the name is found the reference to this object will be returned.
 *
 ******************************************************************************************/
ECU_VarInfo* LabskausFrame::find_ECUVarByName(std::string VarName)
{
    ECU_VarInfo* tmp_VarInfo;
    for(unsigned int idx_i = 0; idx_i < XML_list.size(); idx_i++)
    {
        tmp_VarInfo = &XML_list.at(idx_i);
        if(!VarName.compare(tmp_VarInfo->GetName()))
        {
            return tmp_VarInfo;
        }
    }
    // If the variable had not been found
    return nullptr;
}




/*******************************************************************************************
 * Function: Extract all information from a ECU-XML file and place then inside a vector. This
 *           contains all variables that can be set or monitored.
 *
 ******************************************************************************************/
void LabskausFrame::Read_XML_file(std::string ECU_XML_file_full_path)
{
    // remove the content of the last XML file
    XML_list.clear();
    /*********************************************************************************
    Extract all information from the ECU XML file
    *********************************************************************************/
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    XMLError result;
    result = doc.LoadFile(ECU_XML_file_full_path.c_str());
    if(result != 0)
    {
        std::cerr << "ECU xml file: " << ECU_XML_file_full_path << "could not be loaded"
                  << std::endl;
        return;
    }
    tinyxml2::XMLNode* root = doc.FirstChildElement();
    m_listBox1->Clear();
    // Counter to know how many list elements are available
    int postn = 0;

    if (strcmp(root->Value( ), "measurement_file") != 0) {
        cout << string("bad root: Wrong xml? ") + root->Value( ) << endl;
        }
    else{
        //cout << "Found the element "<< root->Value() <<" in line "<<root->GetLineNum() << endl;
        }
    XMLNode* variable = root->FirstChildElement();
    if (strcmp(variable->Value( ), "variable") != 0) {
        cout << string("bad root: Wrong XML? ") + variable->Value( ) << endl;
        }
    else{
        //cout << "Found the element "<< variable->Value() <<" in line "<<variable->GetLineNum() << endl;
        }
    do{
    //cout << "---------------------" << endl;
        XMLElement* varelement = variable->FirstChildElement();
        std::string var_name;
        std::string var_address;
        std::string var_type;
        std::string var_unit;
        std::string var_des;
        do{
            if (!varelement) {
                cout << "No Child found." << endl;
                }
            else
            {
                std::string str(varelement->GetText());
                if(!str.empty())
                {
                    /*cout << "Found the element "<< varelement->Value() <<" in line "<<varelement->GetLineNum() <<
                    " Content: "<< str <<endl;*/

                    if(strcmp(varelement->Value(),"name")== 0)
                    {
                        /*cout << "Name found" << endl;*/
                        var_name = str;
                    }
                    if(strcmp(varelement->Value(),"address")== 0)
                    {
                        /*cout << "Address found" << endl;*/
                        var_address = str;
                    }
                    if(strcmp(varelement->Value(),"datatype")==0)
                    {
                        /*cout << "Datatype found" << endl;*/
                        var_type = str;
                    }
                    if(strcmp(varelement->Value(),"unit")==0)
                    {
                        /*cout << "Datatype found" << endl;*/
                        var_unit = str;
                    }
                    if(strcmp(varelement->Value(),"description")==0)
                    {
                        /*cout << "Datatype found" << endl;*/
                        var_des = str;
                    }
                }
                else
                {
                    /* cout << "Found the element "<< varelement->Value() <<" in line "
                    << varelement->GetLineNum() << " has more children" << endl; */
                }
            }
            varelement = varelement->NextSiblingElement();
        }while(varelement);

        postn = postn +1;
        m_listBox1->Append(var_name);

        ECU_VarInfo* tmp_var_element = new(ECU_VarInfo);
        tmp_var_element->SetName(var_name);
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << var_address;
        ss >> x;
        tmp_var_element->SetAddress(x);
        tmp_var_element->SetUnit(var_unit);
        tmp_var_element->SetDescription(var_des);
        tmp_var_element->ParseDatatyp(var_type);
        XML_list.push_back(*tmp_var_element);

        variable = variable->NextSiblingElement();
    }while(variable);

    CCP_Master->Set_XMLExtract(&XML_list);
}
