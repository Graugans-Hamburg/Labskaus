#ifndef XMLECUFILE_H
#define XMLECUFILE_H

#include <iostream>
#include <string>


class xmlECUfile
{

    public:
        xmlECUfile();
        int openECU_xml_file();
        virtual ~xmlECUfile();
    protected:
    private:
    std::fstream* ECU_xml_file;
};

#endif // XMLECUFILE_H
