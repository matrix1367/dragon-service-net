#ifndef CMESSAGE_H
#define CMESSAGE_H
#include <string>

class CMessage
{
    public:
        CMessage();
        CMessage(const std::string _name, const std::string _description) : m_Name(_name), m_Description(_description) {}
        virtual ~CMessage();
        std::string GetName() { return m_Name; }
        void SetName(std::string val) { m_Name = val; }
        std::string GetDescription() { return m_Description; }
        void SetDescription(std::string val) { m_Description = val; }
        unsigned int GetID() { return m_ID; }
        void SetID(unsigned int val) { m_ID = val; }
        static CMessage ConvertStrToObj(std::string parm);
    protected:
    private:
        std::string m_Name;
        std::string m_Description;
        unsigned int m_ID;
};

#endif // CMESSAGE_H
