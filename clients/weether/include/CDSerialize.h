#ifndef CDSERIALIZE_H
#define CDSERIALIZE_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

template < class T >
class CDSerialize
{
    public:
        CDSerialize(std::string fileName) : m_fileName(fileName) {}

        bool Write( T& data)
        {
            ofstream out_file(m_fileName.c_str(), ios::out | ios::binary);
            if (!out_file) return false;
            out_file.write((char*) &data, sizeof(T));
            out_file.close();
            return true;
        }

        bool Read(T& data)
        {
            ifstream in_file(m_fileName.c_str(), ios::in | ios::binary);
            if (!in_file) return false;
            in_file.read((char *) &data, sizeof(T));
            in_file.close();
            return true;
        }



    protected:
    private:
        std::string m_fileName;
};

#endif // CDSERIALIZE_H
