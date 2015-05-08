#ifndef CDATABASE_H
#define CDATABASE_H
#include <string>

class CDataBase
{
    public:
        CDataBase();
        virtual ~CDataBase();
        virtual bool Open() = 0;
        virtual bool Close() = 0;
        virtual void Exec(std::string sql) = 0;
    protected:
    private:
};

#endif // CDATABASE_H
