#ifndef CMESSAGE_H
#define CMESSAGE_H
#include <string>

enum TYPE_MESSAGE { TYPE_MESSAGE_NO_SEND, TYPE_MESSAGE_SEND, TYPE_MESSAGE_IMPORTANT_SEND, TYPE_MESSAGE_MORE_IMPORTANT_SEND};

class CMessage
{
    public:
        CMessage();
        CMessage(const std::string _name, const std::string _description) : m_Name(_name), m_Description(_description) {}
        std::string ConvertObjToStr();
        virtual ~CMessage();
        std::string GetName() { return m_Name; }
        void SetName(std::string val) { m_Name = val; }
        std::string GetDescription() { return m_Description; }
        void SetDescription(std::string val) { m_Description = val; }
        unsigned int GetID() { return m_ID; }
        void SetID(unsigned int val) { m_ID = val; }
        void SetTypeTask(TYPE_MESSAGE typeTask);
        TYPE_MESSAGE GetTypeTask();
    protected:
    private:
        std::string m_Name;
        std::string m_Description;
        unsigned int m_ID;
        TYPE_MESSAGE m_typeTask;
};

#endif // CMESSAGE_H
