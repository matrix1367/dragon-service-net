#ifndef CMESSAGE_H
#define CMESSAGE_H
#include "CDLog.h"
#include <string>

enum TYPE_MESSAGE { TYPE_MESSAGE_NO_SEND, TYPE_MESSAGE_SEND, TYPE_MESSAGE_IMPORTANT_SEND, TYPE_MESSAGE_MORE_IMPORTANT_SEND};

class CMessage
{
    public:
        CMessage();
        CMessage(const std::string _name, const std::string _description) : m_Name(_name), m_Description(_description) {}
        CMessage(const std::string _name, const std::string _description, unsigned int id) : m_Name(_name), m_Description(_description), m_ID(id) {}
        virtual ~CMessage();
        std::string GetName() const { return m_Name; }
        void SetName(std::string val) { m_Name = val; }
        std::string GetDescription() const { return m_Description; }
        void SetDescription(std::string val) { m_Description = val; }
        unsigned int GetID() { return m_ID; }
        std::string GetStrID() const {
            return CDLog::ToString(m_ID);
        }
        void SetID(unsigned int val) { m_ID = val; }
        static CMessage ConvertStrToObj(std::string parm, unsigned int idClient);
        void SetTypeTask(TYPE_MESSAGE typeTask);
        TYPE_MESSAGE GetTypeTask() const;
    protected:
    private:
        std::string m_Name;
        std::string m_Description;
        unsigned int m_ID;
        TYPE_MESSAGE m_typeTask;
};

#endif // CMESSAGE_H
