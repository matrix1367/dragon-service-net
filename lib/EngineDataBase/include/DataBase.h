#ifndef CDATABASE_H
#define CDATABASE_H
#include <string>
#include <vector>

class IDataBase
{
    public:
        IDataBase();
        virtual ~IDataBase();
        virtual bool Open() = 0;
        virtual bool Close() = 0;
        virtual bool Query(std::vector<std::vector<std::string> >& results, const std::string& message) = 0;
    protected:
    private:
};

#endif // CDATABASE_H
