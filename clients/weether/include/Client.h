#ifndef CCLIENT_H
#define CCLIENT_H


class CClient
{
    public:
        CClient();
        virtual ~CClient();
        bool Init(void);
    protected:
    private:
        char* m_ipAddres;
        int m_port;
};

#endif // CCLIENT_H
